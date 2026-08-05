#ifndef V8DATAMODEL_TIMESTATE_H
#define V8DATAMODEL_TIMESTATE_H

#include "decomp.h"

#include <G3D/Stopwatch.h>

namespace RBX {

// SIZE 0xb8
class TimeState
{
public:
	TimeState();

	void clear();

	G3D::Stopwatch simulationStopwatch; // 0x00
	G3D::Stopwatch fpsStopwatch;        // 0x58
	float totalVirtualTime;             // 0xb0
};

DECOMP_SIZE_ASSERT(TimeState, 0xb8)

} // namespace RBX

#endif // V8DATAMODEL_TIMESTATE_H
