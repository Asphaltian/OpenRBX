#ifndef HUMANOID_JUMPING_H
#define HUMANOID_JUMPING_H

#include "humanoid/Flying.h"
#include "placeholder.h"
#include "util/Name.h"

namespace RBX {

extern const char sJumping[];

class Jumping : public Named<Flying, sJumping>
{
private:
	bool done;

	static const float kJumpP;
	static const float jumpVelocityGrid;

	float timeout;

	virtual State* onStep(const float stepDt, Controller& controller);
	virtual void onComputeForce(const float dt);

public:
	Jumping(Humanoid* humanoid);

	virtual ~Jumping();
};

} // namespace RBX

#endif // HUMANOID_JUMPING_H
