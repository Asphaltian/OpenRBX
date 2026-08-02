#include "ThumbnailGenerator.h"

#include "script/Script.h"
#include "script/ScriptContext.h"
#include "util/Name.h"
#include "util/RunStateOwner.h"
#include "v8datamodel/Lighting.h"
#include "v8datamodel/ModelInstance.h"
#include "v8datamodel/Selection.h"
#include "v8datamodel/Workspace.h"

const char sThumbnailGenerator[] = "ThumbnailGenerator";

template void RBX::Name::callDoDeclare<sThumbnailGenerator>();
template const RBX::Name& RBX::Name::doDeclare<sThumbnailGenerator>();
template void RBX::Name::callDoDeclare<RBX::sLighting>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sLighting>();
template void RBX::Name::callDoDeclare<RBX::sRunService>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sRunService>();
template void RBX::Name::callDoDeclare<RBX::sModel>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sModel>();
template void RBX::Name::callDoDeclare<RBX::sSelection>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sSelection>();
template void RBX::Name::callDoDeclare<RBX::sScriptContext>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sScriptContext>();
template void RBX::Name::callDoDeclare<RBX::sScript>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sScript>();
template void RBX::Name::callDoDeclare<RBX::sLocalScript>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sLocalScript>();
template void RBX::Name::callDoDeclare<RBX::sWorkspace>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sWorkspace>();
