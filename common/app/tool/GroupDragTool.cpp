#include "tool/GroupDragTool.h"

#include "util/Name.h"

namespace RBX {

char sGroupDragTool[] = "GroupDragTool";

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sGroupDragTool>();
template void RBX::Name::callDoDeclare<RBX::sGroupDragTool>();
