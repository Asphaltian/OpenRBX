#ifndef V8DATAMODEL_TEAMS_H
#define V8DATAMODEL_TEAMS_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <G3D/Color3.h>

namespace RBX {

class Humanoid;

extern const char sTeams[];
// SIZE 0x100
class Teams : public DescribedCreatable<Teams, Instance, sTeams>, public Service
{
public:
	G3D::Color3 getTeamColorForHumanoid(Humanoid* humanoid);

private:
	undefined m_unk0x0f8[0x100 - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(Teams, 0x100)

} // namespace RBX

#endif // V8DATAMODEL_TEAMS_H
