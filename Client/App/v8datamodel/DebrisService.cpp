#include "v8datamodel/DebrisService.h"

namespace RBX {

char sDebrisService[] = "Debris";

} // namespace RBX

template class RBX::FactoryProduct<RBX::DebrisService, RBX::Instance, RBX::sDebrisService>;
template class RBX::Reflection::Described<
	RBX::DebrisService,
	RBX::sDebrisService,
	RBX::FactoryProduct<RBX::DebrisService, RBX::Instance, RBX::sDebrisService> >;
template class RBX::DescribedCreatable<RBX::DebrisService, RBX::Instance, RBX::sDebrisService>;
