#ifndef V8DATAMODEL_WORKSPACE_H
#define V8DATAMODEL_WORKSPACE_H

#include "decomp.h"

namespace RBX {

// SIZE 0x394
class Workspace
{
public:
	static bool showWorldCoord;

private:
	undefined m_unk0x000[0x394 - 0x000]; // 0x000
};

DECOMP_SIZE_ASSERT(Workspace, 0x394)

} // namespace RBX

#endif // V8DATAMODEL_WORKSPACE_H
