#ifndef V8WORLD_WORLD_H
#define V8WORLD_WORLD_H

#include "decomp.h"
#include "v8world/IWorldStage.h"

namespace RBX {

class Assembly;
class ContactManager;
class Joint;
class MotorJoint;
class JointStage;
class Kernel;
class Primitive;

// SIZE 0x94
class World
{
public:
	static bool disableEnvironmentalThrottle;

	void onPrimitiveExtentsChanged(Primitive* primitive);
	void onPrimitiveGeometryTypeChanged(Primitive* primitive);
	void onPrimitiveContactParametersChanged(Primitive* primitive);
	void onPrimitiveCanSleepChanged(Primitive* primitive);
	void onPrimitiveAddedAnchor(Primitive* primitive);
	void onPrimitiveRemovedAnchor(Primitive* primitive);
	void onPrimitiveCanCollideChanged(Primitive* primitive);

	void onMotorAngleChanged(MotorJoint* motorJoint);

	void onAssemblyExtentsChanged(Assembly* assembly);

	void ticklePrimitive(Primitive* primitive, bool value);

	void onPrimitiveTouched(Primitive* p0, Primitive* p1);

	Kernel* getKernel() const;

	int getNumHashNodes() const;
	int getMaxBucketSize() const;

	int getNumBodies() const;
	int getNumPoints() const;
	int getNumConstraints() const;

	int getMetric(IWorldStage::MetricType metricType) const;

	void insertJoint(Joint* joint);
	void removeJoint(Joint* joint);

	void onJointPrimitiveNulling(Joint* joint, Primitive* primitive);
	void onJointPrimitiveSet(Joint* joint, Primitive* primitive);

private:
	undefined m_unk0x00[0x30 - 0x00]; // 0x00
	ContactManager* contactManager;   // 0x30
	JointStage* jointStage;           // 0x34
	undefined m_unk0x38[0x94 - 0x38]; // 0x38
};

DECOMP_SIZE_ASSERT(World, 0x94)

} // namespace RBX

#endif // V8WORLD_WORLD_H
