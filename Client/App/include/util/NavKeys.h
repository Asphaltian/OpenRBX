#ifndef UTIL_NAVKEYS_H
#define UTIL_NAVKEYS_H

#include "placeholder.h"

namespace RBX {

class NavKeys
{
public:
	bool forward_arrow;
	bool backward_arrow;
	bool left_arrow;
	bool right_arrow;
	bool forward_asdw;
	bool backward_asdw;
	bool left_asdw;
	bool right_asdw;
	bool space;

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
	bool asdwKeyDown() const { return forward_asdw || backward_asdw || left_asdw || right_asdw; }
	bool navKeyDown() const { return arrowKeyDown() || asdwKeyDown() || space; }

	float leftRightASDW() const { return left_asdw ? 1.0f : (right_asdw ? -1.0f : 0.0f); }

	float leftRightArrow() const { return left_arrow ? 1.0f : (right_arrow ? -1.0f : 0.0f); }

	float forwardBackwardArrow() const { return forward_arrow ? 1.0f : (backward_arrow ? -1.0f : 0.0f); }

	float forwardBackwardASDW() const { return forward_asdw ? 1.0f : (backward_asdw ? -1.0f : 0.0f); }
};

} // namespace RBX

#endif // UTIL_NAVKEYS_H
