#ifndef HUMANOID_SEATED_H
#define HUMANOID_SEATED_H

#include "decomp.h"
#include "humanoid/Humanoid.h"
#include "util/Name.h"

namespace RBX {

extern const char sSeated[];

// SIZE 0x08
class Seated : public Named<Humanoid::State, sSeated>
{
private:
	virtual State* onStep(const float stepDt, Controller& controller); // vtable+0x0c

	// FUNCTION: WEBSERVICE 0x100d15c0 FOLDED
	virtual void onComputeForce(const float dt) // vtable+0x08
	{
	}

public:
	Seated(Humanoid* humanoid);

	virtual ~Seated(); // vtable+0x04

	// SYNTHETIC: WEBSERVICE 0x10122bc0
	// RBX::Seated::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(Seated, 0x08)

} // namespace RBX

#endif // HUMANOID_SEATED_H
