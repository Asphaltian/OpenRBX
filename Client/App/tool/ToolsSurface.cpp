#include "tool/ToolsSurface.h"

#include "appdraw/DrawAdorn.h"
#include "humanoid/Humanoid.h"
#include "rbxgraphics/Adorn.h"
#include "tool/DragTool.h"
#include "tool/DragUtilities.h"
#include "tool/Dragger.h"
#include "tool/GrabTool.h"
#include "tool/MegaDragger.h"
#include "tool/PartDragTool.h"
#include "util/ContentProvider.h"
#include "util/Math.h"
#include "util/Sound.h"
#include "util/SoundWorld.h"
#include "util/Units.h"
#include "v8datamodel/Commands.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Explosion.h"
#include "v8datamodel/ModelInstance.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Surface.h"
#include "v8datamodel/Workspace.h"
#include "v8kernel/Constants.h"
#include "v8tree/Service.h"
#include "v8xml/SerializerV2.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/LineSegment.h>

namespace RBX {

char sFlatTool[] = "Flat";

char sGlueTool[] = "Glue";

char sWeldTool[] = "Weld";

char sStudsTool[] = "Studs";

char sInletTool[] = "Inlet";

char sHingeTool[] = "Hinge";

char sRightMotorTool[] = "RightMotor";

char sLeftMotorTool[] = "LeftMotor";

char sOscillateMotorTool[] = "OscillateMotor";

char sModelSetFrontTool[] = "ModelSetFront";

char sAnchorTool[] = "Anchor";

char sLockTool[] = "Lock";

char sSlingshotTool[] = "Slingshot";

char sRocketTool[] = "Rocket";

char sResizeTool[] = "Resize";

char sHammerTool[] = "Hammer";

char sLaserTool[] = "Laser";

char sGrabTool[] = "Grab";

char sCloneTool[] = "Clone";

char sGameTool[] = "GameTool";

char sAxisMoveTool[] = "AxisMove";

void FlatTool::VTable0x40(Surface* surface)
{
	surface->getPartInstance()->destroyJoints();
	surface->flat();
	surface->getPartInstance()->join();
}

void HingeTool::VTable0x40(Surface* surface)
{
	surface->getPartInstance()->destroyJoints();
	surface->setSurfaceType(ROTATE);
	surface->setSurfaceInput(Controller::NO_INPUT);
	surface->setParamA(0.0f);
	surface->setParamB(0.0f);
	surface->getPartInstance()->join();
}

void RightMotorTool::VTable0x40(Surface* surface)
{
	surface->getPartInstance()->destroyJoints();
	surface->setSurfaceType(ROTATE_V);
	surface->setSurfaceInput(Controller::RIGHT_TRACK_INPUT);
	surface->setParamA(-0.1f);
	surface->setParamB(0.1f);
	surface->getPartInstance()->join();
}

void LeftMotorTool::VTable0x40(Surface* surface)
{
	surface->getPartInstance()->destroyJoints();
	surface->setSurfaceType(ROTATE_V);
	surface->setSurfaceInput(Controller::LEFT_TRACK_INPUT);
	surface->setParamA(-0.1f);
	surface->setParamB(0.1f);
	surface->getPartInstance()->join();
}

void OscillateMotorTool::VTable0x40(Surface* surface)
{
	surface->getPartInstance()->destroyJoints();
	surface->setSurfaceType(ROTATE_V);
	surface->setSurfaceInput(Controller::SIN_INPUT);
	surface->setParamA(-0.1f);
	surface->setParamB(0.1f);
	surface->getPartInstance()->join();
}

void SurfaceTool::render3dAdorn(Adorn* adorn)
{
	if (part) {
		DrawAdorn::partSurface(part->getPart(), surfaceId, adorn, G3D::Color4(G3D::Color3::orange(), 1.0f));
	}
}

void ModelSetFrontTool::VTable0x40(Surface* surface)
{
	PartInstance* part = surface->getPartInstance();
	if (ModelInstance* model = dynamic_cast<ModelInstance*>(part->getTopLevelPVParent())) {
		model->FUN_00565040(Math::getWorldNormal(surface->getNormalId(), part->getCoordinateFrame()));
	}
}

SurfaceTool::SurfaceTool(Workspace* workspace) : MouseCommand(workspace), surface(NULL)
{
}

void SurfaceTool::onMouseHover(const UIEvent& uiEvent)
{
	PartInstance* hitPart;
	surface = getSurface(uiEvent, hitPart, surfaceId);
	part = shared_from(surface ? hitPart : NULL);
}

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

void GlueTool::VTable0x40(Surface* surface)
{
	surface->getPartInstance()->destroyJoints();
	surface->flat();
	surface->setSurfaceType(GLUE);
	surface->getPartInstance()->join();
	ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(PING_SOUND);
}

void WeldTool::VTable0x40(Surface* surface)
{
	surface->getPartInstance()->destroyJoints();
	surface->flat();
	surface->setSurfaceType(WELD);
	surface->getPartInstance()->join();
	ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(PING_SOUND);
}

void StudsTool::VTable0x40(Surface* surface)
{
	surface->getPartInstance()->destroyJoints();
	surface->flat();
	surface->setSurfaceType(STUDS);
	surface->getPartInstance()->join();
	ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(PING_SOUND);
}

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

void ModelTool::render3dAdorn(Adorn* adorn)
{
	if (m_unk0x20) {
		if (IRenderable* renderable = dynamic_cast<IRenderable*>(m_unk0x20.get())) {
			renderable->render3dSelect(adorn, SELECT_LIMIT);
		}
	}
}

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

ModelTool::ModelTool(Workspace* workspace) : MouseCommand(workspace)
{
}

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

void ModelTool::onMouseHover(const UIEvent& uiEvent)
{
	PartInstance* part = getPart(uiEvent);
	if (part) {
		m_unk0x20 = shared_from(Workspace::findTopInstance(part));
	}
}

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

MouseCommand* AnchorTool::onMouseDown(const UIEvent& uiEvent)
{
	if (m_unk0x20 && !PartInstance::getLocked(m_unk0x20.get())) {
		bool anchored = FUN_0063a090(m_unk0x20.get()) != NULL;
		FUN_00639e20(m_unk0x20.get(), anchored);
		ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(PING_SOUND);
	}
	return this;
}

MouseCommand* LockTool::onMouseDown(const UIEvent& uiEvent)
{
	if (m_unk0x20) {
		bool locked = PartInstance::getLocked(m_unk0x20.get());
		PartInstance::setLocked(m_unk0x20.get(), !locked);
		ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(PING_SOUND);
	}
	return this;
}

const std::string ShootTool::getCursorName() const
{
	if (m_unk0x30 && m_unk0x2c > 10) {
		return "GunWaitCursor";
	}
	return "GunCursor";
}

void ShootTool::onMouseIdle(const UIEvent& uiEvent)
{
	if (m_unk0x30 > 0) {
		m_unk0x30--;
	}
	if (captured()) {
		onMouseMove(uiEvent);
	}
}

PV SlingshotTool::VTable0x44(const UIEvent& uiEvent, const CoordinateFrame& cframe)
{
	G3D::Vector3 hitWorld;
	PartInstance* part = getPart(uiEvent, NULL, hitWorld);
	G3D::Vector3 direction = part ? (hitWorld - cframe.translation).direction() : cframe.lookVector();
	G3D::Vector3 position = cframe.translation + direction * 3.0f;
	if (!part) {
		CoordinateFrame frame(position);
		return PV(frame, Velocity(direction * 85.0f, G3D::Vector3::zero()));
	}
	else {
		G3D::Vector3 delta = hitWorld - position;
		float height = delta.y;
		delta.y = 0.0f;
		float distance = delta.unitize();
		float angle = Math::computeLaunchAngle(
			85.0f, distance, height, Units::kmsAccelerationToRbx(Constants::getKmsGravity()).y
		);
		delta.y = sin(angle);
		delta.x *= cos(angle);
		delta.z *= cos(angle);
		CoordinateFrame frame(position);
		return PV(frame, Velocity(delta * 85.0f, G3D::Vector3::zero()));
	}
}

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

ShootTool::ShootTool(Workspace* workspace, bool p_unk0x28, int p_unk0x2c)
	: MouseCommand(workspace), m_unk0x28(p_unk0x28), m_unk0x2c(p_unk0x2c), m_unk0x30(0)
{
}

boost::shared_ptr<PartInstance> SlingshotTool::VTable0x40()
{
	std::vector<boost::shared_ptr<Instance> > instances;
	ContentProvider::singleton().load(ContentId::fromAssets("Fonts\\SlingshotPellet.rbxm"), instances);
	return boost::shared_polymorphic_downcast<PartInstance>(instances.front());
}

boost::shared_ptr<PartInstance> RocketTool::VTable0x40()
{
	std::vector<boost::shared_ptr<Instance> > instances;
	ContentProvider::singleton().load(ContentId::fromAssets("Fonts\\Rocket.rbxm"), instances);
	return boost::shared_polymorphic_downcast<PartInstance>(instances.front());
}

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

MouseCommand* ShootTool::onMouseDown(const UIEvent& uiEvent)
{
	capture();
	if (m_unk0x30 == 0) {
		FUN_0063ab10(uiEvent);
	}
	return this;
}

void ShootTool::onMouseMove(const UIEvent& uiEvent)
{
	if (m_unk0x28 && m_unk0x30 == 0) {
		FUN_0063ab10(uiEvent);
	}
}

const std::string ResizeTool::getCursorName() const
{
	return m_unk0x2c ? std::string("ResizeCursor") : ArrowToolBase::getCursorName();
}

void ResizeTool::VTable0x40(XmlState* state)
{
	boost::shared_ptr<PVInstance> locked = m_unk0x24.lock();
	if (PartInstance* part = dynamic_cast<PartInstance*>(locked.get())) {
		part->writeResizeData(state);
	}
}

MouseCommand* ResizeTool::onMouseUp(const UIEvent& uiEvent)
{
	if (captured()) {
		releaseCapture();
		if (!m_unk0x24.expired()) {
			std::auto_ptr<RedoState> redoState(new RedoState());
			VTable0x40(redoState.get());
			XmlState* undo = undoState.release();
			XmlState* redo = redoState.release();
			IDataState& dataState = workspace->getDataState();
			const Name& name = getName();
			dataState.pushState(name.toString(), undo, redo);
			workspace->getDataState().setDirty(true);
		}
	}
	return NULL;
}

void ResizeTool::FUN_0063af80(int p_unk0x04)
{
	if (!m_unk0x24.expired()) {
		PartInstance* part = static_cast<PartInstance*>(m_unk0x24.lock().get());
		part->destroyJoints();
		if (part->resize(m_unk0x30, p_unk0x04)) {
			ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(CLICK_SOUND);
		}
		part->join();
	}
}

void ResizeTool::onMouseMove(const UIEvent& uiEvent)
{
	if (!m_unk0x24.expired()) {
		G3D::Vector3 axis = Math::getWorldNormal(m_unk0x30, m_unk0x24.lock()->getLocation());
		G3D::Ray handleRay = G3D::Ray::fromOriginAndDirection(m_unk0x34, axis);
		G3D::Ray mouseRay = getUnitMouseRay(uiEvent);
		G3D::Vector3 closest = Math::closestPointOnRay(handleRay, mouseRay);
		float distance = (closest - m_unk0x34).dot(handleRay.direction) - m_unk0x44;
		if (distance > 1.0f) {
			int amount = G3D::iRound(floor(distance));
			m_unk0x44 += amount;
			FUN_0063af80(amount);
		}
		if (distance < -1.0f) {
			int amount = -G3D::iRound(floor(-distance));
			m_unk0x44 += amount;
			FUN_0063af80(amount);
		}
	}
}

void ResizeTool::FUN_0063b240(const UIEvent& uiEvent)
{
	ServiceClient<Selection> selection(workspace);
	std::vector<boost::shared_ptr<Instance> >::const_iterator it;
	for (it = selection->begin(); it != selection->end(); ++it) {
		if (PartInstance* part = dynamic_cast<PartInstance*>(it->get())) {
			G3D::Ray ray = getUnitMouseRay(uiEvent);
			if (FUN_00662530(part->getExtentsLocal(), part->getLocation(), ray, m_unk0x34, m_unk0x30)) {
				m_unk0x24 = shared_from(part);
				return;
			}
		}
	}
	m_unk0x24.reset();
}

void ResizeTool::render3dAdorn(Adorn* adorn)
{
	ServiceClient<Selection> selection(workspace);
	std::vector<boost::shared_ptr<Instance> >::const_iterator it;
	for (it = selection->begin(); it != selection->end(); ++it) {
		if (PartInstance* part = dynamic_cast<PartInstance*>(it->get())) {
			DrawAdorn::handles3d(
				part->getPartSizeXml(),
				part->getCoordinateFrame(),
				adorn,
				HANDLE_RESIZE,
				workspace->getGCamera().getCoordinateFrame().translation
			);
		}
	}
}

void ResizeTool::render2d(Adorn* adorn)
{
	ServiceClient<Selection> selection(workspace);
	std::vector<boost::shared_ptr<Instance> >::const_iterator it;
	for (it = selection->begin(); it != selection->end(); ++it) {
		if (PartInstance* part = dynamic_cast<PartInstance*>(it->get())) {
			DrawAdorn::handles2d(
				part->getPartSizeXml(),
				part->getCoordinateFrame(),
				workspace->getGCamera(),
				adorn,
				DrawAdorn::resizeColor()
			);
		}
	}
}

void ResizeTool::onMouseHover(const UIEvent& uiEvent)
{
	FUN_0063b240(uiEvent);
	m_unk0x2c = !m_unk0x24.expired();
	ArrowToolBase::onMouseHover(uiEvent);
}

MouseCommand* ResizeTool::onMouseDown(const UIEvent& uiEvent)
{
	FUN_0063b240(uiEvent);
	if (m_unk0x24.expired()) {
		return ArrowToolBase::onMouseDown(uiEvent);
	}

	m_unk0x40 = uiEvent.mousePosition;
	m_unk0x44 = 0;
	m_unk0x48 = 0;
	m_unk0x2c = true;
	capture();
	undoState.reset(new UndoState());
	VTable0x40(undoState.get());
	return this;
}

const std::string HammerTool::getCursorName() const
{
	return part ? "HammerOverCursor" : "HammerCursor";
}

void HammerTool::render3dAdorn(Adorn* adorn)
{
	if (part) {
		part->render3dSelect(adorn, SELECT_NORMAL);
	}
}

void HammerTool::onMouseIdle(const UIEvent& uiEvent)
{
	part = shared_from(getUnlockedPartByLocalCharacter(uiEvent));
}

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

void LaserTool::render3dAdorn(Adorn* adorn)
{
	if (m_unk0x20 > 0) {
		m_unk0x20--;
		adorn->setObjectToWorldMatrix(G3D::CoordinateFrame());
		adorn->lineSegment(G3D::LineSegment::fromTwoPoints(m_unk0x24, m_unk0x30), G3D::Color3::red(), 5.0f);
	}
}

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

GrabTool::~GrabTool()
{
}

GrabTool::GrabTool(Workspace* workspace) : Named<MouseCommand, sGrabTool>(workspace), cursor("ArrowCursor")
{
}

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

const std::string CloneTool::getCursorName() const
{
	if (part) {
		return "CloneOverCursor";
	}
	return "CloneCursor";
}

CloneTool::CloneTool(Workspace* workspace) : Named<MouseCommand, sCloneTool>(workspace)
{
}

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

void CloneTool::onMouseIdle(const UIEvent& uiEvent)
{
	part = shared_from(getUnlockedPartByLocalCharacter(uiEvent));
}

bool GameTool::FUN_0063c2c0(PartInstance* part, const G3D::Vector3& hitWorld) const
{
	return !part->getPartLocked() && !part->lockedInPlace() && characterCanReach(hitWorld);
}

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

GameTool::~GameTool()
{
}

GameTool::GameTool(Workspace* workspace) : Named<MouseCommand, sGameTool>(workspace)
{
}

AxisMoveTool::AxisMoveTool(Workspace* workspace)
	: Named<ArrowToolBase, sAxisMoveTool>(workspace), cursor("ArrowCursor"), m_unk0x44(false)
{
}

MouseCommand* AxisMoveTool::onMouseUp(const UIEvent& uiEvent)
{
	if (m_unk0x44) {
		dragger->finishDragging();
		dragger.release();
		m_unk0x44 = false;
	}
	releaseCapture();
	return NULL;
}

void AxisMoveTool::onMouseMove(const UIEvent& uiEvent)
{
	if (!m_unk0x44) {
		if ((G3D::Vector2(uiEvent.mousePosition) - G3D::Vector2(m_unk0x46)).length() > 4.0f) {
			ServiceClient<FilteredSelection<PVInstance> > selection(workspace);
			dragger.reset(new MegaDragger(NULL, selection->items(), workspace));
			dragger->startDragging();
			dragger->alignAndCleanParts();
			m_unk0x44 = true;
		}
		if (!m_unk0x44) {
			return;
		}
	}

	if (dragger->anyDragPartAlive()) {
		G3D::Vector3 closest = Math::closestPointOnRay(m_unk0x4c, getUnitMouseRay(uiEvent));
		closest = Math::toGrid(closest, FUN_0063c5c0());
		G3D::Vector3 delta = Math::toGrid(closest - m_unk0x6c, FUN_0063c5c0());
		if (delta != G3D::Vector3::zero()) {
			dragger->continueDragging();
			G3D::Vector3 moved = dragger->safeMoveAlongLine(delta);
			if (moved != G3D::Vector3::zero()) {
				m_unk0x6c = Math::toGrid(m_unk0x6c + moved, FUN_0063c5c0());
			}
		}
	}
}

bool AxisMoveTool::FUN_0063cc60(Extents& extents)
{
	ServiceClient<Selection> selection(workspace);
	std::vector<Primitive*> primitives;
	std::for_each(
		selection->begin(), selection->end(), boost::bind(&DragUtilities::getPrimitives2, _1, boost::ref(primitives))
	);
	if (primitives.size() != 0) {
		extents = Dragger::computeExtents(primitives);
		return true;
	}

	return false;
}

bool AxisMoveTool::FUN_0063ce30(const UIEvent& uiEvent, G3D::Vector3& hitPoint, NormalId& normalId)
{
	Extents extents;
	if (FUN_0063cc60(extents)) {
		if (FUN_00662700(extents, getUnitMouseRay(uiEvent), hitPoint, normalId)) {
			return true;
		}
	}
	return false;
}

MouseCommand* AxisMoveTool::onMouseDown(const UIEvent& uiEvent)
{
	G3D::Vector3 hitPoint;
	NormalId normalId;
	if (FUN_0063ce30(uiEvent, hitPoint, normalId)) {
		m_unk0x46 = uiEvent.mousePosition;
		m_unk0x68 = normalId % 3;
		m_unk0x6c = Math::toGrid(hitPoint, FUN_0063c5c0());
		m_unk0x4c = G3D::Ray::fromOriginAndDirection(m_unk0x6c, normalIdToVector3(normalId));
		capture();
		return this;
	}

	return ArrowToolBase::onMouseDown(uiEvent);
}

void AxisMoveTool::render2d(Adorn* adorn)
{
	Extents extents;
	if (FUN_0063cc60(extents)) {
		CoordinateFrame location(extents.center());
		G3D::Vector3 size = extents.size();
		DrawAdorn::handles2d(size, location, workspace->getGCamera(), adorn, G3D::Color3::orange());
	}
}

void AxisMoveTool::render3dAdorn(Adorn* adorn)
{
	Extents extents;
	if (FUN_0063cc60(extents)) {
		CoordinateFrame location(extents.center());
		G3D::Vector3 size = extents.size();
		DrawAdorn::handles3d(
			size, location, adorn, HANDLE_MOVE, workspace->getGCamera().getCoordinateFrame().translation
		);
	}
}

void AxisMoveTool::onMouseIdle(const UIEvent& uiEvent)
{
	ArrowToolBase::onMouseIdle(uiEvent);
	if (captured()) {
		cursor = "DragCursor";
		return;
	}

	G3D::Vector3 hitPoint;
	NormalId normalId;
	if (FUN_0063ce30(uiEvent, hitPoint, normalId)) {
		cursor = "ResizeCursor";
	}
	else {
		cursor = ArrowToolBase::getCursorName();
	}
}

UndoVerb::UndoVerb(DataModel* dataModel) : Verb(dataModel, "Undo"), m_unk0x0c(dataModel)
{
}

RedoVerb::RedoVerb(DataModel* dataModel) : Verb(dataModel, "Redo"), m_unk0x0c(dataModel)
{
}

bool UndoVerb::isEnabled() const
{
	std::string name;
	return m_unk0x0c->canUndo(name);
}

void UndoVerb::doIt(IDataState* dataState)
{
	SerializerV2 serializer;
	serializer.merge(m_unk0x0c->undo()->getData(), m_unk0x0c);
}

bool RedoVerb::isEnabled() const
{
	std::string name;
	return m_unk0x0c->canRedo(name);
}

void RedoVerb::doIt(IDataState* dataState)
{
	SerializerV2 serializer;
	serializer.merge(m_unk0x0c->redo()->getData(), m_unk0x0c);
}

} // namespace RBX
