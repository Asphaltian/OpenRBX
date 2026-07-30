#ifndef NETWORKIDOBJECT_H
#define NETWORKIDOBJECT_H

#include "RakNetTypes.h"
#include "decomp.h"

class NetworkIDManager;

/// Base class for objects that are addressable by NetworkID across the network.
class NetworkIDObject {
public:
	virtual ~NetworkIDObject();

	virtual void SetNetworkIDManager(NetworkIDManager* manager);
	virtual NetworkIDManager* GetNetworkIDManager(void);
	virtual NetworkID GetNetworkID(void);
	virtual void SetNetworkID(NetworkID id);
	virtual void SetParent(void* object);
	virtual void* GetParent(void) const;

protected:
	NetworkID networkID;
	void* parent;
	unsigned int allocationNumber;
	bool callGenerationCode;
	NetworkIDManager* networkIDManager;
};

DECOMP_SIZE_ASSERT(NetworkIDObject, 0x20)

#endif // NETWORKIDOBJECT_H
