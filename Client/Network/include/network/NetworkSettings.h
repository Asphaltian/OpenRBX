#ifndef NETWORK_NETWORKSETTINGS_H
#define NETWORK_NETWORKSETTINGS_H

#include "placeholder.h"
#include "v8datamodel/GlobalSettings.h"

namespace RBX {

extern const char sNetworkSettings[];

class NetworkSettings : public GlobalSettingsItem<NetworkSettings, sNetworkSettings>
{
public:
	NetworkSettings();

	bool clientPhysics;
	float clientPhysicsSpeed;
	float clientPhysicsLifetime;
	bool printPhysicsErrors;
	bool printInstances;
	bool printProperties;
	bool printPacketBuffer;
	bool logPackets;
	int maxDataModelSendBuffer;
	float dataPacketSize;
	float sendRate;
	int maxSendBPS;
	int minExtraPing;
	int extraPingVariance;
};

} // namespace RBX

#endif // NETWORK_NETWORKSETTINGS_H
