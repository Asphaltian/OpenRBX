#ifndef HUMANOID_FALLINGDOWN_H
#define HUMANOID_FALLINGDOWN_H

#include "humanoid/Humanoid.h"
#include "placeholder.h"
#include "util/Name.h"

namespace RBX {

extern const char sFallingDown[];

class FallingDown : public Named<Humanoid::State, sFallingDown>
{
private:
	float time;
	const float timeToGetUp;

	virtual State* onStep(const float stepDt, Controller& controller);

	virtual void onComputeForce(const float dt) {}

public:
	FallingDown(Humanoid* humanoid, float timeToGetUp);

	virtual ~FallingDown();
};

} // namespace RBX

#endif // HUMANOID_FALLINGDOWN_H
