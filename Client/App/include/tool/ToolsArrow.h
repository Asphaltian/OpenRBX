#ifndef TOOL_TOOLSARROW_H
#define TOOL_TOOLSARROW_H

#include "decomp.h"
#include "util/Extents.h"
#include "util/Name.h"
#include "util/NormalId.h"
#include "v8datamodel/MouseCommand.h"
#include "v8datamodel/Selection.h"
#include "v8tree/Service.h"
#include "v8tree/Verb.h"

#include <G3D/Ray.h>
#include <G3D/Rect2D.h>
#include <G3D/Vector2int16.h>
#include <G3D/Vector3.h>
#include <memory>
#include <set>
#include <string>

namespace RBX {

class Workspace;

extern char sBoxSelectCommand[];
// SIZE 0x44
class BoxSelectCommand : public Named<MouseCommand, sBoxSelectCommand>
{
private:
	ServiceClient<Selection> selection;     // 0x20
	bool reverseSelecting;                  // 0x2c
	G3D::Vector2int16 mouseDownView;        // 0x2e
	G3D::Vector2int16 mouseCurrentView;     // 0x32
	std::set<Instance*> previousItemsInBox; // 0x38

	void getMouseInstances(std::set<Instance*>& instances, const UIEvent& uiEvent, G3D::Rect2D& selectBox);
	void selectAnd(const std::set<Instance*>& newItemsInBox);
	void selectReverse(const std::set<Instance*>& newItemsInBox);

public:
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual void onMouseMove(const UIEvent& uiEvent);
	virtual void render2d(Adorn* adorn);

	BoxSelectCommand(Workspace* workspace);
};

DECOMP_SIZE_ASSERT(BoxSelectCommand, 0x44)

// SYNTHETIC: WEBSERVICE 0x100c0110
// SYNTHETIC: ROBLOX 0x00621b30
// RBX::BoxSelectCommand::~BoxSelectCommand

// SYNTHETIC: WEBSERVICE 0x100c01c0
// SYNTHETIC: ROBLOX 0x00621be0
// RBX::BoxSelectCommand::`scalar deleting destructor'

extern char sArrowTool[];

// SIZE 0x24
class __declspec(novtable) ArrowToolBase : public MouseCommand
{
protected:
	bool overInstance; // 0x20

	virtual void onMouseIdle(const UIEvent& uiEvent);
	virtual void onMouseHover(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual const std::string getCursorName() const;

public:
	ArrowToolBase(Workspace* workspace) : MouseCommand(workspace), overInstance(false) {}
};

DECOMP_SIZE_ASSERT(ArrowToolBase, 0x24)

// SIZE 0x24
class ArrowTool : public Named<ArrowToolBase, sArrowTool>
{
public:
	ArrowTool(Workspace* workspace) : Named<ArrowToolBase, sArrowTool>(workspace) {}

	// FUNCTION: WEBSERVICE 0x1006a710
	// FUNCTION: ROBLOX 0x005b4930
	virtual MouseCommand* isSticky() const { return new ArrowTool(workspace); }
};

DECOMP_SIZE_ASSERT(ArrowTool, 0x24)

class MegaDragger;

extern char sAxisMoveTool[];

// SIZE 0x78
class AxisMoveTool : public Named<ArrowToolBase, sAxisMoveTool>
{
private:
	std::auto_ptr<MegaDragger> dragger; // 0x24
	std::string cursor;                 // 0x28
	bool m_unk0x44;                     // 0x44
	G3D::Vector2int16 m_unk0x46;        // 0x46
	G3D::Ray m_unk0x4c;                 // 0x4c
	int m_unk0x68;                      // 0x68
	G3D::Vector3 m_unk0x6c;             // 0x6c

	// FUNCTION: ROBLOX 0x0063c5c0
	static const G3D::Vector3& FUN_0063c5c0()
	{
		static G3D::Vector3 v(1.0f, 0.4f, 1.0f);

		return v;
	}

	bool FUN_0063cc60(Extents& extents);
	bool FUN_0063ce30(const UIEvent& uiEvent, G3D::Vector3& hitPoint, NormalId& normalId);

	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual void onMouseIdle(const UIEvent& uiEvent);
	virtual void onMouseMove(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseUp(const UIEvent& uiEvent);
	virtual const std::string getCursorName() const { return cursor; }

public:
	AxisMoveTool(Workspace* workspace);

	// SYNTHETIC: ROBLOX 0x0063c770
	// RBX::AxisMoveTool::~AxisMoveTool

	// FUNCTION: ROBLOX 0x0063c820
	virtual MouseCommand* isSticky() const { return new AxisMoveTool(workspace); }

	// SYNTHETIC: ROBLOX 0x0063c880
	// RBX::AxisMoveTool::`scalar deleting destructor'

	virtual void render2d(Adorn* adorn);
	virtual void render3dAdorn(Adorn* adorn);
};

DECOMP_SIZE_ASSERT(AxisMoveTool, 0x78)

class DataModel;

// VTABLE: ROBLOX 0x0083a894
// SIZE 0x10
class UndoVerb : public Verb
{
public:
	UndoVerb(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);

private:
	DataModel* m_unk0x0c; // 0x0c
};

DECOMP_SIZE_ASSERT(UndoVerb, 0x10)

// VTABLE: ROBLOX 0x0083a8b4
// SIZE 0x10
class RedoVerb : public Verb
{
public:
	RedoVerb(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);

private:
	DataModel* m_unk0x0c; // 0x0c
};

DECOMP_SIZE_ASSERT(RedoVerb, 0x10)

} // namespace RBX

#endif // TOOL_TOOLSARROW_H
