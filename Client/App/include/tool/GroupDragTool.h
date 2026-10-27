#ifndef TOOL_GROUPDRAGTOOL_H
#define TOOL_GROUPDRAGTOOL_H

#include "decomp.h"
#include "tool/MegaDragger.h"
#include "util/Name.h"
#include "v8datamodel/MouseCommand.h"

#include <G3D/Vector2.h>
#include <G3D/Vector3.h>
#include <boost/weak_ptr.hpp>
#include <memory>
#include <string>
#include <vector>

namespace RBX {

class PartInstance;
class Workspace;

extern char sGroupDragTool[];
// SIZE 0x3c
class GroupDragTool : public Named<MouseCommand, sGroupDragTool>
{
private:
	std::auto_ptr<MegaDragger> megaDragger; // 0x20
	G3D::Vector2 downPoint;                 // 0x24
	bool dragging;                          // 0x2c
	G3D::Vector3 lastHit;                   // 0x30

public:
	virtual MouseCommand* onKeyDown(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual void onMouseIdle(const UIEvent& uiEvent);
	virtual void onMouseMove(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseUp(const UIEvent& uiEvent);

	virtual const std::string getCursorName() const { return dragging ? "GrabRotateCursor" : "DragCursor"; }

	GroupDragTool(
		PartInstance* mousePart,
		const G3D::Vector3& hitPointWorld,
		const std::vector<boost::weak_ptr<PartInstance> >& partArray,
		Workspace* workspace
	);
	virtual ~GroupDragTool();
};

DECOMP_SIZE_ASSERT(GroupDragTool, 0x3c)

// clang-format off
// FUNCTION: WEBSERVICE 0x10125ff0
// RBX::GroupDragTool::getCursorName
// SYNTHETIC: WEBSERVICE 0x10126310
// RBX::GroupDragTool::`scalar deleting destructor'
// clang-format on

} // namespace RBX

#endif // TOOL_GROUPDRAGTOOL_H
