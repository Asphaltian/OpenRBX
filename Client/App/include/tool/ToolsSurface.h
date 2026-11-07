#ifndef TOOL_TOOLSSURFACE_H
#define TOOL_TOOLSSURFACE_H

#include "decomp.h"
#include "util/Name.h"
#include "v8datamodel/MouseCommand.h"

#include <boost/shared_ptr.hpp>
#include <string>

namespace RBX {

class Adorn;
class PartInstance;
class Workspace;

extern char sHammerTool[];

// SIZE 0x28
class HammerTool : public Named<MouseCommand, sHammerTool>
{
private:
	boost::shared_ptr<PartInstance> part; // 0x20

	virtual void render3dAdorn(Adorn* adorn);
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual void onMouseIdle(const UIEvent& uiEvent);
	virtual const std::string getCursorName() const;

public:
	HammerTool(Workspace* workspace) : Named<MouseCommand, sHammerTool>(workspace) {}

	// FUNCTION: ROBLOX 0x005d2170
	virtual MouseCommand* isSticky() const { return new HammerTool(workspace); }

	// SYNTHETIC: ROBLOX 0x005d20f0
	// RBX::HammerTool::~HammerTool

	// SYNTHETIC: ROBLOX 0x005d2200
	// RBX::HammerTool::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(HammerTool, 0x28)

} // namespace RBX

#endif // TOOL_TOOLSSURFACE_H
