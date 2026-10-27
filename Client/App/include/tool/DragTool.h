#ifndef TOOL_DRAGTOOL_H
#define TOOL_DRAGTOOL_H

#include "decomp.h"

#include <G3D/Vector3.h>
#include <vector>

namespace RBX {

class Instance;
class MouseCommand;
class PartInstance;
class UIEvent;
class Workspace;

// SIZE 0x1
class DragTool
{
public:
	static MouseCommand* onMouseDown(
		PartInstance* hitPart,
		const G3D::Vector3& hitWorld,
		const std::vector<Instance*>& dragInstances,
		const UIEvent& uiEvent,
		Workspace* workspace
	);
};

DECOMP_SIZE_ASSERT(DragTool, 0x1)

} // namespace RBX

#endif // TOOL_DRAGTOOL_H
