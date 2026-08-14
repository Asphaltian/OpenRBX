#include "v8datamodel/ForceField.h"

namespace RBX {

char sForceField[] = "ForceField";

} // namespace RBX

template class RBX::FactoryProduct<RBX::ForceField, RBX::Instance, RBX::sForceField>;
template class RBX::Reflection::Described<
	RBX::ForceField,
	RBX::sForceField,
	RBX::FactoryProduct<RBX::ForceField, RBX::Instance, RBX::sForceField> >;
template class RBX::DescribedCreatable<RBX::ForceField, RBX::Instance, RBX::sForceField>;
