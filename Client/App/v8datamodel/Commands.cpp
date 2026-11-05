#include "v8datamodel/Commands.h"

#include "util/Sound.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Selection.h"

namespace RBX {

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

// FUNCTION: ROBLOX 0x0059a2a0
EditSelectionVerb::EditSelectionVerb(VerbContainer* container, std::string name, DataModel* dataModel)
	: Verb(container, name), workspace(shared_from(dataModel ? dataModel->find<Workspace>() : NULL)),
	  selection(dataModel), dataModel(dataModel)
{
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

// STUB: ROBLOX 0x0059b7b0
void CameraModelViewCommand::doIt(IDataState* dataState)
{
	STUB(0x59b7b0);
}

// FUNCTION: ROBLOX 0x0059b800
void FixedCameraCommand::doIt(IDataState* dataState)
{
	getCamera()->setCameraType(Camera::FIXED_CAMERA);
	dataState->clearStack();
	dataState->setDirty(true);
}

// STUB: ROBLOX 0x0059ba50
bool CameraCenterCommand::isEnabled() const
{
	STUB(0x59ba50);
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

} // namespace RBX
