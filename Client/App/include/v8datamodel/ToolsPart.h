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
};

DECOMP_SIZE_ASSERT(PartTool, 0x28)

// SIZE 0x1c
class FillToolColor : public Notifier<FillToolColor, BrickColor>
{
private:
	BrickColor color; // 0x18

public:
	FillToolColor() {}

	BrickColor get() const;
	void set(const BrickColor& value);

	// SYNTHETIC: WEBSERVICE 0x10129d20
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
	virtual const std::string getCursorName() const;

public:
	FillTool(Workspace* workspace);

	virtual MouseCommand* isSticky() const;
};

DECOMP_SIZE_ASSERT(FillTool, 0x28)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021e7c0
// `dynamic initializer for 'RBX::FillTool::color''
// SYNTHETIC: WEBSERVICE 0x102255b0
// `dynamic atexit destructor for 'RBX::FillTool::color''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_TOOLSPART_H
