#include "v8datamodel/Commands.h"

#include "gui/GUI.h"
#include "network/Player.h"
#include "network/Players.h"
#include "util/Math.h"
#include "util/Sound.h"
#include "v8datamodel/Backpack.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/ModelInstance.h"
#include "v8datamodel/Selection.h"
#include "v8world/SpatialHash.h"
#include "v8world/World.h"

namespace RBX {

// FUNCTION: ROBLOX 0x00596660
static void FUN_00596660(DataModel* dataModel)
{
	if (Network::Players::clientIsPresent(dataModel, true) && !Network::Players::serverIsPresent(dataModel, true)) {
		throw std::runtime_error("Insufficinet permissions");
	}
}

// FUNCTION: ROBLOX 0x005966e0
FirstPersonCommand::FirstPersonCommand(DataModel* dataModel)
	: Verb(dataModel, "FirstPersonIndicator"), dataModel(dataModel)
{
}

// FUNCTION: ROBLOX 0x00596730
StatsCommand::StatsCommand(DataModel* dataModel) : Verb(dataModel, "Stats"), dataModel(dataModel)
{
}

// FUNCTION: ROBLOX 0x00596780
ForceAssertionCommand::ForceAssertionCommand(DataModel* dataModel) : Verb(dataModel, "ForceAssertion")
{
}

// FUNCTION: ROBLOX 0x005967c0
EngineStatsCommand::EngineStatsCommand(DataModel* dataModel) : Verb(dataModel, "EngineStats"), dataModel(dataModel)
{
}

// FUNCTION: ROBLOX 0x00596810
JoinCommand::JoinCommand(DataModel* dataModel) : Verb(dataModel, "JoinCommand"), dataModel(dataModel)
{
}

// FUNCTION: ROBLOX 0x00596860
ClearStarterpack::ClearStarterpack(DataModel* dataModel) : Verb(dataModel, "ClearStarterpack"), dataModel(dataModel)
{
}

// FUNCTION: ROBLOX 0x005968b0
ClearBackpack::ClearBackpack(DataModel* dataModel) : Verb(dataModel, "ClearBackpack"), dataModel(dataModel)
{
}

// FUNCTION: ROBLOX 0x00596900
void ClearBackpack::doIt(IDataState* dataState)
{
	FUN_00596660(dataModel);

	if (Network::Player* player = Network::Players::findLocalPlayer(dataModel)) {
		player->getPlayerBackpack()->removeAllChildren();
	}
}

// FUNCTION: ROBLOX 0x00596930
G3D::Matrix3 RotateSelectionVerb::getRotationAxis()
{
	return Math::matrixRotateY();
}

// FUNCTION: ROBLOX 0x00596950
static void FUN_00596950(Controller::ControllerType controllerType, PVInstance* instance)
{
	instance->setControllerType(controllerType);
}

// FUNCTION: ROBLOX 0x00596960
static bool FUN_00596960(PVInstance* instance)
{
	return instance->isControllable();
}

// FUNCTION: ROBLOX 0x00596980
bool CameraTiltUpCommand::isEnabled() const
{
	return getCamera()->canTilt(-1);
}

// FUNCTION: ROBLOX 0x005969a0
bool CameraTiltDownCommand::isEnabled() const
{
	return getCamera()->canTilt(1);
}

// FUNCTION: ROBLOX 0x005969c0
bool CameraZoomInCommand::isEnabled() const
{
	return getCamera()->canZoom(1);
}

// FUNCTION: ROBLOX 0x005969e0
bool CameraZoomOutCommand::isEnabled() const
{
	return getCamera()->canZoom(-1);
}

// FUNCTION: ROBLOX 0x00596a00
bool FixedCameraCommand::isChecked() const
{
	return getCamera()->getCameraType() == Camera::FIXED_CAMERA;
}

// FUNCTION: ROBLOX 0x00596d20
bool FirstPersonCommand::isEnabled() const
{
	return dataModel->getWorkspace()->getCamera()->isFirstPersonCamera();
}

// FUNCTION: ROBLOX 0x00596d50
void StatsCommand::doIt(IDataState* dataState)
{
	GuiRoot* guiRoot = dataModel->getGuiRoot();

	if (TopMenuBar* bar = dynamic_cast<TopMenuBar*>(guiRoot->findFirstChildByName("StatsHud1"))) {
		bar->setVisible(!bar->isVisible());
	}

	if (TopMenuBar* bar = dynamic_cast<TopMenuBar*>(guiRoot->findFirstChildByName("StatsHud2"))) {
		bar->setVisible(!bar->isVisible());
	}
}

// FUNCTION: ROBLOX 0x00596e60
bool StatsCommand::isEnabled() const
{
	GuiRoot* guiRoot = dataModel->getGuiRoot();
	return dynamic_cast<TopMenuBar*>(guiRoot->findFirstChildByName("StatsHud1")) != NULL;
}

// FUNCTION: ROBLOX 0x00596ef0
bool StatsCommand::isChecked() const
{
	GuiRoot* guiRoot = dataModel->getGuiRoot();
	TopMenuBar* bar = dynamic_cast<TopMenuBar*>(guiRoot->findFirstChildByName("StatsHud1"));
	return bar && bar->isVisible();
}

// FUNCTION: ROBLOX 0x00596fa0
void EngineStatsCommand::doIt(IDataState* dataState)
{
	dataModel->getWorkspace()->getWorld()->getContactManager().getSpatialHash().doStats();
}

// FUNCTION: ROBLOX 0x00597040
G3D::Matrix3 TiltSelectionVerb::getRotationAxis()
{
	return Math::matrixTiltQuadrant(Math::toYAxisQuadrant(workspace->getGCamera().getCoordinateFrame()));
}

// FUNCTION: ROBLOX 0x00597090
bool AttachCameraCommand::isChecked() const
{
	return workspace->getCamera()->getCameraType() == Camera::ATTACH_CAMERA;
}

// FUNCTION: ROBLOX 0x005970c0
bool TrackCameraCommand::isChecked() const
{
	return workspace->getCamera()->getCameraType() == Camera::TRACK_CAMERA;
}

// FUNCTION: ROBLOX 0x005970f0
bool FollowCameraCommand::isChecked() const
{
	return workspace->getCamera()->getCameraType() == Camera::FOLLOW_CAMERA;
}

// FUNCTION: ROBLOX 0x00597120
bool WatchCameraCommand::isChecked() const
{
	return workspace->getCamera()->getCameraType() == Camera::WATCH_CAMERA;
}

// FUNCTION: ROBLOX 0x005972c0
bool ClearBackpack::isEnabled() const
{
	if (Network::Player* player = Network::Players::findLocalPlayer(dataModel)) {
		if (Backpack* backpack = player->getPlayerBackpack()) {
			return backpack->numChildren() > 0;
		}
	}

	return false;
}

// FUNCTION: ROBLOX 0x00597450
static bool FUN_00597450(ModelInstance* model)
{
	return model->numChildren() > 0;
}

// FUNCTION: ROBLOX 0x00597310
RunStateVerb::~RunStateVerb()
{
}

// FUNCTION: ROBLOX 0x00597390
EditSelectionVerb::~EditSelectionVerb()
{
}

// FUNCTION: ROBLOX 0x00597680
RunStateVerb::RunStateVerb(std::string name, DataModel* dataModel)
	: Verb(dataModel, name), dataModel(dataModel), runService(dataModel)
{
}

// FUNCTION: ROBLOX 0x00597720
CameraVerb::CameraVerb(std::string name, Workspace* workspace)
	: Verb(workspace, name), workspace(workspace), selection(workspace)
{
}

bool CameraVerb::isEnabled() const
{
	return true;
}

// FUNCTION: ROBLOX 0x00597f30
EditSelectionVerb::EditSelectionVerb(std::string name, DataModel* dataModel)
	: Verb(dataModel, name), workspace(shared_from(dataModel->getWorkspace())), selection(dataModel),
	  dataModel(dataModel)
{
}

// FUNCTION: ROBLOX 0x00597fe0
GroupSelectionVerb::GroupSelectionVerb(DataModel* dataModel) : EditSelectionVerb("Group", dataModel)
{
}

// FUNCTION: ROBLOX 0x00598010
WeldSelectionVerb::WeldSelectionVerb(DataModel* dataModel) : EditSelectionVerb("WeldSelection", dataModel)
{
}

// FUNCTION: ROBLOX 0x00598040
SnapSelectionVerb::SnapSelectionVerb(DataModel* dataModel) : EditSelectionVerb("SnapSelection", dataModel)
{
}

// FUNCTION: ROBLOX 0x00598070
UngroupSelectionVerb::UngroupSelectionVerb(DataModel* dataModel) : EditSelectionVerb("UnGroup", dataModel)
{
}

// FUNCTION: ROBLOX 0x005980a0
DuplicateSelectionVerb::DuplicateSelectionVerb(DataModel* dataModel) : EditSelectionVerb("Duplicate", dataModel)
{
}

// FUNCTION: ROBLOX 0x00598150
AttachCameraCommand::AttachCameraCommand(DataModel* dataModel) : EditSelectionVerb("AttachCamera", dataModel)
{
}

// FUNCTION: ROBLOX 0x00598180
TrackCameraCommand::TrackCameraCommand(DataModel* dataModel) : EditSelectionVerb("TrackCamera", dataModel)
{
}

// FUNCTION: ROBLOX 0x005981b0
FollowCameraCommand::FollowCameraCommand(DataModel* dataModel) : EditSelectionVerb("FollowCamera", dataModel)
{
}

// FUNCTION: ROBLOX 0x00598200
WatchCameraCommand::WatchCameraCommand(DataModel* dataModel) : EditSelectionVerb("WatchCamera", dataModel)
{
}

// FUNCTION: ROBLOX 0x00598230
MoveDownSelectionVerb::MoveDownSelectionVerb(DataModel* dataModel) : EditSelectionVerb("SelectionDown", dataModel)
{
}

// STUB: ROBLOX 0x00599f80
bool JoinCommand::isEnabled() const
{
	STUB(0x599f80);
	return false;
}

// STUB: ROBLOX 0x0059a110
void JoinCommand::doIt(IDataState* dataState)
{
	STUB(0x59a110);
}

// FUNCTION: ROBLOX 0x0059a230
bool ClearStarterpack::isEnabled() const
{
	if (dataModel) {
		if (StarterPackService* starterPack = dataModel->find<StarterPackService>()) {
			return starterPack->numChildren() > 0;
		}
	}

	return false;
}

// FUNCTION: ROBLOX 0x0059a270
void ClearStarterpack::doIt(IDataState* dataState)
{
	FUN_00596660(dataModel);

	if (dataModel) {
		if (StarterPackService* starterPack = dataModel->find<StarterPackService>()) {
			starterPack->removeAllChildren();
		}
	}
}

// FUNCTION: ROBLOX 0x00598960
RotateAxisCommand::RotateAxisCommand(std::string name, DataModel* dataModel) : EditSelectionVerb(name, dataModel)
{
}

// FUNCTION: ROBLOX 0x005989d0
ControllerCommand::ControllerCommand(std::string name, DataModel* dataModel) : EditSelectionVerb(name, dataModel)
{
}

// FUNCTION: ROBLOX 0x00598a40
BoolPropertyVerb::BoolPropertyVerb(const std::string& name, DataModel* dataModel, const char* property)
	: EditSelectionVerb(name, dataModel), propertyName(Name::declare(property, -1))
{
}

// FUNCTION: ROBLOX 0x00598e50
RotateSelectionVerb::RotateSelectionVerb(DataModel* dataModel) : RotateAxisCommand("SelectionRotate", dataModel)
{
}

// FUNCTION: ROBLOX 0x00598e80
TiltSelectionVerb::TiltSelectionVerb(DataModel* dataModel) : RotateAxisCommand("SelectionTilt", dataModel)
{
}

// FUNCTION: ROBLOX 0x00598eb0
PrimaryControllerCommand::PrimaryControllerCommand(DataModel* dataModel) : ControllerCommand("Controller1", dataModel)
{
}

// FUNCTION: ROBLOX 0x00598ee0
SecondaryControllerCommand::SecondaryControllerCommand(DataModel* dataModel)
	: ControllerCommand("Controller2", dataModel)
{
}

// FUNCTION: ROBLOX 0x0059a2a0
EditSelectionVerb::EditSelectionVerb(VerbContainer* container, std::string name, DataModel* dataModel)
	: Verb(container, name), workspace(shared_from(dataModel ? dataModel->find<Workspace>() : NULL)),
	  selection(dataModel), dataModel(dataModel)
{
}

// FUNCTION: ROBLOX 0x0059a350
bool DuplicateSelectionVerb::isEnabled() const
{
	return (dataModel ? dataModel->create<Selection>() : NULL)->size() > 0;
}

// FUNCTION: ROBLOX 0x0059a410
void ControllerCommand::setController(Controller::ControllerType controllerType)
{
	FilteredSelection<PVInstance>* filteredSelection =
		dataModel ? dataModel->create<FilteredSelection<PVInstance> >() : NULL;

	Controller::ControllerType newType =
		filteredSelection->FUN_00598770(controllerType) ? controllerType : Controller::NO_CONTROLLER;

	std::for_each(filteredSelection->begin(), filteredSelection->end(), boost::bind(&FUN_00596950, newType, _1));
}

// STUB: ROBLOX 0x0059a9f0
void GroupSelectionVerb::doIt(IDataState* dataState)
{
	STUB(0x59a9f0);
}

// STUB: ROBLOX 0x0059aaf0
void UngroupSelectionVerb::doIt(IDataState* dataState)
{
	STUB(0x59aaf0);
}

// STUB: ROBLOX 0x0059ad50
void RotateAxisCommand::doIt(IDataState* dataState)
{
	STUB(0x59ad50);
}

// FUNCTION: ROBLOX 0x0059af30
void AttachCameraCommand::doIt(IDataState* dataState)
{
	FilteredSelection<PVInstance>* filteredSelection =
		dataModel ? dataModel->create<FilteredSelection<PVInstance> >() : NULL;
	PVInstance* subject = filteredSelection->items().size() > 0 ? filteredSelection->items().front() : NULL;

	workspace->getCamera()->setCameraType(Camera::ATTACH_CAMERA);
	workspace->getCamera()->setCameraSubject(subject);
	dataState->clearStack();
	dataState->setDirty(true);
}

// FUNCTION: ROBLOX 0x0059aff0
void TrackCameraCommand::doIt(IDataState* dataState)
{
	Selection* currentSelection = dataModel ? dataModel->create<Selection>() : NULL;

	if (PVInstance* subject = dynamic_cast<PVInstance*>(currentSelection->front().get())) {
		workspace->getCamera()->setCameraSubject(subject);
		workspace->getCamera()->setCameraType(Camera::TRACK_CAMERA);
	}

	dataState->clearStack();
	dataState->setDirty(true);
}

// FUNCTION: ROBLOX 0x0059b100
void FollowCameraCommand::doIt(IDataState* dataState)
{
	FilteredSelection<PVInstance>* filteredSelection =
		dataModel ? dataModel->create<FilteredSelection<PVInstance> >() : NULL;
	PVInstance* subject = filteredSelection->items().size() > 0 ? filteredSelection->items().front() : NULL;

	workspace->getCamera()->setCameraType(Camera::FOLLOW_CAMERA);
	workspace->getCamera()->setCameraSubject(subject);
	dataState->clearStack();
	dataState->setDirty(true);
}

// FUNCTION: ROBLOX 0x0059b1c0
void WatchCameraCommand::doIt(IDataState* dataState)
{
	FilteredSelection<PVInstance>* filteredSelection =
		dataModel ? dataModel->create<FilteredSelection<PVInstance> >() : NULL;
	PVInstance* subject = filteredSelection->items().size() > 0 ? filteredSelection->items().front() : NULL;

	workspace->getCamera()->setCameraType(Camera::WATCH_CAMERA);
	workspace->getCamera()->setCameraSubject(subject);
	dataState->clearStack();
	dataState->setDirty(true);
}

// STUB: ROBLOX 0x0059b3a0
void MoveDownSelectionVerb::doIt(IDataState* dataState)
{
	STUB(0x59b3a0);
}

// FUNCTION: ROBLOX 0x0059b4d0
void PrimaryControllerCommand::doIt(IDataState* dataState)
{
	setController(Controller::PRIMARY_CONTROLLER);
	dataState->clearStack();
	dataState->setDirty(true);
}

// FUNCTION: ROBLOX 0x0059b500
void SecondaryControllerCommand::doIt(IDataState* dataState)
{
	setController(Controller::SECONDARY_CONTROLLER);
	dataState->clearStack();
	dataState->setDirty(true);
}

// FUNCTION: ROBLOX 0x0059b530
void CameraVerb::doIt(IDataState* dataState)
{
	ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(STEP_SOUND);
	dataState->clearStack();
	dataState->setDirty(true);
}

// FUNCTION: ROBLOX 0x0059b570
void CameraPanLeftCommand::doIt(IDataState* dataState)
{
	getCamera()->panUnits(-1);
	CameraVerb::doIt(dataState);
}

// FUNCTION: ROBLOX 0x0059b5d0
void CameraPanRightCommand::doIt(IDataState* dataState)
{
	getCamera()->panUnits(1);
	CameraVerb::doIt(dataState);
}

// FUNCTION: ROBLOX 0x0059b630
void CameraTiltUpCommand::doIt(IDataState* dataState)
{
	if (getCamera()->tiltUnits(-1)) {
		CameraVerb::doIt(dataState);
	}
}

// FUNCTION: ROBLOX 0x0059b690
void CameraTiltDownCommand::doIt(IDataState* dataState)
{
	if (getCamera()->tiltUnits(1)) {
		CameraVerb::doIt(dataState);
	}
}

// FUNCTION: ROBLOX 0x0059b6f0
void CameraZoomInCommand::doIt(IDataState* dataState)
{
	if (getCamera()->zoom(1.0f)) {
		CameraVerb::doIt(dataState);
	}
}

// FUNCTION: ROBLOX 0x0059b750
void CameraZoomOutCommand::doIt(IDataState* dataState)
{
	if (getCamera()->zoom(-1.0f)) {
		CameraVerb::doIt(dataState);
	}
}

// FUNCTION: ROBLOX 0x0059b7b0
void CameraModelViewCommand::doIt(IDataState* dataState)
{
	workspace->setImageServerView(workspace->getViewPort());
	ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(STEP_SOUND);
	dataState->clearStack();
	dataState->setDirty(true);
}

// FUNCTION: ROBLOX 0x0059b800
void FixedCameraCommand::doIt(IDataState* dataState)
{
	getCamera()->setCameraType(Camera::FIXED_CAMERA);
	dataState->clearStack();
	dataState->setDirty(true);
}

// STUB: ROBLOX 0x0059b850
bool BoolPropertyVerb::isChecked() const
{
	STUB(0x59b850);
	return false;
}

// STUB: ROBLOX 0x0059b900
void BoolPropertyVerb::doIt(IDataState* dataState)
{
	STUB(0x59b900);
}

// FUNCTION: ROBLOX 0x0059ba50
bool CameraCenterCommand::isEnabled() const
{
	bool hasSelection = ServiceClient<FilteredSelection<PVInstance> >(workspace)->items().size() >= 1;

	if (hasSelection) {
		Camera* camera = getCamera();

		if (camera && camera->getCameraType() == Camera::FIXED_CAMERA) {
			return true;
		}
	}

	return false;
}

// STUB: ROBLOX 0x0059bb40
void CameraCenterCommand::doIt(IDataState* dataState)
{
	STUB(0x59bb40);
}

// FUNCTION: ROBLOX 0x0059bf60
bool EditSelectionVerb::isEnabled() const
{
	return selection->size() > 0;
}

// FUNCTION: ROBLOX 0x0059bfa0
bool GroupSelectionVerb::isEnabled() const
{
	if (selection->size() == 0) {
		return false;
	}

	ServiceClient<Selection> currentSelection(dataModel);
	return currentSelection->size() > 1;
}

// STUB: ROBLOX 0x0059c090
void WeldSelectionVerb::doIt(IDataState* dataState)
{
	STUB(0x59c090);
}

// FUNCTION: ROBLOX 0x0059c160
bool WeldSelectionVerb::isEnabled() const
{
	if (!EditSelectionVerb::isEnabled()) {
		return false;
	}

	return (dataModel ? dataModel->create<Selection>() : NULL)->size() > 0;
}

bool SnapSelectionVerb::isEnabled() const
{
	if (!EditSelectionVerb::isEnabled()) {
		return false;
	}

	return (dataModel ? dataModel->create<Selection>() : NULL)->size() > 0;
}

// STUB: ROBLOX 0x0059c1d0
void SnapSelectionVerb::doIt(IDataState* dataState)
{
	STUB(0x59c1d0);
}

// FUNCTION: ROBLOX 0x0059c2a0
bool UngroupSelectionVerb::isEnabled() const
{
	if (selection->size() == 0) {
		return false;
	}

	FilteredSelection<ModelInstance>* filteredSelection =
		dataModel ? dataModel->create<FilteredSelection<ModelInstance> >() : NULL;
	return filteredSelection->FUN_00598690(&FUN_00597450) != NULL;
}

// STUB: ROBLOX 0x0059c2f0
void DuplicateSelectionVerb::doIt(IDataState* dataState)
{
	STUB(0x59c2f0);
}

// STUB: ROBLOX 0x0059c860
bool AttachCameraCommand::isEnabled() const
{
	if (selection->size() == 0) {
		return false;
	}

	ServiceClient<FilteredSelection<PVInstance> > filteredSelection(dataModel);
	return filteredSelection->items().size() == 1;
}

// FUNCTION: ROBLOX 0x0059c950
bool TrackCameraCommand::isEnabled() const
{
	if (!EditSelectionVerb::isEnabled()) {
		return false;
	}

	return (dataModel ? dataModel->create<Selection>() : NULL)->size() == 1;
}

// FUNCTION: ROBLOX 0x0059c9c0 FOLDED
bool FollowCameraCommand::isEnabled() const
{
	if (selection->size() > 0) {
		FilteredSelection<PVInstance>* filteredSelection =
			dataModel ? dataModel->create<FilteredSelection<PVInstance> >() : NULL;

		if (filteredSelection->items().size() == 1) {
			return true;
		}
	}

	return false;
}

// FUNCTION: ROBLOX 0x0059c9c0 FOLDED
bool WatchCameraCommand::isEnabled() const
{
	if (selection->size() > 0) {
		FilteredSelection<PVInstance>* filteredSelection =
			dataModel ? dataModel->create<FilteredSelection<PVInstance> >() : NULL;

		if (filteredSelection->items().size() == 1) {
			return true;
		}
	}

	return false;
}

// FUNCTION: ROBLOX 0x0059ca20
bool ControllerCommand::isEnabled() const
{
	if (selection->size() == 0) {
		return false;
	}

	FilteredSelection<PVInstance>* filteredSelection =
		dataModel ? dataModel->create<FilteredSelection<PVInstance> >() : NULL;
	return filteredSelection->FUN_00598690(&FUN_00596960) != NULL;
}

// STUB: ROBLOX 0x0059ca70
void CameraZoomExtentsCommand::doIt(IDataState* dataState)
{
	STUB(0x59ca70);
}

bool CameraPanLeftCommand::isEnabled() const
{
	return true;
}

bool CameraPanRightCommand::isEnabled() const
{
	return true;
}

bool FixedCameraCommand::isEnabled() const
{
	return true;
}

void FirstPersonCommand::doIt(IDataState* dataState)
{
}

void ForceAssertionCommand::doIt(IDataState* dataState)
{
}

bool EngineStatsCommand::isEnabled() const
{
	return true;
}

} // namespace RBX
