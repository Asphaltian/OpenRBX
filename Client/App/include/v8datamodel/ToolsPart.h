#ifndef V8DATAMODEL_TOOLSPART_H
#define V8DATAMODEL_TOOLSPART_H

#include "placeholder.h"
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

class PartTool : public MouseCommand
{
private:
	boost::shared_ptr<PartInstance> partInstance;

protected:
	virtual void render3dAdorn(Adorn* adorn);
	virtual void onMouseHover(const UIEvent& uiEvent);

public:
	PartTool(Workspace* workspace);
};

class FillToolColor : public Notifier<FillToolColor, BrickColor>
{
private:
	BrickColor color;

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
};

class FillTool : public Named<PartTool, sFillTool>
{
public:
	static FillToolColor color;

protected:
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);

	virtual const std::string getCursorName() const { return "FillCursor"; }

public:
	FillTool(Workspace* workspace) : Named<PartTool, sFillTool>(workspace) {}

	virtual MouseCommand* isSticky() const { return new FillTool(workspace); }
};

class DropperTool : public Named<PartTool, sDropperTool>
{
protected:
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);

	virtual const std::string getCursorName() const { return "DropperCursor"; }

public:
	DropperTool(Workspace* workspace) : Named<PartTool, sDropperTool>(workspace) {}
};

class ModelSetPrimaryPartTool : public Named<PartTool, sModelSetPrimaryPartTool>
{
protected:
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);

public:
	ModelSetPrimaryPartTool(Workspace* workspace) : Named<PartTool, sModelSetPrimaryPartTool>(workspace) {}
};

} // namespace RBX

#endif // V8DATAMODEL_TOOLSPART_H
