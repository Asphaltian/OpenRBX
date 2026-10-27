#ifndef UTIL_HTTP_H
#define UTIL_HTTP_H

#include "decomp.h"

#include <istream>
#include <string>

namespace RBX {

// SIZE 0x38
class Http
{
public:
	const std::string url; // 0x00

	// FUNCTION: WEBSERVICE 0x10001070
	Http(const std::string& url) : url(url) {}
	Http(const char* url) : url(url) {}

	std::string additionalHeaders; // 0x1c

	void post(std::istream& input, bool compress, std::string& response);
	void get(std::string& response);

	static bool trustCheck(const char* url);
	static bool isScript(const char* url);
	static bool isRobloxSite(const char* url);
	static bool isCached(const char* url);

	// SYNTHETIC: WEBSERVICE 0x1000d910
	// RBX::Http::~Http
};

DECOMP_SIZE_ASSERT(Http, 0x38)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10218160
// RBX::`dynamic initializer for 'trustCheckMutex''
// SYNTHETIC: WEBSERVICE 0x10221420
// RBX::`dynamic atexit destructor for 'trustCheckMutex''
// SYNTHETIC: WEBSERVICE 0x10221430
// `RBX::Http::trustCheck'::`3'::`dynamic atexit destructor for 'skipTrustCheck''
// clang-format on

} // namespace RBX

#endif // UTIL_HTTP_H
