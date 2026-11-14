#ifndef NETWORK_REPLICATOR_H
#define NETWORK_REPLICATOR_H

#include "Streaming.h"
#include "placeholder.h"
#include "reflection/reflection.h"
#include "util/Events.h"
#include "util/RunStateOwner.h"
#include "v8tree/Instance.h"
#include "v8world/Mechanism.h"

#include <PluginInterface.h>
#include <RakNetTypes.h>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <vector>

class PacketLogger;
class RakPeer;
class RakPeerInterface;
struct Packet;

namespace RakNet {
class BitStream;
}

namespace RBX {

class Assembly;
class Mechanism;
class RunService;
class ServiceProvider;

namespace Profiling {
class CodeProfiler;
class ThreadProfiler;
} // namespace Profiling

namespace Reflection {
class ConstProperty;
class Property;
class PropertyDescriptor;
} // namespace Reflection

namespace Stats {
class StatsService;
}

namespace Network {

class Player;
class Players;

extern const char sPeer[];
extern const char sReplicator[];
extern const char sMarker[];

class Marker : public DescribedNonCreatable<Marker, Reflection::DescribedBase, sMarker>, private boost::noncopyable
{
	bool returned;

	Marker() : returned(false) {}

public:
	static boost::shared_ptr<Marker> newMarker() { return boost::shared_ptr<Marker>(new Marker()); }

	long id() const { return reinterpret_cast<long>(this); }
	static Marker* fromId(long id) { return reinterpret_cast<Marker*>(id); }
	void fireReturned() { event_Returned.fire(this); }

	static Reflection::SignalDesc<Marker, void()> event_Returned;
	static Reflection::SignalDesc<Marker, void(float)> event_Progress;
};

class Replicator : public DescribedNonCreatable<Replicator, IdSerializer, sReplicator>, public PluginInterface
{
	enum ValueType
	{
		ValueType_nil = 0,
		ValueType_string = 1,
		ValueType_bool = 2,
		ValueType_int = 3,
		ValueType_float = 4,
		ValueType_double = 5,
		ValueType_BrickColor = 6,
		ValueType_Color3 = 7,
		ValueType_Vector3 = 8,
		ValueType_BrickVector = 9,
		ValueType_CoordinateFrame = 10,
		ValueType_Enum = 11,
		ValueType_Ref = 12,
		ValueType_ContentId = 13
	};

	class Item;
	class NewInstanceItem;
	class DeleteInstanceItem;
	class ChangePropertyItem;
	class MarkerItem;
	class JobSender;
	class ItemSender;
	class ReplicatorStatsItem;

	double lastSendTime;
	MechanismTracker jobStagePos;
	double lastCharacterSendTime;
	boost::shared_ptr<ReplicatorStatsItem> statsItem;

	typedef std::map<const Reflection::PropertyDescriptor*, boost::shared_ptr<SharedStringDictionary> > Strings;

	Strings strings;
	SharedStringDictionary classNames;
	SharedStringDictionary propNames;

protected:
	struct ReplicationData
	{
		boost::shared_ptr<Instance> instance;
		boost::signals::connection childAdded;
		boost::signals::connection changed;
		bool deleteOnDisconnect;
	};

	typedef std::map<const Instance*, ReplicationData> RepConts;

	RepConts replicationContainers;

	typedef std::vector<Instance*> TopReplConts;

	TopReplConts topReplicationContainers;
	Players* players;

	typedef std::list<boost::shared_ptr<Item> > ItemQueue;

	ItemQueue pendingItems;

	typedef std::set<const Instance*> PendingInstances;

	PendingInstances pendingNewInstances;
	PendingInstances pendingDeleteInstances;
	bool sendPhysicsEnabled;
	const Reflection::Property* deserializingProperty;
	Instance* removingInstance;
	std::queue<boost::shared_ptr<Marker> > incomingMarkers;

	typedef std::map<const Name*, boost::shared_ptr<Instance> > DefaultObjects;

	DefaultObjects defaultObjects;
	boost::scoped_ptr<Profiling::CodeProfiler> profileReplication;
	boost::scoped_ptr<Profiling::CodeProfiler> profileDataListening;
	boost::scoped_ptr<Profiling::CodeProfiler> profileDataIn;
	boost::scoped_ptr<Profiling::CodeProfiler> profileDataOut;
	boost::scoped_ptr<Profiling::CodeProfiler> profilePhysicsIn;
	boost::scoped_ptr<Profiling::CodeProfiler> profilePhysicsOut;

	virtual Player* findTargetPlayer();

	Mechanism* findTargetPlayerCharacterMechanism();

private:
	bool receivedGlobals;

public:
	bool disconnected;
	RakPeerInterface* peer;
	const SystemAddress remotePlayerId;

	Replicator(SystemAddress remotePlayerId, RakPeerInterface* peer);
	virtual ~Replicator();

	boost::shared_ptr<Instance> getPlayer() { return shared_from(findTargetPlayer()); }

	static Reflection::BoundProp<int, 1> prop_maxDataModelSendBuffer;
	static Reflection::SignalDesc<Replicator, void(std::string, bool)> event_Disconnection;

	virtual void Update(RakPeerInterface* peer);
	virtual PluginReceiveResult OnReceive(RakPeerInterface* peer, Packet* packet);

	virtual const Name& getClassName() const { return Name::getNullName(); }

	virtual XmlElement* write() { return NULL; }

	boost::shared_ptr<Reflection::DescribedBase> sendMarker();
	bool isSerializePending(const Instance* instance) const;
	bool isPropertyChangedPending(const Reflection::Property& property) const;
	void requestCharacter();
	void closeConnection();

protected:
	static void closeReplicationItem(const std::pair<const Instance* const, ReplicationData>& pair);
	bool disconnectReplicationData(boost::shared_ptr<Instance> instance);
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
	void onChildAdded(boost::shared_ptr<Instance> child);
	void onParentChanged(boost::shared_ptr<Instance> instance);
	void onPropertyChanged(boost::shared_ptr<Instance> instance, const Reflection::PropertyDescriptor* descriptor);
	const Instance* getDefault(const Name& className);

	virtual bool wantReplicate(const Instance* source) const;
	virtual void onSentMarker(long id);

	void readProperties(RakNet::BitStream& inBitstream, Instance* instance);
	void writeProperties(Instance* instance, RakNet::BitStream& outBitStream);

private:
	virtual bool remoteDeleteOnDisconnect(const Instance* instance) const;

	ReplicationData& addReplicationData(boost::shared_ptr<Instance> instance, bool listenToChanges);
	void addTopReplicationContainer(Instance* instance, bool deleteOnDisconnect);
	void connectToReplicationContainers();
	void removeFromPendingProperties(const Instance* instance);
	bool removeFromPendingNewInstances(const Instance* instance);
	bool removeFromPendingDeleteInstances(const Instance* instance);
	bool isReplicationContainer(const Instance* instance) const;
	bool sendItems();
	bool sendPhysicsPacket();
	void sendPhysicsData(RakNet::BitStream& bitStream, const Assembly& assembly);
	void sendPhysicsData(RakNet::BitStream& bitStream, const Mechanism& mechanism);
	void createStatsItems(Stats::StatsService* stats);
	void serializeValue(const Reflection::ConstProperty& property, bool includeLength, RakNet::BitStream& outBitStream);
	void deserializeValue(RakNet::BitStream& inBitstream, Reflection::Property& property);
	void skipValue(RakNet::BitStream& inBitStream, ValueType type);
	void receiveData(Packet* packet);
	void readInstanceNew(RakNet::BitStream& inBitstream);
	void readInstanceDelete(RakNet::BitStream& inBitstream);
	void readChangedProperty(RakNet::BitStream& inBitstream);
	void readMarker(RakNet::BitStream& inBitstream);
	SharedStringDictionary& getSharedDictionary(const Reflection::PropertyDescriptor& descriptor);
	PluginReceiveResult OnReceive_ID_TIMESTAMP(Packet* packet);

	virtual bool canSendItems();

	static void writeValueType(bool includeLength, ValueType vt, RakNet::BitStream& outBitStream);

public:
};

class Peer : public Reflection::Described<Peer, sPeer, Instance>,
			 public PluginInterface,
			 public Listener<RunService, Heartbeat>
{
	boost::scoped_ptr<PacketLogger> logger;
	boost::scoped_ptr<Profiling::ThreadProfiler> profilePacketsThread;

protected:
	const boost::scoped_ptr<RakPeer> rakPeer;

	Peer();
	virtual ~Peer();

	virtual bool askAddChild(const Instance* instance) const
	{
		return dynamic_cast<const Replicator*>(instance) != NULL;
	}

	RakPeerInterface* peerInterface();

	void updateLogger();
	void updateNetworkSimulator();

	virtual void onEvent(const RunService* source, Heartbeat event);
	virtual void Update(RakPeerInterface* peer);
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

public:
};

} // namespace Network
} // namespace RBX

#endif // NETWORK_REPLICATOR_H
