#include "v8world/Assembly2.h"

#include "decomp.h"

#include <cstddef>

namespace RBX {

// FUNCTION: WEBSERVICE 0x10102ac0 FOLDED
Assembly* Assembly::getRootAssembly()
{
	return parent != NULL ? parent->getRootAssembly() : this;
}

// STUB: WEBSERVICE 0x10102b60
Primitive* Assembly::getAssemblyPrimitive()
{
	STUB(0x10102b60);
	return NULL;
}

// STUB: WEBSERVICE 0x10102bf0
void Assembly::notifyMoved()
{
	STUB(0x10102bf0);
}

} // namespace RBX
