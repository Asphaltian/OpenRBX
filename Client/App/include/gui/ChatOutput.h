#ifndef GUI_CHATOUTPUT_H
#define GUI_CHATOUTPUT_H

#include "gui/GUI.h"
#include "gui/ProfanityFilter.h"
#include "placeholder.h"
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

class ChatLine
{
public:
	G3D::Color3 userColor;
	std::string user;
	std::string message;
	float dieTime;
	bool profane;

	ChatLine(Network::Player* p_unk0x04, const std::string& p_unk0x08, float p_unk0x0c);
	ChatLine(const std::string& p_unk0x04, const std::string& p_unk0x08, float p_unk0x0c);
	~ChatLine();
};

class ChatOutput : public GuiItem,
				   public Listener<Network::Players, Network::ChatMessage>,
				   public Listener<RunService, Heartbeat>
{
private:
	RunService* runService;
	Network::Players* players;
	ProfanityFilter filter;
	std::deque<ChatLine*> fifo;
	float time;

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
};

} // namespace RBX

#endif // GUI_CHATOUTPUT_H
