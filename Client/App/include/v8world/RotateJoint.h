#ifndef V8WORLD_ROTATEJOINT_H
#define V8WORLD_ROTATEJOINT_H

#include "placeholder.h"
#include "util/NormalId.h"
#include "v8world/MultiJoint.h"
#include "v8world/SurfaceData.h"

namespace RBX {

class RotateConnector;

class RotateJoint : public MultiJoint
{
public:
	RotateJoint();
	RotateJoint(Primitive* prim0, Primitive* prim1, const CoordinateFrame& coord0, const CoordinateFrame& coord1);

	virtual ~RotateJoint();

private:
	virtual void removeFromKernel();

public:
	Primitive* getAxlePrim() { return getPrimitive(0); }
	Primitive* getHolePrim() { return getPrimitive(1); }

	NormalId getAxleId() { return Matrix3ToNormalId(jointCoord0.rotation); }
	NormalId getHoleId() { return normalIdOpposite(Matrix3ToNormalId(jointCoord1.rotation)); }

	static RotateJoint* canBuildJoint(Primitive* p0, Primitive* p1, NormalId nId0, NormalId nId1);

protected:
	float getTorqueArmLength();
	float getChannelValue(int frameCount);

	RotateConnector* rotateConnector;

private:
	static RotateJoint* surfaceTypeToJoint(
		SurfaceType surfaceType,
		Primitive* prim0,
		Primitive* prim1,
		const CoordinateFrame& coord0,
		const CoordinateFrame& coord1
	);

	virtual void putInKernel(Kernel* _kernel);

	virtual JointType getJointType() const { return ROTATE_JOINT; }
};

class RotatePJoint : public RotateJoint
{
public:
	RotatePJoint() {}
	RotatePJoint(Primitive* prim0, Primitive* prim1, const CoordinateFrame& coord0, const CoordinateFrame& coord1);

private:
	virtual JointType getJointType() const { return ROTATE_P_JOINT; }

	virtual bool canStepUi() const { return true; }

	virtual void stepUi(int uiStepId);
};

class RotateVJoint : public RotateJoint
{
public:
	RotateVJoint() {}
	RotateVJoint(Primitive* prim0, Primitive* prim1, const CoordinateFrame& coord0, const CoordinateFrame& coord1);

private:
	virtual JointType getJointType() const { return ROTATE_V_JOINT; }

	virtual bool canStepUi() const { return true; }

	virtual void stepUi(int frameCount);
};

} // namespace RBX

#endif // V8WORLD_ROTATEJOINT_H
