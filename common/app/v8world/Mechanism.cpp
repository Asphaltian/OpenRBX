#include "v8world/Mechanism.h"

#include "decomp.h"
#include "v8world/Assembly2.h"
#include "v8world/Primitive.h"

#include <cstddef>

namespace RBX {

// FUNCTION: WEBSERVICE 0x10127280
Mechanism* Mechanism::getMechanismFromPrimitive(Primitive* primitive)
{
	if (primitive != NULL) {
		Assembly* assembly = primitive->getAssembly();

		if (assembly != NULL) {
			return assembly->getMechanism();
		}
	}

	return NULL;
}

// STUB: WEBSERVICE 0x101272a0
bool MechanismTracker::tracking() const
{
	return mechanism != NULL;
}

} // namespace RBX
