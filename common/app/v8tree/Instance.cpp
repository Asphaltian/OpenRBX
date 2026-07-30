#include "v8tree/Instance.h"

namespace RBX {

Instance::Instance() : parent(0), archivable(true)
{
}

Instance::~Instance()
{
}

// FUNCTION: WEBSERVICE 0x10047b20
bool Instance::contains(const Instance* instance) const
{
	while (instance != 0) {
		if (instance == this) {
			return true;
		}

		instance = instance->parent;
	}

	return false;
}

} // namespace RBX
