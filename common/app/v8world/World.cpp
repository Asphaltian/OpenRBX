#include "v8world/World.h"

#include "decomp.h"
#include "v8kernel/Kernel.h"
#include "v8world/JointStage.h"
#include "v8world/SpatialHash.h"

namespace RBX {

bool World::disableEnvironmentalThrottle;

// FUNCTION: WEBSERVICE 0x100cef80
Kernel* World::getKernel() const
{
	return jointStage->getKernel();
}

// FUNCTION: WEBSERVICE 0x100cef90
int World::getNumBodies() const
{
	return getKernel()->numBodies();
}

// FUNCTION: WEBSERVICE 0x100cefb0
int World::getNumPoints() const
{
	return getKernel()->numPoints();
}

// FUNCTION: WEBSERVICE 0x100cefd0
int World::getNumConstraints() const
{
	return getKernel()->numConnectors();
}

// FUNCTION: WEBSERVICE 0x100ceff0
int World::getMetric(IWorldStage::MetricType metricType) const
{
	return jointStage->getMetric(metricType);
}

// FUNCTION: WEBSERVICE 0x100cf000
int World::getNumHashNodes() const
{
	return contactManager->getSpatialHash()->getNumHashNodes();
}

// FUNCTION: WEBSERVICE 0x100cf010
int World::getMaxBucketSize() const
{
	return contactManager->getSpatialHash()->getMaxBucketSize();
}

// STUB: WEBSERVICE 0x100cf020
void World::onPrimitiveContactParametersChanged(Primitive* primitive)
{
	STUB(0x100cf020);
}

// STUB: WEBSERVICE 0x100cf050
void World::onPrimitiveExtentsChanged(Primitive* primitive)
{
	STUB(0x100cf050);
}

// STUB: WEBSERVICE 0x100cf060
void World::onAssemblyExtentsChanged(Assembly* assembly)
{
	STUB(0x100cf060);
}

// STUB: WEBSERVICE 0x100cf0c0
void World::onPrimitiveGeometryTypeChanged(Primitive* primitive)
{
	STUB(0x100cf0c0);
}

// FUNCTION: WEBSERVICE 0x100cf0d0
void World::onJointPrimitiveNulling(Joint* joint, Primitive* primitive)
{
	jointStage->onJointPrimitiveNulling(joint, primitive);
}

// FUNCTION: WEBSERVICE 0x100cf0e0
void World::onJointPrimitiveSet(Joint* joint, Primitive* primitive)
{
	jointStage->onJointPrimitiveSet(joint, primitive);
}

// STUB: WEBSERVICE 0x100cf1a0
void World::ticklePrimitive(Primitive* primitive, bool value)
{
	STUB(0x100cf1a0);
}

// STUB: WEBSERVICE 0x100cf1f0
void World::onPrimitiveCanSleepChanged(Primitive* primitive)
{
	STUB(0x100cf1f0);
}

// STUB: WEBSERVICE 0x100cf230
void World::onPrimitiveAddedAnchor(Primitive* primitive)
{
	STUB(0x100cf230);
}

// STUB: WEBSERVICE 0x100cf270
void World::onPrimitiveRemovedAnchor(Primitive* primitive)
{
	STUB(0x100cf270);
}

// STUB: WEBSERVICE 0x100cf2b0
void World::onPrimitiveCanCollideChanged(Primitive* primitive)
{
	STUB(0x100cf2b0);
}

// STUB: WEBSERVICE 0x100cf510
void World::onPrimitiveTouched(Primitive* p0, Primitive* p1)
{
	STUB(0x100cf510);
}

} // namespace RBX
