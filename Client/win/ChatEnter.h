#ifndef CHATENTER_H
#define CHATENTER_H

#include "decomp.h"
#include "gui/GUI.h"

class CChatPrompt;

// SIZE 0x110
class ChatEnter : public RBX::GuiItem
{
public:
	ChatEnter();

	virtual RBX::GuiResponse process(const RBX::GuiEvent& event);

private:
	friend class CChatPrompt;

	CChatPrompt* m_unk0x10c; // 0x10c
};

DECOMP_SIZE_ASSERT(ChatEnter, 0x110)

// TEMPLATE: ROBLOX 0x0040e590
// RBX::Creatable<RBX::Instance>::create<ChatEnter>

#endif // CHATENTER_H
