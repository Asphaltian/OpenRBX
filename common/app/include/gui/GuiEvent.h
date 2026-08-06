#ifndef GUI_GUIEVENT_H
#define GUI_GUIEVENT_H

#include "decomp.h"

namespace RBX {

class GuiEvent;
class GuiItem;

// SIZE 0x8
class GuiResponse
{
public:
	enum ResponseType
	{
		NOT_USED = 0,
		USED = 1,
		USED_AND_FINISHED = 2
	};

	bool wasUsed() const { return response != NOT_USED; }

	GuiItem* getTarget() const { return target; }

private:
	ResponseType response; // 0x00
	GuiItem* target;       // 0x04
};

DECOMP_SIZE_ASSERT(GuiResponse, 0x8)

// SIZE 0x4
class __declspec(novtable) GuiTarget
{
public:
	virtual GuiResponse process(const GuiEvent& event) = 0; // vtable+0x00
};

DECOMP_SIZE_ASSERT(GuiTarget, 0x4)

} // namespace RBX

#endif // GUI_GUIEVENT_H
