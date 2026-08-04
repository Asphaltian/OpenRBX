#include "v8world/SleepStage2.h"

#include "decomp.h"
#include "util/Velocity.h"
#include "v8kernel/Body.h"
#include "v8world/Clump2.h"
#include "v8world/Joint.h"
#include "v8world/Primitive.h"
#include "v8world/SimJobStage.h"

#include <cstddef>

namespace RBX {

SimJobStage* SleepStage::getSimJobStage()
{
	return static_cast<SimJobStage*>(getDownstream());
}

Sim::AssemblyState SleepStage::getState(Assembly* assembly)
{
	return assembly->inPipeline() && assembly->inOrDownstreamOfStage(this) ? assembly->getSleepInfo()->state
																		   : Sim::ANCHORED;
}

SleepStage::AssemblySet* SleepStage::stateToSet(Sim::AssemblyState state)
{
	switch (state) {
	case Sim::RECURSIVE_WAKE_PENDING:
		return &recursiveWakePending;

	case Sim::WAKE_PENDING:
		return &wakePending;

	case Sim::SLEEPING_CHECKING:
		return &sleepingChecking;

	case Sim::SLEEPING_DEEPLY:
		return &sleepingDeeply;

	default:
		return &awake;
	}
}

int SleepStage::getSleepCount(Assembly* assembly)
{
	return assembly->getSleepInfo()->sleepCount;
}

void SleepStage::incrementSleepCount(Assembly* assembly)
{
	assembly->getSleepInfo()->sleepCount++;
}

void SleepStage::resetSleepCount(Assembly* assembly)
{
	assembly->getSleepInfo()->sleepCount = 0;
}

bool SleepStage::isMoving(Assembly* assembly)
{
	Sim::AssemblyState state = getState(assembly);

	return state == Sim::AWAKE || state == Sim::WAKE_PENDING || state == Sim::RECURSIVE_WAKE_PENDING;
}

bool SleepStage::atLeastOneAssemblyMoving(Assembly* a0, Assembly* a1)
{
	return isMoving(a0) || isMoving(a1);
}

bool SleepStage::canThrottle(Assembly* assembly)
{
	return assembly->getAssemblyPrimitiveConst()->getBody()->getCanThrottle();
}

bool SleepStage::isAffecting(Edge* edge)
{
	Sim::EdgeState state = edge->getEdgeState();

	return edge->getEdgeType() != Edge::CONTACT || state == Sim::TOUCHING || state == Sim::TOUCHING_SLEEPING;
}

void SleepStage::wakeAssemblyAndNeighbors(Assembly* assembly)
{
	wakeAssemblyAndNeighbors(assembly, 0);
}

bool SleepStage::edgeIsAwake(Edge* edge)
{
	return edge->inOrDownstreamOfStage(this);
}

bool SleepStage::computeCanSleep(Assembly* assembly)
{
	return !Primitive::disableSleep && assembly->getCanSleep();
}

Sim::EdgeState SleepStage::computeContactState(bool moving, bool wasTouching, bool touching, bool canCollide)
{
	if (moving) {
		return touching && canCollide ? Sim::TOUCHING : Sim::STEPPING;
	}

	return wasTouching && touching && canCollide ? Sim::TOUCHING_SLEEPING : Sim::SLEEPING;
}

// FUNCTION: WEBSERVICE 0x10118cd0
bool SleepStage::forceNeighborAwake(Assembly* assembly)
{
	CoordinateFrame cofm = assembly->getAssemblyPrimitiveConst()->getBody()->getBranchCofmCoordinateFrame();
	float radius = assembly->getMaxRadius();
	bool withinTolerance =
		assembly->getSleepInfo()->runningAverageState.withinTolerance(cofm, radius, 2.0f * sleepTolerance());

	return !withinTolerance;
}

// FUNCTION: WEBSERVICE 0x10118d40
bool SleepStage::okNeighborSleep(Assembly* assembly)
{
	CoordinateFrame cofm = assembly->getAssemblyPrimitiveConst()->getBody()->getBranchCofmCoordinateFrame();
	float radius = assembly->getMaxRadius();

	return assembly->getSleepInfo()->runningAverageState.withinTolerance(cofm, radius, sleepTolerance());
}

// FUNCTION: WEBSERVICE 0x10118da0
bool SleepStage::computeShouldSleep(Assembly* assembly)
{
	CoordinateFrame cofm = assembly->getAssemblyPrimitiveConst()->getBody()->getBranchCofmCoordinateFrame();
	float radius = assembly->getMaxRadius();
	RunningAverageState& runningAverageState = assembly->getSleepInfo()->runningAverageState;

	runningAverageState.update(cofm, radius);

	return runningAverageState.withinTolerance(cofm, radius, sleepTolerance());
}

// FUNCTION: WEBSERVICE 0x10118e20
bool SleepStage::preventNeighborSleep(Assembly* assembly)
{
	if (getState(assembly) == Sim::AWAKE) {
		if (!okNeighborSleep(assembly)) {
			return true;
		}
	}

	return false;
}

// STUB: WEBSERVICE 0x10118e70
Sim::AssemblyState SleepStage::computeAssemblyState(Assembly* assembly)
{
	if (!computeCanSleep(assembly)) {
		return Sim::AWAKE;
	}

	bool allNeighborsSleeping = true;

	for (EdgeIterator it = assembly->externalEdgeBegin(); it != assembly->externalEdgeEnd(); ++it) {
		if (edgeIsAwake(*it)) {
			if (isAffecting(*it)) {
				Assembly* other = assembly->otherAssembly(*it);

				if (getState(other) == Sim::AWAKE) {
					allNeighborsSleeping = false;

					if (forceNeighborAwake(other)) {
						return Sim::AWAKE;
					}
				}
			}
		}
	}

	return allNeighborsSleeping ? Sim::SLEEPING_DEEPLY : Sim::SLEEPING_CHECKING;
}

// FUNCTION: WEBSERVICE 0x10118f90
int SleepStage::getMetric(MetricType metricType)
{
	switch (metricType) {
	case NUM_CONTACTSTAGE_CONTACTS:
		return numContactsInStage;

	case NUM_STEPPING_CONTACTS:
		return steppingContacts.size();

	case NUM_TOUCHING_CONTACTS:
		return numContactsInKernel;

	default:
		return IWorldStage::getMetric(metricType);
	}
}

// STUB: WEBSERVICE 0x10118fd0
bool SleepStage::shouldSleep(Assembly* assembly)
{
	if (!computeCanSleep(assembly)) {
		return false;
	}

	if (!computeShouldSleep(assembly)) {
		return false;
	}

	for (EdgeIterator it = assembly->externalEdgeBegin(); it != assembly->externalEdgeEnd(); ++it) {
		Edge* edge = *it;

		if (edgeIsAwake(edge)) {
			if (isAffecting(edge)) {
				if (preventNeighborSleep(assembly->otherAssembly(edge))) {
					return false;
				}
			}
		}
	}

	return true;
}

// FUNCTION: WEBSERVICE 0x101193f0
void SleepStage::onAssemblyAdded(Assembly* assembly)
{
	assembly->putInPipeline(this);
	assembly->setSleepInfo(new SleepInfo());
	assembly->getAssemblyPrimitiveConst()->getBody()->resetAccumulators();

	wakePending.insert(assembly);
}

// STUB: WEBSERVICE 0x10119530
SleepStage::~SleepStage()
{
}

// FUNCTION: WEBSERVICE 0x10119740
void SleepStage::changeJointState(Joint* joint, Sim::EdgeState state)
{
	bool wasStepping = joint->getEdgeState() == Sim::STEPPING, isStepping = state == Sim::STEPPING;

	if (wasStepping != isStepping) {
		if (wasStepping) {
			steppingJoints.erase(joint);

			getDownstreamWS()->onEdgeRemoving(joint);
		}
		else {
			steppingJoints.insert(joint);

			getDownstreamWS()->onEdgeAdded(joint);
		}
	}

	joint->setEdgeState(state);
}

// STUB: WEBSERVICE 0x101197c0
void SleepStage::changeAssemblyState(Assembly* assembly, Sim::AssemblyState state)
{
	Sim::AssemblyState oldState = getState(assembly);

	AssemblySet* oldSet = stateToSet(oldState);
	AssemblySet* newSet = stateToSet(state);

	oldSet->erase(assembly);

	newSet->insert(assembly);

	if ((oldState == Sim::SLEEPING_CHECKING || oldState == Sim::SLEEPING_DEEPLY) && state != Sim::SLEEPING_CHECKING &&
		state != Sim::SLEEPING_DEEPLY) {
		assembly->getAssemblyPrimitiveConst()->getBody()->resetAccumulators();
	}

	if (state == Sim::AWAKE) {
		if (!assembly->downstreamOfStage(this)) {
			getSimJobStage()->onAssemblyAdded(assembly);

			if (!externalBodyForceAdded) {
				const Primitive* primitive = assembly->getAssemblyPrimitiveConst();
			}
		}
	}
	else if (state == Sim::SLEEPING_CHECKING || state == Sim::SLEEPING_DEEPLY) {
		assembly->getAssemblyPrimitiveConst()->getBody()->setVelocity(Velocity::zero());

		if (assembly->downstreamOfStage(this)) {
			getSimJobStage()->onAssemblyRemoving(assembly);
		}
	}

	resetSleepCount(assembly);
	assembly->getSleepInfo()->state = state;
}

// STUB: WEBSERVICE 0x10119970
void SleepStage::onAssemblyRemoving(Assembly* assembly)
{
	if (getState(assembly) != Sim::SLEEPING_DEEPLY) {
		changeAssemblyState(assembly, Sim::SLEEPING_DEEPLY);
	}

	sleepingDeeply.erase(assembly);

	delete assembly->getSleepInfo();

	assembly->setSleepInfo(NULL);
	assembly->removeFromStage(this);
}

// FUNCTION: WEBSERVICE 0x10119ae0
void SleepStage::onEdgeAdded(Edge* edge)
{
	Assembly* a0 = edge->getPrimitive(0)->getAssembly();
	Assembly* a1 = edge->getPrimitive(1)->getAssembly();

	edge->putInPipeline(this);

	if (edge->getEdgeType() == Edge::CONTACT) {
		steppingContacts.fastAppend(static_cast<Contact*>(edge));
		edge->setEdgeState(Sim::STEPPING);
		numContactsInStage++;
	}
	else {
		edge->setEdgeState(Sim::SLEEPING);
		changeJointState(static_cast<Joint*>(edge), Sim::STEPPING);
	}
}

// STUB: WEBSERVICE 0x10119ba0
SleepStage::SleepStage(IStage* upstream, World* world)
	: IWorldStage(upstream, new SimJobStage(this, world), world), externalBodyForceAdded(false), numContactsInStage(0),
	  numContactsInKernel(0), throttling(false), uiStepId(-1),
	  profilingCollision(new Profiling::CodeProfiler("Collision")), profilingWake(new Profiling::CodeProfiler("Wake")),
	  profilingSleep(new Profiling::CodeProfiler("Sleep"))
{
}

// FUNCTION: WEBSERVICE 0x10119e00
void SleepStage::wakeEvent(Assembly* assembly)
{
	Sim::AssemblyState state = getState(assembly);

	if (state != Sim::ANCHORED && state != Sim::WAKE_PENDING && state != Sim::RECURSIVE_WAKE_PENDING) {
		changeAssemblyState(assembly, Sim::WAKE_PENDING);
	}
}

// FUNCTION: WEBSERVICE 0x10119e50
void SleepStage::recursiveWakeEvent(Assembly* assembly)
{
	Sim::AssemblyState state = getState(assembly);

	if (state != Sim::ANCHORED && state != Sim::RECURSIVE_WAKE_PENDING) {
		changeAssemblyState(assembly, Sim::RECURSIVE_WAKE_PENDING);
	}
}

// FUNCTION: WEBSERVICE 0x10119ea0
void SleepStage::touchEvent(Contact* contact)
{
	for (int i = 0; i < 2; i++) {
		Assembly* assembly = contact->getPrimitive(0)->getAssembly();
		recursiveWakeEvent(assembly);
	}
}

// FUNCTION: WEBSERVICE 0x10119f10
void SleepStage::changeJointState(const std::vector<Joint*>& joints, Sim::EdgeState state)
{
	for (unsigned int i = 0; i < joints.size(); i++) {
		changeJointState(joints[i], state);
	}
}

// FUNCTION: WEBSERVICE 0x10119f70
void SleepStage::changeAssemblyState(const std::vector<Assembly*>& assemblies, Sim::AssemblyState state)
{
	for (unsigned int i = 0; i < assemblies.size(); i++) {
		changeAssemblyState(assemblies[i], state);
	}
}

// FUNCTION: WEBSERVICE 0x10119fd0
void SleepStage::onWakeUpRequest(Assembly* assembly, bool recursive)
{
	if (assembly->inPipeline() && assembly->inOrDownstreamOfStage(this)) {
		if (recursive) {
			recursiveWakeEvent(assembly);
		}
		else {
			wakeEvent(assembly);
		}
	}
}

// FUNCTION: WEBSERVICE 0x1011a020
void SleepStage::stepJoints()
{
	static std::vector<Joint*> toSleeping;

	toSleeping.resize(0);

	for (JointSet::iterator it = steppingJoints.begin(); it != steppingJoints.end(); ++it) {
		Joint* joint = *it;

		Primitive* prim0 = joint->getPrimitive(0);
		Primitive* prim1 = joint->getPrimitive(1);

		if (!throttling || !prim0->getBody()->getCanThrottle() || !prim1->getBody()->getCanThrottle()) {
			if (!atLeastOneAssemblyMoving(prim0->getAssembly(), prim1->getAssembly())) {
				toSleeping.push_back(joint);
			}
		}
	}

	changeJointState(toSleeping, Sim::SLEEPING);
}

// FUNCTION: WEBSERVICE 0x1011a1b0
void SleepStage::stepAssembliesAwake()
{
	static std::vector<Assembly*> toSleepingChecking;

	toSleepingChecking.resize(0);

	for (AssemblySetIt it = awake.begin(); it != awake.end(); ++it) {
		Assembly* assembly = *it;

		if (!throttling || !canThrottle(assembly)) {
			if (shouldSleep(assembly)) {
				incrementSleepCount(assembly);

				if (getSleepCount(assembly) > 20) {
					toSleepingChecking.push_back(assembly);
				}
			}
			else {
				resetSleepCount(assembly);
			}
		}
	}

	changeAssemblyState(toSleepingChecking, Sim::SLEEPING_CHECKING);
}

// FUNCTION: WEBSERVICE 0x1011a310
void SleepStage::stepAssembliesSleepingChecking()
{
	static std::vector<Assembly*> toWake;
	static std::vector<Assembly*> toDeep;

	toWake.resize(0);
	toDeep.resize(0);

	for (AssemblySetIt it = sleepingChecking.begin(); it != sleepingChecking.end(); ++it) {
		Assembly* assembly = *it;

		Sim::AssemblyState state = computeAssemblyState(assembly);

		switch (state) {
		case Sim::AWAKE:
			toWake.push_back(assembly);
			break;

		case Sim::SLEEPING_DEEPLY:
			toDeep.push_back(assembly);
			break;
		}
	}

	changeAssemblyState(toWake, Sim::WAKE_PENDING);
	changeAssemblyState(toDeep, Sim::SLEEPING_DEEPLY);
}

// FUNCTION: WEBSERVICE 0x1011a480
void SleepStage::wakeEvent(Edge* edge)
{
	for (int i = 0; i < 2; i++) {
		wakeEvent(edge->getPrimitive(i)->getAssembly());
	}
}

// STUB: WEBSERVICE 0x1011a500
void SleepStage::changeContactState(Contact* contact, Sim::EdgeState state)
{
	Sim::EdgeState oldState = contact->getEdgeState();

	bool wasStepping = oldState == Sim::STEPPING;
	bool wasTouching = oldState == Sim::TOUCHING;
	bool isStepping = state == Sim::STEPPING;
	bool isTouching = state == Sim::TOUCHING;

	if (wasStepping != isStepping) {
		if (wasStepping) {
			steppingContacts.fastRemove(contact);
		}
	}

	if (wasTouching != isTouching) {
		if (wasTouching) {
			touchingContacts.fastRemove(contact);
		}
	}

	if (wasStepping != isStepping) {
		if (isStepping) {
			steppingContacts.fastAppend(contact);
		}
	}

	if (wasTouching != isTouching) {
		if (isTouching) {
			touchingContacts.fastAppend(contact);
		}
	}

	if (wasTouching != isTouching) {
		if (wasTouching) {
			getDownstreamWS()->onEdgeRemoving(contact);
			numContactsInKernel--;
		}
		else {
			getDownstreamWS()->onEdgeAdded(contact);
			numContactsInKernel++;
		}
	}

	if (oldState == Sim::STEPPING && state == Sim::TOUCHING) {
		touchEvent(contact);
	}
	else if (oldState == Sim::TOUCHING && state == Sim::STEPPING) {
		wakeEvent(contact);
	}

	contact->setEdgeState(state);
}

// FUNCTION: WEBSERVICE 0x1011a680
void SleepStage::onEdgeRemoving(Edge* edge)
{
	if (edge->getEdgeType() == Edge::CONTACT) {
		if (edge->getEdgeState() == Sim::TOUCHING_SLEEPING || edge->getEdgeState() == Sim::TOUCHING) {
			wakeEvent(edge);
		}

		if (edge->getEdgeState() != Sim::SLEEPING) {
			changeContactState(static_cast<Contact*>(edge), Sim::SLEEPING);
		}

		numContactsInStage--;
	}
	else if (edge->getEdgeState() == Sim::SLEEPING) {
		wakeEvent(edge);
	}
	else {
		changeJointState(static_cast<Joint*>(edge), Sim::SLEEPING);
	}

	edge->setEdgeState(Sim::UNDEFINED);
	edge->removeFromStage(this);
}

// FUNCTION: WEBSERVICE 0x1011a720
void SleepStage::wakeEdge(Edge* edge)
{
	Sim::EdgeState state = edge->getEdgeState();

	if (edge->getEdgeType() == Edge::CONTACT) {
		if (state == Sim::TOUCHING_SLEEPING) {
			changeContactState(static_cast<Contact*>(edge), Sim::TOUCHING);
		}
		else if (state == Sim::SLEEPING) {
			changeContactState(static_cast<Contact*>(edge), Sim::STEPPING);
		}
	}
	else if (state == Sim::SLEEPING) {
		changeJointState(static_cast<Joint*>(edge), Sim::STEPPING);
	}
}

// FUNCTION: WEBSERVICE 0x1011a780
void SleepStage::changeContactState(const std::vector<Contact*>& contacts, Sim::EdgeState state)
{
	for (unsigned int i = 0; i < contacts.size(); i++) {
		changeContactState(contacts[i], state);
	}
}

// STUB: WEBSERVICE 0x1011a7e0
void SleepStage::stepContacts(ContactList& contacts)
{
	static std::vector<Contact*> toSleeping;
	static std::vector<Contact*> toStepping;
	static std::vector<Contact*> toTouching;
	static std::vector<Contact*> toTouchingSleeping;

	toSleeping.resize(0);
	toStepping.resize(0);
	toTouching.resize(0);
	toTouchingSleeping.resize(0);

	for (int i = 0; i < contacts.size(); i++) {
		Contact* contact = contacts[i];

		Primitive* prim0 = contact->getPrimitive(0);
		Primitive* prim1 = contact->getPrimitive(1);

		if (!throttling || !prim0->getBody()->getCanThrottle() || !prim1->getBody()->getCanThrottle()) {
			bool moving = atLeastOneAssemblyMoving(prim0->getAssembly(), prim1->getAssembly());

			bool touching = contact->step(uiStepId);

			bool canCollide =
				!prim0->getDragging() && prim0->getCanCollide() && !prim1->getDragging() && prim1->getCanCollide();

			Sim::EdgeState state =
				computeContactState(moving, contact->getEdgeState() == Sim::TOUCHING, touching, canCollide);

			if (state != contact->getEdgeState()) {
				switch (state) {
				case Sim::STEPPING:
					toStepping.push_back(contact);
					break;

				case Sim::SLEEPING:
					toSleeping.push_back(contact);
					break;

				case Sim::TOUCHING:
					toTouching.push_back(contact);
					break;

				case Sim::TOUCHING_SLEEPING:
					toTouchingSleeping.push_back(contact);
					break;
				}
			}
		}
	}

	changeContactState(toSleeping, Sim::SLEEPING);
	changeContactState(toStepping, Sim::STEPPING);
	changeContactState(toTouching, Sim::TOUCHING);
	changeContactState(toTouchingSleeping, Sim::TOUCHING_SLEEPING);
}

// STUB: WEBSERVICE 0x1011ab10
void SleepStage::wakeAssemblyAndNeighbors(Assembly* assembly, int depth)
{
	Sim::AssemblyState state = getState(assembly);

	if (state != Sim::ANCHORED) {
		if (state != Sim::AWAKE) {
			changeAssemblyState(assembly, Sim::AWAKE);
		}

		for (EdgeIterator it = assembly->externalEdgeBegin(); it != assembly->externalEdgeEnd(); ++it) {
			Edge* edge = *it;

			if (edgeIsAwake(edge)) {
				wakeEdge(edge);

				if (isAffecting(edge)) {
					Assembly* other = assembly->otherAssembly(edge);

					if (depth > 0) {
						if (getState(other) != Sim::AWAKE) {
							wakeAssemblyAndNeighbors(other, depth - 1);
						}
					}
					else if (getState(other) == Sim::SLEEPING_DEEPLY) {
						changeAssemblyState(other, Sim::SLEEPING_CHECKING);
					}
				}
			}
		}
	}
}

// STUB: WEBSERVICE 0x1011ac80
void SleepStage::stepAssembliesRecursiveWakePending()
{
	static std::vector<Assembly*> toWake;

	toWake.resize(0);

	for (AssemblySetIt it = recursiveWakePending.begin(); it != recursiveWakePending.end(); ++it) {
		toWake.push_back(*it);
	}

	for (unsigned int i = 0; i < toWake.size(); i++) {
		Assembly* assembly = toWake[i];

		if (getState(assembly) != Sim::RECURSIVE_WAKE_PENDING) {
			changeAssemblyState(assembly, Sim::RECURSIVE_WAKE_PENDING);
		}

		wakeAssemblyAndNeighbors(assembly, 8);
	}
}

// STUB: WEBSERVICE 0x1011afa0
void SleepStage::stepAssembliesWakePending()
{
	static std::vector<Assembly*> toWake;

	toWake.resize(0);

	for (AssemblySetIt it = wakePending.begin(); it != wakePending.end(); ++it) {
		toWake.push_back(*it);
	}

	for (unsigned int i = 0; i < toWake.size(); i++) {
		wakeAssemblyAndNeighbors(toWake[i]);
	}
}

// FUNCTION: WEBSERVICE 0x1011b110
void SleepStage::stepWorld(int worldStepId, int _uiStepId, bool _throttling)
{
	throttling = _throttling;
	uiStepId = _uiStepId;

	{
		Profiling::Mark mark(*profilingWake, false);

		stepAssembliesRecursiveWakePending();
		stepAssembliesWakePending();
	}

	{
		Profiling::Mark mark(*profilingCollision, false);

		stepContacts(steppingContacts);

		stepJoints();

		stepAssembliesRecursiveWakePending();
		stepAssembliesWakePending();
	}

	{
		Profiling::Mark mark(*profilingSleep, false);

		stepAssembliesAwake();

		if (worldStepId % 4 == 0) {
			stepAssembliesSleepingChecking();
		}

		stepAssembliesWakePending();
	}

	stepContacts(touchingContacts);

	stepAssembliesRecursiveWakePending();
	stepAssembliesWakePending();

	externalBodyForceAdded = false;
	getDownstream()->stepWorld(worldStepId, uiStepId, throttling);
}

} // namespace RBX
