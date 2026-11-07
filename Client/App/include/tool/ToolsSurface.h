#ifndef TOOL_TOOLSSURFACE_H
#define TOOL_TOOLSSURFACE_H

#include "decomp.h"
#include "util/Name.h"
#include "v8datamodel/MouseCommand.h"

#include <G3D/Vector3.h>
#include <boost/shared_ptr.hpp>
#include <string>

namespace RBX {

class Adorn;
class PartInstance;
class Workspace;

extern char sHammerTool[];
extern char sLaserTool[];

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

// SIZE 0x3c
class LaserTool : public Named<MouseCommand, sLaserTool>
{
private:
	int m_unk0x20;          // 0x20
	G3D::Vector3 m_unk0x24; // 0x24
	G3D::Vector3 m_unk0x30; // 0x30

	virtual void render3dAdorn(Adorn* adorn);
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);

public:
	LaserTool(Workspace* workspace) : Named<MouseCommand, sLaserTool>(workspace), m_unk0x20(0) {}

	// FUNCTION: ROBLOX 0x005d2530
	virtual const std::string getCursorName() const { return "GunCursor"; }

	// FUNCTION: ROBLOX 0x005d2560
	virtual MouseCommand* isSticky() const { return new LaserTool(workspace); }
};

DECOMP_SIZE_ASSERT(LaserTool, 0x3c)

} // namespace RBX

#endif // TOOL_TOOLSSURFACE_H
