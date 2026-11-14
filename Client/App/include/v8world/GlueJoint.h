#ifndef V8WORLD_GLUEJOINT_H
#define V8WORLD_GLUEJOINT_H

#include "placeholder.h"
#include "util/Face.h"
#include "util/NormalId.h"
#include "v8world/MultiJoint.h"

namespace RBX {

class GlueJoint : public MultiJoint
{
public:
	GlueJoint();
	GlueJoint(Primitive* p0, Primitive* p1, const CoordinateFrame& jointCoord0, const CoordinateFrame& jointCoord1);

	static GlueJoint* canBuildJoint(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1);

private:
	static bool compatibleSurfaces(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1);

	float getMaxForce();

	virtual void putInKernel(Kernel* _kernel);

	virtual JointType getJointType() const { return GLUE_JOINT; }

	virtual bool isBreakable() const { return true; }

	Face overlapInP0;
	Face overlapInP1;
};

} // namespace RBX

#endif // V8WORLD_GLUEJOINT_H
