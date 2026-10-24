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

// STUB: WEBSERVICE 0x10046190
Lighting::Lighting()
{
	STUB(0x10046190);
}

// STUB: WEBSERVICE 0x100463f0
Lighting::~Lighting()
{
	STUB(0x100463f0);
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::Lighting, RBX::Instance, RBX::sLighting>;
template class RBX::Reflection::
	Described<RBX::Lighting, RBX::sLighting, RBX::FactoryProduct<RBX::Lighting, RBX::Instance, RBX::sLighting> >;
template class RBX::DescribedCreatable<RBX::Lighting, RBX::Instance, RBX::sLighting>;
