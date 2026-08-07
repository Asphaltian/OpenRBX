#include "v8datamodel/Selection.h"

namespace RBX {

const char sSelection[] = "Selection";

// STUB: WEBSERVICE 0x1005e870
void Selection::onEvent(const Instance* source, AncestorChanged event)
{
	STUB(0x1005e870);
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::Selection, RBX::Instance, RBX::sSelection>;
template class RBX::Reflection::
	Described<RBX::Selection, RBX::sSelection, RBX::FactoryProduct<RBX::Selection, RBX::Instance, RBX::sSelection> >;
template class RBX::DescribedCreatable<RBX::Selection, RBX::Instance, RBX::sSelection>;
