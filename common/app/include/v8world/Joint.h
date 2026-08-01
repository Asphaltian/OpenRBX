#ifndef V8WORLD_JOINT_H
#define V8WORLD_JOINT_H

#include "decomp.h"
#include "util/NormalId.h"
#include "v8world/Primitive.h"

#include <G3D/CoordinateFrame.h>

namespace RBX {

class IJointOwner;

using G3D::CoordinateFrame;

// SIZE 0x88
class Joint : public Edge
{
public:
	enum JointType
	{
		NO_JOINT = 0,
		ROTATE_JOINT = 1,
		ROTATE_P_JOINT = 2,
		ROTATE_V_JOINT = 3,
		GLUE_JOINT = 4,
		ANCHOR_JOINT = 5,
		WELD_JOINT = 6,
		SNAP_JOINT = 7,
		MOTOR_JOINT = 8,
		FREE_JOINT = 9,
	};

	static bool isJoint(Edge* edge) { return edge->getEdgeType() == JOINT; }

	virtual JointType getJointType() const = 0;                            // vtable+0x14
	virtual bool isBreakable() const;                                      // vtable+0x18
	virtual bool isBroken() const;                                         // vtable+0x1c
	virtual bool joinsFace(Primitive* primitive, NormalId normalId) const; // vtable+0x20
	virtual bool isAligned();                                              // vtable+0x24
	virtual CoordinateFrame align(Primitive* prim0, Primitive* prim1);     // vtable+0x28
	virtual bool canStepUi() const;                                        // vtable+0x2c
	virtual void stepUi(int frameCount);                                   // vtable+0x30

	bool getActive() const { return active; }
	void setActive(bool value) { active = value; }

private:
	IJointOwner* jointOwner;     // 0x20
	bool active;                 // 0x24
	CoordinateFrame jointCoord0; // 0x28
	CoordinateFrame jointCoord1; // 0x58
};

DECOMP_SIZE_ASSERT(Joint, 0x88)

} // namespace RBX

#endif // V8WORLD_JOINT_H
