#include "v8datamodel/Workspace.h"

#include "network/Players.h"
#include "tool/NullTool.h"
#include "tool/ToolsArrow.h"
#include "util/Name.h"
#include "util/NavKeys.h"
#include "util/UserInputBase.h"
#include "util/standardout.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/Flag.h"
#include "v8datamodel/Hopper.h"
#include "v8datamodel/MouseCommand.h"
#include "v8datamodel/Tool.h"
#include "v8datamodel/UserController.h"
#include "v8world/World.h"

namespace RBX {

char sWorkspace[] = "Workspace";

bool Workspace::showWorldCoord;

// FUNCTION: WEBSERVICE 0x1006a0c0
bool Workspace::askAddChild(const Instance* instance) const
{
	return dynamic_cast<const IRenderable*>(instance) != NULL;
}

// FUNCTION: WEBSERVICE 0x1006a0f0
const G3D::GCamera& Workspace::getGCamera() const
{
	return getCamera()->getGCamera();
}

// FUNCTION: WEBSERVICE 0x1006a100
void Workspace::zoomToExtents()
{
	getCamera()->zoomExtents(viewPort);
}

// FUNCTION: WEBSERVICE 0x1006a120
void Workspace::stop()
{
	isSimulating = false;
	getCamera()->autoMode();
}

// FUNCTION: WEBSERVICE 0x1006a140
MouseCommand* newNullTool(Workspace* workspace)
{
	return new NewNullTool(workspace);
}

// FUNCTION: WEBSERVICE 0x1006a7c0
Workspace* Workspace::getMyWorkspaceFast(const Instance* context)
{
	const Instance* workspace = context->getParent();
	const Instance* dataModel = workspace->getParent();

	while (dataModel->getParent() != NULL) {
		workspace = dataModel;
		dataModel = dataModel->getParent();
	}

	return (Workspace*) workspace;
}

// FUNCTION: WEBSERVICE 0x1006a7f0
World* Workspace::getMyWorldFast(const Instance* context)
{
	return getMyWorkspaceFast(context)->getWorld();
}

// FUNCTION: WEBSERVICE 0x1006a830
void Workspace::setNullMouseCommand()
{
	stickyCommand.reset();
	currentCommand.reset(newNullTool(this));
}

// FUNCTION: WEBSERVICE 0x1006a880
void Workspace::render2d(Adorn* adorn)
{
	currentCommand->render2d(adorn);
	render2dItems(adorn);
}

// STUB: WEBSERVICE 0x1006a960
IScriptOwner* Workspace::scriptShouldRun(Script* script)
{
	STUB(0x1006a960);
	return NULL;
}

// FUNCTION: WEBSERVICE 0x1006b2d0
void Workspace::setThrottleEnabled(bool value)
{
	world->setCanThrottle(value);
}

// FUNCTION: WEBSERVICE 0x1006b2e0
void Workspace::joinAllHack()
{
	world->joinAll();
}

// FUNCTION: WEBSERVICE 0x1006b2f0
void Workspace::reset()
{
	isSimulating = false;
	getCamera()->autoMode();
	world->reset();
}

// STUB: WEBSERVICE 0x1006bfd0
Camera* Workspace::getCamera() const
{
	if (!currentCamera || !currentCamera->isDescendentOf(this)) {
		boost::shared_ptr<Camera> camera = Creatable<Instance>::create<Camera>();
		camera->setParent(const_cast<Workspace*>(this));
		const_cast<Workspace*>(this)->currentCamera = camera;
	}

	return currentCamera.get();
}

// FUNCTION: WEBSERVICE 0x1006c460
void Workspace::onChildChanged(Instance* instance, const PropertyChanged& event)
{
	Instance::onChildChanged(instance, event);
	raiseDrawChanged();
}

// FUNCTION: WEBSERVICE 0x1006c490
void Workspace::onDescendentRemoving(const boost::shared_ptr<Instance>& instance)
{
	if (IRenderable* renderable = dynamic_cast<IRenderable*>(instance.get())) {
		onRemoving(renderable);
	}

	RootInstance::onDescendentRemoving(instance);
	raiseDrawChanged();
}

// FUNCTION: WEBSERVICE 0x1006c4f0
void Workspace::onDescendentAdded(Instance* instance)
{
	RootInstance::onDescendentAdded(instance);
	raiseDrawChanged();

	if (IRenderable* renderable = dynamic_cast<IRenderable*>(instance)) {
		onAdded(renderable);
	}

	PVInstance* pvInstance = dynamic_cast<PVInstance*>(instance);
	if (pvInstance && pvInstance->getLegacyOffset()) {
		pvInstance->legacyTraverseState(CoordinateFrame());
		pvInstance->clearLegacyOffset();

		if (legacyOffset) {
			legacyOffset.reset();
		}
	}
}

// STUB: WEBSERVICE 0x1006c9e0
void Workspace::setMouseCommand(MouseCommand* command)
{
	if (!command) {
		if (stickyCommand.get()) {
			command = stickyCommand->isSticky();
		}

		if (!command) {
			if (!Network::Players::findLocalPlayer(this)) {
				command = new ArrowTool(this);
			}
			else {
				command = newNullTool(this);
			}
		}
	}

	if (command != currentCommand.get()) {
		currentCommand.reset(command);

		if (MouseCommand* sticky = command->isSticky()) {
			stickyCommand.reset(sticky);
		}

		Notifier<Workspace, ToolChanged>::raise(ToolChanged());
	}
}

// STUB: WEBSERVICE 0x1006ccb0
void Workspace::runScript(Script* script, ScriptContext* context)
{
	STUB(0x1006ccb0);
}

// FUNCTION: WEBSERVICE 0x1006cd70
Workspace::~Workspace()
{
}

// FUNCTION: WEBSERVICE 0x1006d0d0
Extents Workspace::computeCameraOwnerExtents()
{
	return getExtentsWorld();
}

// FUNCTION: WEBSERVICE 0x1006d230
void Workspace::cameraMoved()
{
	Notifier<Workspace, DrawChanged>::raise(DrawChanged());
}

// FUNCTION: WEBSERVICE 0x1006d410
void Workspace::setDefaultMouseCommand()
{
	stickyCommand.reset();
	setMouseCommand(NULL);
}

// FUNCTION: WEBSERVICE 0x1006d440
void Workspace::releaseScript(Script* script)
{
	pendingScripts.erase(script);
}

// FUNCTION: WEBSERVICE 0x1006ebc0
Workspace* Workspace::findWorkspace(const Instance* context)
{
	return ServiceProvider::find<Workspace>(context);
}

// FUNCTION: WEBSERVICE 0x1006ebd0
Workspace* Workspace::getWorkspaceIfInWorkspace(const Instance* context)
{
	Workspace* workspace = ServiceProvider::find<Workspace>(context);

	if (workspace != NULL && (context == workspace || context->isDescendentOf(workspace))) {
		return workspace;
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x1006ec00
World* Workspace::getWorldIfInWorkspace(const Instance* context)
{
	Workspace* workspace = ServiceProvider::find<Workspace>(context);

	if (workspace != NULL && (context == workspace || context->isDescendentOf(workspace))) {
		return workspace->world.get();
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x1006ec40
bool Workspace::contextInWorkspace(const Instance* context)
{
	return getWorkspaceIfInWorkspace(context) != NULL;
}

// FUNCTION: WEBSERVICE 0x1006ec80
void Workspace::onEvent(const RunService* source, Heartbeat event)
{
	onMovingHeartbeat();

	if (idleMouseEvent.eventType == UIEvent::MOUSE_IDLE) {
		currentCommand->onMouseIdle(idleMouseEvent);
	}

	if (!Network::Players::findLocalCharacter(this)) {
		ControllerService* controllerService = ServiceProvider::find<ControllerService>(this);
		if (controllerService && controllerService->getHardwareDevice()) {
			NavKeys navKeys;
			controllerService->getHardwareDevice()->getNavKeys(navKeys);
			if (navKeys.navKeyDown()) {
				getCamera()->doFly(navKeys, flySteps++);
				return;
			}

			flySteps = 0;
		}
	}
}

// FUNCTION: WEBSERVICE 0x100703e0
Workspace::Workspace(IDataState* dataState)
	: VerbContainer(NULL), dataState(dataState), arrowCameraControls(false), flySteps(0), isSimulating(false),
	  scriptContext(NULL), inMouselookMode(false), profileWorkspaceStep(new Profiling::CodeProfiler("DataModel Step")),
	  profileRunServiceStepped(new Profiling::CodeProfiler("RunService Stepped")),
	  profileRunServiceHeartbeat(new Profiling::CodeProfiler("RunService Heartbeat")),
	  profileRun(new Profiling::ThreadProfiler("Run Thread")), imageServerViewHack(0)
{
	setName("Workspace");
	world->getProfileWorldStep().parent = profileWorkspaceStep.get();
	profileRunServiceStepped->parent = profileWorkspaceStep.get();
	profileRunServiceHeartbeat->parent = profileWorkspaceStep.get();
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sControllerService>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sCamera>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sHopperBin>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sStarterPackService>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sLegacyHopperService>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sTool>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sFlag>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sArrowTool>();
template void RBX::Name::callDoDeclare<RBX::sControllerService>();
template void RBX::Name::callDoDeclare<RBX::sCamera>();
template void RBX::Name::callDoDeclare<RBX::sHopperBin>();
template void RBX::Name::callDoDeclare<RBX::sStarterPackService>();
template void RBX::Name::callDoDeclare<RBX::sLegacyHopperService>();
template void RBX::Name::callDoDeclare<RBX::sTool>();
template void RBX::Name::callDoDeclare<RBX::sFlag>();
template void RBX::Name::callDoDeclare<RBX::sArrowTool>();
