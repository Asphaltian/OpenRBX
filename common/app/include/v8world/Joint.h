#ifndef V8WORLD_JOINT_H
#define V8WORLD_JOINT_H

#include "decomp.h"
#include "util/NormalId.h"
#include "v8world/Primitive.h"

#include <G3D/CoordinateFrame.h>

namespace RBX {

class IJointOwner;

using G3D::CoordinateFrame;

// VTABLE: WEBSERVICE 0x10247fb0
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

	virtual ~Joint(); // vtable+0x00

	// SYNTHETIC: WEBSERVICE 0x1011ea60
	// RBX::Joint::`scalar deleting destructor'

	// FUNCTION: WEBSERVICE 0x10059d20 FOLDED
	virtual EdgeType getEdgeType() const { return JOINT; } // vtable+0x0c

	virtual void setPrimitive(int index, Primitive* primitive); // vtable+0x10

	static bool isJoint(Edge* edge) { return edge->getEdgeType() == JOINT; }

	// FUNCTION: WEBSERVICE 0x10059d20 FOLDED
	virtual JointType getJointType() const { return NO_JOINT; } // vtable+0x14
	// FUNCTION: WEBSERVICE 0x100eb790 FOLDED
	virtual bool isBreakable() const { return false; } // vtable+0x18

	// FUNCTION: WEBSERVICE 0x100eb790 FOLDED
	virtual bool isBroken() const { return false; }                        // vtable+0x1c
	virtual bool joinsFace(Primitive* primitive, NormalId normalId) const; // vtable+0x20

	// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
	virtual bool isAligned() { return true; } // vtable+0x24

	virtual CoordinateFrame align(Primitive* prim0, Primitive* prim1); // vtable+0x28

	// FUNCTION: WEBSERVICE 0x100eb790 FOLDED
	virtual bool canStepUi() const { return false; } // vtable+0x2c

	virtual void stepUi(int frameCount) {} // vtable+0x30

	void setJointOwner(IJointOwner* value);

	bool getActive() const { return active; }
	void setActive(bool value) { active = value; }

private:
	IJointOwner* jointOwner; // 0x20
	bool active;             // 0x24

protected:
	CoordinateFrame jointCoord0; // 0x28
	CoordinateFrame jointCoord1; // 0x58
};

DECOMP_SIZE_ASSERT(Joint, 0x88)

} // namespace RBX

#endif // V8WORLD_JOINT_H
