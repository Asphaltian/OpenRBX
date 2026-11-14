#ifndef HUMANOID_FLYING_H
#define HUMANOID_FLYING_H

#include "humanoid/Balancing.h"
#include "placeholder.h"
#include "util/Name.h"

namespace RBX {

extern const char sFlying[];

class Flying : public Named<Balancing, sFlying>
{
private:
	static const float turnSpeed;
	static const float kTurnP;
	static const float kTurnAccelMax;

	float desiredYRotationalVelocity;

protected:
	virtual State* onStep(const float stepDt, Controller& controller);
	virtual void onComputeForce(const float dt);

	Flying(Humanoid* humanoid);
};

} // namespace RBX

#endif // HUMANOID_FLYING_H
