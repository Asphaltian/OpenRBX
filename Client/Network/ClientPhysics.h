#ifndef NETWORK_CLIENTPHYSICS_H
#define NETWORK_CLIENTPHYSICS_H

#include "decomp.h"
#include "util/RunStateOwner.h"

#include <G3D/CoordinateFrame.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace RBX {

class PartInstance;

namespace Network {

// VTABLE: WEBSERVICE 0x10251ab4
// VTABLE: ROBLOX 0x008149c0
// SIZE 0x50
class ClientPhysics : public Listener<RunService, Heartbeat>
{
private:
	double lastUpdateTime;                  // 0x08
	boost::weak_ptr<RunService> runService; // 0x10
	PartInstance* part;                     // 0x18
	G3D::CoordinateFrame lastPos;           // 0x1c

public:
	ClientPhysics(PartInstance* part);

	void update(const boost::shared_ptr<RunService>& runService);
	void sleep();

	virtual ~ClientPhysics(); // vtable+0x04

	// SYNTHETIC: WEBSERVICE 0x101af520
	// RBX::Network::ClientPhysics::`scalar deleting destructor'

protected:
	virtual void onEvent(const RunService* source, Heartbeat event); // vtable+0x00
};

DECOMP_SIZE_ASSERT(ClientPhysics, 0x50)

} // namespace Network
} // namespace RBX

#endif // NETWORK_CLIENTPHYSICS_H
