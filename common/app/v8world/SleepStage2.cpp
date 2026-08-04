#include "v8world/SleepStage2.h"

#include "decomp.h"

namespace RBX {

// STUB: WEBSERVICE 0x10118f90
int SleepStage::getMetric(MetricType metricType)
{
	STUB(0x10118f90);
	return 0;
}

// STUB: WEBSERVICE 0x101193f0
void SleepStage::onAssemblyAdded(Assembly* assembly)
{
	STUB(0x101193f0);
}

// STUB: WEBSERVICE 0x10119530
SleepStage::~SleepStage()
{
	STUB(0x10119530);
}

// STUB: WEBSERVICE 0x10119970
void SleepStage::onAssemblyRemoving(Assembly* assembly)
{
	STUB(0x10119970);
}

// STUB: WEBSERVICE 0x10119ae0
void SleepStage::onEdgeAdded(Edge* edge)
{
	STUB(0x10119ae0);
}

// STUB: WEBSERVICE 0x10119ba0
SleepStage::SleepStage(IStage* upstream, World* world) : IWorldStage(upstream, NULL, world)
{
	STUB(0x10119ba0);
}

// STUB: WEBSERVICE 0x10119fd0
void SleepStage::onWakeUpRequest(Assembly* assembly, bool recursive)
{
	STUB(0x10119fd0);
}

// STUB: WEBSERVICE 0x1011a680
void SleepStage::onEdgeRemoving(Edge* edge)
{
	STUB(0x1011a680);
}

// STUB: WEBSERVICE 0x1011b580
IStage::StageType SleepStage::getStageType()
{
	return SLEEP_STAGE;
}

} // namespace RBX
