#ifndef UTIL_CONTENTPROVIDER_H
#define UTIL_CONTENTPROVIDER_H

#include "placeholder.h"
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

class ContentId
{
public:
	static ContentId fromUrl(const std::string&);
	static ContentId fromFile(const std::string&);
	static ContentId fromAssets(const std::string& id);
	static ContentId fromMD5Hash(const std::string&);

	ContentId(const std::string& id, const Name& mimeType) : id(id), mimeTypePtr(&mimeType) {}

	ContentId(const char* id) : id(id), mimeTypePtr(&Name::getNullName()) {}

	ContentId(const std::string& id) : id(id), mimeTypePtr(&Name::getNullName()) {}

	ContentId() : mimeTypePtr(&Name::getNullName()) {}

	const Name& mimeType() const { return *mimeTypePtr; }

	const char* c_str() const { return id.c_str(); }

	const std::string& toString() const { return id; }

	bool isNull() const { return id.size() == 0; }

	bool isFile() const { return id.substr(0, 7) == "file://"; }

	bool isAsset() const { return id.substr(0, 11) == "rbxasset://"; }

	bool isHttp() const { return id.substr(0, 4) == "http"; }

private:
	std::string id;
	const Name* mimeTypePtr;
};

bool operator<(const ContentId& a, const ContentId& b);
bool operator!=(const ContentId& a, const ContentId& b);
bool operator==(const ContentId& a, const ContentId& b);

class __declspec(novtable) MD5Hasher
{
public:
	static MD5Hasher* create();

	virtual void addData(std::istream& data) = 0;
	virtual void addData(const std::string& data) = 0;
	virtual void addData(const char* data, unsigned int nBytes) = 0;
	virtual std::string toString() = 0;
	virtual const char* c_str() = 0;

	static std::string convertToLegacyHash(std::string hash);
};

class ContentProvider : boost::noncopyable
{
	boost::mutex requestSync;
	std::list<ContentId> requestQueue;

	struct FailedUrl
	{
		std::string url;
		boost::posix_time::ptime expiration;

		FailedUrl(const char* url);

		bool expired() const { return boost::posix_time::second_clock::local_time() >= expiration; }
	};

	std::list<FailedUrl> failedUrls;
	boost::mutex contentCacheMutex;

	struct CachedContent
	{
		boost::shared_ptr<const std::string> data;
		boost::shared_ptr<const std::string> filename;
	};

	typedef std::map<ContentId, CachedContent> ContentCache;

	ContentCache contentCache;
	std::string assetFolderPath;
	boost::scoped_ptr<worker_thread> requestProcessor;

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

} // namespace RBX

#endif // UTIL_CONTENTPROVIDER_H
