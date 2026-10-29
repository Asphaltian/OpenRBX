#ifndef UTIL_UIEVENT_H
#define UTIL_UIEVENT_H

#include "decomp.h"

#include <G3D/Vector2int16.h>
#include <SDL_keysym.h>

namespace RBX {

class UserInputBase;

// SIZE 0x14
class UIEvent
{
public:
	enum EventType
	{
		NO_EVENT = 0,
		MOUSE_RIGHT_BUTTON_DOWN = 1,
		MOUSE_RIGHT_BUTTON_UP = 2,
		MOUSE_LEFT_BUTTON_DOWN = 3,
		MOUSE_LEFT_BUTTON_UP = 4,
		MOUSE_MOVE = 5,
		MOUSE_DELTA = 6,
		MOUSE_IDLE = 7,
		MOUSE_WHEEL_FORWARD = 8,
		MOUSE_WHEEL_BACKWARD = 9,
		KEY_DOWN = 10,
		KEY_UP = 11
	};

	EventType eventType;      // 0x00
	UserInputBase* userInput; // 0x04
#ifdef _MSC_VER
	union {
		struct
		{
			G3D::Vector2int16 mousePosition; // 0x08
			G3D::Vector2int16 mouseDelta;    // 0x0c
			G3D::Vector2int16 windowSize;    // 0x10
		};
		struct
		{
			SDLKey key; // 0x08
			SDLMod mod; // 0x0c
		};
	};
#else
	G3D::Vector2int16 mousePosition;
	G3D::Vector2int16 mouseDelta;
	G3D::Vector2int16 windowSize;
	SDLKey key;
	SDLMod mod;
#endif

	UIEvent(UserInputBase* userInput, EventType eventType, SDLKey key, SDLMod mod);
	UIEvent(UserInputBase* userInput, EventType eventType, G3D::Vector2int16, G3D::Vector2int16);
	UIEvent() : eventType(NO_EVENT), userInput(NULL) {}

	// FUNCTION: WEBSERVICE 0x10069fa0
	bool isMouseEvent() const
	{
		return eventType == MOUSE_RIGHT_BUTTON_DOWN || eventType == MOUSE_RIGHT_BUTTON_UP ||
			   eventType == MOUSE_LEFT_BUTTON_DOWN || eventType == MOUSE_LEFT_BUTTON_UP || eventType == MOUSE_MOVE ||
			   eventType == MOUSE_DELTA || eventType == MOUSE_IDLE;
	}

	bool isKeyPressedEvent(SDLKey key) const;
	bool isKeyUpEvent(SDLKey key) const;
	bool isKeyPressedWithShiftEvent(SDLKey key) const;
	bool isKeyPressedWithShiftEvent() const;
};

DECOMP_SIZE_ASSERT(UIEvent, 0x14)

} // namespace RBX

#endif // UTIL_UIEVENT_H
