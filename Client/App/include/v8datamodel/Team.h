#ifndef V8DATAMODEL_TEAM_H
#define V8DATAMODEL_TEAM_H

#include "reflection/property.h"
#include "placeholder.h"
#include "v8datamodel/BrickColor.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sTeam[];

class Team : public DescribedCreatable<Team, Instance, sTeam>
{
public:
	Team();
	virtual ~Team();

	virtual bool askSetParent(const Instance* instance) const { return true; }

	virtual bool askAddChild(const Instance* instance) const { return true; }

	int getScore() const;

	BrickColor getTeamColor() const;

	bool getAutoAssignable() const;

	void setScore(int score);
	void setTeamColor(BrickColor color);
	void setAutoAssignable(bool value);

	static Reflection::BoundProp<bool, 1> prop_AutoColorCharacters;

protected:
	int score;
	BrickColor color;
	bool autoAssignable;
	bool autoColorCharacters;
};

} // namespace RBX

#endif // V8DATAMODEL_TEAM_H
