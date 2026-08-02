#include "v8datamodel/Workspace.h"

#include "tool/ToolsArrow.h"
#include "util/Name.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/Flag.h"
#include "v8datamodel/Hopper.h"
#include "v8datamodel/Tool.h"
#include "v8datamodel/UserController.h"

namespace RBX {

char sWorkspace[] = "Workspace";

bool Workspace::showWorldCoord;

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
