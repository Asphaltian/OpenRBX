#ifndef NETWORK_NETWORKSETTINGS_H
#define NETWORK_NETWORKSETTINGS_H

#include "decomp.h"
#include "v8datamodel/GlobalSettings.h"

namespace RBX {

extern const char sNetworkSettings[];

// SIZE 0x124
class NetworkSettings : public GlobalSettingsItem<NetworkSettings, sNetworkSettings>
{
public:
	// SYNTHETIC: WEBSERVICE 0x1019b5a0
	// RBX::NetworkSettings::`scalar deleting destructor'

	NetworkSettings();

	bool clientPhysics;          // 0xf8
	float clientPhysicsSpeed;    // 0xfc
	float clientPhysicsLifetime; // 0x100
	bool printPhysicsErrors;     // 0x104
	bool printInstances;         // 0x105
	bool printProperties;        // 0x106
	bool printPacketBuffer;      // 0x107
	bool logPackets;             // 0x108
	int maxDataModelSendBuffer;  // 0x10c
	float dataPacketSize;        // 0x110
	float sendRate;              // 0x114
	int maxSendBPS;              // 0x118
	int minExtraPing;            // 0x11c
	int extraPingVariance;       // 0x120
};

DECOMP_SIZE_ASSERT(NetworkSettings, 0x124)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021f910
// RBX::`dynamic initializer for 'prop_LogPackets''
// SYNTHETIC: WEBSERVICE 0x1021f940
// RBX::`dynamic initializer for 'prop_PrintInstances''
// SYNTHETIC: WEBSERVICE 0x1021f970
// RBX::`dynamic initializer for 'prop_PrintProperties''
// SYNTHETIC: WEBSERVICE 0x1021f9a0
// RBX::`dynamic initializer for 'prop_PrintPacketBuffer''
// SYNTHETIC: WEBSERVICE 0x1021f9d0
// RBX::`dynamic initializer for 'prop_PrintPhysicsErrors''
// SYNTHETIC: WEBSERVICE 0x1021fa00
// RBX::`dynamic initializer for 'prop_MaxSendBPS''
// SYNTHETIC: WEBSERVICE 0x1021fa30
// RBX::`dynamic initializer for 'prop_MinExtraPing''
// SYNTHETIC: WEBSERVICE 0x1021fa60
// RBX::`dynamic initializer for 'prop_ExtraPingVariance''
// SYNTHETIC: WEBSERVICE 0x1021fa90
// STUB: ROBLOX 0x007ed620
// RBX::`dynamic initializer for 'prop_MaxDataModelSendBuffer''
// SYNTHETIC: WEBSERVICE 0x1021fac0
// RBX::`dynamic initializer for 'prop_DataPacketSize''
// SYNTHETIC: WEBSERVICE 0x1021faf0
// RBX::`dynamic initializer for 'prop_SendRate''
// SYNTHETIC: WEBSERVICE 0x10226140
// RBX::`dynamic atexit destructor for 'prop_PrintProperties''
// SYNTHETIC: WEBSERVICE 0x10226160
// RBX::`dynamic atexit destructor for 'prop_PrintPacketBuffer''
// SYNTHETIC: WEBSERVICE 0x10226180
// RBX::`dynamic atexit destructor for 'prop_PrintPhysicsErrors''
// SYNTHETIC: WEBSERVICE 0x102261a0
// RBX::`dynamic atexit destructor for 'prop_ExtraPingVariance''
// SYNTHETIC: WEBSERVICE 0x102261c0
// SYNTHETIC: ROBLOX 0x007f4ab0
// RBX::`dynamic atexit destructor for 'prop_MaxDataModelSendBuffer''
// SYNTHETIC: WEBSERVICE 0x102261e0
// RBX::`dynamic atexit destructor for 'prop_PrintInstances''
// SYNTHETIC: WEBSERVICE 0x10226200
// RBX::`dynamic atexit destructor for 'prop_LogPackets''
// SYNTHETIC: WEBSERVICE 0x10226220
// RBX::`dynamic atexit destructor for 'prop_MinExtraPing''
// SYNTHETIC: WEBSERVICE 0x10226240
// RBX::`dynamic atexit destructor for 'prop_MaxSendBPS''
// SYNTHETIC: WEBSERVICE 0x10226260
// RBX::`dynamic atexit destructor for 'prop_SendRate''
// SYNTHETIC: WEBSERVICE 0x10226280
// RBX::`dynamic atexit destructor for 'prop_DataPacketSize''
// clang-format on

} // namespace RBX

#endif // NETWORK_NETWORKSETTINGS_H
