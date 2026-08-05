#ifndef V8DATAMODEL_TOOL_H
#define V8DATAMODEL_TOOL_H

#include "decomp.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sTool[];

// SIZE 0x238
class Tool : public Instance
{
public:
	enum ToolState
	{
		NOTHING = 0,
		HAS_HANDLE = 1,
		IN_WORKSPACE = 2,
		IN_CHARACTER = 3,
		HAS_TORSO = 4,
		EQUIPPED = 5,
		ACTIVATED = 6
	};

	// FUNCTION: WEBSERVICE 0x100c9db0
	int getBackendToolState() const { return backendToolState; }

	void setBackendToolState(int value);

private:
	DECOMP_NOINLINE void setBackendToolStateNoReplicate(int value);

	undefined m_unk0x0f8[0x18c - 0x0f8]; // 0x0f8
	int backendToolState;                // 0x18c
	undefined m_unk0x190[0x238 - 0x190]; // 0x190
};

DECOMP_SIZE_ASSERT(Tool, 0x238)

} // namespace RBX

#endif // V8DATAMODEL_TOOL_H
