#ifndef UTIL_USERINPUTBASE_H
#define UTIL_USERINPUTBASE_H

#include "placeholder.h"
#include "util/ContentProvider.h"
#include "util/NavKeys.h"

#include <G3D/ReferenceCount.h>
#include <G3D/Vector2.h>
#include <SDL_keysym.h>

namespace RBX {

class Adorn;
class TextureId;
class TextureProxyBase;

class __declspec(novtable) UserInputBase
{
public:
	enum WrapMode
	{
		WRAP_AUTO = 0,
		WRAP_CENTER = 1,
	};

	UserInputBase();
	void init(Adorn*);

protected:
	virtual G3D::Vector2 getCursorPosition() = 0;

	virtual G3D::ReferenceCountedPointer<TextureProxyBase> getGameCursor(Adorn* adorn);

public:
	virtual void setWrapMode(WrapMode wrapMode) = 0;
	virtual WrapMode getWrapMode() const = 0;
	virtual void centerCursor() = 0;
	virtual bool keyDown(SDLKey key) const = 0;
	virtual void setKeyState(SDLKey key, bool down) = 0;
	virtual void setCursorId(Adorn* adorn, const TextureId& cursorId);

	void getNavKeys(NavKeys& navKeys) const;
	void renderGameCursor(Adorn*);
	bool altKeyDown() const { return keyDown(SDLK_RALT) || keyDown(SDLK_LALT); }
	bool shiftKeyDown() const { return keyDown(SDLK_RSHIFT) || keyDown(SDLK_LSHIFT); }

	bool ctrlKeyDown() const { return keyDown(SDLK_RCTRL) || keyDown(SDLK_LCTRL); }

private:
	ContentId currentCursorId;
	G3D::ReferenceCountedPointer<TextureProxyBase> currentCursor;
};

} // namespace RBX

#endif // UTIL_USERINPUTBASE_H
