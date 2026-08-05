#ifndef HUMANOID_HUMANOID_H
#define HUMANOID_HUMANOID_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"

namespace RBX {

extern char sHumanoid[];

// SIZE 0x1f0
class Humanoid : public DescribedCreatable<Humanoid, Instance, sHumanoid>
{
public:
	// FUNCTION: WEBSERVICE 0x100a0750
	float getMaxHealth() const { return maxHealth; }

	void setMaxHealth(float value);
	// FUNCTION: WEBSERVICE 0x100a0790
	float getWalkRotationalVelocity() const { return walkRotationalVelocity; }

	void setWalkRotationalVelocity(const float& value);
	// FUNCTION: WEBSERVICE 0x100a07b0
	bool getJump() const { return jump; }

	void setJump(bool value);
	// FUNCTION: WEBSERVICE 0x100a07c0
	bool getSit() const { return sit; }

	void setSit(bool value);

private:
	undefined m_unk0x0f8[0x16c - 0x0f8]; // 0x0f8
	float health;                        // 0x16c
	float maxHealth;                     // 0x170
	float walkRotationalVelocity;        // 0x174
	undefined m_unk0x178[0x1ac - 0x178]; // 0x178
	bool jump : 1;                       // 0x1ac
	bool imDead : 1;                     // 0x1ac
	bool hadHeadJoint : 1;               // 0x1ac
	bool sit : 1;                        // 0x1ac
	undefined m_unk0x1b0[0x1f0 - 0x1b0]; // 0x1b0
};

DECOMP_SIZE_ASSERT(Humanoid, 0x1f0)

// clang-format off
// STUB: WEBSERVICE 0x1021b310
// RBX::`dynamic initializer for 'propWalkRotationalVelocity''
// STUB: WEBSERVICE 0x1021b370
// RBX::`dynamic initializer for 'propJump''
// STUB: WEBSERVICE 0x1021b430
// RBX::`dynamic initializer for 'propSit''
// STUB: WEBSERVICE 0x1021b4f0
// RBX::`dynamic initializer for 'propMaxHealth''
// clang-format on

} // namespace RBX

#endif // HUMANOID_HUMANOID_H
