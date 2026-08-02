#ifndef V8DATAMODEL_TEAM_H
#define V8DATAMODEL_TEAM_H

#include "decomp.h"
#include "v8datamodel/BrickColor.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sTeam[];

// SIZE 0x104
class Team : public Instance
{
public:
	void setScore(int value);
	void setTeamColor(BrickColor value);
	void setAutoAssignable(bool value);

private:
	int score;                           // 0x0f8
	BrickColor color;                    // 0x0fc
	bool autoAssignable;                 // 0x100
	bool autoColorCharacters;            // 0x101
	undefined m_unk0x102[0x104 - 0x102]; // 0x102
};

DECOMP_SIZE_ASSERT(Team, 0x104)

} // namespace RBX

#endif // V8DATAMODEL_TEAM_H
