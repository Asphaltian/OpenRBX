#ifndef V8WORLD_CLUMPSTAGE2_H
#define V8WORLD_CLUMPSTAGE2_H

#include "decomp.h"
#include "v8world/IWorldStage.h"
#include "v8world/Joint.h"

#include <set>

namespace RBX {

class Assembly;
class Primitive;

float getPrimitiveSize(Primitive* primitive);

int biggerJointGuid(Joint* joint0, Joint* joint1);
int biggerJointSize(Joint* joint0, Joint* joint1);

Joint* getJoint(Primitive* primitive, Joint::JointType jointType);

namespace JointSort {

bool lighterJoint(Joint* joint0, Joint* joint1);

} // namespace JointSort

// SIZE 0x30
class TreeStage : public IWorldStage
{
public:
	virtual StageType getStageType() const;                                 // vtable+0x04
	virtual void stepWorld(int worldStepId, int uiStepId, bool throttling); // vtable+0x08

	virtual void onEdgeAdded(Edge* edge);         // vtable+0x10
	virtual void onEdgeRemoving(Edge* edge);      // vtable+0x14
	virtual int getMetric(MetricType metricType); // vtable+0x18

	void process();

	void swap(Joint* remove, Joint* add, Primitive* root);

	void traverse(Joint* joint, Primitive* root);

	void dirtyAssemblies(Joint* joint);
	void undirtyAssembly(Assembly* assembly);
	void destroyAssembly(Assembly* assembly);

private:
	int maxTreeDepth;               // 0x10
	bool dirty;                     // 0x14
	std::set<Assembly*> assemblies; // 0x18
	std::set<Edge*> edges;          // 0x24
};

DECOMP_SIZE_ASSERT(TreeStage, 0x30)

// SIZE 0x10
class ClumpStage : public IWorldStage
{
public:
	virtual StageType getStageType() const;                                 // vtable+0x04
	virtual void stepWorld(int worldStepId, int uiStepId, bool throttling); // vtable+0x08

	virtual void onEdgeAdded(Edge* edge);         // vtable+0x10
	virtual void onEdgeRemoving(Edge* edge);      // vtable+0x14
	virtual int getMetric(MetricType metricType); // vtable+0x18
};

DECOMP_SIZE_ASSERT(ClumpStage, 0x10)

} // namespace RBX

#endif // V8WORLD_CLUMPSTAGE2_H
