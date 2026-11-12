#include "Unknown00409ef0.h"

#include "util/Http.h"

#include <exception>

// FUNCTION: ROBLOX 0x00409ef0
Unknown00409ef0::Unknown00409ef0(const char* host) : m_unk0x00(host)
{
}

Unknown00409ef0::~Unknown00409ef0()
{
}

// FUNCTION: ROBLOX 0x00409f10
static bool FUN_00409f10()
{
	OSVERSIONINFO info = {0};
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&info);
	return info.dwMajorVersion >= 6 ? true : false;
}

// FUNCTION: ROBLOX 0x00409f70
std::string Unknown00409ef0::FUN_00409f70(const char* url, const char* ticket)
{
	try {
		static bool enabled = FUN_00409f10();
		if (!enabled) {
			return "";
		}

		std::string response;
		{
			std::string request(url);
			if (ticket) {
				request += "?suggest=";
				request += ticket;
			}

			RBX::Http http(request.c_str());
			http.additionalHeaders = "RBXAuthenticationNegotiation:";
			http.additionalHeaders += m_unk0x00;
			http.additionalHeaders += "\r\n";
			http.get(response);
		}
		return response;
	}
	catch (std::exception&) {
		return "";
	}
}
