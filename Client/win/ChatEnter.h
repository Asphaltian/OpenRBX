#ifndef CHATENTER_H
#define CHATENTER_H

#include "gui/GUI.h"
#include "placeholder.h"

class CChatPrompt;

class ChatEnter : public RBX::GuiItem
{
public:
	ChatEnter();

	virtual RBX::GuiResponse process(const RBX::GuiEvent& event);

private:
	friend class CChatPrompt;

	CChatPrompt* m_unk0x10c;
};

#endif // CHATENTER_H
