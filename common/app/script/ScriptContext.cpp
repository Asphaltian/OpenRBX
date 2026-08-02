#include "script/ScriptContext.h"

#include "util/Name.h"
#include "v8datamodel/DebugSettings.h"
#include "v8datamodel/Stats.h"

namespace RBX {

char sScriptContext[] = "ScriptContext";

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::Stats::sStatsItem>();
template const RBX::Name& RBX::Name::doDeclare<RBX::Stats::sStats>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sDebugSettings>();
template void RBX::Name::callDoDeclare<RBX::Stats::sStatsItem>();
template void RBX::Name::callDoDeclare<RBX::Stats::sStats>();
template void RBX::Name::callDoDeclare<RBX::sDebugSettings>();
