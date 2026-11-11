#include "tool/ToolsSurface.h"

#include "appdraw/DrawAdorn.h"
#include "humanoid/Humanoid.h"
#include "rbxgraphics/Adorn.h"
#include "tool/DragTool.h"
#include "tool/GrabTool.h"
#include "tool/PartDragTool.h"
#include "util/ContentProvider.h"
#include "util/Math.h"
#include "util/Sound.h"
#include "util/SoundWorld.h"
#include "util/Units.h"
#include "v8datamodel/Commands.h"
#include "v8datamodel/Explosion.h"
#include "v8datamodel/ModelInstance.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Surface.h"
#include "v8datamodel/Workspace.h"
#include "v8kernel/Constants.h"
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

// GLOBAL: ROBLOX 0x00948c44
char sModelSetFrontTool[] = "ModelSetFront";

// GLOBAL: ROBLOX 0x00948c58
char sAnchorTool[] = "Anchor";

// GLOBAL: ROBLOX 0x00948c60
char sLockTool[] = "Lock";

// GLOBAL: ROBLOX 0x00948c6c
char sSlingshotTool[] = "Slingshot";

// GLOBAL: ROBLOX 0x00948c78
char sRocketTool[] = "Rocket";

// GLOBAL: ROBLOX 0x00948c84
char sResizeTool[] = "Resize";

// GLOBAL: ROBLOX 0x00948c90
char sHammerTool[] = "Hammer";

// GLOBAL: ROBLOX 0x00948c9c
char sLaserTool[] = "Laser";

// GLOBAL: ROBLOX 0x00948ca8
char sGrabTool[] = "Grab";

// GLOBAL: ROBLOX 0x00948cf8
char sCloneTool[] = "Clone";

// GLOBAL: ROBLOX 0x00948d48
char sGameTool[] = "GameTool";

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

// FUNCTION: ROBLOX 0x006399e0
void ModelSetFrontTool::VTable0x40(Surface* surface)
{
	PartInstance* part = surface->getPartInstance();
	if (ModelInstance* model = dynamic_cast<ModelInstance*>(part->getTopLevelPVParent())) {
		model->FUN_00565040(Math::getWorldNormal(surface->getNormalId(), part->getCoordinateFrame()));
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

void FUN_00639e20(Instance* instance, const bool& value);

struct Unknown00639e20
{
	bool value;

	Unknown00639e20(bool value) : value(value) {}

	void operator()(Instance* instance) const { FUN_00639e20(instance, value); }
};

// FUNCTION: ROBLOX 0x00639e20
void FUN_00639e20(Instance* instance, const bool& value)
{
	bool anchored = value;
	if (PartInstance* part = dynamic_cast<PartInstance*>(instance)) {
		part->setAnchored(anchored);
	}
	else {
		instance->for_eachChild(Unknown00639e20(anchored));
	}
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

// FUNCTION: ROBLOX 0x00639eb0
const std::string AnchorTool::getCursorName() const
{
	if (m_unk0x20) {
		if (m_unk0x28) {
			return "UnAnchorCursor";
		}
		else {
			return "AnchorCursor";
		}
	}
	return "AnchorCursor";
}

// FUNCTION: ROBLOX 0x00639f00
const std::string LockTool::getCursorName() const
{
	if (m_unk0x20) {
		if (PartInstance::getLocked(m_unk0x20.get())) {
			return "UnlockCursor";
		}
		else {
			return "LockCursor";
		}
	}
	return "LockCursor";
}

// FUNCTION: ROBLOX 0x00639f50
ModelTool::ModelTool(Workspace* workspace) : MouseCommand(workspace)
{
}

// FUNCTION: ROBLOX 0x0063a090
static Instance* FUN_0063a090(Instance* instance)
{
	PartInstance* part = dynamic_cast<PartInstance*>(instance);
	if (part && !part->getAnchored()) {
		return part;
	}

	boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > children = instance->getChildren().read();
	if (children) {
		std::vector<boost::shared_ptr<Instance> >::const_iterator end = children->end();
		for (std::vector<boost::shared_ptr<Instance> >::const_iterator iter = children->begin(); iter != end; ++iter) {
			if (FUN_0063a090(iter->get())) {
				return iter->get();
			}
		}
	}
	return NULL;
}

// FUNCTION: ROBLOX 0x0063a200
void ModelTool::onMouseHover(const UIEvent& uiEvent)
{
	PartInstance* part = getPart(uiEvent);
	if (part) {
		m_unk0x20 = shared_from(Workspace::findTopInstance(part));
	}
}

// FUNCTION: ROBLOX 0x0063a2b0
void AnchorTool::onMouseHover(const UIEvent& uiEvent)
{
	boost::shared_ptr<Instance> oldInstance = m_unk0x20;
	ModelTool::onMouseHover(uiEvent);
	if (oldInstance != m_unk0x20) {
		bool anchored;
		if (m_unk0x20) {
			anchored = FUN_0063a090(m_unk0x20.get()) == NULL;
		}
		else {
			anchored = false;
		}
		m_unk0x28 = anchored;
	}
}

// FUNCTION: ROBLOX 0x0063a370
MouseCommand* AnchorTool::onMouseDown(const UIEvent& uiEvent)
{
	if (m_unk0x20 && !PartInstance::getLocked(m_unk0x20.get())) {
		bool anchored = FUN_0063a090(m_unk0x20.get()) != NULL;
		FUN_00639e20(m_unk0x20.get(), anchored);
		ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(PING_SOUND);
	}
	return this;
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

// FUNCTION: ROBLOX 0x0063a410
const std::string ShootTool::getCursorName() const
{
	if (m_unk0x30 && m_unk0x2c > 10) {
		return "GunWaitCursor";
	}
	return "GunCursor";
}

// FUNCTION: ROBLOX 0x0063a460
void ShootTool::onMouseIdle(const UIEvent& uiEvent)
{
	if (m_unk0x30 > 0) {
		m_unk0x30--;
	}
	if (captured()) {
		onMouseMove(uiEvent);
	}
}

// STUB: ROBLOX 0x0063a4b0
PV SlingshotTool::VTable0x44(const UIEvent& uiEvent, const CoordinateFrame& cframe)
{
	G3D::Vector3 hitWorld;
	PartInstance* part = getPart(uiEvent, NULL, hitWorld);
	G3D::Vector3 direction = part ? (hitWorld - cframe.translation).direction() : cframe.lookVector();
	G3D::Vector3 position = cframe.translation + direction * 3.0f;
	if (part) {
		direction = hitWorld - position;
		float height = direction.y;
		direction.y = 0.0f;
		float distance = direction.unitize();
		float angle = Math::computeLaunchAngle(
			85.0f, distance, height, Units::kmsAccelerationToRbx(Constants::getKmsGravity()).y
		);
		direction.y = sin(angle);
		direction.x *= cos(angle);
		direction.z *= cos(angle);
	}

	CoordinateFrame frame(position);
	return PV(frame, Velocity(direction * 85.0f, G3D::Vector3::zero()));
}

// FUNCTION: ROBLOX 0x0063a740
PV RocketTool::VTable0x44(const UIEvent& uiEvent, const CoordinateFrame& cframe)
{
	G3D::Vector3 hitWorld;
	PartInstance* part = getPart(uiEvent, NULL, hitWorld);
	G3D::Vector3 direction = part ? (hitWorld - cframe.translation).direction() : cframe.lookVector();
	CoordinateFrame frame;
	frame.lookAt(direction);
	frame.translation = cframe.translation + 5.0f * direction;
	return PV(frame, Velocity::zero());
}

// FUNCTION: ROBLOX 0x0063a8d0
ShootTool::ShootTool(Workspace* workspace, bool p_unk0x28, int p_unk0x2c)
	: MouseCommand(workspace), m_unk0x28(p_unk0x28), m_unk0x2c(p_unk0x2c), m_unk0x30(0)
{
}

// FUNCTION: ROBLOX 0x0063a910
boost::shared_ptr<PartInstance> SlingshotTool::VTable0x40()
{
	std::vector<boost::shared_ptr<Instance> > instances;
	ContentProvider::singleton().load(ContentId::fromAssets("Fonts\\SlingshotPellet.rbxm"), instances);
	return boost::shared_polymorphic_downcast<PartInstance>(instances.front());
}

// FUNCTION: ROBLOX 0x0063aa10
boost::shared_ptr<PartInstance> RocketTool::VTable0x40()
{
	std::vector<boost::shared_ptr<Instance> > instances;
	ContentProvider::singleton().load(ContentId::fromAssets("Fonts\\Rocket.rbxm"), instances);
	return boost::shared_polymorphic_downcast<PartInstance>(instances.front());
}

// FUNCTION: ROBLOX 0x0063ab10
void ShootTool::FUN_0063ab10(const UIEvent& uiEvent)
{
	m_unk0x30 = m_unk0x2c;
	PartInstance* head = Humanoid::getLocalHeadFromContext(workspace);
	if (head) {
		if (!m_unk0x20) {
			m_unk0x20 = VTable0x40();
		}

		boost::shared_ptr<PartInstance> projectile =
			boost::shared_polymorphic_downcast<PartInstance>(m_unk0x20->clone());
		projectile->setParent(workspace);
		projectile->getPrimitive()->setPV(VTable0x44(uiEvent, head->getCoordinateFrame()));
		ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(VTable0x48());
	}
}

// FUNCTION: ROBLOX 0x0063acb0
MouseCommand* ShootTool::onMouseDown(const UIEvent& uiEvent)
{
	capture();
	if (m_unk0x30 == 0) {
		FUN_0063ab10(uiEvent);
	}
	return this;
}

// FUNCTION: ROBLOX 0x0063ace0
void ShootTool::onMouseMove(const UIEvent& uiEvent)
{
	if (m_unk0x28 && m_unk0x30 == 0) {
		FUN_0063ab10(uiEvent);
	}
}

// FUNCTION: ROBLOX 0x0063ad00
const std::string ResizeTool::getCursorName() const
{
	return m_unk0x2c ? std::string("ResizeCursor") : ArrowToolBase::getCursorName();
}

// FUNCTION: ROBLOX 0x0063adc0
void ResizeTool::VTable0x40(XmlState* state)
{
	boost::shared_ptr<PVInstance> locked = m_unk0x24.lock();
	if (PartInstance* part = dynamic_cast<PartInstance*>(locked.get())) {
		part->FUN_005ad310(state);
	}
}

// STUB: ROBLOX 0x0063ae70
MouseCommand* ResizeTool::onMouseUp(const UIEvent& uiEvent)
{
	STUB(0x63ae70);
	return NULL;
}

// STUB: ROBLOX 0x0063af80
void ResizeTool::FUN_0063af80()
{
	STUB(0x63af80);
}

// STUB: ROBLOX 0x0063b020
void ResizeTool::onMouseMove(const UIEvent& uiEvent)
{
	STUB(0x63b020);
}

// STUB: ROBLOX 0x0063b240
void ResizeTool::FUN_0063b240(const UIEvent& uiEvent)
{
	STUB(0x63b240);
}

// STUB: ROBLOX 0x0063b4b0
void ResizeTool::render3dAdorn(Adorn* adorn)
{
	STUB(0x63b4b0);
}

// STUB: ROBLOX 0x0063b620
void ResizeTool::render2d(Adorn* adorn)
{
	STUB(0x63b620);
}

// STUB: ROBLOX 0x0063b7c0
void ResizeTool::onMouseHover(const UIEvent& uiEvent)
{
	FUN_0063b240(uiEvent);
	m_unk0x2c = !m_unk0x24.expired();
	ArrowToolBase::onMouseHover(uiEvent);
}

// STUB: ROBLOX 0x0063b800
MouseCommand* ResizeTool::onMouseDown(const UIEvent& uiEvent)
{
	STUB(0x63b800);
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

// FUNCTION: ROBLOX 0x0063bce0
void GrabTool::onMouseHover(const UIEvent& uiEvent)
{
	PartInstance* part = getUnlockedPartByLocalCharacter(uiEvent);
	if (!part) {
		cursor = "ArrowFarCursor";
	}
	else if (part->getPartLocked()) {
		cursor = "ArrowCursor";
	}
	else {
		cursor = "DragCursor";
	}
}

// FUNCTION: ROBLOX 0x0063bd40
GrabTool::~GrabTool()
{
}

// FUNCTION: ROBLOX 0x0063bda0
GrabTool::GrabTool(Workspace* workspace) : Named<MouseCommand, sGrabTool>(workspace), cursor("ArrowCursor")
{
}

// FUNCTION: ROBLOX 0x0063bea0
MouseCommand* GrabTool::onMouseDown(const UIEvent& uiEvent)
{
	G3D::Vector3 hitWorld;
	PartInstance* part = getUnlockedPartByLocalCharacter(uiEvent, hitWorld);
	if (part) {
		std::vector<Instance*> dragInstances;
		dragInstances.push_back(getTopSelectable3d(part));
		return DragTool::onMouseDown(part, hitWorld, dragInstances, uiEvent, workspace);
	}
	return NULL;
}

// FUNCTION: ROBLOX 0x0063bf80
const std::string CloneTool::getCursorName() const
{
	if (part) {
		return "CloneOverCursor";
	}
	return "CloneCursor";
}

// FUNCTION: ROBLOX 0x0063bfc0
CloneTool::CloneTool(Workspace* workspace) : Named<MouseCommand, sCloneTool>(workspace)
{
}

// FUNCTION: ROBLOX 0x0063c0a0
MouseCommand* CloneTool::onMouseDown(const UIEvent& uiEvent)
{
	if (part) {
		const G3D::CoordinateFrame& cframe = part->getCoordinateFrame();
		boost::shared_ptr<PartInstance> clone = boost::shared_polymorphic_downcast<PartInstance>(part->clone());
		std::vector<boost::shared_ptr<Instance> > instances;
		instances.push_back(clone);
		workspace->insertInstances(instances, workspace, INSERT_TO_TREE, SUPPRESS_PROMPTS);
		ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(PING_SOUND);

		MouseCommand* dragger = new PartDragTool(clone.get(), clone->getCoordinateFrame().translation, workspace);
		return dragger->onMouseDown(uiEvent);
	}
	return NULL;
}

// FUNCTION: ROBLOX 0x0063b920 FOLDED
void CloneTool::onMouseIdle(const UIEvent& uiEvent)
{
	part = shared_from(getUnlockedPartByLocalCharacter(uiEvent));
}

// FUNCTION: ROBLOX 0x0063c2c0
bool GameTool::FUN_0063c2c0(PartInstance* part, const G3D::Vector3& hitWorld) const
{
	return !part->getPartLocked() && !part->lockedInPlace() && characterCanReach(hitWorld);
}

// FUNCTION: ROBLOX 0x0063c300
void GameTool::onMouseHover(const UIEvent& uiEvent)
{
	if (uiEvent.windowSize.x != 0 && uiEvent.windowSize.y != 0) {
		G3D::Vector3 hitWorld;
		PartInstance* part = getUnlockedPartByLocalCharacter(uiEvent, hitWorld);
		if (!part) {
			cursor = "ArrowFarCursor";
		}
		else if (!FUN_0063c2c0(part, hitWorld)) {
			cursor = "ArrowCursor";
		}
		else {
			cursor = "DragCursor";
		}
	}
}

// FUNCTION: ROBLOX 0x0063c390
MouseCommand* GameTool::onMouseDown(const UIEvent& uiEvent)
{
	G3D::Vector3 hitWorld;
	PartInstance* part = getUnlockedPartByLocalCharacter(uiEvent, hitWorld);
	if (part && FUN_0063c2c0(part, hitWorld)) {
		MouseCommand* dragger = new PartDragTool(part, hitWorld, workspace);
		return dragger->onMouseDown(uiEvent);
	}
	return NULL;
}

// FUNCTION: ROBLOX 0x0063c460
GameTool::~GameTool()
{
}

// FUNCTION: ROBLOX 0x0063c4c0
GameTool::GameTool(Workspace* workspace) : Named<MouseCommand, sGameTool>(workspace)
{
}

} // namespace RBX
