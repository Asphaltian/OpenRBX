#ifndef V8WORLD_JOINTSTAGE_H
#define V8WORLD_JOINTSTAGE_H

#include "decomp.h"
#include "v8world/IWorldStage.h"

namespace RBX {

class Joint;
class Primitive;

// SIZE 0x28
class JointStage : public IWorldStage
{
public:
	virtual StageType getStageType() const;                                 // vtable+0x04
	virtual void stepWorld(int worldStepId, int uiStepId, bool throttling); // vtable+0x08

	virtual void onEdgeAdded(Edge* edge);         // vtable+0x10
	virtual void onEdgeRemoving(Edge* edge);      // vtable+0x14
	virtual int getMetric(MetricType metricType); // vtable+0x18

	void onJointPrimitiveNulling(Joint* joint, Primitive* primitive);
	void onJointPrimitiveSet(Joint* joint, Primitive* primitive);

private:
	undefined m_unk0x10[0x28 - 0x10]; // 0x10
};

DECOMP_SIZE_ASSERT(JointStage, 0x28)

} // namespace RBX

#endif // V8WORLD_JOINTSTAGE_H
