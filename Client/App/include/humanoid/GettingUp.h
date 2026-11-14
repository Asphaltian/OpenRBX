#ifndef HUMANOID_GETTINGUP_H
#define HUMANOID_GETTINGUP_H

#include "humanoid/Balancing.h"
#include "placeholder.h"
#include "util/Name.h"

namespace RBX {

extern const char sGettingUp[];

class GettingUp : public Named<Balancing, sGettingUp>
{
protected:
	virtual State* onStep(const float stepDt, Controller& controller);

public:
	GettingUp(Humanoid* humanoid);

	virtual ~GettingUp();
};

} // namespace RBX

#endif // HUMANOID_GETTINGUP_H
