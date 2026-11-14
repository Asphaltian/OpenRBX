#include "util/ContentProvider.h"

#include "compat.h"
#include "placeholder.h"
#include "util/FileSystem.h"
#include "util/Http.h"
#include "util/StandardOut.h"
#include "v8xml/SerializerV2.h"
#include "v8xml/XmlElement.h"
#include "v8xml/XmlSerializer.h"

#include <G3D/format.h>
#include <algorithm>
#include <atlpath.h>
#include <atlstr.h>
#include <boost/bind.hpp>
#include <boost/iostreams/copy.hpp>
#include <fstream>
#include <istream>
#include <shlobj.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <wincrypt.h>

namespace RBX {

class MD5HasherImpl : public MD5Hasher
{
	HCRYPTPROV hProv;
	HCRYPTHASH hHash;
	std::string result;

public:
	MD5HasherImpl();
	~MD5HasherImpl();

	virtual void addData(std::istream& data);
	virtual void addData(const std::string& data);
	virtual void addData(const char* data, unsigned int nBytes);
	virtual std::string toString();
	virtual const char* c_str();
};

bool ContentProvider::isHttpUrl(const std::string& s)
{
	if (s.find("http://", 0, 7) == 0) {
		return true;
	}
	if (s.find("https://", 0, 8) == 0) {
		return true;
	}
	return false;
}

std::string ContentProvider::assetFolder() const
{
	return assetFolderPath;
}

bool operator<(const ContentId& a, const ContentId& b)
{
	return a.toString() < b.toString();
}

bool operator!=(const ContentId& a, const ContentId& b)
{
	return a.toString() != b.toString();
}

bool operator==(const ContentId& a, const ContentId& b)
{
	return a.toString() == b.toString();
}

ContentId ContentId::fromFile(const std::string& filePath)
{
	if (filePath.find("file://") == 0) {
		return ContentId(filePath);
	}

	std::string header("file://");
	return ContentId(header + filePath);
}

ContentId ContentId::fromAssets(const std::string& filePath)
{
	std::string header("rbxasset://");
	return ContentId(header + filePath);
}

std::string MD5Hasher::convertToLegacyHash(std::string hash)
{
	std::string result;
	for (int i = 0; i < 32; i += 2) {
		if (hash[i] != '0') {
			result += hash[i];
		}
		result += hash[i + 1];
	}
	return result;
}

MD5HasherImpl::MD5HasherImpl() : hProv(0), hHash(0)
{
	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
		StandardOut::singleton()->print(
			MESSAGE_ERROR, "Error during CryptAcquireContext. GetLastError = %d", GetLastError()
		);
	}

	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
		StandardOut::singleton()->print(
			MESSAGE_ERROR, "Error during CryptCreateHash. GetLastError = %d", GetLastError()
		);
	}
}

void MD5HasherImpl::addData(std::istream& data)
{
	char buffer[1024];
	data.clear();
	data.seekg(0, std::ios_base::beg);

	do {
		data.read(buffer, 1024);
		addData(buffer, data.gcount());
	} while (data.gcount() > 0);
}

std::string MD5HasherImpl::toString()
{
	c_str();
	return result;
}

void MD5HasherImpl::addData(const std::string& data)
{
	if (!CryptHashData(hHash, (const BYTE*) data.c_str(), data.size(), 0)) {
		StandardOut::singleton()->print(MESSAGE_ERROR, "Error during CryptHashData. GetLastError = %d", GetLastError());
	}
}

void MD5HasherImpl::addData(const char* data, unsigned int nBytes)
{
	if (!CryptHashData(hHash, (const BYTE*) data, nBytes, 0)) {
		StandardOut::singleton()->print(MESSAGE_ERROR, "Error during CryptHashData. GetLastError = %d", GetLastError());
	}
}

MD5Hasher* MD5Hasher::create()
{
	return new MD5HasherImpl();
}

bool ContentProvider::isRequestQueueEmpty()
{
	boost::mutex::scoped_lock lock(requestSync);
	return requestQueue.empty();
}

const char* MD5HasherImpl::c_str()
{
	if (result.size() == 0) {
		DWORD hashLength;
		DWORD foo = sizeof(DWORD);
		if (!CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE*) &hashLength, &foo, 0)) {
			StandardOut::singleton()->print(
				MESSAGE_ERROR, "Error during CryptGetHashParam. GetLastError = %d", GetLastError()
			);
		}

		BYTE* hash = new BYTE[hashLength];
		if (!CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashLength, 0)) {
			StandardOut::singleton()->print(
				MESSAGE_ERROR, "Error during CryptGetHashParam. GetLastError = %d", GetLastError()
			);
		}

		for (DWORD i = 0; i < hashLength; i++) {
			CString s;
			s.Format("%02x", hash[i]);
			result += s;
		}

		delete[] hash;
	}

	return result.c_str();
}

ContentProvider::FailedUrl::FailedUrl(const char* url)
	: url(url), expiration(boost::posix_time::second_clock::local_time() + boost::posix_time::minutes(5))
{
}

bool ContentProvider::isUrlBad(const char* url)
{
	boost::mutex::scoped_lock lock(requestSync);
	std::list<FailedUrl>::iterator end = failedUrls.end();
	for (std::list<FailedUrl>::iterator iter = failedUrls.begin(); iter != end; ++iter) {
		if (iter->expired()) {
			failedUrls.erase(iter, end);
			break;
		}
		if (iter->url == url) {
			return true;
		}
	}
	return false;
}

static CPath getLocalCachePath(bool createPath)
{
	CPath result;
	if (createPath) {
		static std::string s = FileSystem::getCacheDirectory(true);
		result.m_strPath = s.c_str();
	}
	else {
		static std::string s = FileSystem::getCacheDirectory(false);
		result.m_strPath = s.c_str();
	}
	return result;
}

void ContentProvider::clearFileCache()
{
	CPath path = getLocalCachePath(false);

	if (path.m_strPath != "") {
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = FindFirstFile(path.m_strPath + "*.*", &FindFileData);

		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					DeleteFile(path.m_strPath + FindFileData.cFileName);
				}
			} while (FindNextFile(hFind, &FindFileData));
		}
	}
}

ContentId ContentProvider::registerContent(std::istream& content, const Name& mimeType)
{
	ContentId id;
	{
		boost::scoped_ptr<MD5Hasher> hasher(MD5Hasher::create());
		hasher->addData(content);
		id = ContentId(hasher->toString(), mimeType);
	}
	CPath filePath = getLocalCachePath(true);
	filePath.Append(id.c_str());

	if (!filePath.FileExists()) {
		std::ofstream outStream(filePath, std::ios_base::out | std::ios_base::binary);

		content.clear();
		content.seekg(0, std::ios_base::beg);

		char buffer[1024];
		do {
			content.read(buffer, 1024);
			outStream.write(buffer, content.gcount());
		} while (content.gcount() > 0);
	}

	return id;
}

std::string ContentProvider::findHashFile(ContentId contentId)
{
	CPath cachePath = getLocalCachePath(true);
	CPath filePath = cachePath;
	filePath.Append(contentId.c_str());

	if (!filePath.FileExists()) {
		return "";
	}
	return (LPCSTR) filePath;
}

bool ContentProvider::registerFile(CachedContent* item)
{
	CachedContent temp;
	{
		boost::mutex::scoped_lock lock(contentCacheMutex);
		temp = *item;
	}

	if (!temp.filename) {
		std::istringstream ss(*temp.data);
		ContentId hash = registerContent(ss, Name::getNullName());
		std::string filename = findHashFile(hash);
		if (filename.empty()) {
			return false;
		}

		temp.filename.reset(new std::string(filename));

		boost::mutex::scoped_lock lock(contentCacheMutex);
		*item = temp;
	}

	return true;
}

void ContentProvider::clearContentCache()
{
	boost::mutex::scoped_lock lock(contentCacheMutex);
	contentCache.clear();
}

std::string ContentProvider::findFile(ContentId contentId)
{
	CPath filePath(contentId.c_str() + 7);

	if (!filePath.FileExists()) {
		return "";
	}
	return (LPCSTR) filePath;
}

std::string ContentProvider::findAsset(ContentId contentId)
{
	CPath filePath((assetFolderPath + (contentId.c_str() + 11)).c_str());

	if (!filePath.FileExists()) {
		return "";
	}
	return (LPCSTR) filePath;
}

void ContentProvider::setAssetFolder(const char* sPath)
{
	StandardOut::singleton()->print(MESSAGE_INFO, "setAssetFolder %s", sPath);

	CPath path(sPath);
	if (path.IsRelative()) {
		throw std::runtime_error(G3D::format("The path '%s' is a relative path", (LPCSTR) path));
	}

	if (!path.IsDirectory()) {
		throw std::runtime_error(G3D::format("The path '%s' does not exist", (LPCSTR) path));
	}

	path.AddBackslash();
	assetFolderPath = path;
}

worker_thread::work_result ContentProvider::processRequests()
{
	ContentId id;
	{
		boost::mutex::scoped_lock lock(requestSync);
		if (requestQueue.empty()) {
			return worker_thread::done;
		}
		id = requestQueue.front();
	}

	try {
		boost::shared_ptr<std::string> response(new std::string());
		try {
			Http(id.toString()).get(*response);
		}
		catch (...) {
			boost::mutex::scoped_lock lock(requestSync);
			failedUrls.push_front(id.c_str());
			throw;
		}

		boost::mutex::scoped_lock lock(contentCacheMutex);
		contentCache[id].data = response;
	}
	catch (std::exception& e) {
		StandardOut::singleton()->print(MESSAGE_ERROR, "Content failed for %s because %s", id.c_str(), e.what());
	}

	{
		boost::mutex::scoped_lock lock(requestSync);
		requestQueue.pop_front();
	}

	return worker_thread::more;
}

ContentProvider::CachedContent* ContentProvider::loadContent(ContentId id, HttpRequestType httpRequestType)
{
	if (id.isNull()) {
		return NULL;
	}

	{
		boost::mutex::scoped_lock lock(contentCacheMutex);
		ContentCache::iterator iter = contentCache.find(id);
		if (iter != contentCache.end()) {
			return &iter->second;
		}
	}

	if (id.isFile()) {
		boost::shared_ptr<const std::string> filename(new std::string(findFile(id)));
		if (filename->empty()) {
			return NULL;
		}

		boost::mutex::scoped_lock lock(contentCacheMutex);
		CachedContent& content = contentCache[id];
		content.filename = filename;
		return &content;
	}

	if (id.isAsset()) {
		boost::shared_ptr<const std::string> filename(new std::string(findAsset(id)));
		if (filename->empty()) {
			return NULL;
		}

		boost::mutex::scoped_lock lock(contentCacheMutex);
		CachedContent& content = contentCache[id];
		content.filename = filename;
		return &content;
	}

	{
		boost::shared_ptr<const std::string> filename(new std::string(findHashFile(id)));
		if (!filename->empty()) {
			boost::mutex::scoped_lock lock(contentCacheMutex);
			CachedContent& content = contentCache[id];
			content.filename = filename;
			return &content;
		}
	}

	if (id.isHttp()) {
		switch (httpRequestType) {
		case AsyncHttpRequest: {
			boost::mutex::scoped_lock lock(requestSync);
			if (!isUrlBad(id.c_str())) {
				if (std::find(requestQueue.begin(), requestQueue.end(), id) == requestQueue.end()) {
					requestQueue.push_back(id);
				}
			}
			requestProcessor->wake();
		} break;

		case SyncHttpRequest: {
			boost::shared_ptr<std::string> response(new std::string());
			try {
				Http(id.toString()).get(*response);
			}
			catch (...) {
				boost::mutex::scoped_lock lock(requestSync);
				failedUrls.push_front(id.c_str());
				throw;
			}

			boost::mutex::scoped_lock lock(contentCacheMutex);
			CachedContent& content = contentCache[id];
			content.data = response;
			return &content;
		}
		}
	}

	return NULL;
}

std::string ContentProvider::getFile(ContentId ticket)
{
	CachedContent* content = loadContent(ticket, SyncHttpRequest);
	if (!content || !registerFile(content)) {
		throw std::runtime_error(G3D::format("Unable to load %s", ticket.c_str()));
	}

	return *content->filename;
}

std::auto_ptr<std::istream> ContentProvider::getContent(ContentId ticket)
{
	CachedContent* content = loadContent(ticket, SyncHttpRequest);
	if (!content) {
		throw std::runtime_error(G3D::format("Unable to load %s", ticket.c_str()));
	}

	if (content->data) {
		return std::auto_ptr<std::istream>(new std::istringstream(*content->data));
	}
	else {
		std::istream* stream = new std::ifstream(content->filename->c_str(), std::ios_base::in | std::ios_base::binary);
		return std::auto_ptr<std::istream>(stream);
	}
}

ContentProvider::~ContentProvider()
{
	requestProcessor->join();
}

std::string ContentProvider::getAssetFile(const std::string& filePath)
{
	return getFile(ContentId::fromAssets(filePath));
}

bool ContentProvider::hasContent(ContentId id)
{
	return loadContent(id, NoHttpRequest) != NULL;
}

bool ContentProvider::requestContentFile(ContentId id, std::string& filename)
{
	CachedContent* content = loadContent(id, AsyncHttpRequest);
	if (!content) {
		return false;
	}

	if (!registerFile(content)) {
		return false;
	}

	filename = *content->filename;
	return true;
}

boost::shared_ptr<const std::string> ContentProvider::requestContentString(ContentId id)
{
	CachedContent* content = loadContent(id, AsyncHttpRequest);
	if (!content) {
		return boost::shared_ptr<const std::string>();
	}

	boost::mutex::scoped_lock lock(contentCacheMutex);
	if (!content->data) {
		std::ifstream stream(content->filename->c_str(), std::ios_base::in | std::ios_base::binary);

		std::ostringstream data;
		boost::iostreams::copy(stream, data);
		content->data.reset(new std::string(data.str()));
	}
	return content->data;
}

ContentId ContentProvider::readContent(const char* id, std::istream& stream, unsigned long dataLength)
{
	ContentId result(id);

	if (hasContent(result)) {
		stream.clear();
		stream.seekg(dataLength, std::ios_base::cur);
	}
	else {
		CPath filePath = getLocalCachePath(true);
		filePath.Append(result.c_str());

		std::ofstream content;
		content.open(filePath, std::ios_base::out | std::ios_base::binary);

		for (; dataLength > 0; --dataLength) {
			char data;
			stream.get(data);
			content << data;
		}
	}

	return result;
}

void ContentProvider::load(ContentId id, std::vector<boost::shared_ptr<Instance> >& instances)
{
	boost::scoped_ptr<std::istream> stream(getContent(id));

	TextXmlParser machine(stream->rdbuf());
	boost::scoped_ptr<XmlElement> root(machine.parse());

	SerializerV2().loadInstances(root.get(), instances);
}

ContentProvider::ContentProvider()
{
	char path[MAX_PATH];
	if (SHGetFolderPathAndSubDir(NULL, CSIDL_COMMON_APPDATA, NULL, 0, "Roblox\\content\\", path) == S_OK) {
		ContentProvider::singleton().setAssetFolder(path);
	}
	else {
		StandardOut::singleton()->print(MESSAGE_WARNING, "Failed to get asset folder");
	}

	requestProcessor.reset(new worker_thread(boost::bind(&ContentProvider::processRequests, this), "rbx_content"));
}

} // namespace RBX
