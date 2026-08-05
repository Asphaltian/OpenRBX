#ifndef V8DATAMODEL_SPAWNLOCATION_H
#define V8DATAMODEL_SPAWNLOCATION_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8datamodel/BrickColor.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sSpawnerService[];

extern char sSpawnLocation[];

// SIZE 0x2cc
class SpawnLocation : public Instance
{
public:
	BrickColor getTeamColor() const { return teamColor; }

	void setTeamColor(BrickColor value);

private:
	undefined m_unk0x0f8[0x290 - 0x0f8]; // 0x0f8
	BrickColor teamColor;                // 0x290
	undefined m_unk0x294[0x2a8 - 0x294]; // 0x294
	bool neutral;                        // 0x2a8
	bool allowTeamChangeOnTouch;         // 0x2a9
	undefined m_unk0x2aa[0x2ac - 0x2aa]; // 0x2aa
	int forcefieldDuration;              // 0x2ac
	undefined m_unk0x2b0[0x2cc - 0x2b0]; // 0x2b0
};

DECOMP_SIZE_ASSERT(SpawnLocation, 0x2cc)

// SIZE 0x104
class SpawnerService : public DescribedCreatable<SpawnerService, Instance, sSpawnerService>
{
private:
	undefined m_unk0x0f8[0x104 - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(SpawnerService, 0x104)

} // namespace RBX

#endif // V8DATAMODEL_SPAWNLOCATION_H
