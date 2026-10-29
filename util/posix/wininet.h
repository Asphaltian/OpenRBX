#ifndef POSIX_WININET_H
#define POSIX_WININET_H

#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <windows.h>

typedef void* HINTERNET;
typedef unsigned short INTERNET_PORT;

#define INTERNET_OPEN_TYPE_PRECONFIG 0
#define INTERNET_SERVICE_HTTP 3
#define INTERNET_DEFAULT_HTTP_PORT 80
#define INTERNET_FLAG_SECURE 0x00800000
#define INTERNET_FLAG_NEED_FILE 0x00000010
#define INTERNET_FLAG_KEEP_CONNECTION 0x00400000
#define INTERNET_FLAG_EXISTING_CONNECT 0x20000000
#define HTTP_ADDREQ_FLAG_ADD 0x20000000
#define HTTP_QUERY_STATUS_CODE 19
#define HTTP_QUERY_CONTENT_ENCODING 29
#define HTTP_QUERY_FLAG_NUMBER 0x20000000

typedef struct _INTERNET_BUFFERS
{
	DWORD dwStructSize;
	struct _INTERNET_BUFFERS* Next;
	LPCSTR lpcszHeader;
	DWORD dwHeadersLength;
	DWORD dwHeadersTotal;
	LPVOID lpvBuffer;
	DWORD dwBufferLength;
	DWORD dwBufferTotal;
	DWORD dwOffsetLow;
	DWORD dwOffsetHigh;
} INTERNET_BUFFERS, *LPINTERNET_BUFFERS;

struct PosixInternet
{
	int level;
	std::string host;
	INTERNET_PORT port;
	std::string verb;
	std::string path;
	std::string headers;
	std::string body;
	std::string response;
	std::string encoding;
	DWORD status;
	size_t offset;
	bool sent;
};

inline HINTERNET InternetOpen(LPCSTR, DWORD, LPCSTR, LPCSTR, DWORD)
{
	PosixInternet* h = new PosixInternet();
	h->level = 0;
	h->status = 0;
	h->offset = 0;
	h->sent = false;
	return h;
}

inline HINTERNET InternetConnect(HINTERNET, LPCSTR host, INTERNET_PORT port, LPCSTR, LPCSTR, DWORD, DWORD, DWORD_PTR)
{
	PosixInternet* h = new PosixInternet();
	h->level = 1;
	h->host = host ? host : "";
	h->port = port ? port : INTERNET_DEFAULT_HTTP_PORT;
	h->status = 0;
	h->offset = 0;
	h->sent = false;
	return h;
}

inline HINTERNET HttpOpenRequest(
	HINTERNET connection, LPCSTR verb, LPCSTR path, LPCSTR, LPCSTR, LPCSTR*, DWORD flags, DWORD_PTR
)
{
	PosixInternet* c = (PosixInternet*) connection;
	if (!c || (flags & INTERNET_FLAG_SECURE)) {
		return NULL;
	}
	PosixInternet* h = new PosixInternet();
	h->level = 2;
	h->host = c->host;
	h->port = c->port;
	h->verb = verb ? verb : "GET";
	h->path = path && *path ? path : "/";
	h->status = 0;
	h->offset = 0;
	h->sent = false;
	return h;
}

inline BOOL HttpAddRequestHeaders(HINTERNET request, LPCSTR headers, DWORD length, DWORD)
{
	PosixInternet* h = (PosixInternet*) request;
	std::string s = length == (DWORD) -1 ? std::string(headers) : std::string(headers, length);
	if (!s.empty() && s[s.size() - 1] != '\n') {
		s += "\r\n";
	}
	h->headers += s;
	return TRUE;
}

inline bool posixHttpTransact(PosixInternet* h)
{
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	char port[16];
	snprintf(port, sizeof(port), "%u", (unsigned) h->port);
	struct addrinfo* result = NULL;
	if (getaddrinfo(h->host.c_str(), port, &hints, &result) != 0) {
		return false;
	}
	int fd = -1;
	for (struct addrinfo* ai = result; ai; ai = ai->ai_next) {
		fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
		if (fd < 0) {
			continue;
		}
		if (connect(fd, ai->ai_addr, ai->ai_addrlen) == 0) {
			break;
		}
		close(fd);
		fd = -1;
	}
	freeaddrinfo(result);
	if (fd < 0) {
		return false;
	}
	char length[32];
	snprintf(length, sizeof(length), "%lu", (unsigned long) h->body.size());
	std::string request = h->verb + " " + h->path + " HTTP/1.0\r\nHost: " + h->host + "\r\nContent-Length: " + length +
						  "\r\n" + h->headers + "\r\n" + h->body;
	size_t sent = 0;
	while (sent < request.size()) {
		ssize_t n = send(fd, request.data() + sent, request.size() - sent, 0);
		if (n <= 0) {
			close(fd);
			return false;
		}
		sent += (size_t) n;
	}
	std::string raw;
	char buffer[8192];
	for (;;) {
		ssize_t n = recv(fd, buffer, sizeof(buffer), 0);
		if (n <= 0) {
			break;
		}
		raw.append(buffer, (size_t) n);
	}
	close(fd);
	size_t split = raw.find("\r\n\r\n");
	if (split == std::string::npos) {
		return false;
	}
	std::string head = raw.substr(0, split);
	h->response = raw.substr(split + 4);
	size_t space = head.find(' ');
	h->status = space == std::string::npos ? 0 : (DWORD) atoi(head.c_str() + space + 1);
	std::string lower = head;
	for (size_t i = 0; i < lower.size(); ++i) {
		lower[i] = (char) tolower(lower[i]);
	}
	size_t enc = lower.find("\r\ncontent-encoding:");
	if (enc != std::string::npos) {
		size_t start = enc + 19;
		while (start < head.size() && head[start] == ' ') {
			++start;
		}
		size_t end = head.find("\r\n", start);
		h->encoding = head.substr(start, end == std::string::npos ? std::string::npos : end - start);
	}
	h->offset = 0;
	h->sent = true;
	return true;
}

inline BOOL HttpSendRequest(HINTERNET request, LPCSTR headers, DWORD headersLength, LPVOID body, DWORD bodyLength)
{
	PosixInternet* h = (PosixInternet*) request;
	if (headers && headersLength) {
		HttpAddRequestHeaders(request, headers, headersLength, 0);
	}
	if (body && bodyLength) {
		h->body.assign((const char*) body, bodyLength);
	}
	return posixHttpTransact(h);
}

inline BOOL HttpSendRequestEx(HINTERNET, LPINTERNET_BUFFERS, LPINTERNET_BUFFERS, DWORD, DWORD_PTR)
{
	return TRUE;
}

inline BOOL InternetWriteFile(HINTERNET request, LPCVOID data, DWORD length, LPDWORD written)
{
	PosixInternet* h = (PosixInternet*) request;
	h->body.append((const char*) data, length);
	if (written) {
		*written = length;
	}
	return TRUE;
}

inline BOOL HttpEndRequest(HINTERNET request, LPINTERNET_BUFFERS, DWORD, DWORD_PTR)
{
	return posixHttpTransact((PosixInternet*) request);
}

inline BOOL HttpQueryInfo(HINTERNET request, DWORD level, LPVOID buffer, LPDWORD length, LPDWORD)
{
	PosixInternet* h = (PosixInternet*) request;
	std::string value;
	DWORD query = level & 0xFFFF;
	if (query == HTTP_QUERY_STATUS_CODE) {
		if (level & HTTP_QUERY_FLAG_NUMBER) {
			*(DWORD*) buffer = h->status;
			*length = sizeof(DWORD);
			return TRUE;
		}
		char s[16];
		snprintf(s, sizeof(s), "%u", (unsigned) h->status);
		value = s;
	}
	else if (query == HTTP_QUERY_CONTENT_ENCODING) {
		if (h->encoding.empty()) {
			return FALSE;
		}
		value = h->encoding;
	}
	else {
		return FALSE;
	}
	if (*length < value.size() + 1) {
		*length = (DWORD) value.size() + 1;
		return FALSE;
	}
	memcpy(buffer, value.c_str(), value.size() + 1);
	*length = (DWORD) value.size();
	return TRUE;
}

inline BOOL InternetQueryDataAvailable(HINTERNET request, LPDWORD available, DWORD, DWORD_PTR)
{
	PosixInternet* h = (PosixInternet*) request;
	*available = (DWORD) (h->response.size() - h->offset);
	return TRUE;
}

inline BOOL InternetReadFile(HINTERNET request, LPVOID buffer, DWORD length, LPDWORD read_)
{
	PosixInternet* h = (PosixInternet*) request;
	size_t remaining = h->response.size() - h->offset;
	size_t n = length < remaining ? length : remaining;
	memcpy(buffer, h->response.data() + h->offset, n);
	h->offset += n;
	*read_ = (DWORD) n;
	return TRUE;
}

inline BOOL InternetCloseHandle(HINTERNET handle)
{
	delete (PosixInternet*) handle;
	return TRUE;
}

#endif // POSIX_WININET_H
