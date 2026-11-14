#ifndef TOOL_DRAGTOOL_H
#define TOOL_DRAGTOOL_H

#include "placeholder.h"
#include "util/Extents.h"
#include "util/NormalId.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/Ray.h>
#include <G3D/Vector3.h>
#include <vector>

namespace RBX {

class Instance;
class MouseCommand;
class PartInstance;
class UIEvent;
class Workspace;

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

bool FUN_00662530(
	const Extents& extents,
	const CoordinateFrame& location,
	const G3D::Ray& ray,
	G3D::Vector3& hitPoint,
	NormalId& normalId
);
bool FUN_00662700(const Extents& extents, const G3D::Ray& ray, G3D::Vector3& hitPoint, NormalId& normalId);

} // namespace RBX

#endif // TOOL_DRAGTOOL_H
