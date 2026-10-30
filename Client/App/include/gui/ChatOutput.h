#ifndef GUI_CHATOUTPUT_H
#define GUI_CHATOUTPUT_H

#include "decomp.h"
#include "gui/GUI.h"
#include "gui/ProfanityFilter.h"
#include "util/Events.h"
#include "util/RunStateOwner.h"

#include <G3D/Color3.h>
#include <deque>
#include <string>

namespace RBX {

namespace Network {
class Player;
class Players;
struct ChatMessage;
} // namespace Network

// SIZE 0x4c
class ChatLine
{
public:
	G3D::Color3 userColor; // 0x00
	std::string user;      // 0x0c
	std::string message;   // 0x28
	float dieTime;         // 0x44
	bool profane;          // 0x48

	ChatLine(Network::Player* p_unk0x04, const std::string& p_unk0x08, float p_unk0x0c);
	ChatLine(const std::string& p_unk0x04, const std::string& p_unk0x08, float p_unk0x0c);
	~ChatLine();
};

DECOMP_SIZE_ASSERT(ChatLine, 0x4c)

// SIZE 0x140
class ChatOutput : public GuiItem,
				   public Listener<Network::Players, Network::ChatMessage>,
				   public Listener<RunService, Heartbeat>
{
private:
	RunService* runService;     // 0x114
	Network::Players* players;  // 0x118
	ProfanityFilter filter;     // 0x11c
	std::deque<ChatLine*> fifo; // 0x128
	float time;                 // 0x13c

	void removeOldest();
	bool frontIsOld();
	bool ParseCommand(Network::ChatMessage p_unk0x04);

	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
	virtual void onEvent(const Network::Players* source, Network::ChatMessage event);
	virtual void onEvent(const RunService* source, Heartbeat event);
	virtual void render2d(Adorn* adorn);

public:
	ChatOutput();
	virtual ~ChatOutput();

	// SYNTHETIC: ROBLOX 0x00659310
	// RBX::ChatOutput::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(ChatOutput, 0x140)

// clang-format off
// STUB: ROBLOX 0x006593b0
// ?addListener@?$Notifier@VPlayers@Network@RBX@@UChatMessage@23@@RBX@@QBEXPAV?$Listener@VPlayers@Network@RBX@@UChatMessage@23@@2@@Z
// clang-format on

} // namespace RBX

#endif // GUI_CHATOUTPUT_H
