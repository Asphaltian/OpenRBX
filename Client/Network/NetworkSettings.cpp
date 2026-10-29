#include "network/NetworkSettings.h"

#include "reflection/property.h"

namespace RBX {

const char sNetworkSettings[] = "NetworkSettings";

using namespace Reflection;

BoundProp<bool, 1> prop_LogPackets("LogPackets", "Diagnostics", &NetworkSettings::logPackets);
BoundProp<bool, 1> prop_PrintInstances("PrintInstances", "Diagnostics", &NetworkSettings::printInstances);
BoundProp<bool, 1> prop_PrintProperties("PrintProperties", "Diagnostics", &NetworkSettings::printProperties);
BoundProp<bool, 1> prop_PrintPacketBuffer("PrintPacketBuffer", "Diagnostics", &NetworkSettings::printPacketBuffer);
BoundProp<bool, 1> prop_PrintPhysicsErrors("PrintPhysicsErrors", "Diagnostics", &NetworkSettings::printPhysicsErrors);

BoundProp<int, 1> prop_MaxSendBPS("MaxSendBPS", "Simulator", &NetworkSettings::maxSendBPS);
BoundProp<int, 1> prop_MinExtraPing("MinExtraPing", "Simulator", &NetworkSettings::minExtraPing);
BoundProp<int, 1> prop_ExtraPingVariance("ExtraPingVariance", "Simulator", &NetworkSettings::extraPingVariance);

BoundProp<int, 1> prop_MaxDataModelSendBuffer(
	"MaxDataModelSendBuffer", "Replication", &NetworkSettings::maxDataModelSendBuffer
);

BoundProp<float, 1> prop_DataPacketSize("DataPacketSize", "Replication", &NetworkSettings::dataPacketSize);
BoundProp<float, 1> prop_SendRate("SendRate", "Replication", &NetworkSettings::sendRate);

// FUNCTION: WEBSERVICE 0x1019ba30
// FUNCTION: ROBLOX 0x004c22b0
NetworkSettings::NetworkSettings()
	: clientPhysics(false), clientPhysicsSpeed(1.0f), clientPhysicsLifetime(0.5f), printPhysicsErrors(false),
	  printInstances(false), printProperties(false), printPacketBuffer(false), logPackets(false),
	  maxDataModelSendBuffer(4), dataPacketSize(0.75f), sendRate(40.0f), maxSendBPS(0), minExtraPing(0),
	  extraPingVariance(0)
{
	setName("Network");
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::NetworkSettings, RBX::GlobalSettings::Item, RBX::sNetworkSettings>;
template class RBX::Reflection::Described<
	RBX::NetworkSettings,
	RBX::sNetworkSettings,
	RBX::FactoryProduct<RBX::NetworkSettings, RBX::GlobalSettings::Item, RBX::sNetworkSettings> >;
template class RBX::DescribedCreatable<RBX::NetworkSettings, RBX::GlobalSettings::Item, RBX::sNetworkSettings>;
