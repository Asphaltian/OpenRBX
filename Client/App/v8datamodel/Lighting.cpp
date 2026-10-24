#include "v8datamodel/Lighting.h"

#include "v8datamodel/Sky.h"

namespace RBX {

char sLighting[] = "Lighting";

// FUNCTION: WEBSERVICE 0x100438e0
void Lighting::replaceSky(Sky* sky)
{
	while (Sky* oldSky = findFirstChildOfType<Sky>()) {
		oldSky->setParent(NULL);
	}

	sky->setParent(this);
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::Lighting, RBX::Instance, RBX::sLighting>;
template class RBX::Reflection::
	Described<RBX::Lighting, RBX::sLighting, RBX::FactoryProduct<RBX::Lighting, RBX::Instance, RBX::sLighting> >;
template class RBX::DescribedCreatable<RBX::Lighting, RBX::Instance, RBX::sLighting>;
