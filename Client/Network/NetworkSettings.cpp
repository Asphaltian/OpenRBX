#include "network/NetworkSettings.h"

#include "reflection/property.h"

namespace RBX {

const char sNetworkSettings[] = "NetworkSettings";

using namespace Reflection;

BoundProp<bool, 1> prop_LogPackets("LogPackets", "Diagnostics", &RBX::NetworkSettings::logPackets);
BoundProp<bool, 1> prop_PrintInstances("PrintInstances", "Diagnostics", &RBX::NetworkSettings::printInstances);
BoundProp<bool, 1> prop_PrintProperties("PrintProperties", "Diagnostics", &RBX::NetworkSettings::printProperties);
BoundProp<bool, 1> prop_PrintPacketBuffer("PrintPacketBuffer", "Diagnostics", &RBX::NetworkSettings::printPacketBuffer);
BoundProp<bool, 1> prop_PrintPhysicsErrors(
	"PrintPhysicsErrors",
	"Diagnostics",
	&RBX::NetworkSettings::printPhysicsErrors
);

BoundProp<int, 1> prop_MaxSendBPS("MaxSendBPS", "Simulator", &RBX::NetworkSettings::maxSendBPS);
BoundProp<int, 1> prop_MinExtraPing("MinExtraPing", "Simulator", &RBX::NetworkSettings::minExtraPing);
BoundProp<int, 1> prop_ExtraPingVariance("ExtraPingVariance", "Simulator", &RBX::NetworkSettings::extraPingVariance);

BoundProp<int, 1> prop_MaxDataModelSendBuffer(
	"MaxDataModelSendBuffer",
	"Replication",
	&RBX::NetworkSettings::maxDataModelSendBuffer
);
BoundProp<float, 1> prop_DataPacketSize("DataPacketSize", "Replication", &RBX::NetworkSettings::dataPacketSize);
BoundProp<float, 1> prop_SendRate("SendRate", "Replication", &RBX::NetworkSettings::sendRate);

// FUNCTION: WEBSERVICE 0x1019ba30
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
