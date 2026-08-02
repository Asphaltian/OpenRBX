#include "v8datamodel/Visit.h"

namespace RBX {

char sVisit[] = "Visit";

} // namespace RBX

template class RBX::FactoryProduct<RBX::Visit, RBX::Instance, RBX::sVisit>;
template class RBX::Reflection::
	Described<RBX::Visit, RBX::sVisit, RBX::FactoryProduct<RBX::Visit, RBX::Instance, RBX::sVisit> >;
template class RBX::DescribedCreatable<RBX::Visit, RBX::Instance, RBX::sVisit>;
