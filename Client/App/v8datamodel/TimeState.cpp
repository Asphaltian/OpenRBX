#include "v8datamodel/TimeState.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x100dd970
// FUNCTION: ROBLOX 0x006171b0
TimeState::TimeState() : totalVirtualTime(0)
{
}

// FUNCTION: WEBSERVICE 0x100dd990
// FUNCTION: ROBLOX 0x006171d0
void TimeState::clear()
{
	totalVirtualTime = 0;
}

} // namespace RBX
