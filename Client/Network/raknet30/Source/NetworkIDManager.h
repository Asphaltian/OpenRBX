#ifndef NETWORKIDMANAGER_H
#define NETWORKIDMANAGER_H

#include "RakNetTypes.h"
#include "placeholder.h"

class NetworkIDObject;

/// Maps NetworkIDs to the objects they name.
class NetworkIDManager {
public:
	virtual ~NetworkIDManager();

	void* GET_OBJECT_FROM_ID(NetworkID x);

protected:
	SystemAddress externalSystemAddress;
	unsigned short sharedNetworkID;
	bool isNetworkIDAuthority;
	bool calledSetIsNetworkIDAuthority;
	NetworkIDObject** IDArray;
};

#endif // NETWORKIDMANAGER_H
