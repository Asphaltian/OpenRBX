#ifndef UTIL_USERINPUTBASE_H
#define UTIL_USERINPUTBASE_H

#include "decomp.h"
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
	virtual WrapMode getWrapMode() = 0;                                // vtable+0x0c
	virtual void centerCursor() = 0;                                   // vtable+0x10
	virtual bool keyDown(SDLKey key) = 0;                              // vtable+0x14
	virtual void setKeyState(SDLKey key, bool down) = 0;               // vtable+0x18
	virtual void setCursorId(Adorn* adorn, const TextureId& cursorId); // vtable+0x1c

	void getNavKeys(NavKeys& navKeys);

private:
	undefined m_unk0x04[0x28 - 0x04]; // 0x04
};

DECOMP_SIZE_ASSERT(UserInputBase, 0x28)

} // namespace RBX

#endif // UTIL_USERINPUTBASE_H
