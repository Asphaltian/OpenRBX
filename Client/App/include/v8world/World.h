#ifndef V8WORLD_WORLD_H
#define V8WORLD_WORLD_H

#include "placeholder.h"
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
class CollisionStage;
class Contact;
class ContactManager;
class Joint;
class JointStage;
class Kernel;
class MotorJoint;
class Primitive;
class SimJobStage;
class SleepStage;

struct AutoJoin
{
	AutoJoin(Joint* joint) : joint(joint) {}

	Joint* joint;
};

struct AutoDestroy
{
	AutoDestroy(Joint* joint) : joint(joint) {}

	Joint* joint;
};

class World : public Notifier<World, AutoJoin>, public Notifier<World, AutoDestroy>
{
public:
	ContactManager& getContactManager() { return *contactManager; }

	World();

	virtual ~World();

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

	const G3D::Array<Primitive*>& getPrimitives() const { return primitives.underlyingArray(); }

	Kernel& getKernel();
	const Kernel& getKernel() const;

	ClumpStage* getClumpStage();
	CollisionStage* getCollisionStage();
	const CollisionStage* getCollisionStage() const;

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

	const G3D::Array<Primitive*>& getTouch() const { return touch; }
	const G3D::Array<Primitive*>& getTouchOther() const { return touchOther; }

	void setCanThrottle(bool value) { canThrottle = value; }
	void reset() { worldStepId = 0; }
	int getWorldStepId();

	const Profiling::CodeProfiler& getProfileWorldStep() const { return *profilingWorldStep; }
	Profiling::CodeProfiler& getProfileWorldStep() { return *profilingWorldStep; }
	const Profiling::CodeProfiler& getProfileBroadphase() const { return *profilingBroadphase; }
	const Profiling::CodeProfiler& getProfileUiStep() const { return *profilingUiStep; }

	void onJointPrimitiveNulling(Joint* joint, Primitive* primitive);
	void onJointPrimitiveSet(Joint* joint, Primitive* primitive);
	void assertNotInStep();
	void assertInStep();

private:
	void createJoints(Primitive* p, std::set<Primitive*>* ignoreGroup);
	void destroyJoints(Primitive* p, std::set<Primitive*>* ignoreGroup);

	void destroyJoint(Joint* joint);

	void removeFromBreakable(Joint* j);

	void doBreakJoints();

	ContactManager* contactManager;
	JointStage* jointStage;
	G3D::Array<Primitive*> touch;
	G3D::Array<Primitive*> touchOther;
	bool canThrottle;
	bool inStepCode;
	bool inJointNotification;
	int worldStepId;

	PrimitiveArray primitives;

	std::set<Joint*> breakableJoints;

	int numJoints;
	int numContacts;
	int numLinkCalls;

	G3D::Array<Primitive*> tempPrimitives;

	boost::scoped_ptr<Profiling::CodeProfiler> profilingWorldStep;
	boost::scoped_ptr<Profiling::CodeProfiler> profilingUiStep;
	boost::scoped_ptr<Profiling::CodeProfiler> profilingBroadphase;
};

} // namespace RBX

#endif // V8WORLD_WORLD_H
