#include "tool/ToolsArrow.h"

#include "util/Name.h"

namespace RBX {

char sBoxSelectCommand[] = "BoxSelect";

char sArrowTool[] = "Arrow";

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sBoxSelectCommand>();
template void RBX::Name::callDoDeclare<RBX::sBoxSelectCommand>();
