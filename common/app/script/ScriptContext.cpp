#include "script/ScriptContext.h"

#include "script/LuaAtomicClasses.h"
#include "util/Name.h"
#include "v8datamodel/DebugSettings.h"
#include "v8datamodel/Stats.h"

namespace RBX {

char sScriptContext[] = "ScriptContext";

// STUB: WEBSERVICE 0x1005eb60
void ScriptContext::onEvent(const RunService* source, RunTransition event)
{
	STUB(0x1005eb60);
}

// FUNCTION: WEBSERVICE 0x1005f880
void Lua::CoordinateFrameBridge::pushCoordinateFrame(lua_State* L, G3D::CoordinateFrame value)
{
	pushNewObject(L, value);
}

// FUNCTION: WEBSERVICE 0x1005f8c0
void Lua::Vector3Bridge::pushVector3(lua_State* L, G3D::Vector3 value)
{
	pushNewObject(L, value);
}

// FUNCTION: WEBSERVICE 0x1005f910
void Lua::Color3Bridge::pushColor3(lua_State* L, G3D::Color3 value)
{
	pushNewObject(L, value);
}

// STUB: WEBSERVICE 0x10068880
void ScriptContext::onEvent(const RunService* source, Heartbeat event)
{
	STUB(0x10068880);
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::Stats::sStatsItem>();
template const RBX::Name& RBX::Name::doDeclare<RBX::Stats::sStats>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sDebugSettings>();
template void RBX::Name::callDoDeclare<RBX::Stats::sStatsItem>();
template void RBX::Name::callDoDeclare<RBX::Stats::sStats>();
template void RBX::Name::callDoDeclare<RBX::sDebugSettings>();

template class RBX::FactoryProduct<RBX::ScriptContext, RBX::Instance, RBX::sScriptContext>;
template class RBX::Reflection::Described<
	RBX::ScriptContext,
	RBX::sScriptContext,
	RBX::FactoryProduct<RBX::ScriptContext, RBX::Instance, RBX::sScriptContext> >;
template class RBX::DescribedCreatable<RBX::ScriptContext, RBX::Instance, RBX::sScriptContext>;
