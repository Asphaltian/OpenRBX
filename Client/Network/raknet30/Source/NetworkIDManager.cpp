#include "NetworkIDManager.h"

#include "NetworkIDObject.h"

void* NetworkIDManager::GET_OBJECT_FROM_ID(NetworkID x)
{
	if (x.localSystemAddress == 65535) {
		return 0;
	}

	if (IDArray[x.localSystemAddress]) {
		if (IDArray[x.localSystemAddress]->GetParent()) {
			return IDArray[x.localSystemAddress]->GetParent();
		}

		return IDArray[x.localSystemAddress];
	}

	return 0;
}
