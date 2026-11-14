#ifndef TOOL_TOOLSSURFACE_H
#define TOOL_TOOLSSURFACE_H

#include "tool/ToolsArrow.h"
#include "placeholder.h"
#include "util/Name.h"
#include "util/NormalId.h"
#include "util/PV.h"
#include "util/SoundWorld.h"
#include "v8datamodel/MouseCommand.h"

#include <G3D/Vector2int16.h>
#include <G3D/Vector3.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>

namespace RBX {

class Adorn;
class Instance;
class PartInstance;
class PVInstance;
class Surface;
class Workspace;
class XmlState;

extern char sFlatTool[];
extern char sGlueTool[];
extern char sWeldTool[];
extern char sStudsTool[];
extern char sInletTool[];
extern char sHingeTool[];
extern char sRightMotorTool[];
extern char sLeftMotorTool[];
extern char sOscillateMotorTool[];
extern char sModelSetFrontTool[];
extern char sAnchorTool[];
extern char sLockTool[];
extern char sResizeTool[];
extern char sSlingshotTool[];
extern char sRocketTool[];
extern char sHammerTool[];
extern char sLaserTool[];
extern char sCloneTool[];
extern char sGameTool[];

class SurfaceTool : public MouseCommand
{
protected:
	boost::shared_ptr<PartInstance> part;
	Surface* surface;
	int surfaceId;
	std::string m_unk0x30;

	virtual void render3dAdorn(Adorn* adorn);
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual void onMouseHover(const UIEvent& uiEvent);

	virtual void VTable0x40(Surface* surface) = 0;

public:
	SurfaceTool(Workspace* workspace);
};

class FlatTool : public Named<SurfaceTool, sFlatTool>
{
private:
	virtual void VTable0x40(Surface* surface);

public:
	FlatTool(Workspace* workspace) : Named<SurfaceTool, sFlatTool>(workspace) {}

	virtual MouseCommand* isSticky() const { return new FlatTool(workspace); }

	virtual const std::string getCursorName() const { return "FlatCursor"; }
};

class GlueTool : public Named<SurfaceTool, sGlueTool>
{
private:
	virtual void VTable0x40(Surface* surface);

public:
	GlueTool(Workspace* workspace) : Named<SurfaceTool, sGlueTool>(workspace) {}

	virtual MouseCommand* isSticky() const { return new GlueTool(workspace); }

	virtual const std::string getCursorName() const { return "GlueCursor"; }
};

class WeldTool : public Named<SurfaceTool, sWeldTool>
{
private:
	virtual void VTable0x40(Surface* surface);

public:
	WeldTool(Workspace* workspace) : Named<SurfaceTool, sWeldTool>(workspace) {}

	virtual MouseCommand* isSticky() const { return new WeldTool(workspace); }

	virtual const std::string getCursorName() const { return "WeldCursor"; }
};

class StudsTool : public Named<SurfaceTool, sStudsTool>
{
private:
	virtual void VTable0x40(Surface* surface);

public:
	StudsTool(Workspace* workspace) : Named<SurfaceTool, sStudsTool>(workspace) {}

	virtual MouseCommand* isSticky() const { return new StudsTool(workspace); }

	virtual const std::string getCursorName() const { return "StudsCursor"; }
};

class InletTool : public Named<SurfaceTool, sInletTool>
{
private:
	virtual void VTable0x40(Surface* surface);

public:
	InletTool(Workspace* workspace) : Named<SurfaceTool, sInletTool>(workspace) {}

	virtual MouseCommand* isSticky() const { return new InletTool(workspace); }

	virtual const std::string getCursorName() const { return "InletCursor"; }
};

class HingeTool : public Named<SurfaceTool, sHingeTool>
{
private:
	virtual void VTable0x40(Surface* surface);

public:
	HingeTool(Workspace* workspace) : Named<SurfaceTool, sHingeTool>(workspace) {}

	virtual MouseCommand* isSticky() const { return new HingeTool(workspace); }

	virtual const std::string getCursorName() const { return "HingeCursor"; }
};

class RightMotorTool : public Named<SurfaceTool, sRightMotorTool>
{
private:
	virtual void VTable0x40(Surface* surface);

public:
	RightMotorTool(Workspace* workspace) : Named<SurfaceTool, sRightMotorTool>(workspace) {}

	virtual const std::string getCursorName() const { return "MotorCursor"; }
};

class LeftMotorTool : public Named<SurfaceTool, sLeftMotorTool>
{
private:
	virtual void VTable0x40(Surface* surface);

public:
	LeftMotorTool(Workspace* workspace) : Named<SurfaceTool, sLeftMotorTool>(workspace) {}

	virtual const std::string getCursorName() const { return "MotorCursor"; }
};

class OscillateMotorTool : public Named<SurfaceTool, sOscillateMotorTool>
{
private:
	virtual void VTable0x40(Surface* surface);

public:
	OscillateMotorTool(Workspace* workspace) : Named<SurfaceTool, sOscillateMotorTool>(workspace) {}

	virtual const std::string getCursorName() const { return "MotorCursor"; }
};

class ModelSetFrontTool : public Named<SurfaceTool, sModelSetFrontTool>
{
private:
	virtual void VTable0x40(Surface* surface);

public:
	ModelSetFrontTool(Workspace* workspace) : Named<SurfaceTool, sModelSetFrontTool>(workspace) {}

	virtual const std::string getCursorName() const { return "DirectionCursor"; }
};

class ModelTool : public MouseCommand
{
protected:
	boost::shared_ptr<Instance> m_unk0x20;

	virtual void render3dAdorn(Adorn* adorn);
	virtual void onMouseHover(const UIEvent& uiEvent);

public:
	ModelTool(Workspace* workspace);
};

class LockTool : public Named<ModelTool, sLockTool>
{
private:
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseUp(const UIEvent& uiEvent) { return this; }
	virtual const std::string getCursorName() const;

public:
	LockTool(Workspace* workspace) : Named<ModelTool, sLockTool>(workspace) {}
};

class AnchorTool : public Named<ModelTool, sAnchorTool>
{
private:
	bool m_unk0x28;

	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseUp(const UIEvent& uiEvent) { return this; }
	virtual void onMouseHover(const UIEvent& uiEvent);
	virtual const std::string getCursorName() const;

public:
	AnchorTool(Workspace* workspace) : Named<ModelTool, sAnchorTool>(workspace), m_unk0x28(false) {}
};

class ResizeTool : public Named<ArrowToolBase, sResizeTool>
{
private:
	boost::weak_ptr<PVInstance> m_unk0x24;
	bool m_unk0x2c;
	NormalId m_unk0x30;
	G3D::Vector3 m_unk0x34;
	G3D::Vector2int16 m_unk0x40;
	int m_unk0x44;
	int m_unk0x48;

	void FUN_0063af80(int p_unk0x04);
	void FUN_0063b240(const UIEvent& uiEvent);

	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual void onMouseHover(const UIEvent& uiEvent);
	virtual void onMouseMove(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseUp(const UIEvent& uiEvent);
	virtual const std::string getCursorName() const;

public:
	ResizeTool(Workspace* workspace)
		: Named<ArrowToolBase, sResizeTool>(workspace), m_unk0x2c(false), m_unk0x44(0), m_unk0x48(0)
	{
	}

	virtual MouseCommand* isSticky() const { return new ResizeTool(workspace); }

	virtual void VTable0x40(XmlState* state);

	virtual void render2d(Adorn* adorn);
	virtual void render3dAdorn(Adorn* adorn);
};

class ShootTool : public MouseCommand
{
protected:
	boost::shared_ptr<PartInstance> m_unk0x20;
	bool m_unk0x28;
	int m_unk0x2c;
	int m_unk0x30;

	void FUN_0063ab10(const UIEvent& uiEvent);

	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual void onMouseIdle(const UIEvent& uiEvent);
	virtual void onMouseMove(const UIEvent& uiEvent);

	virtual MouseCommand* onMouseUp(const UIEvent& uiEvent)
	{
		releaseCapture();
		return this;
	}

	virtual const std::string getCursorName() const;

public:
	ShootTool(Workspace* workspace, bool p_unk0x28, int p_unk0x2c);

	virtual boost::shared_ptr<PartInstance> VTable0x40() = 0;
	virtual PV VTable0x44(const UIEvent& uiEvent, const CoordinateFrame& cframe) = 0;
	virtual SoundType VTable0x48() = 0;
};

class SlingshotTool : public Named<ShootTool, sSlingshotTool>
{
private:
	virtual boost::shared_ptr<PartInstance> VTable0x40();
	virtual PV VTable0x44(const UIEvent& uiEvent, const CoordinateFrame& cframe);
	virtual SoundType VTable0x48() { return RUBBERBAND_SOUND; }

public:
	SlingshotTool(Workspace* workspace) : Named<ShootTool, sSlingshotTool>(workspace, true, 5) {}

	virtual MouseCommand* isSticky() const { return new SlingshotTool(workspace); }
};

class RocketTool : public Named<ShootTool, sRocketTool>
{
private:
	virtual boost::shared_ptr<PartInstance> VTable0x40();
	virtual PV VTable0x44(const UIEvent& uiEvent, const CoordinateFrame& cframe);
	virtual SoundType VTable0x48() { return PING_SOUND; }

public:
	RocketTool(Workspace* workspace) : Named<ShootTool, sRocketTool>(workspace, false, 90) {}

	virtual MouseCommand* isSticky() const { return new RocketTool(workspace); }
};

class HammerTool : public Named<MouseCommand, sHammerTool>
{
private:
	boost::shared_ptr<PartInstance> part;

	virtual void render3dAdorn(Adorn* adorn);
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual void onMouseIdle(const UIEvent& uiEvent);
	virtual const std::string getCursorName() const;

public:
	HammerTool(Workspace* workspace) : Named<MouseCommand, sHammerTool>(workspace) {}

	virtual MouseCommand* isSticky() const { return new HammerTool(workspace); }
};

class LaserTool : public Named<MouseCommand, sLaserTool>
{
private:
	int m_unk0x20;
	G3D::Vector3 m_unk0x24;
	G3D::Vector3 m_unk0x30;

	virtual void render3dAdorn(Adorn* adorn);
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);

public:
	LaserTool(Workspace* workspace) : Named<MouseCommand, sLaserTool>(workspace), m_unk0x20(0) {}

	virtual const std::string getCursorName() const { return "GunCursor"; }

	virtual MouseCommand* isSticky() const { return new LaserTool(workspace); }
};

class CloneTool : public Named<MouseCommand, sCloneTool>
{
private:
	boost::shared_ptr<PartInstance> part;

	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual void onMouseIdle(const UIEvent& uiEvent);
	virtual const std::string getCursorName() const;

public:
	CloneTool(Workspace* workspace);

	virtual MouseCommand* isSticky() const { return new CloneTool(workspace); }
};

class GameTool : public Named<MouseCommand, sGameTool>
{
private:
	std::string cursor;

	bool FUN_0063c2c0(PartInstance* part, const G3D::Vector3& hitWorld) const;

	virtual void onMouseIdle(const UIEvent& uiEvent) { onMouseHover(uiEvent); }
	virtual void onMouseHover(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);

	virtual const std::string getCursorName() const { return cursor; }

public:
	GameTool(Workspace* workspace);
	virtual ~GameTool();

	virtual MouseCommand* isSticky() const { return new GameTool(workspace); }
};

} // namespace RBX

#endif // TOOL_TOOLSSURFACE_H
