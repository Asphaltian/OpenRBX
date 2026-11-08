#ifndef V8DATAMODEL_TOOLSPART_H
#define V8DATAMODEL_TOOLSPART_H

#include "decomp.h"
#include "util/Events.h"
#include "util/Name.h"
#include "v8datamodel/BrickColor.h"
#include "v8datamodel/MouseCommand.h"

#include <boost/shared_ptr.hpp>
#include <string>

namespace RBX {

class Adorn;
class PartInstance;

extern char sFillTool[];
extern char sDropperTool[];
extern char sModelSetPrimaryPartTool[];

// SIZE 0x28
class PartTool : public MouseCommand
{
private:
	boost::shared_ptr<PartInstance> partInstance; // 0x20

protected:
	virtual void render3dAdorn(Adorn* adorn);
	virtual void onMouseHover(const UIEvent& uiEvent);

public:
	PartTool(Workspace* workspace);

	// SYNTHETIC: ROBLOX 0x005c03d0
	// RBX::ShootTool::~ShootTool

	// SYNTHETIC: ROBLOX 0x005d1d90
	// RBX::ShootTool::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(PartTool, 0x28)

// SIZE 0x1c
class FillToolColor : public Notifier<FillToolColor, BrickColor>
{
private:
	BrickColor color; // 0x18

public:
	FillToolColor() {}

	BrickColor get() const { return color; }

	void set(const BrickColor& value)
	{
		if (color != value) {
			color = value;
			raise(color);
		}
	}

	// SYNTHETIC: WEBSERVICE 0x10129d20
	// SYNTHETIC: ROBLOX 0x005c04f0
	// RBX::FillToolColor::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(FillToolColor, 0x1c)

// SIZE 0x28
class FillTool : public Named<PartTool, sFillTool>
{
public:
	static FillToolColor color;

protected:
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);

	// FUNCTION: ROBLOX 0x005d1c10
	virtual const std::string getCursorName() const { return "FillCursor"; }

public:
	FillTool(Workspace* workspace) : Named<PartTool, sFillTool>(workspace) {}

	// FUNCTION: ROBLOX 0x005d1c40
	virtual MouseCommand* isSticky() const { return new FillTool(workspace); }
};

DECOMP_SIZE_ASSERT(FillTool, 0x28)

// SIZE 0x28
class DropperTool : public Named<PartTool, sDropperTool>
{
protected:
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);

	// FUNCTION: ROBLOX 0x005d1d60
	virtual const std::string getCursorName() const { return "DropperCursor"; }

public:
	DropperTool(Workspace* workspace) : Named<PartTool, sDropperTool>(workspace) {}
};

DECOMP_SIZE_ASSERT(DropperTool, 0x28)

// SIZE 0x28
class ModelSetPrimaryPartTool : public Named<PartTool, sModelSetPrimaryPartTool>
{
protected:
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);

public:
	ModelSetPrimaryPartTool(Workspace* workspace) : Named<PartTool, sModelSetPrimaryPartTool>(workspace) {}
};

DECOMP_SIZE_ASSERT(ModelSetPrimaryPartTool, 0x28)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021e7c0
// `dynamic initializer for 'RBX::FillTool::color''
// SYNTHETIC: WEBSERVICE 0x102255b0
// `dynamic atexit destructor for 'RBX::FillTool::color''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_TOOLSPART_H
