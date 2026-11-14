#include "v8datamodel/TimeState.h"

namespace RBX {

TimeState::TimeState() : totalVirtualTime(0)
{
}

void TimeState::clear()
{
	totalVirtualTime = 0;
}

} // namespace RBX
