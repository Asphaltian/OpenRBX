#ifndef WEBSERVICE_WEBSERVICEMAPS_H
#define WEBSERVICE_WEBSERVICEMAPS_H

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

// clang-format off
// winsock2.h before anything that pulls in windows.h.
#include <winsock2.h>
// clang-format on

#include <atlsoap.h>

namespace Roblox {

extern const _soapmap* ___Roblox_CWebService_funcs[];
extern const _soapmap* ___Roblox_CWebService_headers[];

} // namespace Roblox

#endif // WEBSERVICE_WEBSERVICEMAPS_H
