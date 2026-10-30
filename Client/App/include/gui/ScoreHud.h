#ifndef GUI_SCOREHUD_H
#define GUI_SCOREHUD_H

#include "decomp.h"
#include "gui/GUI.h"

#include <string>
#include <vector>

namespace RBX {

class RunService;

namespace Network {
class Players;
}

// SIZE 0x20
class BoardData
{
public:
	BoardData() {}

	void SetSize(int rows, int columns);
	void AddField(std::string field, int index);

	std::vector<std::vector<std::string> > data; // 0x00
	std::vector<std::string> fields;             // 0x10

	// SYNTHETIC: ROBLOX 0x0065cd90
	// RBX::BoardData::~BoardData
};

DECOMP_SIZE_ASSERT(BoardData, 0x20)

// SIZE 0x11c
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
	RunService* runService;    // 0x114
	Network::Players* players; // 0x118

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

	// SYNTHETIC: ROBLOX 0x0065a0c0
	// RBX::ScoreHud::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(ScoreHud, 0x11c)

} // namespace RBX

#endif // GUI_SCOREHUD_H
