#ifndef V8DATAMODEL_TEAM_H
#define V8DATAMODEL_TEAM_H

#include "decomp.h"
#include "v8datamodel/BrickColor.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sTeam[];

// SIZE 0x104
class Team : public DescribedCreatable<Team, Instance, sTeam>
{
public:
	int getScore() const { return score; }

	BrickColor getTeamColor() const { return color; }

	bool getAutoAssignable() const { return autoAssignable; }

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

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021bd40
// RBX::`dynamic initializer for 'prop_Score''
// STUB: WEBSERVICE 0x1021bd80
// RBX::`dynamic initializer for 'prop_Color''
// STUB: WEBSERVICE 0x1021bdc0
// RBX::`dynamic initializer for 'prop_AutoAssignable''
// STUB: WEBSERVICE 0x10223a40
// RBX::`dynamic atexit destructor for 'prop_AutoAssignable''
// STUB: WEBSERVICE 0x10223a60
// RBX::`dynamic atexit destructor for 'prop_Color''
// STUB: WEBSERVICE 0x10223a80
// RBX::`dynamic atexit destructor for 'prop_Score''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_TEAM_H
