#ifndef V8DATAMODEL_TIMESTATE_H
#define V8DATAMODEL_TIMESTATE_H

#include "placeholder.h"

#include <G3D/Stopwatch.h>

namespace RBX {

class TimeState
{
public:
	TimeState();

	void clear();

	G3D::Stopwatch simulationStopwatch;
	G3D::Stopwatch fpsStopwatch;
	float totalVirtualTime;
};

} // namespace RBX

#endif // V8DATAMODEL_TIMESTATE_H
