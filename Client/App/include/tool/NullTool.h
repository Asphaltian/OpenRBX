#ifndef TOOL_NULLTOOL_H
#define TOOL_NULLTOOL_H

#include "decomp.h"
#include "util/Name.h"
#include "v8datamodel/MouseCommand.h"

#include <G3D/Vector3.h>
#include <string>

namespace RBX {

class Workspace;

extern char sNewNullTool[];

// SIZE 0x4c
class NewNullTool : public Named<MouseCommand, sNewNullTool>
{
private:
	std::string cursor;    // 0x20
	bool hasWaypoint;      // 0x3c
	G3D::Vector3 waypoint; // 0x40

	virtual void onMouseIdle(const UIEvent& uiEvent);
	virtual void onMouseHover(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	// FUNCTION: WEBSERVICE 0x100c5060
	virtual const std::string getCursorName() const { return cursor; }
	// FUNCTION: WEBSERVICE 0x100c52e0
	virtual MouseCommand* isSticky() const { return new NewNullTool(workspace); }
	// FUNCTION: WEBSERVICE 0x100c5090
	virtual MouseCommand* onMouseUp(const UIEvent& uiEvent)
	{
		releaseCapture();
		return this;
	}
	// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
	virtual bool shouldRender3dAdorn() const { return true; }
	virtual void render3dAdorn(Adorn* adorn);

public:
	NewNullTool(Workspace* workspace);
	virtual ~NewNullTool();

	// SYNTHETIC: WEBSERVICE 0x100c52b0
	// RBX::NewNullTool::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(NewNullTool, 0x4c)

} // namespace RBX

#endif // TOOL_NULLTOOL_H
