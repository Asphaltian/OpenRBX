#include "v8datamodel/Selection.h"

namespace RBX {

const char sSelection[] = "Selection";

// STUB: WEBSERVICE 0x1005e1f0
void Selection::removeFromSelection(const Instance* instance)
{
	STUB(0x1005e1f0);
}

// STUB: WEBSERVICE 0x1005e870
void Selection::onEvent(const Instance* source, AncestorChanged event)
{
	const Instance* sourceRoot = source->getParent() != NULL ? source->getParent()->getRootAncestor() : source;

	if (sourceRoot != getRootAncestor()) {
		removeFromSelection(source);
	}
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::Selection, RBX::Instance, RBX::sSelection>;
template class RBX::Reflection::
	Described<RBX::Selection, RBX::sSelection, RBX::FactoryProduct<RBX::Selection, RBX::Instance, RBX::sSelection> >;
template class RBX::DescribedCreatable<RBX::Selection, RBX::Instance, RBX::sSelection>;
