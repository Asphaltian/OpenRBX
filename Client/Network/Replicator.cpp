#include "network/Replicator.h"

#include "network/NetworkSettings.h"
#include "network/Player.h"
#include "network/Players.h"
#include "util/Log.h"
#include "util/Name.h"
#include "util/Profiling.h"
#include "util/StandardOut.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/ModelInstance.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Stats.h"

#include <BitStream.h>
#include <G3D/System.h>
#include <GetTime.h>
#include <PacketLogger.h>
#include <RakPeer.h>
#include <stdio.h>

// SIZE 0x210
class FilePacketLogger : public PacketLogger
{
	FILE* packetLogFile; // 0x20c

public:
	FilePacketLogger(RBX::Network::Peer* peer);

	virtual ~FilePacketLogger()
	{
		if (packetLogFile) {
			fclose(packetLogFile);
		}
	}

	virtual void OnAttach(RakPeerInterface* peer);
	virtual void WriteLog(const char* str);
};

DECOMP_SIZE_ASSERT(FilePacketLogger, 0x210)

namespace RBX {
namespace Network {

const char sPeer[] = "NetworkPeer";
const char sReplicator[] = "NetworkReplicator";

// FUNCTION: WEBSERVICE 0x100d15c0 FOLDED
void Replicator::onSentMarker(long id)
{
}

} // namespace Network
} // namespace RBX

// FUNCTION: WEBSERVICE 0x1019bd10
void FilePacketLogger::OnAttach(RakPeerInterface* peer)
{
	PacketLogger::OnAttach(peer);

	LogHeader();
}

// FUNCTION: WEBSERVICE 0x1019bd30
void FilePacketLogger::WriteLog(const char* str)
{
	if (packetLogFile) {
		fputs(str, packetLogFile);
	}
}

// FUNCTION: WEBSERVICE 0x1019bd50
static void checkDisconnect(RBX::Instance* instance)
{
	RBX::Network::Replicator* replicator = dynamic_cast<RBX::Network::Replicator*>(instance);
	if (replicator && replicator->disconnected) {
		replicator->setParent(NULL);
	}
}

namespace RBX {
namespace Network {

// FUNCTION: WEBSERVICE 0x1019bd90
void Replicator::writeValueType(bool includeLength, ValueType vt, RakNet::BitStream& outBitStream)
{
	if (includeLength) {
		unsigned char c = vt;
		outBitStream.WriteBits(&c, 4);
	}
}

// FUNCTION: WEBSERVICE 0x1019bdc0
bool Replicator::wantReplicate(const Instance* source) const
{
	return dynamic_cast<const Camera*>(source) ? false : true;
}

// FUNCTION: WEBSERVICE 0x1019bf10
bool Replicator::canSendItems()
{
	return receivedGlobals;
}

// FUNCTION: WEBSERVICE 0x1019bf20
void Replicator::closeConnection()
{
	peer->CloseConnection(remotePlayerId, true);

	setParent(NULL);
}

} // namespace Network
} // namespace RBX

// FUNCTION: WEBSERVICE 0x1019c7c0
FilePacketLogger::FilePacketLogger(RBX::Network::Peer* peer) : packetLogFile(NULL)
{
	std::string pathName = RBX::Log::current()->logFile;
	size_t pos = pathName.rfind('\\');
	pathName.erase(pos + 1);

	char filename[256];
	sprintf(filename, "PacketLog%i.csv", RakNet::GetTime());
	pathName += filename;

	packetLogFile = fopen(pathName.c_str(), "wt");
	if (packetLogFile) {
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_INFO, "Logging packets to %s", pathName.c_str());
	}
	else {
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_WARNING, "Failed to create log file %s", pathName.c_str());
	}
}

namespace RBX {
namespace Network {

// FUNCTION: WEBSERVICE 0x1019c9f0 FOLDED
RakPeerInterface* Peer::peerInterface()
{
	return rakPeer.get();
}

// FUNCTION: WEBSERVICE 0x1019ca00
void Replicator::closeReplicationItem(const std::pair<const Instance* const, ReplicationData>& pair)
{
	pair.second.childAdded.disconnect();
	pair.second.changed.disconnect();
	if (pair.second.deleteOnDisconnect) {
		pair.second.instance->setParent(NULL);
	}
}

// FUNCTION: WEBSERVICE 0x1019cd10
bool Replicator::remoteDeleteOnDisconnect(const Instance* instance) const
{
	if (instance->isDescendentOf(players)) {
		return true;
	}

	return instance == Players::findLocalCharacter(instance) ? true : false;
}

// FUNCTION: WEBSERVICE 0x1019cd50
void Peer::Update(RakPeerInterface* peer)
{
	profilePacketsThread->sample(static_cast<RakPeer*>(peer)->processPacketsThreadHandle);

	PluginInterface::Update(peer);
}

// FUNCTION: WEBSERVICE 0x1019cd80
Player* Replicator::findTargetPlayer()
{
	return players ? players->getLocalPlayer() : NULL;
}

// FUNCTION: WEBSERVICE 0x1019cda0
Mechanism* Replicator::findTargetPlayerCharacterMechanism()
{
	if (Player* player = findTargetPlayer()) {
		if (ModelInstance* character = player->getCharacter()) {
			if (PartInstance* part = character->getPrimaryPart()) {
				return Mechanism::getMechanismFromPrimitive(part->getPrimitive());
			}
		}
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x1019dfe0
void Peer::onEvent(const RunService* source, Heartbeat event)
{
	double timeUp = G3D::System::getTick() + event.step / 5.0;

	do {
		Packet* packet = rakPeer->Receive();
		if (!packet) {
			break;
		}

		rakPeer->DeallocatePacket(packet);
	} while (G3D::System::getTick() < timeUp);

	for_eachChild(checkDisconnect);
}

// FUNCTION: WEBSERVICE 0x1019e990
bool Replicator::isSerializePending(const Instance* instance) const
{
	return pendingNewInstances.find(instance) != pendingNewInstances.end();
}

// FUNCTION: WEBSERVICE 0x1019fa80
Peer::~Peer()
{
	rakPeer->DetachPlugin(this);
}

// FUNCTION: WEBSERVICE 0x1019fc50
void Peer::updateLogger()
{
	if (!NetworkSettings::singleton().logPackets) {
		logger.reset();
	}
	else if (!logger) {
		logger.reset(new FilePacketLogger(this));
		rakPeer->AttachPlugin(logger.get());
	}
}

// FUNCTION: WEBSERVICE 0x1019fd30
void Peer::updateNetworkSimulator()
{
	rakPeer->ApplyNetworkSimulator(
		NetworkSettings::singleton().maxSendBPS,
		NetworkSettings::singleton().minExtraPing,
		NetworkSettings::singleton().extraPingVariance
	);
}

// FUNCTION: WEBSERVICE 0x101a4ad0
Peer::Peer() : profilePacketsThread(new Profiling::ThreadProfiler("Packets Thread")), rakPeer(new RakPeer())
{
	rakPeer->AttachPlugin(this);
}

// FUNCTION: WEBSERVICE 0x101a5840
void Peer::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	Stats::StatsService* stats = oldProvider ? oldProvider->find<Stats::StatsService>() : NULL;
	if (stats) {
		boost::shared_ptr<Stats::Item> item =
			shared_from_polymorphic_downcast<Stats::Item>(stats->findFirstChildByName("Network"));
		if (item) {
			item->setParent(NULL);
		}
	}

	stopListening(oldProvider ? oldProvider->find<RunService>() : NULL, this);

	Instance::onServiceProvider(oldProvider, newProvider);

	startListening(newProvider ? newProvider->find<RunService>() : NULL, this);

	stats = newProvider ? newProvider->find<Stats::StatsService>() : NULL;
	if (stats) {
		boost::shared_ptr<Stats::Item> item = Creatable<Instance>::create<Stats::Item>();
		item->setName("Network");
		item->setParent(stats);
		item->createBoundChildItem(*profilePacketsThread);
	}
}

// STUB: WEBSERVICE 0x101a5a40
void Replicator::Update(RakPeerInterface* peer)
{
	STUB(0x101a5a40);
}

// STUB: WEBSERVICE 0x101a8eb0
void Replicator::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	STUB(0x101a8eb0);
}

// STUB: WEBSERVICE 0x101a9220
PluginReceiveResult Replicator::OnReceive(RakPeerInterface* peer, Packet* packet)
{
	STUB(0x101a9220);
	return RR_CONTINUE_PROCESSING;
}

// FUNCTION: WEBSERVICE 0x101aa110
Replicator::~Replicator()
{
	if (peer) {
		peer->DetachPlugin(this);
	}
}

// FUNCTION: WEBSERVICE 0x101aa600
Replicator::Replicator(SystemAddress remotePlayerId, RakPeerInterface* peer)
	: lastSendTime(G3D::System::getLocalTime()), lastCharacterSendTime(G3D::System::getLocalTime()), players(NULL),
	  sendPhysicsEnabled(false), deserializingProperty(NULL), removingInstance(NULL),
	  profileReplication(new Profiling::CodeProfiler("Replication")),
	  profileDataListening(new Profiling::CodeProfiler("Data Listening")),
	  profileDataIn(new Profiling::CodeProfiler("Data In")), profileDataOut(new Profiling::CodeProfiler("Data Out")),
	  profilePhysicsIn(new Profiling::CodeProfiler("Physics In")),
	  profilePhysicsOut(new Profiling::CodeProfiler("Physics Out")), receivedGlobals(false), disconnected(false),
	  peer(peer), remotePlayerId(remotePlayerId)
{
	peer->AttachPlugin(this);

	setName(remotePlayerId.ToString());

	profileDataListening->parent = profileReplication.get();
	profileDataIn->parent = profileReplication.get();
	profileDataOut->parent = profileReplication.get();
	profilePhysicsIn->parent = profileReplication.get();
	profilePhysicsOut->parent = profileReplication.get();
}

} // namespace Network
} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::Network::sReplicator>();
template void RBX::Name::callDoDeclare<RBX::Network::sReplicator>();
