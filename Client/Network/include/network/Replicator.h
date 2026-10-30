#ifndef NETWORK_REPLICATOR_H
#define NETWORK_REPLICATOR_H

#include "Streaming.h"
#include "decomp.h"
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

// VTABLE: WEBSERVICE 0x10250ba4
// VTABLE: ROBLOX 0x00813b1c
// SIZE 0x10
class Marker : public DescribedNonCreatable<Marker, Reflection::DescribedBase, sMarker>, private boost::noncopyable
{
	bool returned; // 0x0c

	Marker() : returned(false) {}

public:
	// FUNCTION: WEBSERVICE 0x101a9b70
	// FUNCTION: ROBLOX 0x004d2ca0
	static boost::shared_ptr<Marker> newMarker() { return boost::shared_ptr<Marker>(new Marker()); }

	long id() const { return reinterpret_cast<long>(this); }
	static Marker* fromId(long id) { return reinterpret_cast<Marker*>(id); }
	void fireReturned() { event_Returned.fire(this); }

	static Reflection::SignalDesc<Marker, void()> event_Returned;
	static Reflection::SignalDesc<Marker, void(float)> event_Progress;

	// SYNTHETIC: WEBSERVICE 0x101a89d0
	// SYNTHETIC: ROBLOX 0x004d1950
	// RBX::Network::Marker::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(Marker, 0x10)

// SIZE 0x2c30
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

	double lastSendTime;                              // 0xf28
	MechanismTracker jobStagePos;                     // 0xf30
	double lastCharacterSendTime;                     // 0xf38
	boost::shared_ptr<ReplicatorStatsItem> statsItem; // 0xf40

	typedef std::map<const Reflection::PropertyDescriptor*, boost::shared_ptr<SharedStringDictionary> > Strings;

	Strings strings;                   // 0xf48
	SharedStringDictionary classNames; // 0xf54
	SharedStringDictionary propNames;  // 0x1d74

protected:
	// SIZE 0x2c
	struct ReplicationData
	{
		boost::shared_ptr<Instance> instance;  // 0x00
		boost::signals::connection childAdded; // 0x08
		boost::signals::connection changed;    // 0x18
		bool deleteOnDisconnect;               // 0x28

		// SYNTHETIC: WEBSERVICE 0x1019cc70
		// SYNTHETIC: ROBLOX 0x004c3610
		// RBX::Network::Replicator::ReplicationData::~ReplicationData

		// SYNTHETIC: WEBSERVICE 0x1019ce80
		// SYNTHETIC: ROBLOX 0x004c3830
		// RBX::Network::Replicator::ReplicationData::ReplicationData
	};

	typedef std::map<const Instance*, ReplicationData> RepConts;

	RepConts replicationContainers; // 0x2b94

	typedef std::vector<Instance*> TopReplConts;

	TopReplConts topReplicationContainers; // 0x2ba0
	Players* players;                      // 0x2bb0

	typedef std::list<boost::shared_ptr<Item> > ItemQueue;

	ItemQueue pendingItems; // 0x2bb4

	typedef std::set<const Instance*> PendingInstances;

	PendingInstances pendingNewInstances;                   // 0x2bc0
	PendingInstances pendingDeleteInstances;                // 0x2bcc
	bool sendPhysicsEnabled;                                // 0x2bd8
	const Reflection::Property* deserializingProperty;      // 0x2bdc
	Instance* removingInstance;                             // 0x2be0
	std::queue<boost::shared_ptr<Marker> > incomingMarkers; // 0x2be4

	typedef std::map<const Name*, boost::shared_ptr<Instance> > DefaultObjects;

	DefaultObjects defaultObjects;                                   // 0x2bf8
	boost::scoped_ptr<Profiling::CodeProfiler> profileReplication;   // 0x2c04
	boost::scoped_ptr<Profiling::CodeProfiler> profileDataListening; // 0x2c08
	boost::scoped_ptr<Profiling::CodeProfiler> profileDataIn;        // 0x2c0c
	boost::scoped_ptr<Profiling::CodeProfiler> profileDataOut;       // 0x2c10
	boost::scoped_ptr<Profiling::CodeProfiler> profilePhysicsIn;     // 0x2c14
	boost::scoped_ptr<Profiling::CodeProfiler> profilePhysicsOut;    // 0x2c18

	virtual Player* findTargetPlayer(); // vtable+0x40

	Mechanism* findTargetPlayerCharacterMechanism();

private:
	bool receivedGlobals; // 0x2c1c

public:
	bool disconnected;                  // 0x2c1d
	RakPeerInterface* peer;             // 0x2c20
	const SystemAddress remotePlayerId; // 0x2c24

	Replicator(SystemAddress remotePlayerId, RakPeerInterface* peer);
	virtual ~Replicator();

	// FUNCTION: WEBSERVICE 0x1019f930
	// STUB: ROBLOX 0x004c64e0
	boost::shared_ptr<Instance> getPlayer() { return shared_from(findTargetPlayer()); }

	static Reflection::BoundProp<int, 1> prop_maxDataModelSendBuffer;
	static Reflection::SignalDesc<Replicator, void(std::string, bool)> event_Disconnection;

	virtual void Update(RakPeerInterface* peer);
	virtual PluginReceiveResult OnReceive(RakPeerInterface* peer, Packet* packet);

	// FUNCTION: WEBSERVICE 0x100c7730 FOLDED
	// FUNCTION: ROBLOX 0x0040ddc0 FOLDED
	virtual const Name& getClassName() const { return Name::getNullName(); }

	// FUNCTION: WEBSERVICE 0x10059d20 FOLDED
	// FUNCTION: ROBLOX 0x00407390 FOLDED
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

	virtual bool wantReplicate(const Instance* source) const; // vtable+0x44
	virtual void onSentMarker(long id);                       // vtable+0x48

	void readProperties(RakNet::BitStream& inBitstream, Instance* instance);
	void writeProperties(Instance* instance, RakNet::BitStream& outBitStream);

private:
	virtual bool remoteDeleteOnDisconnect(const Instance* instance) const; // vtable+0x4c

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

	virtual bool canSendItems(); // vtable+0x50

	static void writeValueType(bool includeLength, ValueType vt, RakNet::BitStream& outBitStream);

public:
	// SYNTHETIC: WEBSERVICE 0x101aa560
	// RBX::Network::Replicator::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(Replicator, 0x2c30)

// clang-format off
// TEMPLATE: WEBSERVICE 0x101a3850
// TEMPLATE: ROBLOX 0x004cbc80
// RBX::Reflection::Described<RBX::Network::Replicator,&RBX::Network::sReplicator,RBX::NonFactoryProduct<RBX::Network::IdSerializer,&RBX::Network::sReplicator> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x101a9f90
// TEMPLATE: ROBLOX 0x004d30d0
// RBX::NonFactoryProduct<RBX::Network::IdSerializer,&RBX::Network::sReplicator>::NonFactoryProduct<RBX::Network::IdSerializer,&RBX::Network::sReplicator>
// TEMPLATE: WEBSERVICE 0x101aa0d0
// TEMPLATE: ROBLOX 0x004d3200
// RBX::NonFactoryProduct<RBX::Network::IdSerializer,&RBX::Network::sReplicator>::getClassName
// TEMPLATE: WEBSERVICE 0x101aa510
// TEMPLATE: ROBLOX 0x004d3630
// RBX::Reflection::Described<RBX::Network::Replicator,&RBX::Network::sReplicator,RBX::NonFactoryProduct<RBX::Network::IdSerializer,&RBX::Network::sReplicator> >::Described<RBX::Network::Replicator,&RBX::Network::sReplicator,RBX::NonFactoryProduct<RBX::Network::IdSerializer,&RBX::Network::sReplicator> >
// TEMPLATE: WEBSERVICE 0x101aa580
// TEMPLATE: ROBLOX 0x004d36a0
// RBX::DescribedNonCreatable<RBX::Network::Replicator,RBX::Network::IdSerializer,&RBX::Network::sReplicator>::DescribedNonCreatable<RBX::Network::Replicator,RBX::Network::IdSerializer,&RBX::Network::sReplicator>
// SYNTHETIC: WEBSERVICE 0x102262a0
// SYNTHETIC: ROBLOX 0x007f10b0
// `RBX::Reflection::Described<RBX::Network::Replicator,&RBX::Network::sReplicator,RBX::NonFactoryProduct<RBX::Network::IdSerializer,&RBX::Network::sReplicator> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// clang-format on

// SIZE 0x10c
class Peer : public Reflection::Described<Peer, sPeer, Instance>,
			 public PluginInterface,
			 public Listener<RunService, Heartbeat>
{
	boost::scoped_ptr<PacketLogger> logger;                            // 0x100
	boost::scoped_ptr<Profiling::ThreadProfiler> profilePacketsThread; // 0x104

protected:
	const boost::scoped_ptr<RakPeer> rakPeer; // 0x108

	Peer();
	virtual ~Peer();

	// FUNCTION: WEBSERVICE 0x10196760
	// FUNCTION: ROBLOX 0x004b8940
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
	// SYNTHETIC: WEBSERVICE 0x101a13b0
	// RBX::Network::Peer::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(Peer, 0x10c)

// clang-format off
// TEMPLATE: WEBSERVICE 0x1018ba90
// TEMPLATE: ROBLOX 0x004b84d0
// RBX::Reflection::Described<RBX::Network::Peer,&RBX::Network::sPeer,RBX::Instance>::classDescriptor
// TEMPLATE: WEBSERVICE 0x101a43d0
// TEMPLATE: ROBLOX 0x004ccbc0
// RBX::Reflection::Described<RBX::Network::Peer,&RBX::Network::sPeer,RBX::Instance>::Described<RBX::Network::Peer,&RBX::Network::sPeer,RBX::Instance>
// SYNTHETIC: WEBSERVICE 0x1021fb20
// `dynamic initializer for 'RBX::Network::Replicator::event_Disconnection''
// SYNTHETIC: WEBSERVICE 0x1021fb50
// `dynamic initializer for 'func_SendMarker''
// SYNTHETIC: WEBSERVICE 0x1021fb80
// `dynamic initializer for 'func_requestCharacter''
// SYNTHETIC: WEBSERVICE 0x1021fbb0
// `dynamic initializer for 'func_closeConnection''
// SYNTHETIC: WEBSERVICE 0x1021fbe0
// `dynamic initializer for 'prop_RemotePlayer''
// SYNTHETIC: WEBSERVICE 0x1021fc10
// `dynamic initializer for 'RBX::Network::Marker::event_Returned''
// SYNTHETIC: WEBSERVICE 0x10225d70
// SYNTHETIC: ROBLOX 0x007f0e60
// `RBX::Reflection::Described<RBX::Network::Peer,&RBX::Network::sPeer,RBX::Instance>::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102262c0
// `dynamic atexit destructor for 'func_closeConnection''
// SYNTHETIC: WEBSERVICE 0x102262d0
// `dynamic atexit destructor for 'func_requestCharacter''
// SYNTHETIC: WEBSERVICE 0x102262e0
// `dynamic atexit destructor for 'prop_RemotePlayer''
// SYNTHETIC: WEBSERVICE 0x102262f0
// `dynamic atexit destructor for 'func_SendMarker''
// SYNTHETIC: WEBSERVICE 0x10226300
// `dynamic atexit destructor for 'RBX::Network::Marker::event_Returned''
// SYNTHETIC: WEBSERVICE 0x10226310
// `dynamic atexit destructor for 'RBX::Network::Replicator::event_Disconnection''
// clang-format on

} // namespace Network
} // namespace RBX

#endif // NETWORK_REPLICATOR_H
