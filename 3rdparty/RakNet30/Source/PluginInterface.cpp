/// \file
///
/// This file is part of RakNet Copyright 2003 Kevin Jenkins.
///
/// Usage of RakNet is subject to the appropriate license agreement.
/// Creative Commons Licensees are subject to the
/// license found at
/// http://creativecommons.org/licenses/by-nc/2.5/
/// Single application licensees are subject to the license found at
/// http://www.rakkarsoft.com/SingleApplicationLicense.html
/// Custom license users are subject to the terms therein.
/// GPL license users are subject to the GNU General Public
/// License as published by the Free
/// Software Foundation; either version 2 of the License, or (at your
/// option) any later version.

#include "PluginInterface.h"

// FUNCTION: WEBSERVICE 0x101aaa50
PluginInterface::PluginInterface()
{
}

// FUNCTION: WEBSERVICE 0x101aaa60
PluginInterface::~PluginInterface()
{
}

#ifdef _MSC_VER
#pragma warning( push )
#endif

#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
// FUNCTION: WEBSERVICE 0x100d15c0 FOLDED
void PluginInterface::OnAttach(RakPeerInterface *peer)
{
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
// FUNCTION: WEBSERVICE 0x100d15c0 FOLDED
void PluginInterface::OnDetach(RakPeerInterface *peer)
{
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
// FUNCTION: WEBSERVICE 0x100d15c0 FOLDED
void PluginInterface::OnStartup(RakPeerInterface *peer)
{
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
// FUNCTION: WEBSERVICE 0x100d15c0 FOLDED
void PluginInterface::Update(RakPeerInterface *peer)
{
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
// FUNCTION: WEBSERVICE 0x101aaa70
PluginReceiveResult PluginInterface::OnReceive(RakPeerInterface *peer, Packet *packet)
{
	return RR_CONTINUE_PROCESSING;
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
// FUNCTION: WEBSERVICE 0x100d15c0 FOLDED
void PluginInterface::OnShutdown(RakPeerInterface *peer)
{
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
// FUNCTION: WEBSERVICE 0x101aaa80
void PluginInterface::OnCloseConnection(RakPeerInterface *peer, SystemAddress systemAddress)
{
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
// FUNCTION: WEBSERVICE 0x101aaa90 FOLDED
void PluginInterface::OnDirectSocketSend(const char *data, const unsigned bitsUsed, SystemAddress remoteSystemAddress)
{
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
// FUNCTION: WEBSERVICE 0x101aaa90 FOLDED
void PluginInterface::OnDirectSocketReceive(const char *data, const unsigned bitsUsed, SystemAddress remoteSystemAddress)
{
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
// FUNCTION: WEBSERVICE 0x101aaaa0
void PluginInterface::OnInternalPacket(InternalPacket *internalPacket, unsigned frameNumber, SystemAddress remoteSystemAddress, RakNetTime time, bool isSend)
{
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif
