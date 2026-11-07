#include "tool/ToolsSurface.h"

#include "util/Sound.h"
#include "util/SoundWorld.h"
#include "v8datamodel/Explosion.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Workspace.h"
#include "v8tree/Service.h"

namespace RBX {

// GLOBAL: ROBLOX 0x00948c90
char sHammerTool[] = "Hammer";

// FUNCTION: ROBLOX 0x0063b8d0
const std::string HammerTool::getCursorName() const
{
	return part ? "HammerOverCursor" : "HammerCursor";
}

// FUNCTION: ROBLOX 0x0063b900
void HammerTool::render3dAdorn(Adorn* adorn)
{
	if (part) {
		part->render3dSelect(adorn, SELECT_NORMAL);
	}
}

// FUNCTION: ROBLOX 0x0063b920
void HammerTool::onMouseIdle(const UIEvent& uiEvent)
{
	part = shared_from(getUnlockedPartByLocalCharacter(uiEvent));
}

// FUNCTION: ROBLOX 0x0063b9c0
MouseCommand* HammerTool::onMouseDown(const UIEvent& uiEvent)
{
	if (part) {
		boost::shared_ptr<Explosion> explosion = Creatable<Instance>::create<Explosion>();
		Explosion::propPosition.setValue(explosion.get(), part->getCoordinateFrame().translation);
		Explosion::propBlastPressure.setValue(explosion.get(), 0.0f);
		explosion->setParent(workspace);
		part->setParent(NULL);
		ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(BOMB_SOUND);
	}
	return NULL;
}

} // namespace RBX
