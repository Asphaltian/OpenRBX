#include "v8datamodel/PVInstance.h"

#include "reflection/Property.h"

namespace RBX {

static Reflection::PropDescriptor<PVInstance, bool> prop_ControllerFlagShown;

// FUNCTION: WEBSERVICE 0x1009a4e0
void PVInstance::setShowControllerFlag(bool value)
{
	if (showControllerFlag != value) {
		showControllerFlag = value;
		raisePropertyChanged(prop_ControllerFlagShown);
	}
}

} // namespace RBX
