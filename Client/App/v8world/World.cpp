#include "v8world/World.h"

#include "placeholder.h"
#include "util/standardout.h"
#include "v8kernel/Constants.h"
#include "v8kernel/Kernel.h"
#include "v8world/Assembly2.h"
#include "v8world/Clump2.h"
#include "v8world/ClumpStage2.h"
#include "v8world/Contact.h"
#include "v8world/Joint.h"
#include "v8world/JointBuilder.h"
#include "v8world/JointStage.h"
#include "v8world/SimJobStage.h"
#include "v8world/SleepStage2.h"
#include "v8world/SpatialHash.h"

#include <G3D/System.h>
#include <G3D/g3dmath.h>
#include <algorithm>
#include <cmath>

namespace RBX {

bool World::disableEnvironmentalThrottle;

Kernel& World::getKernel()
{
	return *jointStage->getKernel();
}

const Kernel& World::getKernel() const
{
	return *jointStage->getKernel();
}

int World::getNumBodies() const
{
	return getKernel().numBodies();
}

int World::getNumPoints() const
{
	return getKernel().numPoints();
}

int World::getNumConstraints() const
{
	return getKernel().numConnectors();
}

int World::getMetric(IWorldStage::MetricType metricType) const
{
	return jointStage->getMetric(metricType);
}

int World::getNumHashNodes() const
{
	return contactManager->getSpatialHash().getNodesOut();
}

int World::getMaxBucketSize() const
{
	return contactManager->getSpatialHash().getMaxBucket();
}

void World::onPrimitiveContactParametersChanged(Primitive* p)
{
	Contact* c = p->getFirstContact();

	while (c != NULL) {
		c->onPrimitiveContactParametersChanged();
		c = p->getNextContact(c);
	}
}

void World::onPrimitiveExtentsChanged(Primitive* p)
{
	contactManager->onPrimitiveExtentsChanged(p);
}

void World::onAssemblyExtentsChanged(Assembly* a)
{
	PrimIterator pEnd = a->assemblyPrimEnd();

	for (PrimIterator it = a->assemblyPrimBegin(); it != pEnd; ++it) {
		contactManager->onPrimitiveExtentsChanged(*it);
	}
}

void World::onPrimitiveGeometryTypeChanged(Primitive* p)
{
	contactManager->onPrimitiveGeometryTypeChanged(p);
}

void World::onJointPrimitiveNulling(Joint* j, Primitive* p)
{
	jointStage->onJointPrimitiveNulling(j, p);
}

void World::onJointPrimitiveSet(Joint* j, Primitive* p)
{
	jointStage->onJointPrimitiveSet(j, p);
}

void World::insertContact(Contact* c)
{
	jointStage->onEdgeAdded(c);
	numContacts++;
}

void World::destroyContact(Contact* c)
{
	jointStage->onEdgeRemoving(c);
	delete c;
	numContacts--;
}

ClumpStage* World::getClumpStage()
{
	return static_cast<ClumpStage*>(jointStage->findStage(IStage::CLUMP_STAGE));
}

SleepStage* World::getSleepStage()
{
	return static_cast<SleepStage*>(jointStage->findStage(IStage::SLEEP_STAGE));
}

SimJobStage& World::getSimJobStage()
{
	return *static_cast<SimJobStage*>(jointStage->findStage(IStage::SIMJOB_STAGE));
}

const SleepStage* World::getSleepStage() const
{
	return static_cast<const SleepStage*>(jointStage->findStage(IStage::SLEEP_STAGE));
}

void World::ticklePrimitive(Primitive* p, bool recursive)
{
	Assembly* assembly = p->getAssembly();

	if (assembly != NULL) {
		getSleepStage()->onWakeUpRequest(assembly, recursive);
	}
}

void World::onPrimitiveCanSleepChanged(Primitive* p)
{
	getClumpStage()->onPrimitiveCanSleepChanged(p);
}

void World::onPrimitiveAddedAnchor(Primitive* p)
{
	getClumpStage()->onPrimitiveAddedAnchor(p);
}

void World::onPrimitiveRemovedAnchor(Primitive* p)
{
	getClumpStage()->onPrimitiveRemovedAnchor(p);
}

void World::onPrimitiveCanCollideChanged(Primitive* p)
{
	getClumpStage()->onPrimitiveCanCollideChanged(p);
}

void World::onMotorAngleChanged(MotorJoint* m)
{
	getClumpStage()->onMotorAngleChanged(m);
}

void World::update()
{
	getClumpStage()->process();
}

void World::addedBodyForce()
{
	getSleepStage()->addedBodyForce();
}

void World::onPrimitiveTouched(Primitive* touchP, Primitive* touchOtherP)
{
	touch.append(touchP);
	touchOther.append(touchOtherP);
}

void World::computeFallen(G3D::Array<Primitive*>& fallen) const
{
	const SleepStage::AssemblySet& awake = getSleepStage()->getAwakeAssemblies();

	for (SleepStage::CAssemblySetIt cIt = awake.begin(); cIt != awake.end(); cIt++) {

		Assembly* assembly = *cIt;

		if (assembly->getAssemblyPrimitive()->getCoordinateFrame().translation.y < -500.0f) {

			PrimIterator pEnd = assembly->assemblyPrimEnd();

			for (PrimIterator it = assembly->assemblyPrimBegin(); it != pEnd; ++it) {

				Primitive* p = *it;

				if (p->getCoordinateFrame().translation.y < -500.0f) {
					fallen.append(p);
				}
			}
		}
	}
}

void World::insertPrimitive(Primitive* p)
{
	primitives.fastAppend(p);
	p->world = this;

	jointStage->onPrimitiveAdded(p);
	contactManager->onPrimitiveAdded(p);
}

static bool inIgnoreGroup(std::set<Primitive*>* ignoreGroup, Primitive* p)
{
	return ignoreGroup != NULL && ignoreGroup->find(p) != ignoreGroup->end();
}

inline void World::destroyJoint(Joint* joint)
{
	inJointNotification = true;
	Notifier<World, AutoDestroy>::raise(AutoDestroy(joint));
	inJointNotification = false;
}

inline void World::removeFromBreakable(Joint* j)
{
	if (j->isBreakable()) {
		breakableJoints.erase(j);
	}
}

void World::doBreakJoints()
{
	std::set<Joint*>::iterator it = breakableJoints.begin();

	while (it != breakableJoints.end()) {

		Joint* joint = *it;
		++it;

		if (joint->inKernel() && joint->isBroken()) {
			destroyJoint(joint);
		}
	}
}

float World::step(float desiredInterval)
{
	Profiling::Mark mark(*profilingWorldStep, true);

	update();

	double startTime = G3D::System::getTick();

	bool throttling = false;

	int steps = std::max(1, G3D::iRound((float) floor(Constants::worldStepsPerSec() * desiredInterval)));

	for (int j = 0; j < steps; j++) {

		int uiStepId = worldStepId / Constants::worldStepsPerUiStep();

		if (worldStepId % Constants::worldStepsPerUiStep() == 0) {

			Profiling::Mark mark(*profilingUiStep, false);

			doBreakJoints();

			touch.resize(0, false);
			touchOther.resize(0, false);

			inStepCode = true;

			getClumpStage()->stepUi(uiStepId);

			getSimJobStage().notifyMovingPrimitives();

			inStepCode = false;
		}

		{
			inStepCode = true;

			Profiling::Mark mark(*profilingBroadphase, false);

			contactManager->stepWorld();

			inStepCode = false;
		}

		inStepCode = true;

		jointStage->stepWorld(worldStepId, uiStepId, throttling);

		inStepCode = false;

		throttling = !disableEnvironmentalThrottle && canThrottle &&
					 G3D::System::getTick() > startTime + Constants::worldDt() * (j + 1);

		worldStepId++;
	}

	return Constants::worldDt() * steps;
}

void World::insertJoint(Joint* j)
{
	Primitive* p0 = j->getPrimitive(0);
	Primitive* p1 = j->getPrimitive(1);

	if (p0 != NULL && p1 != NULL) {
		Joint* existing = Primitive::getJoint(p0, p1);

		if (existing != NULL) {
			destroyJoint(existing);
		}
	}

	jointStage->onEdgeAdded(j);

	numJoints++;

	if (j->isBreakable()) {
		breakableJoints.insert(j);
	}
}

void World::destroyJoints(Primitive* p, std::set<Primitive*>* ignoreGroup)
{
	Joint* j = p->getFirstJoint();

	while (j != NULL) {
		Joint* joint = j;
		j = p->getNextJoint(j);

		if (Joint::isAutoJoinJoint(joint)) {
			Primitive* other = joint->otherPrimitive(p);

			if (!inIgnoreGroup(ignoreGroup, other)) {
				destroyJoint(joint);
			}
		}
	}
}

void World::destroyJoints(Primitive* p)
{
	destroyJoints(p, NULL);
}

void World::createJoints(Primitive* p, std::set<Primitive*>* ignoreGroup)
{
	numLinkCalls++;

	tempPrimitives.resize(0, false);

	contactManager->getPrimitivesTouchingExtents(p->getFastFuzzyExtents(), p, tempPrimitives);

	for (int i = 0; i < tempPrimitives.size(); i++) {

		Primitive* other = tempPrimitives[i];

		if (!inIgnoreGroup(ignoreGroup, other) && Primitive::getJoint(p, other) == NULL) {

			Joint* joint = JointBuilder::canJoin(p, other);

			if (joint != NULL) {

				insertJoint(joint);

				inJointNotification = true;
				Notifier<World, AutoJoin>::raise(AutoJoin(joint));
				inJointNotification = false;
			}
		}
	}
}

void World::createJoints(Primitive* p)
{
	createJoints(p, NULL);
}

World::~World()
{
	delete jointStage;

	delete contactManager;
}

void World::removeJoint(Joint* j)
{
	removeFromBreakable(j);

	jointStage->onEdgeRemoving(j);

	numJoints--;
}

void World::joinAll()
{
	for (int i = 0; i < getNumPrimitives(); i++) {
		createJoints(primitives[i]);
	}
}

void World::removePrimitive(Primitive* p)
{
	destroyJoints(p);

	contactManager->onPrimitiveRemoved(p);

	jointStage->onPrimitiveRemoving(p);

	primitives.fastRemove(p);

	p->world = NULL;
}

World::World()
	: contactManager(new ContactManager(this)), jointStage(new JointStage(NULL, this)), canThrottle(true),
	  inStepCode(false), inJointNotification(false), worldStepId(0), numJoints(0), numContacts(0), numLinkCalls(0),
	  profilingWorldStep(new Profiling::CodeProfiler("World Step")),
	  profilingUiStep(new Profiling::CodeProfiler("UI Step")),
	  profilingBroadphase(new Profiling::CodeProfiler("Broadphase"))
{
	profilingBroadphase->parent = profilingWorldStep.get();
	profilingUiStep->parent = profilingWorldStep.get();

	getSleepStage()->profilingCollision->parent = profilingWorldStep.get();
	getSleepStage()->profilingWake->parent = profilingWorldStep.get();
	getSleepStage()->profilingSleep->parent = profilingWorldStep.get();

	getKernel().profilingKernel->parent = profilingWorldStep.get();
}

void World::destroyJointsToWorld(const G3D::Array<Primitive*>& primitives)
{
	std::set<Primitive*> ignore;

	for (int i = 0; i < primitives.size(); i++) {
		ignore.insert(primitives[i]);
	}

	for (int i = 0; i < primitives.size(); i++) {
		destroyJoints(primitives[i], &ignore);
	}
}

void World::createJointsToWorld(const G3D::Array<Primitive*>& primitives)
{
	std::set<Primitive*> ignore;

	for (int i = 0; i < primitives.size(); i++) {
		ignore.insert(primitives[i]);
	}

	for (int i = 0; i < primitives.size(); i++) {
		createJoints(primitives[i], &ignore);
	}
}

} // namespace RBX
