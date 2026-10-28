#include "v8datamodel/DataModel.h"

#include "compat.h"
#include "gui/GUI.h"
#include "network/Players.h"
#include "reflection/reflection.h"
#include "reflection/signal.h"
#include "script/ScriptContext.h"
#include "util/ContentProvider.h"
#include "util/Http.h"
#include "util/IMetric.h"
#include "util/Log.h"
#include "util/Name.h"
#include "util/Sound.h"
#include "v8datamodel/Accoutrement.h"
#include "v8datamodel/Backpack.h"
#include "v8datamodel/JointInstance.h"
#include "v8datamodel/JointsService.h"
#include "v8datamodel/Lighting.h"
#include "v8datamodel/LocalBackpack.h"
#include "v8datamodel/Teams.h"
#include "v8datamodel/TimeState.h"
#include "v8datamodel/Workspace.h"
#include "v8kernel/Kernel.h"
#include "v8tree/Service.h"
#include "v8world/Contact.h"
#include "v8world/World.h"

#include <G3D/format.h>
#include <sstream>

namespace RBX {

const char sDataModel[] = "DataModel";

typedef boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > Instances;

static Reflection::BoundFuncDesc<DataModel, Instances(ContentId), 1> getContentFunctionOld(
	&DataModel::get,
	"get",
	"url",
	Reflection::FunctionDescriptor::AnyCaller
);
static Reflection::BoundFuncDesc<DataModel, Instances(ContentId), 1> getContentFunction(
	&DataModel::get,
	"GetObjects",
	"url",
	Reflection::FunctionDescriptor::AnyCaller
);

static Reflection::BoundFuncDesc<DataModel, void(ContentId), 1> loadFunction(
	&DataModel::loadContent,
	"Load",
	"url",
	Reflection::FunctionDescriptor::NeedTrustedCaller
);
static Reflection::BoundFuncDesc<DataModel, void(ContentId), 1> saveFunction(
	&DataModel::save,
	"Save",
	"url",
	Reflection::FunctionDescriptor::NeedTrustedCaller
);

static Reflection::BoundFuncDesc<DataModel, std::string(std::string, bool), 2> httpGetFunction(
	&DataModel::httpGet,
	"HttpGet",
	"url",
	"synchronous",
	false,
	Reflection::FunctionDescriptor::AnyCaller
);

static Reflection::BoundFuncDesc<DataModel, std::string(std::string, std::string, bool), 3> httpPostFunction(
	&DataModel::httpPost,
	"HttpPost",
	"url",
	"data",
	"synchronous",
	false,
	Reflection::FunctionDescriptor::NeedTrustedCaller
);

static Reflection::BoundFuncDesc<DataModel, void(), 0> sanitizeFunction(
	&DataModel::clearContents,
	"ClearContent",
	Reflection::FunctionDescriptor::NeedTrustedCaller
);
static Reflection::BoundFuncDesc<DataModel, void(), 0> closeFunction(
	&DataModel::close,
	"Close",
	Reflection::FunctionDescriptor::NeedTrustedCaller
);

static Reflection::BoundFuncDesc<DataModel, void(std::string), 1> func_SetUiMessage(
	&DataModel::setUiMessage,
	"SetMessage",
	"message",
	Reflection::FunctionDescriptor::NeedTrustedCaller
);
static Reflection::BoundFuncDesc<DataModel, void(), 0> func_ClearUIMessage(
	&DataModel::clearUiMessage,
	"ClearMessage",
	Reflection::FunctionDescriptor::NeedTrustedCaller
);
static Reflection::BoundFuncDesc<DataModel, void(), 0> func_SetUIMessageBrickCount(
	&DataModel::setUiMessageBrickCount,
	"SetMessageBrickCount",
	Reflection::FunctionDescriptor::NeedTrustedCaller
);

static Reflection::SignalDesc<DataModel, void()> event_Closing("Close");

// FUNCTION: WEBSERVICE 0x10071620
std::string DataModel::doHttpGet(std::string url)
{
	std::string response;
	Http(url.c_str()).get(response);
	return response;
}

// FUNCTION: WEBSERVICE 0x100716e0
std::string DataModel::doHttpPost(std::string url, std::string data)
{
	std::string response;
#ifdef COMPAT_MODE
	std::istringstream input(data);
	Http(url).post(input, true, response);
#else
	Http(url).post(std::istringstream(data), true, response);
#endif
	return response;
}

// FUNCTION: WEBSERVICE 0x10071800
void DataModel::close()
{
	Verb* verb = getVerb("Exit");
	if (!verb) {
		throw std::runtime_error("Couldn't find Exit Verb");
	}
	verb->doIt(NULL);
}

// FUNCTION: WEBSERVICE 0x10071890
static std::string report(const G3D::Stopwatch& timer)
{
	char buffer[256];
	if (timer.smoothElapsedTime() > 0) {
		std::string t = Log::formatTime(timer.smoothElapsedTime());
		sprintf(buffer, "%s (%.3gfps)", t.c_str(), 1.0 / timer.smoothElapsedTime());
		return buffer;
	}

	return Log::formatTime(timer.smoothElapsedTime());
}

// FUNCTION: WEBSERVICE 0x100725c0
void DataModel::onChildAdded(Instance* child)
{
	ServiceProvider::onChildAdded(child);

	if (child->getClassName() == "NetworkClient") {
		Verb* verb = getVerb("LockPlayMode");
		if (verb) {
			verb->doIt(NULL);
		}

		workspace->setNullMouseCommand();
	}
}

// STUB: WEBSERVICE 0x10072640
float DataModel::step(float distributedGameTime)
{
	STUB(0x10072640);

	return 0.0f;
}

// FUNCTION: WEBSERVICE 0x10072810
void DataModel::onEvent(const RunService* source, RunTransition event)
{
	switch (event.newState) {
	case RS_NORMAL:
		if (event.oldState == RS_RUNNING) {
			workspace->reset();
		}
		timeState->clear();
		break;
	case RS_RUNNING:
		workspace->start();
		break;
	case RS_PAUSED:
		workspace->stop();
		break;
	}
}

float DataModel::getSimTime() const
{
	return timeState->totalVirtualTime;
}

// FUNCTION: WEBSERVICE 0x10072860
std::string DataModel::evaluate(const std::string& valueName) const
{
	World* world = workspace->getWorld();

	if (valueName == "timer") {
		return StringConverter<float>::convertToString(getSimTime());
	}
	if (valueName == "FPS") {
		return G3D::format("%.3g", timeState->fpsStopwatch.smoothFPS());
	}
	if (valueName == "simDC") {
		return report(timeState->simulationStopwatch);
	}
	if (valueName == "drawId") {
		return StringConverter<int>::convertToString(drawId);
	}
	if (valueName == "graphicsMode") {
		return tempMetric->getMetric(valueName);
	}
	if (valueName == "FRM") {
		return tempMetric->getMetric(valueName);
	}
	if (valueName == "Shadows") {
		return tempMetric->getMetric(valueName);
	}

	if (valueName == "antiAliasing") {
		return tempMetric->getMetric(valueName);
	}
	if (valueName == "bevels") {
		return tempMetric->getMetric(valueName);
	}
	if (valueName == "videoMemory") {
		return tempMetric->getMetric(valueName);
	}
	if (valueName == "GfxTime") {
		return tempMetric->getMetric(valueName);
	}

	if (valueName == "numPrimitives") {
		return StringConverter<int>::convertToString(world->getNumPrimitives());
	}
	if (valueName == "numJoints") {
		return StringConverter<int>::convertToString(world->getNumJoints());
	}
	if (valueName == "numContacts") {
		return StringConverter<int>::convertToString(world->getNumContacts());
	}
	if (valueName == "numContactsInCollisionStage") {
		return StringConverter<int>::convertToString(world->getMetric(IWorldStage::NUM_CONTACTSTAGE_CONTACTS));
	}
	if (valueName == "numSteppingContacts") {
		return StringConverter<int>::convertToString(world->getMetric(IWorldStage::NUM_STEPPING_CONTACTS));
	}
	if (valueName == "numTouchingContacts") {
		return StringConverter<int>::convertToString(world->getMetric(IWorldStage::NUM_TOUCHING_CONTACTS));
	}
	if (valueName == "maxTreeDepth") {
		return StringConverter<int>::convertToString(world->getMetric(IWorldStage::MAX_TREE_DEPTH));
	}
	if (valueName == "contactPairHitRatio") {
		return StringConverter<float>::convertToString(BlockBlockContact::contactPairHitRatio());
	}

	if (valueName == "numLinkCalls") {
		return StringConverter<int>::convertToString(world->getNumLinkCalls());
	}
	if (valueName == "numHashNodes") {
		return StringConverter<int>::convertToString(world->getNumHashNodes());
	}
	if (valueName == "maxBucketSize") {
		return StringConverter<int>::convertToString(world->getMaxBucketSize());
	}

	if (valueName == "numBodies") {
		return StringConverter<int>::convertToString(world->getNumBodies());
	}
	if (valueName == "numConstraints") {
		return StringConverter<int>::convertToString(world->getNumConstraints());
	}
	if (valueName == "numPoints") {
		return StringConverter<int>::convertToString(world->getNumPoints());
	}
	if (valueName == "energyBody") {
		return StringConverter<float>::convertToString(world->getKernel()->bodyKineticEnergy());
	}
	if (valueName == "energyConnector") {
		return StringConverter<float>::convertToString(world->getKernel()->connectorSpringEnergy());
	}
	if (valueName == "energyTotal") {
		return StringConverter<float>::convertToString(world->getKernel()->totalKineticEnergy());
	}

	return "?";
}

// STUB: WEBSERVICE 0x10073460
void DataModel::loadContent(ContentId contentId)
{
	STUB(0x10073460);
}

// FUNCTION: WEBSERVICE 0x10073de0
void DataModel::Lock::doLock(const DataModel* dataModel)
{
	if (dataModel) {
		mutex = dataModel->mutex;
		lock = new boost::recursive_mutex::scoped_lock(*mutex);
	}
}

// FUNCTION: WEBSERVICE 0x100743f0
DataModel::Lock::~Lock()
{
	delete lock;
}

// FUNCTION: WEBSERVICE 0x10079120
void DataModel::initializeContents()
{
	workspace->setParent(this);
	guiRoot->setParent(this);
	guiHooks->setParent(this);

	localBackpack = shared_from(create<LocalBackpack>());

	starterPackService = shared_from(create<StarterPackService>());

	runService = shared_from(create<RunService>());

	create<Soundscape::SoundService>();

	workspace->setDefaultMouseCommand();

	workspace->setVerbParent(this);

	runService->start();

	startListening(runService, this);

	create<JointsService>();
}

// FUNCTION: WEBSERVICE 0x100793b0
void DataModel::clearContents()
{
	workspace->setMouseCommand(NULL);

	workspace->removeAllChildren();
	workspace->setCamera(NULL);

	starterPackService->removeAllChildren();

	Teams* teams = find<Teams>();
	if (teams) {
		teams->removeAllChildren();
	}

	Lighting* lighting = find<Lighting>();
	if (lighting) {
		lighting->removeAllChildren();
	}

	ScriptContext* context = find<ScriptContext>();
	if (context) {
		context->closeState();
		ScriptContext::propScriptsDisabled.setValue(context, false);
	}
}

// STUB: WEBSERVICE 0x10079a70
void DataModel::save(ContentId contentId)
{
	STUB(0x10079a70);
}

// STUB: WEBSERVICE 0x10079bf0
boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > DataModel::get(ContentId contentId)
{
	STUB(0x10079bf0);
	return boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > >();
}

void DataModel::raiseClose()
{
	Notifier<ServiceProvider, Closing>::raise(Closing());
	event_Closing.fire(this);
}

// FUNCTION: WEBSERVICE 0x1007b450
DataModel::~DataModel()
{
	stopListening(runService, this);
}

// FUNCTION: WEBSERVICE 0x1007bb20
void DataModel::closeDataModel(boost::shared_ptr<DataModel> dataModel, bool joinRunThread)
{
	dataModel->isInitialized = false;

	boost::shared_ptr<RunService> runService;
	{
		DataModel::Lock lock(dataModel);
		runService = dataModel->runService;
	}

	if (runService) {
		runService->endRunThread(joinRunThread);
	}

	DataModel::Lock lock(dataModel);

	dataModel->raiseClose();

	dataModel->clearContents();
	dataModel->removeAllChildren();
	dataModel->clearServices();
}

// FUNCTION: WEBSERVICE 0x1007bcd0
DataModel::DataModel()
	: VerbContainer(NULL), workspace(Creatable<Instance>::create<Workspace>(this)),
	  guiRoot(Creatable<Instance>::create<GuiRoot>()), guiHooks(Creatable<Instance>::create<GuiRoot>()),
	  guiTarget(NULL), forceArrowCursor(true), drawId(0), dirty(false), isInitialized(false),
	  mutex(new boost::recursive_mutex()), timeState(new TimeState())
{
	setName("Level");
}

// FUNCTION: WEBSERVICE 0x1007c120
boost::shared_ptr<DataModel> DataModel::createDataModel()
{
	ContentProvider::singleton().clearContentCache();

	boost::shared_ptr<DataModel> dataModel = Creatable<Instance>::create<DataModel>();

	{
		DataModel::Lock lock(dataModel);
		dataModel->initializeContents();
		dataModel->isInitialized = true;
	}

	return dataModel;
}

// STUB: WEBSERVICE 0x1007c4a0
std::string DataModel::httpGet(std::string url, bool synchronous)
{
	STUB(0x1007c4a0);
	return std::string();
}

// STUB: WEBSERVICE 0x1007c6b0
std::string DataModel::httpPost(std::string url, std::string data, bool synchronous)
{
	STUB(0x1007c6b0);
	return std::string();
}

} // namespace RBX

template RBX::JointsService* RBX::ServiceProvider::create<RBX::JointsService>() const;
template RBX::LocalBackpack* RBX::ServiceProvider::create<RBX::LocalBackpack>() const;

template const RBX::Name& RBX::Name::doDeclare<RBX::sServiceProvider>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sLocalBackpackItem>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sLocalBackpack>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sSnap>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sWeld>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sGlue>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sRotate>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sRotateP>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sRotateV>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sMotor>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sBackpack>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sTeams>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sAccoutrement>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sHat>();
template const RBX::Name& RBX::Name::doDeclare<RBX::Network::sPlayers>();
template void RBX::Name::callDoDeclare<RBX::sServiceProvider>();
template void RBX::Name::callDoDeclare<RBX::sLocalBackpackItem>();
template void RBX::Name::callDoDeclare<RBX::sLocalBackpack>();
template void RBX::Name::callDoDeclare<RBX::sSnap>();
template void RBX::Name::callDoDeclare<RBX::sWeld>();
template void RBX::Name::callDoDeclare<RBX::sGlue>();
template void RBX::Name::callDoDeclare<RBX::sRotate>();
template void RBX::Name::callDoDeclare<RBX::sRotateP>();
template void RBX::Name::callDoDeclare<RBX::sRotateV>();
template void RBX::Name::callDoDeclare<RBX::sMotor>();
template void RBX::Name::callDoDeclare<RBX::sBackpack>();
template void RBX::Name::callDoDeclare<RBX::sTeams>();
template void RBX::Name::callDoDeclare<RBX::sAccoutrement>();
template void RBX::Name::callDoDeclare<RBX::sHat>();
template void RBX::Name::callDoDeclare<RBX::Network::sPlayers>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sDataModel>();
template void RBX::Name::callDoDeclare<RBX::sDataModel>();
template const RBX::FactoryProduct<RBX::LocalBackpackItem, RBX::Widget, RBX::sLocalBackpackItem>::Creator
	RBX::FactoryProduct<RBX::LocalBackpackItem, RBX::Widget, RBX::sLocalBackpackItem>::creator;
template const RBX::FactoryProduct<RBX::LocalBackpack, RBX::RelativePanel, RBX::sLocalBackpack>::Creator
	RBX::FactoryProduct<RBX::LocalBackpack, RBX::RelativePanel, RBX::sLocalBackpack>::creator;
template const RBX::FactoryProduct<RBX::Motor, RBX::AutoJoint, RBX::sMotor>::Creator
	RBX::FactoryProduct<RBX::Motor, RBX::AutoJoint, RBX::sMotor>::creator;
