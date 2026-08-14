#include "humanoid/FallingDown.h"

#include "util/Name.h"

namespace RBX {

const char sFallingDown[] = "FallingDown";

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sFallingDown>();
template void RBX::Name::callDoDeclare<RBX::sFallingDown>();
