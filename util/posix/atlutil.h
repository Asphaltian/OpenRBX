#ifndef POSIX_ATLUTIL_H
#define POSIX_ATLUTIL_H

#include <atlstr.h>
#include <string>
#include <windows.h>

enum ATL_URL_SCHEME
{
	ATL_URL_SCHEME_UNKNOWN = -1,
	ATL_URL_SCHEME_FTP = 0,
	ATL_URL_SCHEME_GOPHER = 1,
	ATL_URL_SCHEME_HTTP = 2,
	ATL_URL_SCHEME_HTTPS = 3,
	ATL_URL_SCHEME_FILE = 4,
};

class CUrl
{
public:
	CUrl() : scheme(ATL_URL_SCHEME_UNKNOWN), port(0) {}

	BOOL CrackUrl(const char* url, DWORD = 0)
	{
		std::string s = url ? url : "";
		size_t colon = s.find("://");
		if (colon == std::string::npos) {
			return FALSE;
		}
		std::string schemeName = s.substr(0, colon);
		for (size_t i = 0; i < schemeName.size(); ++i) {
			schemeName[i] = (char) tolower(schemeName[i]);
		}
		scheme = schemeName == "http"    ? ATL_URL_SCHEME_HTTP
				 : schemeName == "https" ? ATL_URL_SCHEME_HTTPS
				 : schemeName == "ftp"   ? ATL_URL_SCHEME_FTP
				 : schemeName == "file"  ? ATL_URL_SCHEME_FILE
										 : ATL_URL_SCHEME_UNKNOWN;
		std::string rest = s.substr(colon + 3);
		size_t slash = rest.find('/');
		std::string authority = slash == std::string::npos ? rest : rest.substr(0, slash);
		path = slash == std::string::npos ? "/" : rest.substr(slash);
		size_t extra = path.find_first_of("?#");
		if (extra != std::string::npos) {
			extraInfo = path.substr(extra);
			path = path.substr(0, extra);
		}
		else {
			extraInfo = "";
		}
		size_t at = authority.find('@');
		if (at != std::string::npos) {
			std::string user = authority.substr(0, at);
			authority = authority.substr(at + 1);
			size_t sep = user.find(':');
			userName = user.substr(0, sep);
			password = sep == std::string::npos ? "" : user.substr(sep + 1);
		}
		size_t portSep = authority.find(':');
		hostName = authority.substr(0, portSep);
		port = portSep != std::string::npos     ? atoi(authority.c_str() + portSep + 1)
			   : scheme == ATL_URL_SCHEME_HTTPS ? 443
												: 80;
		return TRUE;
	}

	ATL_URL_SCHEME GetScheme() const { return scheme; }
	const char* GetHostName() const { return hostName.c_str(); }
	DWORD GetHostNameLength() const { return (DWORD) hostName.size(); }
	const char* GetUrlPath() const { return path.c_str(); }
	const char* GetExtraInfo() const { return extraInfo.c_str(); }
	const char* GetUserName() const { return userName.c_str(); }
	const char* GetPassword() const { return password.c_str(); }
	unsigned short GetPortNumber() const { return (unsigned short) port; }

private:
	ATL_URL_SCHEME scheme;
	std::string hostName;
	std::string path;
	std::string extraInfo;
	std::string userName;
	std::string password;
	int port;
};

#endif // POSIX_ATLUTIL_H
