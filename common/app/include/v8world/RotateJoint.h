#ifndef V8WORLD_ROTATEJOINT_H
#define V8WORLD_ROTATEJOINT_H

#include "decomp.h"
#include "util/NormalId.h"
#include "v8world/MultiJoint.h"
#include "v8world/SurfaceData.h"

namespace RBX {

class RotateConnector;

// VTABLE: WEBSERVICE 0x10248028
// SIZE 0xc4
class RotateJoint : public MultiJoint
{
public:
	RotateJoint();
	RotateJoint(Primitive* prim0, Primitive* prim1, const CoordinateFrame& coord0, const CoordinateFrame& coord1);

	virtual ~RotateJoint(); // vtable+0x00

	// SYNTHETIC: WEBSERVICE 0x1011f0b0
	// RBX::RotateJoint::`scalar deleting destructor'

	virtual void removeFromKernel(); // vtable+0x08

	Primitive* getAxlePrim() const { return getPrimitive(0); }
	Primitive* getHolePrim() const { return getPrimitive(1); }

	NormalId getAxleId() const { return Matrix3ToNormalId(jointCoord0.rotation); }
	NormalId getHoleId() const { return normalIdOpposite(Matrix3ToNormalId(jointCoord1.rotation)); }

	static RotateJoint* canBuildJoint(Primitive* p0, Primitive* p1, NormalId nId0, NormalId nId1);

protected:
	float getTorqueArmLength();
	float getChannelValue(int frameCount);

	RotateConnector* rotateConnector; // 0xc0

private:
	static RotateJoint* surfaceTypeToJoint(
		SurfaceType surfaceType,
		Primitive* prim0,
		Primitive* prim1,
		const CoordinateFrame& coord0,
		const CoordinateFrame& coord1
	);

	virtual void putInKernel(Kernel* _kernel); // vtable+0x04

	// FUNCTION: WEBSERVICE 0x10108040 FOLDED
	virtual JointType getJointType() const { return ROTATE_JOINT; } // vtable+0x14
};

DECOMP_SIZE_ASSERT(RotateJoint, 0xc4)

// VTABLE: WEBSERVICE 0x1023cf98
// SIZE 0xc4
class RotatePJoint : public RotateJoint
{
public:
	RotatePJoint() {}
	RotatePJoint(Primitive* prim0, Primitive* prim1, const CoordinateFrame& coord0, const CoordinateFrame& coord1);

	// SYNTHETIC: WEBSERVICE 0x100d7ba0
	// RBX::RotateVJoint::`scalar deleting destructor'

private:
	// FUNCTION: WEBSERVICE 0x100a7740 FOLDED
	virtual JointType getJointType() const { return ROTATE_P_JOINT; } // vtable+0x14

	// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
	virtual bool canStepUi() const { return true; } // vtable+0x2c

	virtual void stepUi(int uiStepId); // vtable+0x30
};

DECOMP_SIZE_ASSERT(RotatePJoint, 0xc4)

// VTABLE: WEBSERVICE 0x1023cfd0
// SIZE 0xc4
class RotateVJoint : public RotateJoint
{
public:
	RotateVJoint() {}
	RotateVJoint(Primitive* prim0, Primitive* prim1, const CoordinateFrame& coord0, const CoordinateFrame& coord1);

private:
	// FUNCTION: WEBSERVICE 0x100d7a70 FOLDED
	virtual JointType getJointType() const { return ROTATE_V_JOINT; } // vtable+0x14

	// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
	virtual bool canStepUi() const { return true; } // vtable+0x2c

	virtual void stepUi(int frameCount); // vtable+0x30
};

DECOMP_SIZE_ASSERT(RotateVJoint, 0xc4)

} // namespace RBX

#endif // V8WORLD_ROTATEJOINT_H
