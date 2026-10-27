#ifndef GUI_GUIEVENT_H
#define GUI_GUIEVENT_H

#include "decomp.h"
#include "util/UIEvent.h"

namespace RBX {

class GuiTarget;

// SIZE 0x18
class GuiEvent : public UIEvent
{
public:
	void* iDataState; // 0x14
};

DECOMP_SIZE_ASSERT(GuiEvent, 0x18)

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

	bool wasUsed() { return response != NOT_USED; }
	bool wasNotUsed() { return response == NOT_USED; }
	bool wasFinished() { return response == USED_AND_FINISHED; }

	GuiTarget* getTarget() { return target; }

	static GuiResponse notUsed() { return GuiResponse(NOT_USED, NULL); }
	static GuiResponse used() { return GuiResponse(USED, NULL); }
	static GuiResponse used(GuiTarget* target) { return GuiResponse(USED, target); }
	static GuiResponse finished() { return GuiResponse(USED_AND_FINISHED, NULL); }

private:
	GuiResponse(ResponseType response, GuiTarget* target) : response(response), target(target) {}

public:

private:
	ResponseType response; // 0x00
	GuiTarget* target;     // 0x04
};

DECOMP_SIZE_ASSERT(GuiResponse, 0x8)

// SIZE 0x4
class GuiTarget
{
public:
	// FUNCTION: WEBSERVICE 0x1006a090
	virtual GuiResponse process(const GuiEvent& guiEvent) { return GuiResponse::notUsed(); } // vtable+0x00
};

DECOMP_SIZE_ASSERT(GuiTarget, 0x4)

} // namespace RBX

#endif // GUI_GUIEVENT_H
