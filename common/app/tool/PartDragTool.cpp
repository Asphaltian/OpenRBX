#include "tool/PartDragTool.h"

#include "util/Name.h"

namespace RBX {

char sPartDragTool[] = "PartDragTool";

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sPartDragTool>();
template void RBX::Name::callDoDeclare<RBX::sPartDragTool>();
