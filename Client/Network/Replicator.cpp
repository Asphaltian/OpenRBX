#include "network/Replicator.h"

#include "PacketIds.h"
#include "network/NetworkSettings.h"
#include "network/Player.h"
#include "network/Players.h"
#include "security/SecurityContext.h"
#include "util/Log.h"
#include "util/Name.h"
#include "util/Profiling.h"
#include "util/StandardOut.h"
#include "util/scopedassign.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/Hopper.h"
#include "v8datamodel/JointsService.h"
#include "v8datamodel/Lighting.h"
#include "v8datamodel/ModelInstance.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Stats.h"
#include "v8datamodel/Teams.h"
#include "v8datamodel/Workspace.h"

#include <BitStream.h>
#include <G3D/System.h>
#include <GetTime.h>
#include <PacketLogger.h>
#include <RakNetStatistics.h>
#include <RakPeer.h>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <stdexcept>
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

// SIZE 0x148
class Replicator::ReplicatorStatsItem : public Stats::Item
{
	Item* waitingRefs;                              // 0x120
	Item* packetLoss;                               // 0x124
	Item* ping;                                     // 0x128
	Item* instanceSize;                             // 0x12c
	Item* bps;                                      // 0x130
	const boost::shared_ptr<Replicator> replicator; // 0x134
	const RakNetStatistics* statistics;             // 0x13c

public:
	unsigned int instanceCount; // 0x140
	unsigned int instanceBits;  // 0x144

	ReplicatorStatsItem(const boost::shared_ptr<Replicator>& replicator, const RakNetStatistics* statistics);

	virtual void update();
};

// SIZE 0x8
class __declspec(novtable) Replicator::Item : boost::noncopyable
{
protected:
	Replicator& replicator; // 0x04

	Item(Replicator& replicator) : replicator(replicator) {}

public:
	enum ItemType
	{
		ItemTypeEnd = 0,
		ItemTypeDelete = 1,
		ItemTypeNew = 2,
		ItemTypeChangeProperty = 3,
		ItemTypeMarker = 4
	};

	virtual ~Item() {}

	virtual void write(RakNet::BitStream& bitStream) = 0;
	static void writeItemType(RakNet::BitStream& stream, ItemType value);
	static void readItemType(RakNet::BitStream& stream, ItemType& value);
};

// VTABLE: WEBSERVICE 0x102504fc
// SIZE 0x10
class Replicator::NewInstanceItem : public Item
{
	const boost::shared_ptr<Instance> instance; // 0x08

public:
	NewInstanceItem(Replicator& replicator, const boost::shared_ptr<Instance>& instance);
	virtual void write(RakNet::BitStream& bitStream);
};

// VTABLE: WEBSERVICE 0x10250608
// SIZE 0x10
class Replicator::DeleteInstanceItem : public Item
{
	boost::shared_ptr<const Instance> instance; // 0x08

public:
	DeleteInstanceItem(Replicator& replicator, const boost::shared_ptr<const Instance>& instance);
	virtual void write(RakNet::BitStream& bitStream);
};

// VTABLE: WEBSERVICE 0x102503a4
// SIZE 0xc
class Replicator::MarkerItem : public Item
{
	long id; // 0x08

public:
	MarkerItem(Replicator& replicator, long id) : Item(replicator), id(id) {}

	virtual void write(RakNet::BitStream& bitStream);
};

// VTABLE: WEBSERVICE 0x10250498
// SIZE 0x14
class Replicator::ChangePropertyItem : public Item
{
	const Reflection::PropertyDescriptor& desc;       // 0x08
	const boost::shared_ptr<const Instance> instance; // 0x0c

public:
	ChangePropertyItem(
		Replicator& replicator,
		const boost::shared_ptr<const Instance>& instance,
		const Reflection::PropertyDescriptor& desc
	);
	virtual void write(RakNet::BitStream& bitStream);
};

// SIZE 0x124
class Replicator::ItemSender : boost::noncopyable
{
	Replicator& replicator;      // 0x00
	RakPeerInterface* peer;      // 0x04
	RakNet::BitStream bitStream; // 0x08

	void openPacket() { bitStream << (unsigned char) ID_DATA; }

	void closePacket()
	{
		Item::writeItemType(bitStream, Item::ItemTypeEnd);
		peer->Send(&bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, replicator.remotePlayerId, false);
		bitStream.Reset();
	}

	const int maxStreamSize; // 0x11c

public:
	bool sentItems; // 0x120

	ItemSender(Replicator& replicator, RakPeerInterface* peer);
	~ItemSender();

	bool send(Item& item)
	{
		if (bitStream.GetNumberOfBytesUsed() >= maxStreamSize) {
			return false;
		}

		if (bitStream.GetNumberOfBitsUsed() == 0) {
			openPacket();
		}

		item.write(bitStream);
		sentItems = true;
		return true;
	}
};

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

// FUNCTION: WEBSERVICE 0x1019c760
void Replicator::Item::writeItemType(RakNet::BitStream& stream, ItemType value)
{
	if (value >= ItemTypeDelete && value <= ItemTypeChangeProperty) {
		stream.WriteBits((const unsigned char*) &value, 2);
	}
	else {
		unsigned char zero = 0;
		stream.WriteBits(&zero, 2);
		stream.Write(value);
	}
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

// FUNCTION: WEBSERVICE 0x1019ca30
void Replicator::skipValue(RakNet::BitStream& inBitStream, ValueType type)
{
	switch (type) {
	case ValueType_string: {
		std::string value;
		inBitStream >> value;
		break;
	}
	case ValueType_bool: {
		bool value;
		inBitStream >> value;
		break;
	}
	case ValueType_int: {
		int value;
		inBitStream >> value;
		break;
	}
	case ValueType_float: {
		float value;
		inBitStream >> value;
		break;
	}
	case ValueType_double: {
		double value;
		inBitStream >> value;
		break;
	}
	case ValueType_BrickColor: {
		BrickColor value;
		inBitStream >> value;
		break;
	}
	case ValueType_Color3: {
		G3D::Color3 value;
		inBitStream >> value;
		break;
	}
	case ValueType_Vector3: {
		G3D::Vector3 value;
		inBitStream >> value;
		break;
	}
	case ValueType_BrickVector: {
		G3D::Vector3 value;
		readBrickVector(inBitStream, value);
		break;
	}
	case ValueType_CoordinateFrame: {
		G3D::CoordinateFrame value;
		inBitStream >> value;
		break;
	}
	case ValueType_Enum: {
		int value;
		inBitStream.ReadCompressed(value);
		break;
	}
	case ValueType_Ref: {
		int id;
		inBitStream >> id;
		break;
	}
	case ValueType_ContentId: {
		ContentId value;
		inBitStream >> value;
		break;
	}
	}
}

// FUNCTION: WEBSERVICE 0x1019cc30
inline Replicator::ChangePropertyItem::ChangePropertyItem(
	Replicator& replicator,
	const boost::shared_ptr<const Instance>& instance,
	const Reflection::PropertyDescriptor& desc
)
	: Item(replicator), desc(desc), instance(instance)
{
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

// FUNCTION: WEBSERVICE 0x1019cdf0
inline Replicator::ItemSender::~ItemSender()
{
	if (bitStream.GetNumberOfBitsUsed() > 0) {
		closePacket();
	}
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

// FUNCTION: WEBSERVICE 0x1019f9f0
inline Replicator::NewInstanceItem::NewInstanceItem(Replicator& replicator, const boost::shared_ptr<Instance>& instance)
	: Item(replicator), instance(instance)
{
	replicator.pendingNewInstances.insert(instance.get());
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

// FUNCTION: WEBSERVICE 0x1019fd80
inline Replicator::DeleteInstanceItem::DeleteInstanceItem(
	Replicator& replicator,
	const boost::shared_ptr<const Instance>& instance
)
	: Item(replicator), instance(instance)
{
	replicator.pendingDeleteInstances.insert(instance.get());
}

// STUB: WEBSERVICE 0x1019fe60
inline void Replicator::MarkerItem::write(RakNet::BitStream& bitStream)
{
	writeItemType(bitStream, ItemTypeMarker);
	bitStream << (int) id;

	if (NetworkSettings::singleton().printInstances) {
		StandardOut::singleton()
			->print(MESSAGE_INFO, "Replication: Sending marker %d to %s", id, replicator.remotePlayerId.ToString());
	}

	replicator.onSentMarker(id);
}

// FUNCTION: WEBSERVICE 0x1019ff90
inline Replicator::ItemSender::ItemSender(Replicator& replicator, RakPeerInterface* peer)
	: replicator(replicator), peer(peer),
	  maxStreamSize(
		  G3D::iRound(NetworkSettings::singleton().dataPacketSize * (peer->GetMTUSize(replicator.remotePlayerId) - 128))
	  ),
	  sentItems(false)
{
}

// FUNCTION: WEBSERVICE 0x101a1410
bool Replicator::disconnectReplicationData(boost::shared_ptr<Instance> instance)
{
	RepConts::iterator iter = replicationContainers.find(instance.get());
	if (iter == replicationContainers.end()) {
		return false;
	}

	instance->visitChildren(boost::bind(&Replicator::disconnectReplicationData, this, _1));

	closeReplicationItem(*iter);
	replicationContainers.erase(iter);

	return true;
}

// FUNCTION: WEBSERVICE 0x101a1580
void Replicator::onChildAdded(boost::shared_ptr<Instance> child)
{
	Profiling::Mark mark(*profileReplication, false);
	Profiling::Mark mark2(*profileDataListening, false);

	if (!wantReplicate(child.get())) {
		return;
	}

	if (replicationContainers.find(child.get()) == replicationContainers.end()) {
		addReplicationData(child, true);

		pendingItems.push_back(boost::shared_ptr<Item>(new NewInstanceItem(*this, child)));

		child->visitChildren(boost::bind(&Replicator::onChildAdded, this, _1));
	}
}

inline bool Replicator::removeFromPendingNewInstances(const Instance* instance)
{
	bool removed = pendingNewInstances.erase(instance) != 0;
	return removed;
}

// FUNCTION: WEBSERVICE 0x101a18d0
void Replicator::onParentChanged(boost::shared_ptr<Instance> instance)
{
	if (replicationContainers.find(instance->getParent()) != replicationContainers.end()) {
		pendingItems.push_back(boost::shared_ptr<Item>(new ChangePropertyItem(*this, instance, Instance::propParent)));
	}
	else {
		disconnectReplicationData(instance);

		if (!removeFromPendingNewInstances(instance.get())) {
			pendingItems.push_back(boost::shared_ptr<Item>(new DeleteInstanceItem(*this, instance)));
		}
	}
}

// FUNCTION: WEBSERVICE 0x101a1c20
void Replicator::onPropertyChanged(
	boost::shared_ptr<Instance> instance,
	const Reflection::PropertyDescriptor* descriptor
)
{
	Profiling::Mark mark(*profileReplication, false);
	Profiling::Mark mark2(*profileDataListening, false);

	if (instance.get() == removingInstance) {
		return;
	}

	if (deserializingProperty && Reflection::Property(*descriptor, instance.get()) == *deserializingProperty) {
		return;
	}

	if (isSerializePending(instance.get())) {
		return;
	}

	if (descriptor == &Instance::propParent) {
		onParentChanged(instance);
		return;
	}

	if (!descriptor->canStreamWrite()) {
		return;
	}

	Player* player = findTargetPlayer();
	ModelInstance* character = player ? player->getCharacter() : NULL;
	if (character && instance->isDescendentOf(character)) {
		pendingItems.push_front(boost::shared_ptr<Item>(new ChangePropertyItem(*this, instance, *descriptor)));
	}
	else {
		pendingItems.push_back(boost::shared_ptr<Item>(new ChangePropertyItem(*this, instance, *descriptor)));
	}
}

// FUNCTION: WEBSERVICE 0x101a24c0
bool Replicator::sendItems()
{
	if (!canSendItems()) {
		return false;
	}

	Profiling::Mark mark(*profileDataOut, false);
	ItemSender sender(*this, peer);

	while (pendingItems.size() > 0) {
		boost::shared_ptr<Item> item = pendingItems.front();
		if (item) {
			if (!sender.send(*item)) {
				break;
			}
		}
		pendingItems.pop_front();
	}

	return sender.sentItems;
}

// FUNCTION: WEBSERVICE 0x101a3b60
inline void Replicator::DeleteInstanceItem::write(RakNet::BitStream& bitStream)
{
	if (replicator.pendingDeleteInstances.erase(instance.get())) {
		if (!instance) {
			StandardOut::singleton()
				->print(MESSAGE_ERROR, "Replication: ~NULL >> %s", replicator.remotePlayerId.ToString());
		}
		else {
			try {
				writeItemType(bitStream, ItemTypeDelete);

				replicator.serializeId(bitStream, instance.get());

				if (NetworkSettings::singleton().printInstances) {
					StandardOut::singleton()->print(
						MESSAGE_INFO,
						"Replication: ~%s:%s >> %s",
						instance->getClassName().c_str(),
						instance->getGuid().readableString(4).c_str(),
						replicator.remotePlayerId.ToString()
					);
				}
			}
			catch (std::exception& e) {
				StandardOut::singleton()->print(
					MESSAGE_ERROR,
					"Replication: ~%s >> %s, %s",
					instance->getClassName().c_str(),
					replicator.remotePlayerId.ToString(),
					e.what()
				);
			}
		}
	}
}

// FUNCTION: WEBSERVICE 0x101a3da0
void Replicator::requestCharacter()
{
	RakNet::BitStream bitStream;
	bitStream << (unsigned char) ID_REQUEST_CHARACTER;

	Player* player = findTargetPlayer();

	if (!player) {
		throw std::runtime_error("Attempting to send a Character request without a local Player");
	}

	serializeId(bitStream, player);

	if (NetworkSettings::singleton().printInstances) {
		Guid::Data id = player->getGuid().data;
		StandardOut::singleton()
			->print(MESSAGE_INFO, "Replicator: Requesting character for %s", id.readableString(4).c_str());
	}

	peer->Send(&bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, remotePlayerId, false);
}

// FUNCTION: WEBSERVICE 0x101a3f80
void Replicator::readInstanceDelete(RakNet::BitStream& inBitstream)
{
	Instance* instance;
	Guid::Data id;
	if (deserializeInstanceRef(inBitstream, instance, id)) {
		boost::shared_ptr<Instance> hold = shared_from(instance);

		if (NetworkSettings::singleton().printInstances) {
			StandardOut::singleton()->print(
				MESSAGE_INFO,
				"Replication: ~%s:%d << %s",
				instance->getClassName().c_str(),
				instance->getGuid().readableString(4).c_str(),
				remotePlayerId.ToString()
			);
		}

		if (disconnectReplicationData(hold)) {
			removeFromPendingNewInstances(instance);

			ScopedAssign<Instance*> assign(removingInstance, instance);
			instance->setParent(NULL);
		}
	}
	else if (NetworkSettings::singleton().printInstances) {
		StandardOut::singleton()->print(MESSAGE_INFO, "Replication ~??? << %s", remotePlayerId.ToString());
	}

	resolvePendingReferences(NULL, id);
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

// FUNCTION: WEBSERVICE 0x101a6200
Replicator::ReplicatorStatsItem::ReplicatorStatsItem(
	const boost::shared_ptr<Replicator>& replicator,
	const RakNetStatistics* statistics
)
	: replicator(replicator), statistics(statistics), instanceCount(0), instanceBits(0)
{
	bps = createChildItem("Bytes/s");
	bps->createBoundChildItem("Bandwidth Exceeded", statistics->bandwidthExceeded);
	packetLoss = bps->createChildItem("Packet Loss");

	ping = createChildItem("Ping");

	Item* replication = createBoundChildItem(*replicator->profileReplication);
	replication->createBoundChildItem(*replicator->profileDataListening);
	replication->createBoundChildItem(*replicator->profileDataOut);
	replication->createBoundChildItem(*replicator->profilePhysicsOut);
	replication->createBoundChildItem(*replicator->profileDataIn);
	replication->createBoundChildItem(*replicator->profilePhysicsIn);

	instanceSize = replication->createChildItem("Instance size (In)");

	waitingRefs = createChildItem("Waiting Refs");
}

// FUNCTION: WEBSERVICE 0x101a6430
void Replicator::ReplicatorStatsItem::update()
{
	if (replicator->peer) {
		bps->formatMem((unsigned int) statistics->bitsPerSecond / 8);

		ping->formatValue(
			replicator->peer->GetLastPing(replicator->remotePlayerId),
			"%d avg:%d best:%d",
			replicator->peer->GetLastPing(replicator->remotePlayerId),
			replicator->peer->GetAveragePing(replicator->remotePlayerId),
			replicator->peer->GetLowestPing(replicator->remotePlayerId)
		);
		waitingRefs->formatValue(replicator->numWaitingRefs());

		float loss = (float) statistics->messagesTotalBitsResent / (float) statistics->totalBitsSent;
		packetLoss->formatValue(loss, "%.1g%%", 100.0 * loss);

		instanceSize->formatMem(instanceCount ? instanceBits / (8 * instanceCount) : 0);
	}
}

// FUNCTION: WEBSERVICE 0x101a6690
const Instance* Replicator::getDefault(const Name& className)
{
	Security::Impersonator impersonate(Security::Replicator);

	DefaultObjects::iterator iter = defaultObjects.find(&className);
	if (iter == defaultObjects.end()) {
		boost::shared_ptr<Instance> instance = Instance::create(className);
		if (!instance) {
			StandardOut::singleton()
				->print(MESSAGE_ERROR, "Replication: Can't create default object of type %s", className.c_str());
		}
		defaultObjects[&className] = instance;
		return instance.get();
	}

	return iter->second.get();
}

// FUNCTION: WEBSERVICE 0x101a6e20
void Replicator::createStatsItems(Stats::StatsService* stats)
{
	if (statsItem) {
		statsItem->setParent(NULL);
		statsItem.reset();
	}

	if (stats) {
		boost::shared_ptr<Stats::Item> network =
			shared_from_polymorphic_downcast<Stats::Item>(stats->findFirstChildByName("Network"));
		if (network) {
			statsItem = Creatable<Instance>::create<ReplicatorStatsItem>(
				shared_from(this),
				peer->GetStatistics(remotePlayerId)
			);
			statsItem->setName(getName());
			statsItem->setParent2(network);
		}
	}
}

// FUNCTION: WEBSERVICE 0x101a7030
SharedStringDictionary& Replicator::getSharedDictionary(const Reflection::PropertyDescriptor& descriptor)
{
	Strings::iterator iter = strings.find(&descriptor);
	if (iter == strings.end()) {
		boost::shared_ptr<SharedStringDictionary> result(new SharedStringDictionary());
		strings[&descriptor] = result;
		return *result;
	}

	return *iter->second;
}

// FUNCTION: WEBSERVICE 0x101a7180
void Replicator::serializeValue(
	const Reflection::ConstProperty& property,
	bool includeLength,
	RakNet::BitStream& outBitStream
)
{
	if (property.getDescriptor().type == Reflection::Type::singleton<std::string>()) {
		writeValueType(includeLength, ValueType_string, outBitStream);
		getSharedDictionary(property.getDescriptor()).serializeString(property, outBitStream);
	}
	else if (property.getDescriptor().type == Reflection::Type::singleton<bool>()) {
		writeValueType(includeLength, ValueType_bool, outBitStream);
		outBitStream << property.getValue<bool>();
	}
	else if (property.getDescriptor().type == Reflection::Type::singleton<int>()) {
		writeValueType(includeLength, ValueType_int, outBitStream);
		outBitStream << property.getValue<int>();
	}
	else if (property.getDescriptor().type == Reflection::Type::singleton<float>()) {
		writeValueType(includeLength, ValueType_float, outBitStream);
		outBitStream << property.getValue<float>();
	}
	else if (property.getDescriptor().type == Reflection::Type::singleton<double>()) {
		writeValueType(includeLength, ValueType_double, outBitStream);
		serialize<double>(property, outBitStream);
	}
	else if (property.getDescriptor().type == Reflection::Type::singleton<BrickColor>()) {
		writeValueType(includeLength, ValueType_BrickColor, outBitStream);
		serialize<BrickColor>(property, outBitStream);
	}
	else if (property.getDescriptor().type == Reflection::Type::singleton<G3D::Color3>()) {
		writeValueType(includeLength, ValueType_Color3, outBitStream);
		serialize<G3D::Color3>(property, outBitStream);
	}
	else if (property.getDescriptor().type == Reflection::Type::singleton<G3D::Vector3>()) {
		if (property.getDescriptor() == PartInstance::prop_Size) {
			writeValueType(includeLength, ValueType_BrickVector, outBitStream);
			writeBrickVector(outBitStream, property.getValue<G3D::Vector3>());
		}
		else {
			writeValueType(includeLength, ValueType_Vector3, outBitStream);
			serialize<G3D::Vector3>(property, outBitStream);
		}
	}
	else if (property.getDescriptor().type == Reflection::Type::singleton<G3D::CoordinateFrame>()) {
		writeValueType(includeLength, ValueType_CoordinateFrame, outBitStream);
		serialize<G3D::CoordinateFrame>(property, outBitStream);
	}
	else if (dynamic_cast<const Reflection::EnumPropertyDescriptor*>(&property.getDescriptor())) {
		writeValueType(includeLength, ValueType_Enum, outBitStream);
		serializeEnum(property, outBitStream);
	}
	else if (dynamic_cast<const Reflection::RefPropertyDescriptor*>(&property.getDescriptor())) {
		writeValueType(includeLength, ValueType_Ref, outBitStream);
		serializeRef(property, outBitStream);
	}
	else if (property.getDescriptor().type == Reflection::Type::singleton<ContentId>()) {
		writeValueType(includeLength, ValueType_ContentId, outBitStream);
		serialize<ContentId>(property, outBitStream);
	}
}

inline bool Replicator::isReplicationContainer(const Instance* instance) const
{
	return replicationContainers.find(instance) != replicationContainers.end();
}

// FUNCTION: WEBSERVICE 0x101a7480
void Replicator::deserializeValue(RakNet::BitStream& inBitStream, Reflection::Property& property)
{
	ScopedAssign<const Reflection::Property*> assign(deserializingProperty, &property);

	if (property.getDescriptor().type == Reflection::Type::singleton<std::string>()) {
		getSharedDictionary(property.getDescriptor()).deserializeString(property, inBitStream);
	}
	else if (property.getDescriptor().type == Reflection::Type::singleton<bool>()) {
		deserialize<bool>(property, inBitStream);
	}
	else if (property.getDescriptor().type == Reflection::Type::singleton<int>()) {
		deserialize<int>(property, inBitStream);
	}
	else if (property.getDescriptor().type == Reflection::Type::singleton<float>()) {
		deserialize<float>(property, inBitStream);
	}
	else if (property.getDescriptor().type == Reflection::Type::singleton<double>()) {
		deserialize<double>(property, inBitStream);
	}
	else if (property.getDescriptor().type == Reflection::Type::singleton<BrickColor>()) {
		deserialize<BrickColor>(property, inBitStream);
	}
	else if (property.getDescriptor().type == Reflection::Type::singleton<G3D::Color3>()) {
		deserialize<G3D::Color3>(property, inBitStream);
	}
	else if (property.getDescriptor().type == Reflection::Type::singleton<G3D::Vector3>()) {
		if (property.getDescriptor() == PartInstance::prop_Size) {
			G3D::Vector3 value;
			readBrickVector(inBitStream, value);
			property.setValue(value);
		}
		else {
			deserialize<G3D::Vector3>(property, inBitStream);
		}
	}
	else if (property.getDescriptor().type == Reflection::Type::singleton<G3D::CoordinateFrame>()) {
		deserialize<G3D::CoordinateFrame>(property, inBitStream);
	}
	else if (dynamic_cast<const Reflection::EnumPropertyDescriptor*>(&property.getDescriptor())) {
		deserializeEnum(property, inBitStream);
	}
	else if (dynamic_cast<const Reflection::RefPropertyDescriptor*>(&property.getDescriptor())) {
		deserializeRef(property, inBitStream);
	}
	else if (property.getDescriptor().type == Reflection::Type::singleton<ContentId>()) {
		deserialize<ContentId>(property, inBitStream);
	}
}

// FUNCTION: WEBSERVICE 0x101a76d0
inline void Replicator::ChangePropertyItem::write(RakNet::BitStream& bitStream)
{
	if (replicator.isReplicationContainer(instance.get())) {
		writeItemType(bitStream, ItemTypeChangeProperty);

		const Name& name = desc.name;

		replicator.serializeId(bitStream, instance.get());

		replicator.propNames.send(bitStream, name.toString());

		if (NetworkSettings::singleton().printProperties) {
			StandardOut::singleton()->print(
				MESSAGE_INFO,
				"Replication: %s:%s.%s >> %s",
				instance->getClassName().c_str(),
				instance->getGuid().readableString(4).c_str(),
				name.c_str(),
				replicator.remotePlayerId.ToString()
			);
		}

		replicator.serializeValue(Reflection::ConstProperty(desc, instance.get()), true, bitStream);
	}
}

// FUNCTION: WEBSERVICE 0x101a78c0
Replicator::ReplicationData& Replicator::addReplicationData(boost::shared_ptr<Instance> instance, bool listenToChanges)
{
	std::pair<RepConts::iterator, bool> pair =
		replicationContainers.insert(RepConts::value_type(instance.get(), ReplicationData()));

	if (pair.second) {
		ReplicationData& data = pair.first->second;
		data.instance = instance;
		data.deleteOnDisconnect = false;
		data.childAdded =
			Instance::event_childAdded.connect(instance.get(), boost::bind(&Replicator::onChildAdded, this, _1), 0);
		if (listenToChanges) {
			data.changed = Instance::event_propertyChanged.connect(
				instance.get(),
				boost::bind(&Replicator::onPropertyChanged, this, instance, _1),
				0
			);
		}
		return data;
	}
	else {
		ReplicationData& oldData = pair.first->second;
		if (!listenToChanges) {
			oldData.changed.disconnect();
		}
		else if (!oldData.changed.connected()) {
			oldData.changed = Instance::event_propertyChanged.connect(
				instance.get(),
				boost::bind(&Replicator::onPropertyChanged, this, instance, _1),
				0
			);
		}
		return oldData;
	}
}

// FUNCTION: WEBSERVICE 0x101a7cf0
void Replicator::readProperties(RakNet::BitStream& inBitstream, Instance* instance)
{
	Reflection::MemberDescriptorContainer<Reflection::PropertyDescriptor>::Iterator iter = instance->properties_begin();
	Reflection::MemberDescriptorContainer<Reflection::PropertyDescriptor>::Iterator end = instance->properties_end();
	while (iter != end) {
		Reflection::Property property = *iter;

		if (property.getDescriptor().canStreamWrite()) {
			if (typeid(bool) == property.getDescriptor().type.type) {
				deserializeValue(inBitstream, property);
			}
			else {
				bool equalValues;
				inBitstream >> equalValues;
				if (!equalValues) {
					deserializeValue(inBitstream, property);
				}
			}
		}

		++iter;
	}
}

// FUNCTION: WEBSERVICE 0x101a8aa0
void Replicator::writeProperties(Instance* instance, RakNet::BitStream& outBitStream)
{
	const Instance* defaultInstance = getDefault(instance->getClassName());

	Reflection::MemberDescriptorContainer<Reflection::PropertyDescriptor>::Iterator iter = instance->properties_begin();
	Reflection::MemberDescriptorContainer<Reflection::PropertyDescriptor>::Iterator end = instance->properties_end();
	while (iter != end) {
		Reflection::ConstProperty property = *iter;
		const Reflection::PropertyDescriptor& descriptor = property.getDescriptor();

		if (descriptor.canStreamWrite()) {
			if (typeid(bool) == descriptor.type.type) {
				serializeValue(property, false, outBitStream);
				if (NetworkSettings::singleton().printInstances && NetworkSettings::singleton().printProperties) {
					StandardOut::singleton()->print(
						MESSAGE_INFO,
						"   %s %s, %d bit",
						descriptor.type.name.c_str(),
						descriptor.name.c_str(),
						1
					);
				}
			}
			else if (defaultInstance && descriptor.equalValues(instance, defaultInstance)) {
				outBitStream << true;
				if (NetworkSettings::singleton().printInstances && NetworkSettings::singleton().printProperties) {
					StandardOut::singleton()->print(
						MESSAGE_INFO,
						"   %s %s, 1 bit (default)",
						descriptor.type.name.c_str(),
						descriptor.name.c_str()
					);
				}
			}
			else {
				int start = outBitStream.GetNumberOfBitsUsed();
				outBitStream << false;
				serializeValue(property, false, outBitStream);
				if (NetworkSettings::singleton().printInstances && NetworkSettings::singleton().printProperties) {
					StandardOut::singleton()->print(
						MESSAGE_INFO,
						"   %s %s, %d bits",
						descriptor.type.name.c_str(),
						descriptor.name.c_str(),
						outBitStream.GetNumberOfBitsUsed() - start
					);
				}
			}
		}

		++iter;
	}
}

// FUNCTION: WEBSERVICE 0x101a8e40
void Replicator::addTopReplicationContainer(Instance* instance, bool replicateProperties)
{
	if (instance) {
		topReplicationContainers.push_back(instance);

		addReplicationData(shared_from(instance), replicateProperties);

		instance->visitChildren(boost::bind(&Replicator::onChildAdded, this, _1));
	}
}

// FUNCTION: WEBSERVICE 0x101a8eb0
void Replicator::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	std::for_each(replicationContainers.begin(), replicationContainers.end(), boost::bind(&closeReplicationItem, _1));
	replicationContainers.clear();
	pendingItems.clear();
	topReplicationContainers.clear();

	DescribedNonCreatable<Replicator, IdSerializer, sReplicator>::onServiceProvider(oldProvider, newProvider);

	createStatsItems(newProvider ? newProvider->find<Stats::StatsService>() : NULL);

	if (newProvider) {
		Instance* lighting = newProvider->create<Lighting>();
		addTopReplicationContainer(lighting, true);
		Instance* starterPack = newProvider->create<StarterPackService>();
		addTopReplicationContainer(starterPack, false);
		Instance* workspace = newProvider->find<Workspace>();
		addTopReplicationContainer(workspace, false);
		Instance* joints = newProvider->create<JointsService>();
		addTopReplicationContainer(joints, false);
		players = newProvider->create<Players>();
		addTopReplicationContainer(players, false);
		Instance* teams = newProvider->create<Teams>();
		addTopReplicationContainer(teams, false);
	}
	else if (peer) {
		peer->DetachPlugin(this);
		peer = NULL;
	}
}

// STUB: WEBSERVICE 0x101a9220
PluginReceiveResult Replicator::OnReceive(RakPeerInterface* peer, Packet* packet)
{
	STUB(0x101a9220);
	return RR_CONTINUE_PROCESSING;
}

// FUNCTION: WEBSERVICE 0x101a9890
inline void Replicator::NewInstanceItem::write(RakNet::BitStream& bitStream)
{
	if (replicator.pendingNewInstances.erase(instance.get())) {
		writeItemType(bitStream, ItemTypeNew);

		replicator.serializeId(bitStream, instance.get());

		if (NetworkSettings::singleton().printInstances) {
			StandardOut::singleton()->print(
				MESSAGE_INFO,
				"Replication: %s:%s >> %s",
				instance->getClassName().c_str(),
				instance->getGuid().readableString(4).c_str(),
				replicator.remotePlayerId.ToString()
			);
		}

		replicator.classNames.send(bitStream, instance->getClassName().toString());

		bool deleteOnDisconnect = replicator.remoteDeleteOnDisconnect(instance.get());

		bitStream << deleteOnDisconnect;

		replicator.writeProperties(instance.get(), bitStream);

		Reflection::ConstProperty property(Instance::propParent, instance.get());

		replicator.serializeValue(property, false, bitStream);
	}
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
