#include "util/Http.h"

#include "compat.h"
#include "util/ContentProvider.h"
#include "util/StandardOut.h"

#include <G3D/format.h>
#include <atlbase.h>
#include <atlstr.h>
#include <atlutil.h>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <sstream>
#include <stdexcept>
#include <wininet.h>

// SIZE 0x4
class CHINTERNET : boost::noncopyable
{
	HINTERNET handle; // 0x00

public:
	CHINTERNET(HINTERNET handle) : handle(handle) {}
	CHINTERNET() : handle(NULL) {}

	CHINTERNET& operator=(HINTERNET handle)
	{
		this->handle = handle;
		return *this;
	}

	operator bool() { return handle != NULL; }
	operator void*() { return handle; }

	// FUNCTION: WEBSERVICE 0x10034b30
	~CHINTERNET() { InternetCloseHandle(handle); }
};

// SIZE 0x4
class HttpRequest_source : public boost::iostreams::device<boost::iostreams::input>
{
	HINTERNET request; // 0x00

public:
	HttpRequest_source(HINTERNET request) : request(request) {}

	int read(char* s, int n);
};

// SIZE 0x4
class String_sink : public boost::iostreams::device<boost::iostreams::output>
{
public:
	std::string& s; // 0x00

	String_sink(std::string& s) : s(s) {}

	int write(const char* s, int n)
	{
		this->s.append(s, n);
		return n;
	}
};

namespace RBX {

// FUNCTION: WEBSERVICE 0x10034b40
// FUNCTION: ROBLOX 0x00583210
void ThrowIfFailure(bool success, const char* message)
{
	if (!success) {
		DWORD error = GetLastError();

		char buffer[256];
		if (!FormatMessage(
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				error,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
				buffer,
				256,
				NULL
			)) {
			throw std::runtime_error(G3D::format("%s, err=0x%X", message, error));
		}

		throw std::runtime_error(G3D::format("%s, %s", message, buffer));
	}
}

} // namespace RBX

// FUNCTION: WEBSERVICE 0x10035950
// FUNCTION: ROBLOX 0x00583e70
int HttpRequest_source::read(char* s, int n)
{
	DWORD numBytes;
	if (!InternetQueryDataAvailable(request, &numBytes, 0, 0)) {
		numBytes = 0;
	}
	if (numBytes == 0) {
		return -1;
	}

	DWORD bytesRead;
	if (!InternetReadFile(request, s, n, &bytesRead)) {
		throw std::runtime_error("InternetReadFile failed");
	}
	return bytesRead;
}

namespace RBX {

// FUNCTION: WEBSERVICE 0x1003a9e0
// FUNCTION: ROBLOX 0x00588a50
bool Http::isScript(const char* url)
{
	CUrl crack;
	crack.CrackUrl(url);

	switch (crack.GetScheme()) {
	case ATL_URL_SCHEME_FTP:
	case ATL_URL_SCHEME_HTTP:
	case ATL_URL_SCHEME_HTTPS:
		return false;
	}

	CString sUrl(url);
	if (sUrl.Find("javascript:") == 0) {
		return true;
	}
	if (sUrl.Find("jscript:") == 0) {
		return true;
	}
	return false;
}

// FUNCTION: WEBSERVICE 0x1003ab10
// FUNCTION: ROBLOX 0x00588b80
bool Http::isRobloxSite(const char* url)
{
	CUrl crack;
	crack.CrackUrl(url);

	switch (crack.GetScheme()) {
	case ATL_URL_SCHEME_FTP:
	case ATL_URL_SCHEME_HTTP:
	case ATL_URL_SCHEME_HTTPS: {
		CString hostName = crack.GetHostName();
		hostName.MakeLower();
		if (hostName.Right(10) == "roblox.com") {
			return true;
		}
		return false;
	}

	default:
		return false;
	}
}

static boost::mutex trustCheckMutex;

// FUNCTION: WEBSERVICE 0x1003ac90
// FUNCTION: ROBLOX 0x00588d00
bool Http::trustCheck(const char* url)
{
	{
		boost::mutex::scoped_lock lock(trustCheckMutex);
		static boost::scoped_ptr<bool> skipTrustCheck;
		if (!skipTrustCheck) {
			DWORD value = 0;
			CRegKey key;

			if (key.Open(HKEY_CURRENT_USER, "Software\\ROBLOX Corporation\\Roblox", KEY_READ) == ERROR_SUCCESS) {
				key.QueryDWORDValue("SkipTrustCheck", value);
			}

			if (key.Open(HKEY_LOCAL_MACHINE, "Software\\Roblox", KEY_READ) == ERROR_SUCCESS) {
				key.QueryDWORDValue("SkipTrustCheck", value);
			}

			skipTrustCheck.reset(new bool(value == 1));
		}

		if (*skipTrustCheck) {
			return true;
		}
	}

	if (CString("about:blank") == url) {
		return true;
	}

	if (isRobloxSite(url)) {
		return true;
	}

	if (isScript(url)) {
		return true;
	}

	CUrl crack;
	crack.CrackUrl(url);

	switch (crack.GetScheme()) {
	case ATL_URL_SCHEME_FTP:
	case ATL_URL_SCHEME_HTTP:
	case ATL_URL_SCHEME_HTTPS: {
		CString hostName = crack.GetHostName();
		hostName.MakeLower();
		if (hostName.Right(21) == "googlesyndication.com") {
			return true;
		}
	} break;

	default: {
		CString sUrl(url);
		if (sUrl.Find("res:") == 0) {
			return true;
		}
	}
	}

	return false;
}

// STUB: WEBSERVICE 0x1003c960
// STUB: ROBLOX 0x0058a9d0
static void httpGetPost(
	bool isPost,
	const std::string& url,
	std::istream& data,
	bool compressData,
	const char* additionalHeaders,
	std::string& response
)
{
	if (url.empty()) {
		throw std::runtime_error("empty url");
	}

	if (!Http::trustCheck(url.c_str())) {
		throw std::runtime_error(G3D::format("trust check failed for %s", url.c_str()));
	}

	CUrl u;
	u.CrackUrl(url.c_str());

	if (u.GetHostNameLength() == 0) {
		throw std::runtime_error(G3D::format("'%s' is missing a hostName", url.c_str()));
	}

	CHINTERNET session(InternetOpen("Roblox", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0));
	CHINTERNET connection(InternetConnect(
		session, u.GetHostName(), u.GetPortNumber(), u.GetUserName(), u.GetPassword(), INTERNET_SERVICE_HTTP, 0, 1
	));

	CString s = u.GetUrlPath();
	s += u.GetExtraInfo();

	CHINTERNET request(HttpOpenRequest(
		connection,
		isPost ? "POST" : "GET",
		s,
		NULL,
		NULL,
		NULL,
		INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NEED_FILE,
		1
	));
	if (!request) {
		throw std::runtime_error(G3D::format("HttpOpenRequest failed for %s", url.c_str()));
	}

	if (additionalHeaders) {
		if (!HttpAddRequestHeaders(request, additionalHeaders, strlen(additionalHeaders), HTTP_ADDREQ_FLAG_ADD)) {
			ThrowIfFailure(false, "HttpAddRequestHeaders failed");
		}
	}

	const char* acceptEncoding = "Accept-Encoding: gzip\r\n";
	if (!HttpAddRequestHeaders(request, acceptEncoding, strlen(acceptEncoding), HTTP_ADDREQ_FLAG_ADD)) {
		ThrowIfFailure(false, "HttpAddRequestHeaders failed");
	}

	boost::iostreams::gzip_params gzipParams;

	if (!isPost) {
		if (!HttpSendRequest(request, NULL, 0, NULL, 0)) {
			ThrowIfFailure(false, "HttpSendRequest failed");
		}
	}
	else {
		std::string uploadData;

		try {
			boost::iostreams::filtering_ostream out;
			if (compressData) {
				std::string contentEncodingHeader = "Content-Encoding: gzip\r\n";
				if (!HttpAddRequestHeaders(
						request, contentEncodingHeader.c_str(), contentEncodingHeader.size(), HTTP_ADDREQ_FLAG_ADD
					)) {
					ThrowIfFailure(false, "HttpAddRequestHeaders failed");
				}
				out.push(boost::iostreams::gzip_compressor(gzipParams));
			}
			out.push(String_sink(uploadData));
			boost::iostreams::copy(data, out);
		}
		catch (boost::iostreams::gzip_error& e) {
			throw std::runtime_error(
				G3D::format("Upload GZip error %d, ZLib error %d, '%s'", e.error(), e.zlib_error_code(), url.c_str())
			);
		}

		const size_t uploadSize = uploadData.size();

		if (uploadSize > 10000) {
			std::string contentSizeHeader = G3D::format("Roblox-Content-Size: %d\r\n", uploadSize);
			if (!HttpAddRequestHeaders(
					request, contentSizeHeader.c_str(), contentSizeHeader.size(), HTTP_ADDREQ_FLAG_ADD
				)) {
				ThrowIfFailure(false, "HttpAddRequestHeaders failed");
			}

			std::string contentFirstHeader = G3D::format("Roblox-Content-First: %d\r\n", uploadData[0]);
			if (!HttpAddRequestHeaders(
					request, contentFirstHeader.c_str(), contentFirstHeader.size(), HTTP_ADDREQ_FLAG_ADD
				)) {
				ThrowIfFailure(false, "HttpAddRequestHeaders failed");
			}

			std::string contentLastHeader = G3D::format("Roblox-Content-Last: %d\r\n", uploadData[uploadSize - 1]);
			if (!HttpAddRequestHeaders(
					request, contentLastHeader.c_str(), contentLastHeader.size(), HTTP_ADDREQ_FLAG_ADD
				)) {
				ThrowIfFailure(false, "HttpAddRequestHeaders failed");
			}

			boost::scoped_ptr<MD5Hasher> hasher(MD5Hasher::create());
			hasher->addData(uploadData);
			std::string contentHashHeader = G3D::format("Roblox-Content-Hash: %s\r\n", hasher->c_str());
			if (!HttpAddRequestHeaders(
					request, contentHashHeader.c_str(), contentHashHeader.size(), HTTP_ADDREQ_FLAG_ADD
				)) {
				ThrowIfFailure(false, "HttpAddRequestHeaders failed");
			}
		}

		INTERNET_BUFFERS buffer = {0};
		buffer.dwStructSize = sizeof(INTERNET_BUFFERS);
		buffer.dwBufferTotal = uploadSize;

		if (!HttpSendRequestEx(request, &buffer, NULL, 0, 0)) {
			throw std::runtime_error("HttpSendRequestEx failed");
		}

		try {
			DWORD bytesWritten;
			if (!InternetWriteFile(request, uploadData.c_str(), uploadSize, &bytesWritten)) {
				ThrowIfFailure(false, "InternetWriteFile failed");
			}

			if (bytesWritten != uploadSize) {
				throw std::runtime_error("Failed to upload content");
			}
		}
		catch (...) {
			HttpEndRequest(request, NULL, 0, 0);
			throw;
		}

		if (!HttpEndRequest(request, NULL, 0, 0)) {
			ThrowIfFailure(false, "HttpEndRequest failed");
		}
	}

	{
		DWORD statusCode;
		{
			char szBuffer[80];
			DWORD dwLen = 80;
			if (!HttpQueryInfo(request, HTTP_QUERY_STATUS_CODE, szBuffer, &dwLen, NULL)) {
				ThrowIfFailure(false, "HttpQueryInfo failed");
			}

			statusCode = atol(szBuffer);
		}

		try {
			boost::iostreams::filtering_istream in;

			{
				char buffer[256];
				DWORD length = 256;
				if (HttpQueryInfo(request, HTTP_QUERY_CONTENT_ENCODING, buffer, &length, NULL)) {
					if (std::string(buffer) == "gzip") {
						in.push(boost::iostreams::gzip_decompressor());
					}
				}
			}

			in.push(HttpRequest_source(request));
			boost::iostreams::copy(in, String_sink(response));
		}
		catch (boost::iostreams::gzip_error& e) {
			throw std::runtime_error(
				G3D::format("GZip error %d, ZLib error %d, '%s'", e.error(), e.zlib_error_code(), url.c_str())
			);
		}
		catch (boost::iostreams::zlib_error& e) {
			throw std::runtime_error(G3D::format("ZLib error %d, '%s'", e.error(), url.c_str()));
		}

		if (statusCode != 200) {
			throw std::runtime_error(G3D::format("statusCode = %d", statusCode));
		}
	}
}

// FUNCTION: WEBSERVICE 0x1003d2c0
// FUNCTION: ROBLOX 0x0058b330
void Http::post(std::istream& input, bool compress, std::string& response)
{
	if (additionalHeaders.empty()) {
		httpGetPost(true, url, input, compress, NULL, response);
	}
	else {
		httpGetPost(true, url, input, compress, additionalHeaders.c_str(), response);
	}
}

// FUNCTION: WEBSERVICE 0x1003d320
// FUNCTION: ROBLOX 0x0058b390
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
