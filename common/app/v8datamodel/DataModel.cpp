#include "v8datamodel/DataModel.h"

#include "network/Players.h"
#include "util/Name.h"
#include "v8datamodel/Accoutrement.h"
#include "v8datamodel/Backpack.h"
#include "v8datamodel/JointInstance.h"
#include "v8datamodel/LocalBackpack.h"
#include "v8datamodel/Teams.h"
#include "v8tree/Service.h"

namespace RBX {

const char sDataModel[] = "DataModel";

// STUB: WEBSERVICE 0x1000cd40
DataModel::Lock::Lock(boost::shared_ptr<const DataModel> dataModel)
{
	STUB(0x1000cd40);
}

// STUB: WEBSERVICE 0x10072640
float DataModel::step(float distributedGameTime)
{
	STUB(0x10072640);

	return 0.0f;
}

// STUB: WEBSERVICE 0x100743f0
DataModel::Lock::~Lock()
{
	STUB(0x100743f0);
}

} // namespace RBX

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
