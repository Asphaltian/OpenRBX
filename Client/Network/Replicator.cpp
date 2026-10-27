#include "network/Replicator.h"

#include "network/NetworkSettings.h"
#include "util/Log.h"
#include "util/Name.h"
#include "util/Profiling.h"
#include "util/StandardOut.h"
#include "v8datamodel/Stats.h"

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

// STUB: WEBSERVICE 0x10196760
bool Peer::askAddChild(const Instance* instance) const
{
	STUB(0x10196760);
	return false;
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

// STUB: WEBSERVICE 0x1019bd50
static void checkDisconnect(RBX::Instance* instance)
{
	STUB(0x1019bd50);
}

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

// FUNCTION: WEBSERVICE 0x1019cd50
void Peer::Update(RakPeerInterface* peer)
{
	profilePacketsThread->sample(static_cast<RakPeer*>(peer)->processPacketsThreadHandle);

	PluginInterface::Update(peer);
}

// FUNCTION: WEBSERVICE 0x1019dfe0
void Peer::onEvent(const RunService* source, Heartbeat event)
{
	double maxTime = G3D::System::getTick() + event.step / 5.0;

	do {
		Packet* packet = rakPeer->Receive();
		if (!packet) {
			break;
		}

		rakPeer->DeallocatePacket(packet);
	} while (G3D::System::getTick() < maxTime);

	for_eachChild(checkDisconnect);
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

} // namespace Network
} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::Network::sReplicator>();
template void RBX::Name::callDoDeclare<RBX::Network::sReplicator>();
