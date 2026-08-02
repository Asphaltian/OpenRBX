#include "v8datamodel/Accoutrement.h"

#include "reflection/property.h"

namespace RBX {

const char sHat[] = "Hat";

const char sAccoutrement[] = "Accoutrement";

static Reflection::PropDescriptor<Accoutrement, Accoutrement::AccoutrementState> prop_BackendAccoutrementState;

// FUNCTION: WEBSERVICE 0x100dcfb0
void Accoutrement::setBackendAccoutrementState(AccoutrementState value)
{
	if (value != backendAccoutrementState) {
		backendAccoutrementState = value;
		raisePropertyChanged(prop_BackendAccoutrementState);
	}
}

} // namespace RBX
