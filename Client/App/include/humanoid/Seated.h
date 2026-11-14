#ifndef HUMANOID_SEATED_H
#define HUMANOID_SEATED_H

#include "humanoid/Humanoid.h"
#include "placeholder.h"
#include "util/Name.h"

namespace RBX {

extern const char sSeated[];

class Seated : public Named<Humanoid::State, sSeated>
{
private:
	virtual State* onStep(const float stepDt, Controller& controller);

	virtual void onComputeForce(const float dt) {}

public:
	Seated(Humanoid* humanoid);

	virtual ~Seated();
};

} // namespace RBX

#endif // HUMANOID_SEATED_H
