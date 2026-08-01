#include "v8world/Joint.h"

#include "decomp.h"
#include "v8world/Primitive.h"

namespace RBX {

// STUB: WEBSERVICE 0x100d7b80
CoordinateFrame Joint::align(Primitive* prim0, Primitive* prim1)
{
	return prim1->getCoordinateFrame();
}

// FUNCTION: WEBSERVICE 0x101019b0 FOLDED
bool Joint::joinsFace(Primitive* primitive, NormalId normalId) const
{
	return false;
}

// FUNCTION: WEBSERVICE 0x1011e870
Joint::~Joint()
{
}

// FUNCTION: WEBSERVICE 0x1011e880
void Joint::setJointOwner(IJointOwner* value)
{
	jointOwner = value;
}

// STUB: WEBSERVICE 0x1011eac0
void Joint::setPrimitive(int index, Primitive* primitive)
{
	STUB(0x1011eac0);
}

} // namespace RBX
