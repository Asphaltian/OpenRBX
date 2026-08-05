#ifndef V8WORLD_WORLD_H
#define V8WORLD_WORLD_H

#include "decomp.h"
#include "util/Events.h"
#include "util/IndexArray.h"
#include "util/Profiling.h"
#include "v8world/IWorldStage.h"
#include "v8world/Primitive.h"

#include <boost/scoped_ptr.hpp>
#include <set>

namespace RBX {

class Assembly;
class ClumpStage;
class Contact;
class ContactManager;
class Joint;
class MotorJoint;
class JointStage;
class Kernel;
class Primitive;
class SimJobStage;
class SleepStage;

// SIZE 0x04
struct AutoJoin
{
	AutoJoin(Joint* joint) : joint(joint) {}

	Joint* joint; // 0x00
};

// SIZE 0x04
struct AutoDestroy
{
	AutoDestroy(Joint* joint) : joint(joint) {}

	Joint* joint; // 0x00
};

// clang-format off
// VTABLE: WEBSERVICE 0x1023c550 RBX::Notifier<RBX::World, struct RBX::AutoDestroy>
// VTABLE: WEBSERVICE 0x1023c560 RBX::Notifier<RBX::World, struct RBX::AutoJoin>
// clang-format on
// SIZE 0x94
class World : public Notifier<World, AutoJoin>, public Notifier<World, AutoDestroy>
{
public:
	World();

	virtual ~World(); // vtable+0x00

	// SYNTHETIC: WEBSERVICE 0x100d0790
	// RBX::World::`scalar deleting destructor'

	static bool disableEnvironmentalThrottle;

	void onPrimitiveExtentsChanged(Primitive* p);
	void onPrimitiveGeometryTypeChanged(Primitive* p);
	void onPrimitiveContactParametersChanged(Primitive* p);
	void onPrimitiveCanSleepChanged(Primitive* p);
	void onPrimitiveAddedAnchor(Primitive* p);
	void onPrimitiveRemovedAnchor(Primitive* p);
	void onPrimitiveCanCollideChanged(Primitive* p);

	void onMotorAngleChanged(MotorJoint* motorJoint);

	void onAssemblyExtentsChanged(Assembly* assembly);

	void ticklePrimitive(Primitive* p, bool value);

	void onPrimitiveTouched(Primitive* touchP, Primitive* touchOtherP);

	void computeFallen(G3D::Array<Primitive*>& fallen) const;

	typedef IndexArray<Primitive, &Primitive::worldIndexFunc> PrimitiveArray;

	const G3D::Array<Primitive*>& getPrimitives() const { return primitives.getArray(); }

	Kernel* getKernel() const;

	ClumpStage* getClumpStage();

	SimJobStage& getSimJobStage();

	SleepStage* getSleepStage();
	const SleepStage* getSleepStage() const;

	float step(float desiredInterval);

	void update();

	void addedBodyForce();

	void joinAll();

	void createJointsToWorld(const G3D::Array<Primitive*>& primitives);
	void destroyJointsToWorld(const G3D::Array<Primitive*>& primitives);

	void insertPrimitive(Primitive* p);
	void removePrimitive(Primitive* p);

	void createJoints(Primitive* p);
	void destroyJoints(Primitive* p);

	int getNumBodies() const;
	int getNumPoints() const;
	int getNumConstraints() const;

	int getNumHashNodes() const;
	int getMaxBucketSize() const;

	int getNumLinkCalls() const { return numLinkCalls; }

	int getNumContacts() const { return numContacts; }

	int getNumPrimitives() const { return primitives.size(); }

	int getNumJoints() const { return numJoints; }

	int getMetric(IWorldStage::MetricType metricType) const;

	void insertJoint(Joint* j);
	void removeJoint(Joint* j);

	void insertContact(Contact* c);
	void destroyContact(Contact* c);

	void onJointPrimitiveNulling(Joint* joint, Primitive* primitive);
	void onJointPrimitiveSet(Joint* joint, Primitive* primitive);

private:
	void createJoints(Primitive* p, std::set<Primitive*>* ignoreGroup);
	void destroyJoints(Primitive* p, std::set<Primitive*>* ignoreGroup);

	void destroyJoint(Joint* joint);

	void removeFromBreakable(Joint* j);

	void doBreakJoints();

	ContactManager* contactManager;    // 0x30
	JointStage* jointStage;            // 0x34
	G3D::Array<Primitive*> touch;      // 0x38
	G3D::Array<Primitive*> touchOther; // 0x44
	bool canThrottle;                  // 0x50
	bool inStepCode;                   // 0x51
	bool inJointNotification;          // 0x52
	int worldStepId;                   // 0x54

	PrimitiveArray primitives; // 0x58

	std::set<Joint*> breakableJoints; // 0x64

	int numJoints;    // 0x70
	int numContacts;  // 0x74
	int numLinkCalls; // 0x78

	G3D::Array<Primitive*> tempPrimitives; // 0x7c

	boost::scoped_ptr<Profiling::CodeProfiler> profilingWorldStep;  // 0x88
	boost::scoped_ptr<Profiling::CodeProfiler> profilingUiStep;     // 0x8c
	boost::scoped_ptr<Profiling::CodeProfiler> profilingBroadphase; // 0x90
};

DECOMP_SIZE_ASSERT(World, 0x94)

} // namespace RBX

#endif // V8WORLD_WORLD_H
