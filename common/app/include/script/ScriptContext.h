#ifndef SCRIPT_SCRIPTCONTEXT_H
#define SCRIPT_SCRIPTCONTEXT_H

#include "decomp.h"
#include "util/Handle.h"
#include "util/RunStateOwner.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

namespace RBX {

extern char sScriptContext[];
// SIZE 0x168
class ScriptContext : public DescribedCreatable<ScriptContext, Instance, sScriptContext>,
					  public Listener<RunService, RunTransition>,
					  public Listener<RunService, Heartbeat>,
					  public Service
{
protected:
	virtual void onEvent(const RunService* source, RunTransition event);
	virtual void onEvent(const RunService* source, Heartbeat event);

private:
	undefined m_unk0x100[0x168 - 0x100]; // 0x100
};

DECOMP_SIZE_ASSERT(ScriptContext, 0x168)

} // namespace RBX

#endif // SCRIPT_SCRIPTCONTEXT_H
