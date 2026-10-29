#ifndef UTIL_USERINPUTBASE_H
#define UTIL_USERINPUTBASE_H

#include "decomp.h"
#include "util/ContentProvider.h"
#include "util/NavKeys.h"

#include <G3D/ReferenceCount.h>
#include <G3D/Vector2.h>
#include <SDL_keysym.h>

namespace RBX {

class Adorn;
class TextureId;
class TextureProxyBase;

// SIZE 0x28
class __declspec(novtable) UserInputBase
{
public:
	enum WrapMode
	{
		WRAP_AUTO = 0,
		WRAP_CENTER = 1,
	};

protected:
	virtual G3D::Vector2 getCursorPosition() = 0; // vtable+0x00

	virtual G3D::ReferenceCountedPointer<TextureProxyBase> getGameCursor(Adorn* adorn); // vtable+0x04

public:
	virtual void setWrapMode(WrapMode wrapMode) = 0;                   // vtable+0x08
	virtual WrapMode getWrapMode() const = 0;                          // vtable+0x0c
	virtual void centerCursor() = 0;                                   // vtable+0x10
	virtual bool keyDown(SDLKey key) const = 0;                        // vtable+0x14
	virtual void setKeyState(SDLKey key, bool down) = 0;               // vtable+0x18
	virtual void setCursorId(Adorn* adorn, const TextureId& cursorId); // vtable+0x1c

	void getNavKeys(NavKeys& navKeys) const;
	void renderGameCursor(Adorn*);
	bool altKeyDown() const { return keyDown(SDLK_RALT) || keyDown(SDLK_LALT); }
	bool shiftKeyDown() const { return keyDown(SDLK_RSHIFT) || keyDown(SDLK_LSHIFT); }

	// FUNCTION: WEBSERVICE 0x100bf0b0
	bool ctrlKeyDown() const { return keyDown(SDLK_RCTRL) || keyDown(SDLK_LCTRL); }

private:
	ContentId currentCursorId;                                    // 0x04
	G3D::ReferenceCountedPointer<TextureProxyBase> currentCursor; // 0x24
};

DECOMP_SIZE_ASSERT(UserInputBase, 0x28)

} // namespace RBX

#endif // UTIL_USERINPUTBASE_H
