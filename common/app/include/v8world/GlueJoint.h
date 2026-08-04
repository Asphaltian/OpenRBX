#ifndef V8WORLD_GLUEJOINT_H
#define V8WORLD_GLUEJOINT_H

#include "decomp.h"
#include "util/Face.h"
#include "util/NormalId.h"
#include "v8world/MultiJoint.h"

namespace RBX {

// VTABLE: WEBSERVICE 0x10248114
// SIZE 0x120
class GlueJoint : public MultiJoint
{
public:
	GlueJoint();
	GlueJoint(Primitive* prim0, Primitive* prim1, const CoordinateFrame& coord0, const CoordinateFrame& coord1);

	// SYNTHETIC: WEBSERVICE 0x10120320
	// RBX::GlueJoint::`scalar deleting destructor'

	static GlueJoint* canBuildJoint(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1);

private:
	static bool compatibleSurfaces(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1);

	float getMaxForce();

	virtual void putInKernel(Kernel* _kernel); // vtable+0x04

	// FUNCTION: WEBSERVICE 0x10120280
	virtual JointType getJointType() const { return GLUE_JOINT; } // vtable+0x14

	// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
	virtual bool isBreakable() const { return true; } // vtable+0x18

	Face overlapInP0; // 0xc0
	Face overlapInP1; // 0xf0
};

DECOMP_SIZE_ASSERT(GlueJoint, 0x120)

} // namespace RBX

#endif // V8WORLD_GLUEJOINT_H
