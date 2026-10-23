#ifndef HUMANOID_FLYING_H
#define HUMANOID_FLYING_H

#include "decomp.h"
#include "humanoid/Balancing.h"
#include "util/Name.h"

namespace RBX {

extern const char sFlying[];

// SIZE 0x2c
class Flying : public Named<Balancing, sFlying>
{
private:
	static const float turnSpeed;
	static const float kTurnP;
	static const float kTurnAccelMax;

	float desiredYRotationalVelocity; // 0x28

protected:
	virtual State* onStep(const float stepDt, Controller& controller); // vtable+0x0c
	virtual void onComputeForce(const float dt);                       // vtable+0x08

	Flying(Humanoid* humanoid);
};

DECOMP_SIZE_ASSERT(Flying, 0x2c)

} // namespace RBX

#endif // HUMANOID_FLYING_H
