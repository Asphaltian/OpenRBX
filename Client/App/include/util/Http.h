#ifndef UTIL_HTTP_H
#define UTIL_HTTP_H

#include "placeholder.h"

#include <istream>
#include <string>

namespace RBX {

class Http
{
public:
	const std::string url;

	Http(const std::string& url) : url(url) {}

	Http(const char* url) : url(url) {}

	std::string additionalHeaders;

	void post(std::istream& input, bool compress, std::string& response);
	void get(std::string& response);

	static bool trustCheck(const char* url);
	static bool isScript(const char* url);
	static bool isRobloxSite(const char* url);
	static bool isCached(const char* url);
};

} // namespace RBX

#endif // UTIL_HTTP_H
