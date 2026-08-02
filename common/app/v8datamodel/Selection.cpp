#include "v8datamodel/Selection.h"

namespace RBX {

const char sSelection[] = "Selection";

} // namespace RBX

template class RBX::FactoryProduct<RBX::Selection, RBX::Instance, RBX::sSelection>;
template class RBX::Reflection::
	Described<RBX::Selection, RBX::sSelection, RBX::FactoryProduct<RBX::Selection, RBX::Instance, RBX::sSelection> >;
template class RBX::DescribedCreatable<RBX::Selection, RBX::Instance, RBX::sSelection>;
