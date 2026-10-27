#include "util/Http.h"

#include "util/StandardOut.h"

#include <sstream>

namespace RBX {

// STUB: WEBSERVICE 0x1003c960
static void httpGetPost(
	bool isPost,
	const std::string& url,
	std::istream& data,
	bool compressData,
	const char* additionalHeaders,
	std::string& response
)
{
	STUB(0x1003c960);
}

// STUB: WEBSERVICE 0x1003d2c0
void Http::post(std::istream& input, bool compress, std::string& response)
{
	if (additionalHeaders.empty()) {
		httpGetPost(true, url, input, compress, NULL, response);
	}
	else {
		httpGetPost(true, url, input, compress, additionalHeaders.c_str(), response);
	}
}

// STUB: WEBSERVICE 0x1003d320
void Http::get(std::string& response)
{
	const char* h = NULL;
	if (!additionalHeaders.empty()) {
		h = additionalHeaders.c_str();
	}

	std::istringstream dummy;
	try {
		httpGetPost(false, url, dummy, false, h, response);
	}
	catch (...) {
		StandardOut::singleton()->print(MESSAGE_WARNING, "httpGet failed, trying again");
		httpGetPost(false, url, dummy, false, h, response);
	}
}

} // namespace RBX
