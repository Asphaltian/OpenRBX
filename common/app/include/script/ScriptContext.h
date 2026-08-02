#ifndef SCRIPT_SCRIPTCONTEXT_H
#define SCRIPT_SCRIPTCONTEXT_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"

namespace RBX {

extern char sScriptContext[];
// SIZE 0x168
class ScriptContext : public DescribedCreatable<ScriptContext, Instance, sScriptContext>
{
private:
	undefined m_unk0x0f8[0x168 - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(ScriptContext, 0x168)

} // namespace RBX

#endif // SCRIPT_SCRIPTCONTEXT_H
