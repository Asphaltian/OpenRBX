#ifndef TOOL_PARTDRAGTOOL_H
#define TOOL_PARTDRAGTOOL_H

#include "decomp.h"
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
// SIZE 0x40
class PartDragTool : public Named<MouseCommand, sPartDragTool>
{
private:
	std::auto_ptr<RunDragger> runDragger;   // 0x20
	std::auto_ptr<MegaDragger> megaDragger; // 0x24
	G3D::Vector2 downPoint;                 // 0x28
	bool dragging;                          // 0x30
	G3D::Vector3 hitWorld;                  // 0x34

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

DECOMP_SIZE_ASSERT(PartDragTool, 0x40)

// clang-format off
// FUNCTION: WEBSERVICE 0x10126660
// FUNCTION: ROBLOX 0x00662b20
// RBX::PartDragTool::getCursorName
// SYNTHETIC: WEBSERVICE 0x10126690
// SYNTHETIC: ROBLOX 0x00662b50
// RBX::PartDragTool::`scalar deleting destructor'
// clang-format on

} // namespace RBX

#endif // TOOL_PARTDRAGTOOL_H
