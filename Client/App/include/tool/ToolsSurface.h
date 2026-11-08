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
class Instance;
class PartInstance;
class Surface;
class Workspace;

extern char sFlatTool[];
extern char sGlueTool[];
extern char sWeldTool[];
extern char sStudsTool[];
extern char sInletTool[];
extern char sHingeTool[];
extern char sRightMotorTool[];
extern char sLeftMotorTool[];
extern char sOscillateMotorTool[];
extern char sAnchorTool[];
extern char sLockTool[];
extern char sHammerTool[];
extern char sLaserTool[];
extern char sCloneTool[];
extern char sGameTool[];

// SIZE 0x4c
class SurfaceTool : public MouseCommand
{
protected:
	boost::shared_ptr<PartInstance> part; // 0x20
	Surface* surface;                     // 0x28
	int surfaceId;                        // 0x2c
	std::string m_unk0x30;                // 0x30

	virtual void render3dAdorn(Adorn* adorn);
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual void onMouseHover(const UIEvent& uiEvent);

	virtual void VTable0x40(Surface* surface) = 0; // vtable+0x40

public:
	SurfaceTool(Workspace* workspace);

	// SYNTHETIC: ROBLOX 0x005d0d80
	// RBX::SurfaceTool::~SurfaceTool

	// SYNTHETIC: ROBLOX 0x005d1580
	// RBX::SurfaceTool::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(SurfaceTool, 0x4c)

// SIZE 0x4c
class FlatTool : public Named<SurfaceTool, sFlatTool>
{
private:
	virtual void VTable0x40(Surface* surface);

public:
	FlatTool(Workspace* workspace) : Named<SurfaceTool, sFlatTool>(workspace) {}

	// FUNCTION: ROBLOX 0x005d0e70
	virtual MouseCommand* isSticky() const { return new FlatTool(workspace); }

	// FUNCTION: ROBLOX 0x005d0e40
	virtual const std::string getCursorName() const { return "FlatCursor"; }
};

DECOMP_SIZE_ASSERT(FlatTool, 0x4c)

// SIZE 0x4c
class GlueTool : public Named<SurfaceTool, sGlueTool>
{
private:
	virtual void VTable0x40(Surface* surface);

public:
	GlueTool(Workspace* workspace) : Named<SurfaceTool, sGlueTool>(workspace) {}

	// FUNCTION: ROBLOX 0x005d0fc0
	virtual MouseCommand* isSticky() const { return new GlueTool(workspace); }

	// FUNCTION: ROBLOX 0x005d0f90
	virtual const std::string getCursorName() const { return "GlueCursor"; }
};

DECOMP_SIZE_ASSERT(GlueTool, 0x4c)

// SIZE 0x4c
class WeldTool : public Named<SurfaceTool, sWeldTool>
{
private:
	virtual void VTable0x40(Surface* surface);

public:
	WeldTool(Workspace* workspace) : Named<SurfaceTool, sWeldTool>(workspace) {}

	// FUNCTION: ROBLOX 0x005d1110
	virtual MouseCommand* isSticky() const { return new WeldTool(workspace); }

	// FUNCTION: ROBLOX 0x005d10e0
	virtual const std::string getCursorName() const { return "WeldCursor"; }
};

DECOMP_SIZE_ASSERT(WeldTool, 0x4c)

// SIZE 0x4c
class StudsTool : public Named<SurfaceTool, sStudsTool>
{
private:
	virtual void VTable0x40(Surface* surface);

public:
	StudsTool(Workspace* workspace) : Named<SurfaceTool, sStudsTool>(workspace) {}

	// FUNCTION: ROBLOX 0x005d1280
	virtual MouseCommand* isSticky() const { return new StudsTool(workspace); }

	// FUNCTION: ROBLOX 0x005d1250
	virtual const std::string getCursorName() const { return "StudsCursor"; }
};

DECOMP_SIZE_ASSERT(StudsTool, 0x4c)

// SIZE 0x4c
class InletTool : public Named<SurfaceTool, sInletTool>
{
private:
	virtual void VTable0x40(Surface* surface);

public:
	InletTool(Workspace* workspace) : Named<SurfaceTool, sInletTool>(workspace) {}

	// FUNCTION: ROBLOX 0x005d13d0
	virtual MouseCommand* isSticky() const { return new InletTool(workspace); }

	// FUNCTION: ROBLOX 0x005d13a0
	virtual const std::string getCursorName() const { return "InletCursor"; }
};

DECOMP_SIZE_ASSERT(InletTool, 0x4c)

// SIZE 0x4c
class HingeTool : public Named<SurfaceTool, sHingeTool>
{
private:
	virtual void VTable0x40(Surface* surface);

public:
	HingeTool(Workspace* workspace) : Named<SurfaceTool, sHingeTool>(workspace) {}

	// FUNCTION: ROBLOX 0x005d1520
	virtual MouseCommand* isSticky() const { return new HingeTool(workspace); }

	// FUNCTION: ROBLOX 0x005d14f0
	virtual const std::string getCursorName() const { return "HingeCursor"; }
};

DECOMP_SIZE_ASSERT(HingeTool, 0x4c)

// SIZE 0x4c
class RightMotorTool : public Named<SurfaceTool, sRightMotorTool>
{
private:
	virtual void VTable0x40(Surface* surface);

public:
	RightMotorTool(Workspace* workspace) : Named<SurfaceTool, sRightMotorTool>(workspace) {}

	// FUNCTION: ROBLOX 0x005d17e0
	virtual const std::string getCursorName() const { return "MotorCursor"; }
};

DECOMP_SIZE_ASSERT(RightMotorTool, 0x4c)

// SIZE 0x4c
class LeftMotorTool : public Named<SurfaceTool, sLeftMotorTool>
{
private:
	virtual void VTable0x40(Surface* surface);

public:
	LeftMotorTool(Workspace* workspace) : Named<SurfaceTool, sLeftMotorTool>(workspace) {}

	virtual const std::string getCursorName() const { return "MotorCursor"; }
};

DECOMP_SIZE_ASSERT(LeftMotorTool, 0x4c)

// SIZE 0x4c
class OscillateMotorTool : public Named<SurfaceTool, sOscillateMotorTool>
{
private:
	virtual void VTable0x40(Surface* surface);

public:
	OscillateMotorTool(Workspace* workspace) : Named<SurfaceTool, sOscillateMotorTool>(workspace) {}

	virtual const std::string getCursorName() const { return "MotorCursor"; }
};

DECOMP_SIZE_ASSERT(OscillateMotorTool, 0x4c)

// SIZE 0x28
class ModelTool : public MouseCommand
{
protected:
	boost::shared_ptr<Instance> m_unk0x20; // 0x20

	virtual void render3dAdorn(Adorn* adorn);
	virtual void onMouseHover(const UIEvent& uiEvent);

public:
	ModelTool(Workspace* workspace);
};

DECOMP_SIZE_ASSERT(ModelTool, 0x28)

// SIZE 0x28
class LockTool : public Named<ModelTool, sLockTool>
{
private:
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseUp(const UIEvent& uiEvent) { return this; }
	virtual const std::string getCursorName() const;

public:
	LockTool(Workspace* workspace) : Named<ModelTool, sLockTool>(workspace) {}
};

DECOMP_SIZE_ASSERT(LockTool, 0x28)

// SIZE 0x2c
class AnchorTool : public Named<ModelTool, sAnchorTool>
{
private:
	bool m_unk0x28; // 0x28

	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseUp(const UIEvent& uiEvent) { return this; }
	virtual void onMouseHover(const UIEvent& uiEvent);
	virtual const std::string getCursorName() const;

public:
	AnchorTool(Workspace* workspace) : Named<ModelTool, sAnchorTool>(workspace), m_unk0x28(false) {}
};

DECOMP_SIZE_ASSERT(AnchorTool, 0x2c)

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
	// RBX::CloneTool::~CloneTool

	// SYNTHETIC: ROBLOX 0x005d2200
	// RBX::CloneTool::`scalar deleting destructor'
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

// SIZE 0x28
class CloneTool : public Named<MouseCommand, sCloneTool>
{
private:
	boost::shared_ptr<PartInstance> part; // 0x20

	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual void onMouseIdle(const UIEvent& uiEvent);
	virtual const std::string getCursorName() const;

public:
	CloneTool(Workspace* workspace);

	// FUNCTION: ROBLOX 0x0063bff0
	virtual MouseCommand* isSticky() const { return new CloneTool(workspace); }
};

DECOMP_SIZE_ASSERT(CloneTool, 0x28)

// SIZE 0x3c
class GameTool : public Named<MouseCommand, sGameTool>
{
private:
	std::string cursor; // 0x20

	bool FUN_0063c2c0(PartInstance* part, const G3D::Vector3& hitWorld) const;

	virtual void onMouseIdle(const UIEvent& uiEvent) { onMouseHover(uiEvent); }
	virtual void onMouseHover(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);

	// FUNCTION: ROBLOX 0x006227f0 FOLDED
	virtual const std::string getCursorName() const { return cursor; }

public:
	GameTool(Workspace* workspace);
	virtual ~GameTool();

	// FUNCTION: ROBLOX 0x0063c540
	virtual MouseCommand* isSticky() const { return new GameTool(workspace); }

	// SYNTHETIC: ROBLOX 0x0063c520
	// RBX::GameTool::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(GameTool, 0x3c)

} // namespace RBX

#endif // TOOL_TOOLSSURFACE_H
