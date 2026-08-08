#include "util/ContentProvider.h"

#include "decomp.h"

#include <istream>

namespace RBX {

// STUB: WEBSERVICE 0x10005a80
ContentProvider& ContentProvider::singleton()
{
	static ContentProvider sing;

	STUB(0x10005a80);

	return sing;
}

// STUB: WEBSERVICE 0x1004f1c0
bool ContentId::isAsset() const
{
	STUB(0x1004f1c0);
	return false;
}

// STUB: WEBSERVICE 0x1004f230
bool ContentId::isHttp() const
{
	STUB(0x1004f230);
	return false;
}

// STUB: WEBSERVICE 0x1004f310
bool operator<(const ContentId& a, const ContentId& b)
{
	STUB(0x1004f310);
	return false;
}

// FUNCTION: WEBSERVICE 0x1004f320
bool operator!=(const ContentId& a, const ContentId& b)
{
	return a.toString() != b.toString();
}

// FUNCTION: WEBSERVICE 0x1004f330
bool operator==(const ContentId& a, const ContentId& b)
{
	return a.toString() == b.toString();
}

// STUB: WEBSERVICE 0x100535f0
std::auto_ptr<std::istream> ContentProvider::getContent(ContentId ticket)
{
	STUB(0x100535f0);
	return std::auto_ptr<std::istream>();
}

} // namespace RBX
