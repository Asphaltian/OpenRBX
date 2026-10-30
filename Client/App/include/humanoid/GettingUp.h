#ifndef HUMANOID_GETTINGUP_H
#define HUMANOID_GETTINGUP_H

#include "decomp.h"
#include "humanoid/Balancing.h"
#include "util/Name.h"

namespace RBX {

extern const char sGettingUp[];

// SIZE 0x28
class GettingUp : public Named<Balancing, sGettingUp>
{
protected:
	virtual State* onStep(const float stepDt, Controller& controller); // vtable+0x0c

public:
	GettingUp(Humanoid* humanoid);

	virtual ~GettingUp(); // vtable+0x04

	// SYNTHETIC: WEBSERVICE 0x10123320
	// SYNTHETIC: ROBLOX 0x006643e0
	// RBX::GettingUp::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(GettingUp, 0x28)

} // namespace RBX

#endif // HUMANOID_GETTINGUP_H
