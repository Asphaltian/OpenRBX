#include "util/UserInputBase.h"

#include "rbxgraphics/TextureProxyBase.h"
#include "util/ContentProvider.h"
#include "util/TextureId.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x100d0ec0
// FUNCTION: ROBLOX 0x005d8b60
void UserInputBase::getNavKeys(NavKeys& navKeys) const
{
	navKeys.forward_arrow = keyDown(SDLK_UP);
	navKeys.backward_arrow = keyDown(SDLK_DOWN);
	navKeys.left_arrow = keyDown(SDLK_LEFT);
	navKeys.right_arrow = keyDown(SDLK_RIGHT);
	navKeys.forward_asdw = keyDown(SDLK_w);
	navKeys.backward_asdw = keyDown(SDLK_s);
	navKeys.left_asdw = keyDown(SDLK_a);
	navKeys.right_asdw = keyDown(SDLK_d);
	navKeys.space = keyDown(SDLK_SPACE);
}

// FUNCTION: ROBLOX 0x005d8c00
void UserInputBase::init(Adorn* adorn)
{
	setCursorId(adorn, ContentId::fromAssets("Textures\\ArrowCursor.png"));
}

// STUB: ROBLOX 0x005d8ce0
UserInputBase::UserInputBase()
{
}

// FUNCTION: ROBLOX 0x005d8d40
G3D::ReferenceCountedPointer<TextureProxyBase> UserInputBase::getGameCursor(Adorn* adorn)
{
	return currentCursor;
}

// STUB: ROBLOX 0x005d8d70
void UserInputBase::setCursorId(Adorn* adorn, const TextureId& cursorId)
{
	STUB(0x5d8d70);
}

} // namespace RBX
