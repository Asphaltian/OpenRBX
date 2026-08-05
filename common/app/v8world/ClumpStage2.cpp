#include "v8world/ClumpStage2.h"

#include "decomp.h"
#include "util/Guid.h"
#include "v8world/Assembly2.h"
#include "v8world/AssemblyStage2.h"
#include "v8world/MotorJoint.h"
#include "v8world/Primitive.h"
#include "v8world/RigidJoint.h"
#include "v8world/SleepStage2.h"

#include <algorithm>
#include <cmath>
#include <cstddef>

namespace RBX {

// FUNCTION: WEBSERVICE 0x1011b330
float getPrimitiveSize(Primitive* primitive)
{
	const Vector3& size = primitive->getGeometry()->getGridSize();

	float area;
	float other;

	if (size.x < size.y) {
		if (size.x < size.z) {
			area = size.z * size.y;
			goto haveArea;
		}

		other = size.y;
	}
	else {
		other = size.y < size.z ? size.z : size.y;
	}

	area = other * size.x;

haveArea:;

	float gridArea = floor(area);

	return (float) (G3D::iRound(gridArea) * (unsigned __int64) primitive->getSizeMultiplier());
}

// FUNCTION: WEBSERVICE 0x1011b3d0
int biggerJointGuid(Joint* joint0, Joint* joint1)
{
	const Guid* guid0 = &joint0->getPrimitive(0)->getGuid();
	const Guid* other0 = &joint1->getPrimitive(0)->getGuid();
	const Guid* guid1 = joint0->getPrimitive(1) != NULL ? &joint0->getPrimitive(1)->getGuid() : NULL;
	const Guid* other1 = joint1->getPrimitive(1) != NULL ? &joint1->getPrimitive(1)->getGuid() : NULL;

	return Guid::compare(guid0, guid1, other0, other1);
}

// FUNCTION: WEBSERVICE 0x1011b440
Joint* getJoint(Primitive* primitive, Joint::JointType jointType)
{
	Joint* joint = primitive->getFirstJoint();

	while (joint != NULL) {
		if (joint->getJointType() == jointType) {
			return joint;
		}

		joint = primitive->getNextJoint(joint);
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x1011b5e0
int biggerJointSize(Joint* joint0, Joint* joint1)
{
	float size0 = getPrimitiveSize(joint0->getPrimitive(0));

	if (joint0->getPrimitive(1) != NULL) {
		size0 = G3D::max(size0, getPrimitiveSize(joint0->getPrimitive(1)));
	}

	float size1 = getPrimitiveSize(joint1->getPrimitive(0));

	if (joint1->getPrimitive(1) != NULL) {
		size1 = G3D::max(size1, getPrimitiveSize(joint1->getPrimitive(1)));
	}

	if (size1 < size0) {
		return 1;
	}

	if (size0 < size1) {
		return -1;
	}

	return 0;
}

namespace JointSort {

// FUNCTION: WEBSERVICE 0x1011b690
bool lighterJoint(Joint* joint0, Joint* joint1)
{
	if (joint0 == joint1) {
		return false;
	}

	Joint::JointType type0 = joint0->getJointType();
	Joint::JointType type1 = joint1->getJointType();

	if (type0 != type1) {
		return type0 < type1;
	}

	int bigger = biggerJointSize(joint0, joint1);

	if (bigger == 1) {
		return true;
	}

	if (bigger == -1) {
		return false;
	}

	bigger = biggerJointGuid(joint0, joint1);

	if (bigger == 1) {
		return true;
	}

	if (bigger == -1) {
		return false;
	}

	return joint0 < joint1;
}

} // namespace JointSort

// FUNCTION: WEBSERVICE 0x1011b740
DECOMP_NOINLINE void ClumpStage::onPrimitiveAdded(Primitive* p)
{
	p->putInPipeline(this);
	p->putInPipeline(getDownstream());

	p->setClumpDepth(1);

	Joint* joint;

	if (p->getAnchor()) {
		joint = new AnchorJoint(p);
	}
	else {
		joint = new FreeJoint(p);
	}

	joint->putInPipeline(this);

	getDownstreamWS()->onEdgeAdded(joint);
}

// FUNCTION: WEBSERVICE 0x1011b840
DECOMP_NOINLINE void ClumpStage::onPrimitiveRemoving(Primitive* p)
{
	Joint* joint = p->getFirstJoint();

	getDownstreamWS()->onEdgeRemoving(joint);

	IStage* downstream = getDownstream();

	p->setClumpDepth(-1);
	p->removeFromStage(downstream);

	joint->removeFromPipeline(this);
	delete joint;

	p->removeFromStage(this);
}

// FUNCTION: WEBSERVICE 0x1011b8a0
void ClumpStage::onPrimitiveAddedAnchor(Primitive* p)
{
	Joint* freeJoint = getJoint(p, Joint::FREE_JOINT);

	Joint* anchorJoint = new AnchorJoint(p);

	anchorJoint->putInPipeline(this);

	getDownstreamWS()->onEdgeAdded(anchorJoint);

	onEdgeRemoving(freeJoint);
}

// FUNCTION: WEBSERVICE 0x1011b950
void ClumpStage::onPrimitiveRemovedAnchor(Primitive* p)
{
	Joint* anchorJoint = getJoint(p, Joint::ANCHOR_JOINT);

	Joint* freeJoint = new FreeJoint(p);

	freeJoint->putInPipeline(this);

	getDownstreamWS()->onEdgeAdded(freeJoint);

	onEdgeRemoving(anchorJoint);
}

int TreeStage::getClumpDepth(Primitive* p)
{
	return p != NULL ? p->getClumpDepth() : 0;
}

void TreeStage::dirtyAssembly(Assembly* a)
{
	assemblies.insert(a);
	dirty = true;
}

void TreeStage::insertEdge(Edge* e)
{
	if (!e->getInEdgeList()) {
		edges.insert(e);
		dirty = true;
		e->setInEdgeList(true);
	}
}

void TreeStage::eraseEdge(Edge* e)
{
	if (e->getInEdgeList()) {
		edges.erase(e);
		e->setInEdgeList(false);
	}
}

// FUNCTION: WEBSERVICE 0x1011ba00
Primitive* TreeStage::heavyParent(int index, Primitive* primitive, Joint*& heaviest, int& heaviestIndex)
{
	Joint* joint = primitive->getFirstJoint();

	while (joint != NULL) {
		if (joint->getActive()) {
			Primitive* other = primitive == joint->getPrimitive(0) ? joint->getPrimitive(1) : joint->getPrimitive(0);

			int depth = other != NULL ? other->getClumpDepth() : 0;

			if (depth == primitive->getClumpDepth() - 1) {
				if (heaviest == NULL || JointSort::lighterJoint(heaviest, joint)) {
					heaviest = joint;
					heaviestIndex = index;
				}

				return other;
			}
		}

		joint = primitive->getNextJoint(joint);
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x1011ba80
void TreeStage::cleanAssembly(Assembly* assembly)
{
	if (!assembly->inPipeline()) {
		assembly->putInPipeline(this);
	}

	bool inKernel = assembly->downstreamOfStage(this);
	bool wanted = !assembly->getAnchored();

	if (inKernel != wanted) {
		if (wanted) {
			static_cast<AssemblyStage*>(getDownstream())->onAssemblyAdded(assembly);
		}
		else {
			static_cast<AssemblyStage*>(getDownstream())->onAssemblyRemoving(assembly);
		}
	}
	else if (inKernel) {
		static_cast<AssemblyStage*>(getDownstream())->wakeAssembly(assembly);
	}
}

// FUNCTION: WEBSERVICE 0x1011bb00
void TreeStage::cleanEdge(Edge* edge)
{
	Assembly* assembly0 = edge->getPrimitive(0)->getAssembly();
	Assembly* assembly1 = edge->getPrimitive(1)->getAssembly();

	bool inKernel = edge->downstreamOfStage(this);
	bool wanted = assembly0 != assembly1 && !(assembly0->getAnchored() && assembly1->getAnchored());

	if (inKernel != wanted) {
		if (inKernel) {
			static_cast<IWorldStage*>(getDownstream())->onEdgeRemoving(edge);
		}
		else {
			static_cast<IWorldStage*>(getDownstream())->onEdgeAdded(edge);
		}
	}
}

// FUNCTION: WEBSERVICE 0x1011bb90
int TreeStage::getMetric(MetricType metricType)
{
	switch (metricType) {
	case MAX_TREE_DEPTH:
		return maxTreeDepth;
	default:
		return static_cast<IWorldStage*>(getDownstream())->getMetric(metricType);
	}
}

// FUNCTION: WEBSERVICE 0x1011bbb0
void ClumpStage::stepUi(int frameCount)
{
	getTreeStage()->getAssemblyStage()->stepUi(frameCount);
}

// FUNCTION: WEBSERVICE 0x1011bbc0
void TreeStage::findHeaviestUpstream(Primitive* p0, Primitive* p1, Joint*& answer, int& heavySide)
{
	int depth0 = getClumpDepth(p0);
	int depth1 = getClumpDepth(p1);

	if (depth0 != depth1) {
		if (depth0 > depth1) {
			findHeaviestUpstream(heavyParent(0, p0, answer, heavySide), p1, answer, heavySide);
		}
		else {
			findHeaviestUpstream(p0, heavyParent(1, p1, answer, heavySide), answer, heavySide);
		}

		return;
	}

	if (p0 == p1) {
		return;
	}

	Primitive* next0 = p0 != NULL ? heavyParent(0, p0, answer, heavySide) : NULL;
	Primitive* next1 = p1 != NULL ? heavyParent(1, p1, answer, heavySide) : NULL;

	findHeaviestUpstream(next0, next1, answer, heavySide);
}

// FUNCTION: WEBSERVICE 0x1011bcc0
void TreeStage::process()
{
	if (dirty) {
		std::set<Assembly*>::iterator aIt;

		for (aIt = assemblies.begin(); aIt != assemblies.end(); ++aIt) {
			cleanAssembly(*aIt);
		}

		assemblies.clear();

		while (edges.size() != 0) {
			std::set<Edge*>::iterator eIt = edges.begin();

			cleanEdge(*eIt);

			(*eIt)->setInEdgeList(false);
			edges.erase(eIt);
		}

		dirty = false;
	}
}

// FUNCTION: WEBSERVICE 0x1011bde0
void TreeStage::buildDownstreamTree(Primitive* p, std::set<Primitive*>& tree)
{
	Joint* j = p->getFirstJoint();

	while (j != NULL) {
		if (j->getActive() && Joint::isJoint(j) && j->getJointType() >= Joint::ANCHOR_JOINT) {
			Primitive* other = j->otherPrimitive(p);

			if (other != NULL && other->getClumpDepth() == p->getClumpDepth() + 1) {
				tree.insert(other);
				buildDownstreamTree(other, tree);
			}
		}

		j = p->getNextJoint(j);
	}
}

// FUNCTION: WEBSERVICE 0x1011be80
void ClumpStage::process()
{
	getTreeStage()->process();
}

void ClumpStage::onPrimitiveCanCollideChanged(Primitive* p)
{
}

void ClumpStage::onMotorAngleChanged(MotorJoint* motorJoint)
{
}

// FUNCTION: WEBSERVICE 0x1011be90
void ClumpStage::onPrimitiveCanSleepChanged(Primitive* p)
{
	getTreeStage()->process();

	Assembly* a = p->getAssembly();

	bool wasCanSleep = a->getCanSleep();

	a->onPrimitiveCanSleepChanged(p);

	if (wasCanSleep != a->getCanSleep()) {
		IStage* stage = this;

		while (stage->getStageType() != SLEEP_STAGE) {
			stage = stage->getDownstream();
		}

		static_cast<SleepStage*>(stage)->onWakeUpRequest(a, false);
	}
}

// FUNCTION: WEBSERVICE 0x1011bf30
TreeStage::~TreeStage()
{
}

// FUNCTION: WEBSERVICE 0x1011bff0
void TreeStage::stepWorld(int worldStepId, int uiStepId, bool throttling)
{
	process();

	getDownstream()->stepWorld(worldStepId, uiStepId, throttling);
}

// FUNCTION: WEBSERVICE 0x1011c010
void TreeStage::dirtyAssemblies(Joint* j)
{
	for (int i = 0; i < 2; i++) {
		Primitive* p = j->getPrimitive(i);

		if (p != NULL) {
			Assembly* a = p->getAssembly();

			if (a != NULL) {
				dirtyAssembly(a);
			}
		}
	}
}

// FUNCTION: WEBSERVICE 0x1011c060
void TreeStage::undirtyAssembly(Assembly* a)
{
	assemblies.erase(a);

	if (a->inPipeline()) {
		if (a->downstreamOfStage(this)) {
			getAssemblyStage()->onAssemblyRemoving(a);
		}

		a->removeFromPipeline(this);
	}
}

// FUNCTION: WEBSERVICE 0x1011c0b0
void TreeStage::destroyAssembly(Assembly* a)
{
	undirtyAssembly(a);

	delete a;
}

// FUNCTION: WEBSERVICE 0x1011c110
TreeStage::TreeStage(IStage* upstream, World* world)
	: IWorldStage(upstream, new AssemblyStage(this, world), world), maxTreeDepth(0), dirty(false)
{
}

// FUNCTION: WEBSERVICE 0x10108040 FOLDED
IStage::StageType ClumpStage::getStageType()
{
	return CLUMP_STAGE;
}

// FUNCTION: WEBSERVICE 0x1011c200
void TreeStage::rebuildClump(Joint* joint, Primitive* parent)
{
	maxTreeDepth = std::max(maxTreeDepth, getClumpDepth(parent) + 1);

	Primitive* child = joint->otherPrimitive(parent);

	Clump* clump = child->getClump();
	bool isRoot = clump != NULL && clump->getRootPrimitive() == child;

	if (RigidJoint::isRigidJoint(joint)) {
		if (isRoot) {
			destroyAssembly(clump);
		}

		Assembly::addRigidChild(parent, static_cast<RigidJoint*>(joint), child);
		return;
	}

	if (MotorJoint::isMotorJoint(joint)) {
		Assembly* assembly = isRoot ? clump : new Clump(child);

		undirtyAssembly(assembly);

		Assembly::addMotorChild(parent, static_cast<MotorJoint*>(joint), child);
		return;
	}

	Joint::JointType jointType = joint->getJointType();

	if (jointType == Joint::FREE_JOINT || jointType == Joint::ANCHOR_JOINT) {
		Clump* assembly = isRoot ? clump : new Clump(child);

		Assembly::addGroundChild(child);

		assemblies.insert(assembly);
		dirty = true;
	}
}

// FUNCTION: WEBSERVICE 0x1011c3d0
ClumpStage::ClumpStage(IStage* upstream, World* world) : IWorldStage(upstream, new TreeStage(this, world), world)
{
}

// FUNCTION: WEBSERVICE 0x1011c440
Joint* TreeStage::findLightestDownstream(Primitive* p, Primitive*& newParent)
{
	std::set<Primitive*> downstreamTree;

	downstreamTree.insert(p);

	buildDownstreamTree(p, downstreamTree);

	Joint* answer = NULL;

	std::set<Primitive*>::iterator it;

	for (it = downstreamTree.begin(); it != downstreamTree.end(); ++it) {
		Primitive* p = *it;

		Joint* j = p->getFirstJoint();

		while (j != NULL) {
			if (!j->getActive() && Joint::isJoint(j) && j->getJointType() >= Joint::ANCHOR_JOINT) {
				Primitive* other = j->otherPrimitive(p);

				if (other == NULL || downstreamTree.find(other) == downstreamTree.end()) {
					if (answer == NULL || JointSort::lighterJoint(j, answer)) {
						answer = j;
						newParent = other;
					}
				}
			}

			j = p->getNextJoint(j);
		}
	}

	return answer;
}

// FUNCTION: WEBSERVICE 0x1011c630
void TreeStage::traverse(Joint* joint, Primitive* parent)
{
	rebuildClump(joint, parent);

	Primitive* child = joint->otherPrimitive(parent);

	Edge* e = child->getFirstEdge();

	while (e != NULL) {
		if (Joint::isJoint(e) && static_cast<Joint*>(e)->getJointType() >= Joint::ANCHOR_JOINT) {
			if (static_cast<Joint*>(e)->getActive()) {
				Primitive* other = e->otherPrimitive(child);

				if (other != NULL && other != parent) {
					traverse(static_cast<Joint*>(e), child);
				}
			}
		}
		else {
			insertEdge(e);
		}

		e = child->getNextEdge(e);
	}
}

// FUNCTION: WEBSERVICE 0x1011c6e0
void TreeStage::swap(Joint* deactivate, Joint* activate, Primitive* newParent)
{
	if (deactivate != NULL) {
		dirtyAssemblies(deactivate);
		deactivate->setActive(false);

		if (activate == NULL) {
			destroyAssembly(deactivate->getPrimitive(0)->getClump());
			return;
		}
	}
	else if (activate == NULL) {
		return;
	}

	dirtyAssemblies(activate);
	activate->setActive(true);
	traverse(activate, newParent);
}

// FUNCTION: WEBSERVICE 0x1011c740
void TreeStage::insertJoint(Joint* j)
{
	int heavySide = 0;
	Joint* deActivate = NULL;

	findHeaviestUpstream(j->getPrimitive(0), j->getPrimitive(1), deActivate, heavySide);

	if (deActivate == NULL || JointSort::lighterJoint(j, deActivate)) {
		swap(deActivate, j, j->getPrimitive((heavySide + 1) % 2));
		dirty = true;
	}
}

// FUNCTION: WEBSERVICE 0x1011c7c0
void TreeStage::onEdgeAdded(Edge* edge)
{
	Primitive::insertEdge(edge);
	edge->putInPipeline(this);

	if (Joint::isJoint(edge) && static_cast<Joint*>(edge)->getJointType() >= Joint::ANCHOR_JOINT) {
		insertJoint(static_cast<Joint*>(edge));
		return;
	}

	insertEdge(edge);
}

// FUNCTION: WEBSERVICE 0x1011c840
void TreeStage::onEdgeRemoving(Edge* edge)
{
	if (Joint::isJoint(edge) && static_cast<Joint*>(edge)->getJointType() >= Joint::ANCHOR_JOINT) {
		Joint* joint = static_cast<Joint*>(edge);

		if (joint->getActive()) {
			Primitive* downstream = Primitive::downstreamPrimitive(joint);

			Primitive* newParent = NULL;
			Joint* lightest = findLightestDownstream(downstream, newParent);

			swap(joint, lightest, newParent);
			dirty = true;
		}
	}
	else {
		if (edge->downstreamOfStage(this)) {
			static_cast<IWorldStage*>(getDownstream())->onEdgeRemoving(edge);
		}

		eraseEdge(edge);
	}

	edge->removeFromStage(this);
	Primitive::removeEdge(edge);
}

} // namespace RBX
