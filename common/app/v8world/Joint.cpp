#include "v8world/Joint.h"

#include "decomp.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x1011e880
void Joint::setJointOwner(IJointOwner* value)
{
	jointOwner = value;
}

} // namespace RBX
