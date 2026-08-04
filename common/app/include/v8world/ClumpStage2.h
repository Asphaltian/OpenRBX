#ifndef V8WORLD_CLUMPSTAGE2_H
#define V8WORLD_CLUMPSTAGE2_H

#include "decomp.h"
#include "v8world/IWorldStage.h"
#include "v8world/Joint.h"

#include <set>

namespace RBX {

class Assembly;
class AssemblyStage;
class Primitive;

float getPrimitiveSize(Primitive* primitive);

int biggerJointGuid(Joint* joint0, Joint* joint1);
int biggerJointSize(Joint* joint0, Joint* joint1);

Joint* getJoint(Primitive* primitive, Joint::JointType jointType);

namespace JointSort {

bool lighterJoint(Joint* joint0, Joint* joint1);

} // namespace JointSort

// VTABLE: WEBSERVICE 0x10247edc
// SIZE 0x30
class TreeStage : public IWorldStage
{
public:
	TreeStage(IStage* upstream, World* world);

	virtual ~TreeStage(); // vtable+0x00

	// SYNTHETIC: WEBSERVICE 0x1011c1e0
	// RBX::TreeStage::`scalar deleting destructor'

	// FUNCTION: WEBSERVICE 0x100a7740 FOLDED
	virtual StageType getStageType() { return TREE_STAGE; } // vtable+0x04

	virtual void stepWorld(int worldStepId, int uiStepId, bool throttling); // vtable+0x08

	virtual void onEdgeAdded(Edge* edge);         // vtable+0x10
	virtual void onEdgeRemoving(Edge* edge);      // vtable+0x14
	virtual int getMetric(MetricType metricType); // vtable+0x18

	void process();

	Primitive* heavyParent(int index, Primitive* primitive, Joint*& heaviest, int& heaviestIndex);
	void findHeaviestUpstream(Primitive* p0, Primitive* p1, Joint*& answer, int& heavySide);

	void insertJoint(Joint* j);
	Joint* findLightestDownstream(Primitive* p, Primitive*& newParent);

	void swap(Joint* deactivate, Joint* activate, Primitive* newParent);

	int getClumpDepth(Primitive* p);

	void dirtyAssembly(Assembly* a);

	void insertEdge(Edge* e);
	void eraseEdge(Edge* e);

	void buildDownstreamTree(Primitive* p, std::set<Primitive*>& tree);
	void rebuildClump(Joint* joint, Primitive* parent);
	void traverse(Joint* joint, Primitive* parent);

	AssemblyStage* getAssemblyStage() { return reinterpret_cast<AssemblyStage*>(getDownstream()); }

	void cleanAssembly(Assembly* assembly);
	void cleanEdge(Edge* edge);

	void dirtyAssemblies(Joint* j);
	void undirtyAssembly(Assembly* a);
	void destroyAssembly(Assembly* a);

private:
	int maxTreeDepth;               // 0x10
	bool dirty;                     // 0x14
	std::set<Assembly*> assemblies; // 0x18
	std::set<Edge*> edges;          // 0x24
};

DECOMP_SIZE_ASSERT(TreeStage, 0x30)

// VTABLE: WEBSERVICE 0x10247e4c
// SIZE 0x10
class ClumpStage : public IWorldStage
{
public:
	ClumpStage(IStage* upstream, World* world);

	virtual ~ClumpStage() {} // vtable+0x00

	void onPrimitiveCanSleepChanged(Primitive* p);

	void onPrimitiveAddedAnchor(Primitive* p);
	void onPrimitiveRemovedAnchor(Primitive* p);

	// SYNTHETIC: WEBSERVICE 0x1011b710
	// RBX::ClumpStage::`scalar deleting destructor'

	virtual StageType getStageType(); // vtable+0x04

	// FUNCTION: WEBSERVICE 0x101187e0 FOLDED
	virtual void stepWorld(int worldStepId, int uiStepId, bool throttling) // vtable+0x08
	{
		getDownstream()->stepWorld(worldStepId, uiStepId, throttling);
	}

	void onPrimitiveAdded(Primitive* p);
	void onPrimitiveRemoving(Primitive* p);

	void process();

	void stepUi(int frameCount);

private:
	TreeStage* getTreeStage() { return reinterpret_cast<TreeStage*>(getDownstream()); }
};

DECOMP_SIZE_ASSERT(ClumpStage, 0x10)

} // namespace RBX

#endif // V8WORLD_CLUMPSTAGE2_H
