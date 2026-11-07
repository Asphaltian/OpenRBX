#include "tool/ToolsSurface.h"

#include "appdraw/DrawAdorn.h"
#include "humanoid/Humanoid.h"
#include "rbxgraphics/Adorn.h"
#include "util/Sound.h"
#include "util/SoundWorld.h"
#include "v8datamodel/Commands.h"
#include "v8datamodel/Explosion.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Surface.h"
#include "v8datamodel/Workspace.h"
#include "v8tree/Service.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/LineSegment.h>

namespace RBX {

// GLOBAL: ROBLOX 0x00948bec
char sFlatTool[] = "Flat";

// GLOBAL: ROBLOX 0x00948bf4
char sGlueTool[] = "Glue";

// GLOBAL: ROBLOX 0x00948bfc
char sWeldTool[] = "Weld";

// GLOBAL: ROBLOX 0x00948c04
char sStudsTool[] = "Studs";

// GLOBAL: ROBLOX 0x00948c0c
char sInletTool[] = "Inlet";

// GLOBAL: ROBLOX 0x00948c14
char sHingeTool[] = "Hinge";

// GLOBAL: ROBLOX 0x00948c1c
char sRightMotorTool[] = "RightMotor";

// GLOBAL: ROBLOX 0x00948c28
char sLeftMotorTool[] = "LeftMotor";

// GLOBAL: ROBLOX 0x00948c34
char sOscillateMotorTool[] = "OscillateMotor";

// GLOBAL: ROBLOX 0x00948c60
char sLockTool[] = "Lock";

// GLOBAL: ROBLOX 0x00948c90
char sHammerTool[] = "Hammer";

// GLOBAL: ROBLOX 0x00948c9c
char sLaserTool[] = "Laser";

// FUNCTION: ROBLOX 0x00639820
void FlatTool::VTable0x40(Surface* surface)
{
	surface->getPartInstance()->destroyJoints();
	surface->flat();
	surface->getPartInstance()->join();
}

// FUNCTION: ROBLOX 0x00639840
void HingeTool::VTable0x40(Surface* surface)
{
	surface->getPartInstance()->destroyJoints();
	surface->setSurfaceType(ROTATE);
	surface->setSurfaceInput(Controller::NO_INPUT);
	surface->setParamA(0.0f);
	surface->setParamB(0.0f);
	surface->getPartInstance()->join();
}

// FUNCTION: ROBLOX 0x00639890
void RightMotorTool::VTable0x40(Surface* surface)
{
	surface->getPartInstance()->destroyJoints();
	surface->setSurfaceType(ROTATE_V);
	surface->setSurfaceInput(Controller::RIGHT_TRACK_INPUT);
	surface->setParamA(-0.1f);
	surface->setParamB(0.1f);
	surface->getPartInstance()->join();
}

// FUNCTION: ROBLOX 0x006398e0
void LeftMotorTool::VTable0x40(Surface* surface)
{
	surface->getPartInstance()->destroyJoints();
	surface->setSurfaceType(ROTATE_V);
	surface->setSurfaceInput(Controller::LEFT_TRACK_INPUT);
	surface->setParamA(-0.1f);
	surface->setParamB(0.1f);
	surface->getPartInstance()->join();
}

// FUNCTION: ROBLOX 0x00639930
void OscillateMotorTool::VTable0x40(Surface* surface)
{
	surface->getPartInstance()->destroyJoints();
	surface->setSurfaceType(ROTATE_V);
	surface->setSurfaceInput(Controller::SIN_INPUT);
	surface->setParamA(-0.1f);
	surface->setParamB(0.1f);
	surface->getPartInstance()->join();
}

// FUNCTION: ROBLOX 0x00639980
void SurfaceTool::render3dAdorn(Adorn* adorn)
{
	if (part) {
		DrawAdorn::partSurface(part->getPart(), surfaceId, adorn, G3D::Color4(G3D::Color3::orange(), 1.0f));
	}
}

// FUNCTION: ROBLOX 0x00639a80
SurfaceTool::SurfaceTool(Workspace* workspace) : MouseCommand(workspace), surface(NULL)
{
}

// FUNCTION: ROBLOX 0x00639af0
void SurfaceTool::onMouseHover(const UIEvent& uiEvent)
{
	PartInstance* hitPart;
	surface = getSurface(uiEvent, hitPart, surfaceId);
	part = shared_from(surface ? hitPart : NULL);
}

// FUNCTION: ROBLOX 0x00639ba0
MouseCommand* SurfaceTool::onMouseDown(const UIEvent& uiEvent)
{
	onMouseHover(uiEvent);
	if (surface) {
		undoState.reset(new UndoState());
		undoState->addAllProperties(*part);
		VTable0x40(surface);
		ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(PING_SOUND);

		std::auto_ptr<RedoState> redoState(new RedoState());
		redoState->addAllProperties(*part);
		workspace->getDataState().pushState(m_unk0x30, undoState.release(), redoState.release());
		workspace->getDataState().setDirty(true);
	}
	return NULL;
}

// FUNCTION: ROBLOX 0x00639d20
void GlueTool::VTable0x40(Surface* surface)
{
	surface->getPartInstance()->destroyJoints();
	surface->flat();
	surface->setSurfaceType(GLUE);
	surface->getPartInstance()->join();
	ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(PING_SOUND);
}

// FUNCTION: ROBLOX 0x00639d60
void WeldTool::VTable0x40(Surface* surface)
{
	surface->getPartInstance()->destroyJoints();
	surface->flat();
	surface->setSurfaceType(WELD);
	surface->getPartInstance()->join();
	ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(PING_SOUND);
}

// FUNCTION: ROBLOX 0x00639da0
void StudsTool::VTable0x40(Surface* surface)
{
	surface->getPartInstance()->destroyJoints();
	surface->flat();
	surface->setSurfaceType(STUDS);
	surface->getPartInstance()->join();
	ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(PING_SOUND);
}

// FUNCTION: ROBLOX 0x00639de0
void InletTool::VTable0x40(Surface* surface)
{
	surface->getPartInstance()->destroyJoints();
	surface->flat();
	surface->setSurfaceType(INLET);
	surface->getPartInstance()->join();
	ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(PING_SOUND);
}

// FUNCTION: ROBLOX 0x00639e70
void ModelTool::render3dAdorn(Adorn* adorn)
{
	if (m_unk0x20) {
		if (IRenderable* renderable = dynamic_cast<IRenderable*>(m_unk0x20.get())) {
			renderable->render3dSelect(adorn, SELECT_LIMIT);
		}
	}
}

// STUB: ROBLOX 0x00639f00
const std::string LockTool::getCursorName() const
{
	if (m_unk0x20 && PartInstance::getLocked(m_unk0x20.get())) {
		return "UnlockCursor";
	}
	return "LockCursor";
}

// FUNCTION: ROBLOX 0x00639f50
ModelTool::ModelTool(Workspace* workspace) : MouseCommand(workspace)
{
}

// FUNCTION: ROBLOX 0x0063a200
void ModelTool::onMouseHover(const UIEvent& uiEvent)
{
	PartInstance* part = getPart(uiEvent);
	if (part) {
		m_unk0x20 = shared_from(Workspace::findTopInstance(part));
	}
}

// FUNCTION: ROBLOX 0x0063a3d0
MouseCommand* LockTool::onMouseDown(const UIEvent& uiEvent)
{
	if (m_unk0x20) {
		bool locked = PartInstance::getLocked(m_unk0x20.get());
		PartInstance::setLocked(m_unk0x20.get(), !locked);
		ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(PING_SOUND);
	}
	return this;
}

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
