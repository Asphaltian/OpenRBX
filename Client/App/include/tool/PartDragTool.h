#ifndef TOOL_PARTDRAGTOOL_H
#define TOOL_PARTDRAGTOOL_H

#include "placeholder.h"
#include "tool/MegaDragger.h"
#include "tool/RunDragger.h"
#include "util/Name.h"
#include "v8datamodel/MouseCommand.h"

#include <G3D/Vector2.h>
#include <G3D/Vector3.h>
#include <memory>
#include <string>

namespace RBX {

class Adorn;
class PartInstance;
class Workspace;

extern char sPartDragTool[];
class PartDragTool : public Named<MouseCommand, sPartDragTool>
{
private:
	std::auto_ptr<RunDragger> runDragger;
	std::auto_ptr<MegaDragger> megaDragger;
	G3D::Vector2 downPoint;
	bool dragging;
	G3D::Vector3 hitWorld;

public:
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual void onMouseIdle(const UIEvent& uiEvent);
	virtual void onMouseMove(const UIEvent& uiEvent);
	virtual void onMouseDelta(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseUp(const UIEvent& uiEvent);
	virtual MouseCommand* onKeyDown(const UIEvent& uiEvent);
	virtual void render3dAdorn(Adorn* adorn);

	virtual const std::string getCursorName() const { return dragging ? "GrabRotateCursor" : "DragCursor"; }

	PartDragTool(PartInstance* mousePart, const G3D::Vector3& hitPointWorld, Workspace* workspace);
	virtual ~PartDragTool();
};

} // namespace RBX

#endif // TOOL_PARTDRAGTOOL_H
