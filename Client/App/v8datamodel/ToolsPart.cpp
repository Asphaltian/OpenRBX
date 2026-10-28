#include "v8datamodel/ToolsPart.h"

#include "util/Name.h"
#include "v8datamodel/custommesh.h"

template const RBX::Name& RBX::Name::doDeclare<RBX::sSpecialShape>();
template void RBX::Name::callDoDeclare<RBX::sSpecialShape>();

namespace RBX {

char sFillTool[] = "Fill";

FillToolColor FillTool::color;

} // namespace RBX
