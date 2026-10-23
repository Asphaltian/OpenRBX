#ifndef HUMANOID_JUMPING_H
#define HUMANOID_JUMPING_H

#include "decomp.h"
#include "humanoid/Flying.h"
#include "util/Name.h"

namespace RBX {

extern const char sJumping[];

// SIZE 0x34
class Jumping : public Named<Flying, sJumping>
{
private:
	bool done; // 0x2c

	static const float kJumpP;
	static const float jumpVelocityGrid;

	float timeout; // 0x30

	virtual State* onStep(const float stepDt, Controller& controller); // vtable+0x0c
	virtual void onComputeForce(const float dt);                       // vtable+0x08

public:
	Jumping(Humanoid* humanoid);

	virtual ~Jumping(); // vtable+0x04

	// SYNTHETIC: WEBSERVICE 0x10122fd0
	// RBX::Jumping::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(Jumping, 0x34)

} // namespace RBX

#endif // HUMANOID_JUMPING_H
