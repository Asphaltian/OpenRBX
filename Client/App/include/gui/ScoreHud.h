#ifndef GUI_SCOREHUD_H
#define GUI_SCOREHUD_H

#include "gui/GUI.h"
#include "placeholder.h"

#include <string>
#include <vector>

namespace RBX {

class RunService;

namespace Network {
class Players;
}

class BoardData
{
public:
	BoardData() {}

	void SetSize(int rows, int columns);
	void AddField(std::string field, int index);

	std::vector<std::vector<std::string> > data;
	std::vector<std::string> fields;
};

class ScoreHud : public GuiItem, public Listener<Instance, ChildAdded>, public Listener<Instance, ChildRemoved>
{
public:
	enum BoardType
	{
		PLAYER_LIST = 0,
		FFA = 1,
		TEAM_DEATHMATCH = 2,
		TEAM_LIST = 3
	};

protected:
	RunService* runService;
	Network::Players* players;

	void BuildBoardData(BoardData& boardData);
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
	virtual void onEvent(const Instance* source, ChildRemoved event);
	virtual void onEvent(const Instance* source, ChildAdded event);
	virtual void render2d(Adorn* adorn);
	void renderTeamDeathmatchLeaderboard(Adorn* adorn, BoardType boardType);
	void renderFFALeaderboard(Adorn* adorn);
	BoardType DetermineBoardType();

public:
	ScoreHud(const ServiceProvider* serviceProvider);
	virtual ~ScoreHud();
};

} // namespace RBX

#endif // GUI_SCOREHUD_H
