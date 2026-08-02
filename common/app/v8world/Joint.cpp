#include "v8world/Joint.h"

#include "decomp.h"
#include "v8world/Primitive.h"
#include "v8world/World.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x100d7b80
CoordinateFrame Joint::align(Primitive* prim0, Primitive* prim1)
{
	return CoordinateFrame();
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

// FUNCTION: WEBSERVICE 0x1011eac0
void Joint::setPrimitive(int index, Primitive* primitive)
{
	if (primitive != getPrimitive(index)) {

		World* world = getWorld();

		if (world != NULL && getPrimitive(index) != NULL) {
			world->onJointPrimitiveNulling(this, getPrimitive(index));
		}

		Edge::setPrimitive(index, primitive);

		if (world != NULL && primitive != NULL) {
			world->onJointPrimitiveSet(this, primitive);
		}
	}
}

} // namespace RBX
