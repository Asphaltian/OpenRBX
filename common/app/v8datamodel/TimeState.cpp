#include "v8datamodel/TimeState.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x100dd970
TimeState::TimeState() : totalVirtualTime(0)
{
}

// FUNCTION: WEBSERVICE 0x100dd990
void TimeState::clear()
{
	totalVirtualTime = 0;
}

} // namespace RBX
