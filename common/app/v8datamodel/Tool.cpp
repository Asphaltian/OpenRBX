#include "v8datamodel/Tool.h"

#include "reflection/Property.h"
#include "util/Name.h"
#include "v8datamodel/Mouse.h"
#include "v8datamodel/ToolMouseCommand.h"

namespace RBX {

const char sTool[] = "Tool";

static Reflection::PropDescriptor<Tool, Tool::ToolState> prop_BackendToolState;

// STUB: WEBSERVICE 0x100cc300
void Tool::setBackendToolStateNoReplicate(ToolState value)
{
	STUB(0x100cc300);
}

// STUB: WEBSERVICE 0x100cc480
void Tool::setBackendToolState(ToolState value)
{
	if (value != backendToolState) {
		setBackendToolStateNoReplicate(value);
		raisePropertyChanged(prop_BackendToolState);
	}
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sToolMouseCommand>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sMouse>();
template void RBX::Name::callDoDeclare<RBX::sToolMouseCommand>();
template void RBX::Name::callDoDeclare<RBX::sMouse>();
