#ifndef TOOL_GROUPDRAGTOOL_H
#define TOOL_GROUPDRAGTOOL_H

#include "placeholder.h"
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
class GroupDragTool : public Named<MouseCommand, sGroupDragTool>
{
private:
	std::auto_ptr<MegaDragger> megaDragger;
	G3D::Vector2 downPoint;
	bool dragging;
	G3D::Vector3 lastHit;

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

} // namespace RBX

#endif // TOOL_GROUPDRAGTOOL_H
