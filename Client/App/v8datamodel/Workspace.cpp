#include "v8datamodel/Workspace.h"

#include "humanoid/Humanoid.h"
#include "network/Players.h"
#include "rbxgraphics/Adorn.h"
#include "reflection/reflection.h"
#include "script/ScriptContext.h"
#include "tool/NullTool.h"
#include "tool/ToolsArrow.h"
#include "util/Name.h"
#include "util/NavKeys.h"
#include "util/RunStateOwner.h"
#include "util/UserInputBase.h"
#include "util/standardout.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/Flag.h"
#include "v8datamodel/Hopper.h"
#include "v8datamodel/ModelInstance.h"
#include "v8datamodel/MouseCommand.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Selection.h"
#include "v8datamodel/Stats.h"
#include "v8datamodel/Tool.h"
#include "v8datamodel/UserController.h"
#include "v8kernel/Kernel.h"
#include "v8world/SleepStage2.h"
#include "v8world/World.h"
#include "v8xml/SerializerV2.h"
#include "v8xml/XmlElement.h"
#include "v8xml/XmlSerializer.h"

#include <boost/bind.hpp>

namespace RBX {

char sWorkspace[] = "Workspace";

bool Workspace::showWorldCoord;
bool Workspace::showHashGrid;

static Reflection::
	BoundFuncDesc<Workspace, boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > >(ContentId), 1>
		workspace_insertContent(
			&Workspace::insertContent, "InsertContent", "url", Reflection::FunctionDescriptor::NeedTrustedCaller
		);
static Reflection::
	BoundFuncDesc<Workspace, void(boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > >), 1>
		workspace_makeJoints(
			&Workspace::makeJoints, "MakeJoints", "objects", Reflection::FunctionDescriptor::NeedTrustedCaller
		);
static Reflection::
	BoundFuncDesc<Workspace, void(boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > >), 1>
		workspace_breakJoints(
			&Workspace::breakJoints, "BreakJoints", "objects", Reflection::FunctionDescriptor::NeedTrustedCaller
		);
static Reflection::BoundFuncDesc<Workspace, void(bool), 1> workspace_SetThrottleEnabled(
	&Workspace::setThrottleEnabled,
	"SetPhysicsThrottleEnabled",
	"value",
	Reflection::FunctionDescriptor::NeedTrustedCaller
);

static Reflection::BoundFuncDesc<Workspace, void(), 0> workspace_zoomToExtents(
	&Workspace::zoomToExtents, "ZoomToExtents", Reflection::FunctionDescriptor::NeedTrustedCaller
);

static Reflection::RefPropDescriptor<Workspace, Camera> currentCameraProxyProp(
	"CurrentCamera", "Data", &Workspace::getCamera, &Workspace::setCamera
);

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

// FUNCTION: WEBSERVICE 0x1006a1a0
static double getFPS(Profiling::CodeProfiler* profiler)
{
	return profiler->getData(3.0).getActualFPS();
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

// FUNCTION: WEBSERVICE 0x1006a8b0
static void DrawAdorn(boost::shared_ptr<Instance> wsi, Adorn* adorn, SelectState selectState, Workspace* workspace)
{
	if (IRenderable* renderable = dynamic_cast<IRenderable*>(wsi.get())) {
		if (workspace->contains(wsi.get())) {
			renderable->render3dSelect(adorn, selectState);
		}
	}
}

// FUNCTION: WEBSERVICE 0x1006a960
IScriptOwner* Workspace::scriptShouldRun(Script* script)
{
	bool shouldRun;
	if (!dynamic_cast<LocalScript*>(script)) {
		shouldRun = Network::Players::backendProcessing(this, true);
	}
	else {
		ModelInstance* character = Network::Players::findLocalCharacter(this);
		shouldRun = character && script->isDescendentOf(character);
	}

	return shouldRun ? this : NULL;
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

template <int i>
static void wrapper(Instance* instance)
{
	PartInstance* part = dynamic_cast<PartInstance*>(instance);
	if (part) {
		if (i) {
			part->join();
		}
		else {
			part->destroyJoints();
		}
	}
	else {
		instance->for_eachChild(wrapper<i>);
	}
}

// FUNCTION: WEBSERVICE 0x1006bfd0
Camera* Workspace::getCamera() const
{
	if (!currentCamera || !currentCamera->isDescendentOf(this)) {
		boost::shared_ptr<Camera> newCam = Creatable<Instance>::create<Camera>();
		Workspace* workspace = const_cast<Workspace*>(this);
		newCam->setParent(workspace);
		const_cast<Workspace*>(this)->currentCamera = newCam;
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

template <int i>
static void wrapper2(boost::shared_ptr<Instance> instance)
{
	wrapper<i>(instance.get());
}

// FUNCTION: WEBSERVICE 0x1006c5b0
void Workspace::makeJoints(boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > instances)
{
	std::for_each(instances->begin(), instances->end(), wrapper2<1>);
}

// FUNCTION: WEBSERVICE 0x1006c650
void Workspace::breakJoints(boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > instances)
{
	std::for_each(instances->begin(), instances->end(), wrapper2<0>);
}

// FUNCTION: WEBSERVICE 0x1006c6f0
void clearEmptiedModels(boost::shared_ptr<Instance> test, Instance* workspace)
{
	if (test.get() != workspace) {
		if (ModelInstance* model = dynamic_cast<ModelInstance*>(test.get())) {
			if (!Humanoid::modelIsCharacter(model)) {
				if (!model->findFirstChildOfType<PartInstance>() && !model->findFirstChildOfType<ModelInstance>()) {
					boost::shared_ptr<Instance> oldParent = shared_from(model->getParent());
					model->setParent(NULL);
					clearEmptiedModels(oldParent, workspace);
				}
			}
		}

		if (Flag* flag = dynamic_cast<Flag*>(test.get())) {
			if (!flag->findFirstChildOfType<PartInstance>()) {
				boost::shared_ptr<Instance> oldParent = shared_from(flag->getParent());
				flag->setParent(NULL);
				clearEmptiedModels(oldParent, workspace);
			}
		}
	}
}

// FUNCTION: WEBSERVICE 0x1006c890
void Workspace::handleFallenParts(const G3D::Array<boost::shared_ptr<PartInstance> >& fallenParts)
{
	for (int i = 0; i < fallenParts.size(); ++i) {
		boost::shared_ptr<PartInstance> part = fallenParts[i];
		boost::shared_ptr<Instance> oldParent = shared_from(part->getParent());
		part->setParent(NULL);
		clearEmptiedModels(oldParent, this);
	}
}

// STUB: WEBSERVICE 0x1006c9e0
void Workspace::setMouseCommand(MouseCommand* newMouseCommand)
{
	if (!newMouseCommand) {
		if (stickyCommand.get()) {
			newMouseCommand = stickyCommand->isSticky();
		}

		if (!newMouseCommand) {
			if (!Network::Players::findLocalPlayer(this)) {
				newMouseCommand = new ArrowTool(this);
			}
			else {
				newMouseCommand = newNullTool(this);
			}
		}
	}

	if (newMouseCommand != currentCommand.get()) {
		currentCommand.reset(newMouseCommand);

		if (MouseCommand* sticky = newMouseCommand->isSticky()) {
			stickyCommand.reset(sticky);
		}

		Notifier<Workspace, ToolChanged>::raise(ToolChanged());
	}
}

// FUNCTION: WEBSERVICE 0x1006cb00
GuiResponse Workspace::process(const GuiEvent& event)
{
	switch (event.eventType) {
	default:
		return GuiResponse::notUsed();

	case UIEvent::KEY_DOWN:
		currentCommand->onPeekKeyDown(event);
		if (!currentCommand->captured()) {
			return GuiResponse::notUsed();
		}
		setMouseCommand(currentCommand->onKeyDown(event));
		break;

	case UIEvent::MOUSE_RIGHT_BUTTON_DOWN:
		inMouselookMode = true;
		event.userInput->setWrapMode(UserInputBase::WRAP_CENTER);
		break;

	case UIEvent::MOUSE_RIGHT_BUTTON_UP:
		inMouselookMode = false;
		event.userInput->setWrapMode(UserInputBase::WRAP_AUTO);
		break;

	case UIEvent::MOUSE_LEFT_BUTTON_DOWN:
		if (!currentCommand->captured()) {
			setMouseCommand(currentCommand->onMouseDown(event));
		}
		break;

	case UIEvent::MOUSE_MOVE:
		if (currentCommand->captured()) {
			currentCommand->onMouseMove(event);
		}
		else {
			currentCommand->onMouseHover(event);
		}
		break;

	case UIEvent::MOUSE_DELTA:
		if (currentCommand->captured()) {
			currentCommand->onMouseDelta(event);
		}
		break;

	case UIEvent::MOUSE_LEFT_BUTTON_UP:
		setMouseCommand(currentCommand->onMouseUp(event));
		break;
	}

	if (event.isMouseEvent()) {
		idleMouseEvent = event;
		idleMouseEvent.eventType = UIEvent::MOUSE_IDLE;
	}

	return GuiResponse::used(currentCommand->captured() ? this : NULL);
}

// FUNCTION: WEBSERVICE 0x1006ccb0
void Workspace::runScript(Script* script, ScriptContext* context)
{
	if (Network::Players::backendProcessing(this, true) && !isSimulating) {
		pendingScripts.insert(script);
	}
	else {
		context->addScript(script);
	}
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

// FUNCTION: WEBSERVICE 0x1006d280
float Workspace::step(float timeInterval)
{
	timeInterval = world->step(timeInterval);

	G3D::Array<boost::shared_ptr<PartInstance> > touchParts;
	G3D::Array<boost::shared_ptr<PartInstance> > touchOtherParts;
	G3D::Array<Primitive*> fallen;
	G3D::Array<boost::shared_ptr<PartInstance> > fallenParts;

	PartInstance::primitivesToParts(world->getTouch(), touchParts);
	PartInstance::primitivesToParts(world->getTouchOther(), touchOtherParts);

	world->computeFallen(fallen);
	PartInstance::primitivesToParts(fallen, fallenParts);
	handleFallenParts(fallenParts);

	for (int i = 0; i < touchParts.size(); ++i) {
		touchParts[i]->onTouchThisStep(touchOtherParts[i]);
	}

	raiseDrawChanged();

	return timeInterval;
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

// FUNCTION: WEBSERVICE 0x1006ed50
void Workspace::setImageServerView(const G3D::Rect2D& viewPortRect)
{
	HopperBin* superHack = NULL;

	for (unsigned int i = 0; i < numChildren(); ++i) {
		HopperBin* hopperBin = dynamic_cast<HopperBin*>(getChild(i));
		ILocation* location = dynamic_cast<ILocation*>(getChild(i));
		IRenderable* renderable = dynamic_cast<IRenderable*>(getChild(i));

		if (location && renderable) {
			CoordinateFrame modelCoord = location->getLocation();

			getCamera()->setImageServerViewNoLerp(modelCoord, viewPortRect);
		}

		if (hopperBin) {
			superHack = hopperBin;
		}
	}

	if (superHack) {
		superHack->setParent(ServiceProvider::create<StarterPackService>(this));
	}

	imageServerViewHack = imageServerViewHack <= 0;
}

// FUNCTION: WEBSERVICE 0x1006eeb0
void Workspace::insertContent(
	ContentId contentId, std::vector<boost::shared_ptr<Instance> >& items, InsertMode insertMode, PromptMode promptMode
)
{
	std::auto_ptr<std::istream> stream = ContentProvider::singleton().getContent(contentId);

	TextXmlParser machine(stream->rdbuf());
	std::auto_ptr<XmlElement> root = machine.parse();

	SerializerV2().loadInstances(root.get(), items);
	if (items.size() > 0) {
		insertInstances(items, this, insertMode, promptMode);
	}
}

// FUNCTION: WEBSERVICE 0x1006f030
static void append(const boost::shared_ptr<Instance>& item, std::vector<boost::shared_ptr<Instance> >* values)
{
	values->push_back(item);
}

// FUNCTION: WEBSERVICE 0x1006f040
void Workspace::start()
{
	isSimulating = true;
	getCamera()->alwaysMode();

	if (scriptContext) {
		std::vector<boost::shared_ptr<Script> > startScripts;

		std::set<Script*>::const_iterator iter = pendingScripts.begin();
		std::set<Script*>::const_iterator end = pendingScripts.end();
		while (iter != end) {
			startScripts.push_back(shared_from(*iter));
			++iter;
		}

		pendingScripts.clear();

		std::for_each(
			startScripts.begin(),
			startScripts.end(),
			boost::bind(&ScriptContext::addScript, scriptContext, boost::bind(&boost::shared_ptr<Script>::get, _1))
		);
	}
}

// FUNCTION: WEBSERVICE 0x1006f380
boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > Workspace::insertContent(ContentId contentId)
{
	std::vector<boost::shared_ptr<Instance> > items;
	insertContent(contentId, items, INSERT_TO_TREE, SUPPRESS_PROMPTS);

	boost::shared_ptr<std::vector<boost::shared_ptr<Instance> > > values(
		new std::vector<boost::shared_ptr<Instance> >()
	);

	std::for_each(items.begin(), items.end(), boost::bind(&append, _1, values.get()));
	return values;
}

// FUNCTION: WEBSERVICE 0x1006f6e0
void Workspace::render3dAdorn(Adorn* adorn)
{
	viewPort = adorn->getViewport();

	ServiceClient<Selection> sel(this);
	if (sel) {
		std::for_each(sel->begin(), sel->end(), boost::bind(&DrawAdorn, _1, adorn, SELECT_NORMAL, this));
	}

	render3dAdornItems(adorn);

	if (showWorldCoord) {
		adorn->setObjectToWorldMatrix(CoordinateFrame());
		adorn->axes(
			G3D::Color4(G3D::Color3::red()), G3D::Color4(G3D::Color3::green()), G3D::Color4(G3D::Color3::blue()), 50.0f
		);
	}

	if (showHashGrid) {
		adorn->setObjectToWorldMatrix(CoordinateFrame());
		adorn->box(
			G3D::AABox(G3D::Vector3(28, 4, 12), G3D::Vector3(32, 8, 16)),
			G3D::Color4(1.0f, 0.2f, 0.2f, 0.5f),
			G3D::Color4::clear()
		);
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

// SIZE 0x120
class WorkspaceStatsItem : public Stats::Item
{
public:
	WorkspaceStatsItem(const Workspace* workspace, const World* world);
};

DECOMP_SIZE_ASSERT(WorkspaceStatsItem, 0x120)

// FUNCTION: WEBSERVICE 0x100707e0
void Workspace::setCamera(Camera* value)
{
	if (value != currentCamera.get()) {
		if (currentCamera) {
			currentCamera->setParent(NULL);
		}

		currentCamera = shared_from(value);
		raisePropertyChanged(currentCameraProxyProp);
	}
}

// FUNCTION: WEBSERVICE 0x10070a20
WorkspaceStatsItem::WorkspaceStatsItem(const Workspace* workspace, const World* world)
{
	setName("Workspace");

	createChildItem<double>("FPS", boost::bind(&getFPS, workspace->profileWorkspaceStep.get()));
	createBoundChildItem(*workspace->profileRun);

	Stats::Item* pWorkspaceStep = createBoundChildItem(*workspace->profileWorkspaceStep);
	Stats::Item* worldStep = pWorkspaceStep->createBoundChildItem(world->getProfileWorldStep());
	worldStep->createBoundChildItem(world->getProfileUiStep());
	worldStep->createBoundChildItem(world->getProfileBroadphase());
	worldStep->createBoundChildItem(*world->getSleepStage()->profilingWake);
	worldStep->createBoundChildItem(*world->getSleepStage()->profilingCollision);
	worldStep->createBoundChildItem(*world->getSleepStage()->profilingSleep);
	worldStep->createBoundChildItem(*world->getKernel()->profilingKernel);
	pWorkspaceStep->createBoundChildItem(*workspace->profileRunServiceStepped);
	pWorkspaceStep->createBoundChildItem(*workspace->profileRunServiceHeartbeat);

	Stats::Item* worldItem = createChildItem("World");
	worldItem->createChildItem<int>("Primitives", boost::bind(&World::getNumPrimitives, world));
	worldItem->createChildItem<int>("Joints", boost::bind(&World::getNumJoints, world));
	worldItem->createChildItem<int>("Contacts", boost::bind(&World::getNumContacts, world));

	Stats::Item* contacts = createChildItem("Contacts");
	contacts->createChildItem<int>(
		"CtctStageCtcts", boost::bind(&World::getMetric, world, IWorldStage::NUM_CONTACTSTAGE_CONTACTS)
	);
	contacts->createChildItem<int>(
		"SteppingCtcts", boost::bind(&World::getMetric, world, IWorldStage::NUM_STEPPING_CONTACTS)
	);
	contacts->createChildItem<int>(
		"TouchingCtcts", boost::bind(&World::getMetric, world, IWorldStage::NUM_TOUCHING_CONTACTS)
	);
	contacts->createChildItem<int>("MaxTreeDepth", boost::bind(&World::getMetric, world, IWorldStage::MAX_TREE_DEPTH));
	contacts->createChildItem<int>("# link(p)", boost::bind(&World::getNumLinkCalls, world));
	contacts->createChildItem<int>("Hash Nodes Out", boost::bind(&World::getNumHashNodes, world));
	contacts->createChildItem<int>("Max Bucket Size", boost::bind(&World::getMaxBucketSize, world));

	Stats::Item* kernel = createChildItem("Kernel");
	kernel->createChildItem<int>("Bodies", boost::bind(&World::getNumBodies, world));
	kernel->createChildItem<int>("Constraints", boost::bind(&World::getNumConstraints, world));
	kernel->createChildItem<int>("Points", boost::bind(&World::getNumPoints, world));
}

// FUNCTION: WEBSERVICE 0x10070ff0
void Workspace::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	stopListening(oldProvider ? oldProvider->find<RunService>() : NULL, this);

	if (statsItem) {
		statsItem->setParent(NULL);
		statsItem.reset();
	}

	Instance::onServiceProvider(oldProvider, newProvider);

	scriptContext = newProvider ? newProvider->create<ScriptContext>() : NULL;

	if (newProvider) {
		if (Stats::StatsService* stats = newProvider->create<Stats::StatsService>()) {
			statsItem = Creatable<Instance>::create<WorkspaceStatsItem>(this, world.get());
			statsItem->setParent(stats);
		}
	}

	startListening(newProvider ? newProvider->find<RunService>() : NULL, this);
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
