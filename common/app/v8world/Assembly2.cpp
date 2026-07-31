#include "v8world/Assembly2.h"

#include <cstddef>

namespace RBX {

// FUNCTION: WEBSERVICE 0x10102ac0 FOLDED
Assembly* Assembly::getRootAssembly()
{
	return parent != NULL ? parent->getRootAssembly() : this;
}

} // namespace RBX
