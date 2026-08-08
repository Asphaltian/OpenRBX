#ifndef V8DATAMODEL_FLAGSTAND_H
#define V8DATAMODEL_FLAGSTAND_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8datamodel/BrickColor.h"
#include "v8datamodel/PartInstance.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

namespace RBX {

extern const char sFlagStandService[];

extern char sFlagStand[];

// SIZE 0x2c4
class FlagStand : public DescribedCreatable<FlagStand, PartInstance, sFlagStand>
{
public:
	void setTeamColor(BrickColor value);

private:
	undefined m_unk0x290[0x2a4 - 0x290]; // 0x290
	BrickColor teamColor;                // 0x2a4
};

DECOMP_SIZE_ASSERT(FlagStand, 0x2c4)

// SIZE 0x104
class FlagStandService : public DescribedCreatable<FlagStandService, Instance, sFlagStandService>, public Service
{
private:
	undefined m_unk0x0f8[0x104 - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(FlagStandService, 0x104)

} // namespace RBX

#endif // V8DATAMODEL_FLAGSTAND_H
