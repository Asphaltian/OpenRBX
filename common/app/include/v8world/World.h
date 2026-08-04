#ifndef V8WORLD_WORLD_H
#define V8WORLD_WORLD_H

#include "decomp.h"
#include "util/IndexArray.h"
#include "v8world/IWorldStage.h"
#include "v8world/Primitive.h"

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

	typedef IndexArray<Primitive, &Primitive::worldIndexFunc> PrimitiveArray;

	const PrimitiveArray& getPrimitives() const { return primitives; }

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
	undefined m_unk0x00[0x30 - 0x00];  // 0x00
	ContactManager* contactManager;    // 0x30
	JointStage* jointStage;            // 0x34
	G3D::Array<Primitive*> touch;      // 0x38
	G3D::Array<Primitive*> touchOther; // 0x44
	bool canThrottle;                  // 0x50
	bool inStepCode;                   // 0x51
	bool inJointNotification;          // 0x52
	int worldStepId;                   // 0x54

	PrimitiveArray primitives; // 0x58

	undefined m_unk0x64[0x94 - 0x64]; // 0x64
};

DECOMP_SIZE_ASSERT(World, 0x94)

} // namespace RBX

#endif // V8WORLD_WORLD_H
