#ifndef UTIL_CONTENTPROVIDER_H
#define UTIL_CONTENTPROVIDER_H

#include "decomp.h"
#include "util/Name.h"

#include <boost/noncopyable.hpp>
#include <iosfwd>
#include <memory>
#include <string>

namespace RBX {

// SIZE 0x20
class ContentId
{
public:
	ContentId() : mimeTypePtr(&Name::getNullName()) {}

	ContentId(const std::string& id) : id(id), mimeTypePtr(&Name::getNullName()) {}

	static ContentId fromAssets(const std::string& id);

	const Name& mimeType() const { return *mimeTypePtr; }

	const char* c_str() const { return id.c_str(); }

	const std::string& toString() const { return id; }

	bool isNull() const { return id.empty(); }

	bool isAsset() const;
	bool isHttp() const;

private:
	std::string id;          // 0x00
	const Name* mimeTypePtr; // 0x1c
};

DECOMP_SIZE_ASSERT(ContentId, 0x20)

bool operator<(const ContentId& a, const ContentId& b);
bool operator!=(const ContentId& a, const ContentId& b);
bool operator==(const ContentId& a, const ContentId& b);

// SIZE 0x54
class ContentProvider : public boost::noncopyable
{
public:
	static ContentProvider& singleton();

	std::auto_ptr<std::istream> getContent(ContentId ticket);

private:
	~ContentProvider();

	undefined m_unk0x00[0x54]; // 0x00
};

DECOMP_SIZE_ASSERT(ContentProvider, 0x54)

} // namespace RBX

#endif // UTIL_CONTENTPROVIDER_H
