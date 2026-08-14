#include "tool/NullTool.h"

#include "util/Name.h"

namespace RBX {

char sNewNullTool[] = "NewNullTool";

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sNewNullTool>();
template void RBX::Name::callDoDeclare<RBX::sNewNullTool>();
