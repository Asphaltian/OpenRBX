#include "v8datamodel/Tool.h"

#include "reflection/Property.h"

namespace RBX {

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
