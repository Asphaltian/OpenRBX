#ifndef UTIL_NAVKEYS_H
#define UTIL_NAVKEYS_H

#include "decomp.h"

namespace RBX {

// SIZE 0x09
class NavKeys
{
public:
	bool forward_arrow;  // 0x00
	bool backward_arrow; // 0x01
	bool left_arrow;     // 0x02
	bool right_arrow;    // 0x03
	bool forward_asdw;   // 0x04
	bool backward_asdw;  // 0x05
	bool left_asdw;      // 0x06
	bool right_asdw;     // 0x07
	bool space;          // 0x08

	NavKeys()
		: forward_arrow(false), backward_arrow(false), left_arrow(false), right_arrow(false), forward_asdw(false),
		  backward_asdw(false), left_asdw(false), right_asdw(false), space(false)
	{
	}

	bool forward() const;
	bool backward() const;
	bool left() const;
	bool right() const;

	bool arrowKeyDown() const { return forward_arrow || backward_arrow || left_arrow || right_arrow; }
	// FUNCTION: WEBSERVICE 0x1006a020
	bool asdwKeyDown() const { return forward_asdw || backward_asdw || left_asdw || right_asdw; }
	// FUNCTION: WEBSERVICE 0x1006a050
	bool navKeyDown() const { return arrowKeyDown() || asdwKeyDown() || space; }

	// FUNCTION: WEBSERVICE 0x100c5400
	float leftRightASDW() const { return left_asdw ? 1.0f : (right_asdw ? -1.0f : 0.0f); }

	// FUNCTION: WEBSERVICE 0x100c5420
	float leftRightArrow() const { return left_arrow ? 1.0f : (right_arrow ? -1.0f : 0.0f); }

	float forwardBackwardArrow() const { return forward_arrow ? 1.0f : (backward_arrow ? -1.0f : 0.0f); }

	// FUNCTION: WEBSERVICE 0x100c5440
	float forwardBackwardASDW() const { return forward_asdw ? 1.0f : (backward_asdw ? -1.0f : 0.0f); }
};

DECOMP_SIZE_ASSERT(NavKeys, 0x09)

} // namespace RBX

#endif // UTIL_NAVKEYS_H
