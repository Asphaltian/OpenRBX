#include "v8datamodel/PVInstance.h"

#include "reflection/property.h"

namespace RBX {

static Reflection::PropDescriptor<PVInstance, bool> prop_ControllerFlagShown(
	"ControllerFlagShown",
	"Appearance",
	&RBX::PVInstance::getShowControllerFlag,
	&RBX::PVInstance::setShowControllerFlag
);

// STUB: WEBSERVICE 0x1005a7f0
bool PVInstance::isControllable() const
{
	STUB(0x1005a7f0);
	return false;
}

// FUNCTION: WEBSERVICE 0x1009a4e0
void PVInstance::setShowControllerFlag(bool _showControllerFlag)
{
	if (showControllerFlag != _showControllerFlag) {
		showControllerFlag = _showControllerFlag;
		raisePropertyChanged(prop_ControllerFlagShown);
	}
}

} // namespace RBX
