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
};

DECOMP_SIZE_ASSERT(NavKeys, 0x09)

} // namespace RBX

#endif // UTIL_NAVKEYS_H
