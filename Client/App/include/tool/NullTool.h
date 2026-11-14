#ifndef TOOL_NULLTOOL_H
#define TOOL_NULLTOOL_H

#include "placeholder.h"
#include "util/Name.h"
#include "v8datamodel/MouseCommand.h"

#include <G3D/Vector3.h>
#include <string>

namespace RBX {

class Workspace;

extern char sNullTool[];
extern char sNewNullTool[];

class NullTool : public Named<MouseCommand, sNullTool>
{
private:
	virtual const std::string getCursorName() const { return "ArrowFarCursor"; }

public:
	NullTool(Workspace* workspace);
	virtual ~NullTool() {}

	virtual MouseCommand* isSticky() const { return new NullTool(workspace); }

	virtual MouseCommand* onMouseUp(const UIEvent& uiEvent)
	{
		releaseCapture();
		return this;
	}
};

class NewNullTool : public Named<MouseCommand, sNewNullTool>
{
private:
	std::string cursor;
	bool hasWaypoint;
	G3D::Vector3 waypoint;

	virtual void onMouseIdle(const UIEvent& uiEvent);
	virtual void onMouseHover(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual const std::string getCursorName() const { return cursor; }
	virtual MouseCommand* isSticky() const { return new NewNullTool(workspace); }
	virtual MouseCommand* onMouseUp(const UIEvent& uiEvent)
	{
		releaseCapture();
		return this;
	}
	virtual bool shouldRender3dAdorn() const { return true; }
	virtual void render3dAdorn(Adorn* adorn);

public:
	NewNullTool(Workspace* workspace);
	virtual ~NewNullTool();
};

} // namespace RBX

#endif // TOOL_NULLTOOL_H
