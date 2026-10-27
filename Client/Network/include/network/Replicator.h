#ifndef NETWORK_REPLICATOR_H
#define NETWORK_REPLICATOR_H

#include "decomp.h"
#include "reflection/reflection.h"
#include "util/Events.h"
#include "util/RunStateOwner.h"
#include "v8tree/Instance.h"

#include <PluginInterface.h>
#include <boost/scoped_ptr.hpp>

class PacketLogger;
class RakPeer;
class RakPeerInterface;

namespace RBX {

class RunService;
class ServiceProvider;

namespace Profiling {
class ThreadProfiler;
}

namespace Network {

extern const char sPeer[];
extern const char sReplicator[];

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

	virtual bool askAddChild(const Instance* instance) const;

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
// RBX::Reflection::Described<RBX::Network::Peer,&RBX::Network::sPeer,RBX::Instance>::classDescriptor
// TEMPLATE: WEBSERVICE 0x101a43d0
// RBX::Reflection::Described<RBX::Network::Peer,&RBX::Network::sPeer,RBX::Instance>::Described<RBX::Network::Peer,&RBX::Network::sPeer,RBX::Instance>
// SYNTHETIC: WEBSERVICE 0x10225d70
// `RBX::Reflection::Described<RBX::Network::Peer,&RBX::Network::sPeer,RBX::Instance>::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// clang-format on

} // namespace Network
} // namespace RBX

#endif // NETWORK_REPLICATOR_H
