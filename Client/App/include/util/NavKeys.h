#ifndef UTIL_NAVKEYS_H
#define UTIL_NAVKEYS_H

#include "decomp.h"

namespace RBX {

// SIZE 0x09
class NavKeys
{
public:
	NavKeys()
		: forward_arrow(false), backward_arrow(false), left_arrow(false), right_arrow(false), forward_asdw(false),
		  backward_asdw(false), left_asdw(false), right_asdw(false), space(false)
	{
	}

	bool arrowKeyDown() const { return forward_arrow || backward_arrow || left_arrow || right_arrow; }
	// FUNCTION: WEBSERVICE 0x1006a020
	bool asdwKeyDown() const { return forward_asdw || backward_asdw || left_asdw || right_asdw; }
	// FUNCTION: WEBSERVICE 0x1006a050
	bool navKeyDown() const { return arrowKeyDown() || asdwKeyDown() || space; }

	bool forward_arrow;  // 0x00
	bool backward_arrow; // 0x01
	bool left_arrow;     // 0x02
	bool right_arrow;    // 0x03
	bool forward_asdw;   // 0x04
	bool backward_asdw;  // 0x05
	bool left_asdw;      // 0x06
	bool right_asdw;     // 0x07
	bool space;          // 0x08
};

DECOMP_SIZE_ASSERT(NavKeys, 0x09)

} // namespace RBX

#endif // UTIL_NAVKEYS_H
