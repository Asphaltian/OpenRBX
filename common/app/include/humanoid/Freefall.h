#ifndef HUMANOID_FREEFALL_H
#define HUMANOID_FREEFALL_H

#include "decomp.h"
#include "humanoid/Flying.h"
#include "util/Name.h"

namespace RBX {

extern const char sFreefall[];

// SIZE 0x2c
class Freefall : public Named<Flying, sFreefall>
{
public:
	Freefall(Humanoid* humanoid);

private:
	virtual State* onStep(const float stepDt, Controller& controller); // vtable+0x0c
};

DECOMP_SIZE_ASSERT(Freefall, 0x2c)

} // namespace RBX

#endif // HUMANOID_FREEFALL_H
