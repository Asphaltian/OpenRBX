#include "util/Name.h"
#include "v8datamodel/custommesh.h"

template const RBX::Name& RBX::Name::doDeclare<RBX::sSpecialShape>();
template void RBX::Name::callDoDeclare<RBX::sSpecialShape>();
