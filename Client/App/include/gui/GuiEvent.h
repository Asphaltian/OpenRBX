#ifndef GUI_GUIEVENT_H
#define GUI_GUIEVENT_H

#include "placeholder.h"
#include "util/UIEvent.h"

namespace RBX {

class GuiTarget;
class IDataState;

class GuiEvent : public UIEvent
{
public:
	IDataState* iDataState;

	GuiEvent(const UIEvent& event, IDataState* iDataState) : UIEvent(event), iDataState(iDataState) {}
};

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
	ResponseType response;
	GuiTarget* target;
};

class GuiTarget
{
public:
	virtual GuiResponse process(const GuiEvent& guiEvent) { return GuiResponse::notUsed(); }
};

} // namespace RBX

#endif // GUI_GUIEVENT_H
