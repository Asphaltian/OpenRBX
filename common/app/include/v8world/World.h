#ifndef V8WORLD_WORLD_H
#define V8WORLD_WORLD_H

#include "decomp.h"

namespace RBX {

class Assembly;
class Primitive;

// SIZE 0x94
class World
{
public:
	void onPrimitiveExtentsChanged(Primitive* primitive);
	void onPrimitiveGeometryTypeChanged(Primitive* primitive);
	void onPrimitiveContactParametersChanged(Primitive* primitive);
	void onPrimitiveCanSleepChanged(Primitive* primitive);
	void onPrimitiveAddedAnchor(Primitive* primitive);
	void onPrimitiveRemovedAnchor(Primitive* primitive);
	void onPrimitiveCanCollideChanged(Primitive* primitive);

	void onAssemblyExtentsChanged(Assembly* assembly);

	void ticklePrimitive(Primitive* primitive, bool value);

	void onPrimitiveTouched(Primitive* p0, Primitive* p1);

private:
	undefined m_unk0x00[0x94 - 0x00]; // 0x00
};

DECOMP_SIZE_ASSERT(World, 0x94)

} // namespace RBX

#endif // V8WORLD_WORLD_H
