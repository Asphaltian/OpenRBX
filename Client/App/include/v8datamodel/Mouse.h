#ifndef V8DATAMODEL_MOUSE_H
#define V8DATAMODEL_MOUSE_H

#include "decomp.h"
#include "util/TextureId.h"
#include "util/UIEvent.h"
#include "v8tree/Instance.h"

#include <G3D/CoordinateFrame.h>
#include <stdexcept>

namespace RBX {

class MouseCommand;
class PartInstance;

extern const char sMouse[];

// SIZE 0x130
class Mouse : public DescribedNonCreatable<Mouse, Instance, sMouse>
{
private:
	UIEvent lastEvent;     // 0xf8
	TextureId icon;        // 0x10c
	MouseCommand* command; // 0x12c

	// FUNCTION: WEBSERVICE 0x10117350
	// FUNCTION: ROBLOX 0x00656ae0
	void cacheUIEvent(const UIEvent& uiEvent)
	{
		lastEvent = uiEvent;
		lastEvent.eventType = UIEvent::NO_EVENT;
	}

	// FUNCTION: WEBSERVICE 0x10116f40
	// FUNCTION: ROBLOX 0x00640510
	void checkActive() const
	{
		if (command == NULL) {
			throw std::runtime_error("This Mouse is no longer active");
		}
	}

public:
	// FUNCTION: WEBSERVICE 0x100cbf60
	// FUNCTION: ROBLOX 0x00610b30
	Mouse() : command(NULL) {}

	// SYNTHETIC: WEBSERVICE 0x100cc0d0
	// SYNTHETIC: ROBLOX 0x00610ca0
	// RBX::Mouse::~Mouse

	// SYNTHETIC: WEBSERVICE 0x100cc120
	// SYNTHETIC: ROBLOX 0x00610cf0
	// RBX::Mouse::`scalar deleting destructor'

	void update(const UIEvent& uiEvent);
	G3D::CoordinateFrame getHit() const;
	G3D::CoordinateFrame getOrigin() const;
	PartInstance* getTarget() const;
	void setCommand(MouseCommand* value) { command = value; }

	// FUNCTION: WEBSERVICE 0x10116fb0
	// FUNCTION: ROBLOX 0x00640580
	TextureId getIcon() const
	{
		checkActive();
		return icon;
	}

	// FUNCTION: WEBSERVICE 0x10117520
	// FUNCTION: ROBLOX 0x00656d00
	void setIcon(const TextureId& value) { icon = value; }

	// FUNCTION: WEBSERVICE 0x10117390
	// FUNCTION: ROBLOX 0x00656b20
	int getX() const
	{
		checkActive();
		return lastEvent.mousePosition.x;
	}

	// FUNCTION: WEBSERVICE 0x101173b0
	// FUNCTION: ROBLOX 0x00656b40
	int getY() const
	{
		checkActive();
		return lastEvent.mousePosition.y;
	}

	// FUNCTION: WEBSERVICE 0x101173d0
	// FUNCTION: ROBLOX 0x00656b60
	int getViewSizeX() const
	{
		checkActive();
		return lastEvent.windowSize.x;
	}

	// FUNCTION: WEBSERVICE 0x101173f0
	// FUNCTION: ROBLOX 0x00656b80
	int getViewSizeY() const
	{
		checkActive();
		return lastEvent.windowSize.y;
	}
};

DECOMP_SIZE_ASSERT(Mouse, 0x130)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021e420
// RBX::`dynamic initializer for 'desc_Mouse_Move''
// SYNTHETIC: WEBSERVICE 0x1021e450
// RBX::`dynamic initializer for 'desc_Mouse_Idle''
// SYNTHETIC: WEBSERVICE 0x1021e480
// RBX::`dynamic initializer for 'desc_Mouse_Button1Down''
// SYNTHETIC: WEBSERVICE 0x1021e4b0
// RBX::`dynamic initializer for 'desc_Mouse_Button2Down''
// SYNTHETIC: WEBSERVICE 0x1021e4e0
// RBX::`dynamic initializer for 'desc_Mouse_Button1Up''
// SYNTHETIC: WEBSERVICE 0x1021e510
// RBX::`dynamic initializer for 'desc_Mouse_Button2Up''
// SYNTHETIC: WEBSERVICE 0x1021e540
// RBX::`dynamic initializer for 'desc_Mouse_WheelForward''
// SYNTHETIC: WEBSERVICE 0x1021e570
// RBX::`dynamic initializer for 'desc_Mouse_WheelBackward''
// SYNTHETIC: WEBSERVICE 0x1021e5a0
// RBX::`dynamic initializer for 'desc_Mouse_KeyDown''
// SYNTHETIC: WEBSERVICE 0x1021e5c0
// RBX::`dynamic initializer for 'desc_Hit''
// SYNTHETIC: WEBSERVICE 0x1021e5f0
// RBX::`dynamic initializer for 'desc_Origin''
// SYNTHETIC: WEBSERVICE 0x1021e620
// RBX::`dynamic initializer for 'desc_Target''
// SYNTHETIC: WEBSERVICE 0x1021e650
// RBX::`dynamic initializer for 'desc_Mouse_X''
// SYNTHETIC: WEBSERVICE 0x1021e680
// RBX::`dynamic initializer for 'desc_Mouse_Y''
// SYNTHETIC: WEBSERVICE 0x1021e6b0
// RBX::`dynamic initializer for 'desc_Mouse_ViewSizeX''
// SYNTHETIC: WEBSERVICE 0x1021e6e0
// RBX::`dynamic initializer for 'desc_Mouse_ViewSizeY''
// SYNTHETIC: WEBSERVICE 0x1021e710
// RBX::`dynamic initializer for 'desc_Mouse_Icon''
// SYNTHETIC: WEBSERVICE 0x10225150
// RBX::`dynamic atexit destructor for 'desc_Target''
// SYNTHETIC: WEBSERVICE 0x10225170
// RBX::`dynamic atexit destructor for 'desc_Mouse_ViewSizeX''
// SYNTHETIC: WEBSERVICE 0x10225190
// RBX::`dynamic atexit destructor for 'desc_Mouse_ViewSizeY''
// SYNTHETIC: WEBSERVICE 0x102251b0
// RBX::`dynamic atexit destructor for 'desc_Origin''
// SYNTHETIC: WEBSERVICE 0x102251d0
// RBX::`dynamic atexit destructor for 'desc_Hit''
// SYNTHETIC: WEBSERVICE 0x102251f0
// RBX::`dynamic atexit destructor for 'desc_Mouse_Y''
// SYNTHETIC: WEBSERVICE 0x10225210
// RBX::`dynamic atexit destructor for 'desc_Mouse_X''
// SYNTHETIC: WEBSERVICE 0x10225230
// RBX::`dynamic atexit destructor for 'desc_Mouse_Icon''
// SYNTHETIC: WEBSERVICE 0x10225250
// RBX::`dynamic atexit destructor for 'desc_Mouse_Idle''
// SYNTHETIC: WEBSERVICE 0x10225260
// RBX::`dynamic atexit destructor for 'desc_Mouse_Button1Down''
// SYNTHETIC: WEBSERVICE 0x10225270
// RBX::`dynamic atexit destructor for 'desc_Mouse_Button2Down''
// SYNTHETIC: WEBSERVICE 0x10225280
// RBX::`dynamic atexit destructor for 'desc_Mouse_Button1Up''
// SYNTHETIC: WEBSERVICE 0x10225290
// RBX::`dynamic atexit destructor for 'desc_Mouse_Button2Up''
// SYNTHETIC: WEBSERVICE 0x102252a0
// RBX::`dynamic atexit destructor for 'desc_Mouse_WheelForward''
// SYNTHETIC: WEBSERVICE 0x102252b0
// RBX::`dynamic atexit destructor for 'desc_Mouse_WheelBackward''
// SYNTHETIC: WEBSERVICE 0x102252c0
// RBX::`dynamic atexit destructor for 'desc_Mouse_KeyDown''
// SYNTHETIC: WEBSERVICE 0x102252d0
// RBX::`dynamic atexit destructor for 'desc_Mouse_Move''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_MOUSE_H
