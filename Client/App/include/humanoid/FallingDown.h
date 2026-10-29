#ifndef HUMANOID_FALLINGDOWN_H
#define HUMANOID_FALLINGDOWN_H

#include "decomp.h"
#include "humanoid/Humanoid.h"
#include "util/Name.h"

namespace RBX {

extern const char sFallingDown[];

// SIZE 0x10
class FallingDown : public Named<Humanoid::State, sFallingDown>
{
private:
	float time;              // 0x08
	const float timeToGetUp; // 0x0c

	virtual State* onStep(const float stepDt, Controller& controller); // vtable+0x0c

	// FUNCTION: WEBSERVICE 0x100d15c0 FOLDED
	// FUNCTION: ROBLOX 0x0045e450 FOLDED
	virtual void onComputeForce(const float dt) // vtable+0x08
	{
	}

public:
	FallingDown(Humanoid* humanoid, float timeToGetUp);

	virtual ~FallingDown(); // vtable+0x04

	// SYNTHETIC: WEBSERVICE 0x10107fb0
	// RBX::FallingDown::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(FallingDown, 0x10)

} // namespace RBX

#endif // HUMANOID_FALLINGDOWN_H
