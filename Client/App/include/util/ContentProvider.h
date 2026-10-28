#ifndef UTIL_CONTENTPROVIDER_H
#define UTIL_CONTENTPROVIDER_H

#include "decomp.h"
#include "util/Name.h"
#include "util/boost.hpp"

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <iosfwd>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace RBX {

class Instance;

// clang-format off
// FUNCTION: WEBSERVICE 0x10005a80
// RBX::ContentProvider::singleton
// clang-format on

// SIZE 0x20
class ContentId
{
public:
	static ContentId fromUrl(const std::string&);
	static ContentId fromFile(const std::string&);
	static ContentId fromAssets(const std::string& id);
	static ContentId fromMD5Hash(const std::string&);

	ContentId(const std::string& id, const Name& mimeType) : id(id), mimeTypePtr(&mimeType) {}

	// FUNCTION: WEBSERVICE 0x1004f100
	ContentId(const char* id) : id(id), mimeTypePtr(&Name::getNullName()) {}

	ContentId(const std::string& id) : id(id), mimeTypePtr(&Name::getNullName()) {}

	ContentId() : mimeTypePtr(&Name::getNullName()) {}

	const Name& mimeType() const { return *mimeTypePtr; }

	const char* c_str() const { return id.c_str(); }

	const std::string& toString() const { return id; }

	bool isNull() const { return id.empty(); }

	// FUNCTION: WEBSERVICE 0x1004f150
	bool isFile() const { return id.substr(0, 7) == "file://"; }

	// FUNCTION: WEBSERVICE 0x1004f1c0
	bool isAsset() const { return id.substr(0, 11) == "rbxasset://"; }

	// FUNCTION: WEBSERVICE 0x1004f230
	bool isHttp() const { return id.substr(0, 4) == "http"; }

private:
	std::string id;          // 0x00
	const Name* mimeTypePtr; // 0x1c
};

DECOMP_SIZE_ASSERT(ContentId, 0x20)

bool operator<(const ContentId& a, const ContentId& b);
bool operator!=(const ContentId& a, const ContentId& b);
bool operator==(const ContentId& a, const ContentId& b);

// SIZE 0x4
class __declspec(novtable) MD5Hasher
{
public:
	static MD5Hasher* create();

	virtual void addData(std::istream& data) = 0;                    // vtable+0x08
	virtual void addData(const std::string& data) = 0;               // vtable+0x04
	virtual void addData(const char* data, unsigned int nBytes) = 0; // vtable+0x00
	virtual std::string toString() = 0;                              // vtable+0x0c
	virtual const char* c_str() = 0;                                 // vtable+0x10

	static std::string convertToLegacyHash(std::string hash);
};

DECOMP_SIZE_ASSERT(MD5Hasher, 0x4)

// SIZE 0x54
class ContentProvider : boost::noncopyable
{
	boost::mutex requestSync;          // 0x00
	std::list<ContentId> requestQueue; // 0x08

	// SIZE 0x28
	struct FailedUrl
	{
		std::string url;                     // 0x00
		boost::posix_time::ptime expiration; // 0x20

		FailedUrl(const char* url);

		bool expired() const { return boost::posix_time::second_clock::local_time() >= expiration; }
	};

	std::list<FailedUrl> failedUrls; // 0x14
	boost::mutex contentCacheMutex;  // 0x20

	// SIZE 0x10
	struct CachedContent
	{
		boost::shared_ptr<const std::string> data;     // 0x00
		boost::shared_ptr<const std::string> filename; // 0x08
	};

	typedef std::map<ContentId, CachedContent> ContentCache;

	ContentCache contentCache;                         // 0x28
	std::string assetFolderPath;                       // 0x34
	boost::scoped_ptr<worker_thread> requestProcessor; // 0x50

	ContentProvider();
	~ContentProvider();

public:
	static ContentProvider& singleton()
	{
		static ContentProvider sing;
		return sing;
	}

	ContentId registerContent(const char* filename, const Name& mimeType);
	ContentId registerContent(std::istream& content, const Name& mimeType);

	void clearFileCache();

	bool isUrlBad(const char* url);

	void load(ContentId id, std::vector<boost::shared_ptr<Instance> >& instances);

	void clearContentCache();

	bool isRequestQueueEmpty();

	bool hasContent(ContentId id);

	boost::shared_ptr<const std::string> requestContentString(ContentId id);

	bool requestContentFile(ContentId id, std::string& filename);

	boost::shared_ptr<const std::string> getContentString(ContentId id);

	std::auto_ptr<std::istream> getContent(ContentId ticket);

	std::string getFile(ContentId id);

	std::string getAssetFile(const std::string& id);

	void setAssetFolder(const char* path);

	std::string assetFolder() const;

	static bool isUrl(const std::string& s);
	static bool isHttpUrl(const std::string& s);

	ContentId readContent(const char* url, std::istream& stream, unsigned long size);

	enum HttpRequestType
	{
		NoHttpRequest,
		AsyncHttpRequest,
		SyncHttpRequest
	};

private:
	CachedContent* loadContent(ContentId id, HttpRequestType requestType);
	worker_thread::work_result processRequests();
	std::string findFile(ContentId id);
	std::string findAsset(ContentId id);
	std::string findHashFile(ContentId id);
	bool registerFile(CachedContent* content);
};

DECOMP_SIZE_ASSERT(ContentProvider, 0x54)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x100547e0
// RBX::ContentProvider::CachedContent::~CachedContent
// SYNTHETIC: WEBSERVICE 0x10220f90
// `RBX::ContentProvider::singleton'::`2'::`dynamic atexit destructor for 'sing''
// SYNTHETIC: WEBSERVICE 0x102217f0
// `RBX::getLocalCachePath'::`7'::`dynamic atexit destructor for 's''
// SYNTHETIC: WEBSERVICE 0x10221800
// `RBX::getLocalCachePath'::`5'::`dynamic atexit destructor for 's''
// clang-format on

} // namespace RBX

#endif // UTIL_CONTENTPROVIDER_H
