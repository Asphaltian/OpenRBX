#include "tool/ToolsSurface.h"

#include "humanoid/Humanoid.h"
#include "rbxgraphics/Adorn.h"
#include "util/Sound.h"
#include "util/SoundWorld.h"
#include "v8datamodel/Explosion.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Workspace.h"
#include "v8tree/Service.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/LineSegment.h>

namespace RBX {

// GLOBAL: ROBLOX 0x00948c90
char sHammerTool[] = "Hammer";

// GLOBAL: ROBLOX 0x00948c9c
char sLaserTool[] = "Laser";

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

// FUNCTION: ROBLOX 0x0063bad0
void LaserTool::render3dAdorn(Adorn* adorn)
{
	if (m_unk0x20 > 0) {
		m_unk0x20--;
		adorn->setObjectToWorldMatrix(G3D::CoordinateFrame());
		adorn->lineSegment(G3D::LineSegment::fromTwoPoints(m_unk0x24, m_unk0x30), G3D::Color3::red(), 5.0f);
	}
}

// FUNCTION: ROBLOX 0x0063bb80
MouseCommand* LaserTool::onMouseDown(const UIEvent& uiEvent)
{
	PartInstance* head = Humanoid::getLocalHeadFromContext(workspace);
	if (head) {
		m_unk0x24 = head->getCoordinateFrame().translation;
		G3D::Vector3 hitWorld;
		PartInstance* part = getPart(uiEvent, NULL, hitWorld);
		if (part) {
			boost::shared_ptr<Explosion> explosion = Creatable<Instance>::create<Explosion>();
			Explosion::propPosition.setValue(explosion.get(), hitWorld);
			Explosion::propBlastPressure.setValue(explosion.get(), 0.0f);
			explosion->setParent(workspace);
			if (!part->getPartLocked()) {
				part->remove();
			}
			ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(SNAP_SOUND);
		}
		m_unk0x20 = 4;
	}
	return this;
}

} // namespace RBX
