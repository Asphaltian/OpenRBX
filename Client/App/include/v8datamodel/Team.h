#ifndef V8DATAMODEL_TEAM_H
#define V8DATAMODEL_TEAM_H

#include "decomp.h"
#include "reflection/property.h"
#include "v8datamodel/BrickColor.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sTeam[];

// SIZE 0x104
class Team : public DescribedCreatable<Team, Instance, sTeam>
{
public:
	Team();
	virtual ~Team();

	// SYNTHETIC: WEBSERVICE 0x100be200
	// RBX::Team::`scalar deleting destructor'

	// FUNCTION: WEBSERVICE 0x100e44b0 FOLDED
	// FUNCTION: ROBLOX 0x00633110 FOLDED
	virtual bool askSetParent(const Instance* instance) const { return true; }

	// FUNCTION: WEBSERVICE 0x100e44b0 FOLDED
	// FUNCTION: ROBLOX 0x00633110 FOLDED
	virtual bool askAddChild(const Instance* instance) const { return true; }

	int getScore() const;

	BrickColor getTeamColor() const;

	bool getAutoAssignable() const;

	void setScore(int score);
	void setTeamColor(BrickColor color);
	void setAutoAssignable(bool value);

	static Reflection::BoundProp<bool, 1> prop_AutoColorCharacters;

protected:
	int score;                // 0x0f8
	BrickColor color;         // 0x0fc
	bool autoAssignable;      // 0x100
	bool autoColorCharacters; // 0x101
};

DECOMP_SIZE_ASSERT(Team, 0x104)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021bd40
// RBX::`dynamic initializer for 'prop_Score''
// SYNTHETIC: WEBSERVICE 0x1021bd80
// RBX::`dynamic initializer for 'prop_Color''
// SYNTHETIC: WEBSERVICE 0x1021bdc0
// RBX::`dynamic initializer for 'prop_AutoAssignable''
// SYNTHETIC: WEBSERVICE 0x1021be00
// STUB: ROBLOX 0x007ead80 FOLDED
// `dynamic initializer for 'RBX::Team::prop_AutoColorCharacters''
// SYNTHETIC: WEBSERVICE 0x10223a20
// STUB: ROBLOX 0x007f3130
// `dynamic atexit destructor for 'RBX::Team::prop_AutoColorCharacters''
// SYNTHETIC: WEBSERVICE 0x10223a40
// RBX::`dynamic atexit destructor for 'prop_AutoAssignable''
// SYNTHETIC: WEBSERVICE 0x10223a60
// RBX::`dynamic atexit destructor for 'prop_Color''
// SYNTHETIC: WEBSERVICE 0x10223a80
// RBX::`dynamic atexit destructor for 'prop_Score''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_TEAM_H
