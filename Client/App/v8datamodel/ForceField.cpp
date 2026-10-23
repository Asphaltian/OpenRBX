#include "v8datamodel/ForceField.h"

namespace RBX {

char sForceField[] = "ForceField";

// STUB: WEBSERVICE 0x100e9540
bool ForceField::partInForceField(PartInstance* part)
{
	STUB(0x100e9540);
	return false;
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::ForceField, RBX::Instance, RBX::sForceField>;
template class RBX::Reflection::Described<
	RBX::ForceField,
	RBX::sForceField,
	RBX::FactoryProduct<RBX::ForceField, RBX::Instance, RBX::sForceField> >;
template class RBX::DescribedCreatable<RBX::ForceField, RBX::Instance, RBX::sForceField>;
