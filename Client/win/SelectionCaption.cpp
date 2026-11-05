#include "FilteredListener.h"
#include "v8datamodel/Selection.h"

template class RBX::MarshaledListener<RBX::Selection, RBX::SelectionChanged, RBX::Selection, RBX::SelectionChanged>;
template class RBX::FilteredListener<RBX::Selection, RBX::SelectionChanged, RBX::Selection, RBX::SelectionChanged>;
