#ifndef HUMANOID_FREEFALL_H
#define HUMANOID_FREEFALL_H

#include "humanoid/Flying.h"
#include "placeholder.h"
#include "util/Name.h"

namespace RBX {

extern const char sFreefall[];

class Freefall : public Named<Flying, sFreefall>
{
public:
	Freefall(Humanoid* humanoid);

private:
	virtual State* onStep(const float stepDt, Controller& controller);
};

} // namespace RBX

#endif // HUMANOID_FREEFALL_H
