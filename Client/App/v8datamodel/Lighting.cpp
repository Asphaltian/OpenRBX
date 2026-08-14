#include "v8datamodel/Lighting.h"

namespace RBX {

char sLighting[] = "Lighting";

} // namespace RBX

template class RBX::FactoryProduct<RBX::Lighting, RBX::Instance, RBX::sLighting>;
template class RBX::Reflection::
	Described<RBX::Lighting, RBX::sLighting, RBX::FactoryProduct<RBX::Lighting, RBX::Instance, RBX::sLighting> >;
template class RBX::DescribedCreatable<RBX::Lighting, RBX::Instance, RBX::sLighting>;
