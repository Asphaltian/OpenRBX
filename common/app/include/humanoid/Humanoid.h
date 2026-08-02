#ifndef HUMANOID_HUMANOID_H
#define HUMANOID_HUMANOID_H

#include "decomp.h"
#include "v8tree/Instance.h"

namespace RBX {

extern char sHumanoid[];

// SIZE 0x1f0
class Humanoid : public Instance
{
public:
	void setMaxHealth(float value);
	void setWalkRotationalVelocity(const float& value);
	void setJump(bool value);
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

} // namespace RBX

#endif // HUMANOID_HUMANOID_H
