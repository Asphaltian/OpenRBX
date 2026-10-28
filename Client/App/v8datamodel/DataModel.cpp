#include "v8datamodel/DataModel.h"

#include "network/Players.h"
#include "reflection/reflection.h"
#include "reflection/signal.h"
#include "util/ContentProvider.h"
#include "util/Name.h"
#include "v8datamodel/Accoutrement.h"
#include "v8datamodel/Backpack.h"
#include "v8datamodel/JointInstance.h"
#include "v8datamodel/JointsService.h"
#include "v8datamodel/LocalBackpack.h"
#include "v8datamodel/Teams.h"
#include "v8tree/Service.h"

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

// STUB: WEBSERVICE 0x10071800
void DataModel::close()
{
	STUB(0x10071800);
}

// STUB: WEBSERVICE 0x10072640
float DataModel::step(float distributedGameTime)
{
	STUB(0x10072640);

	return 0.0f;
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

// STUB: WEBSERVICE 0x100793b0
void DataModel::clearContents()
{
	STUB(0x100793b0);
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

// STUB: WEBSERVICE 0x1007bb20
void DataModel::closeDataModel(boost::shared_ptr<DataModel> dataModel, bool joinRunThread)
{
	STUB(0x1007bb20);
}

// STUB: WEBSERVICE 0x1007c120
boost::shared_ptr<DataModel> DataModel::createDataModel()
{
	STUB(0x1007c120);
	return boost::shared_ptr<DataModel>();
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
