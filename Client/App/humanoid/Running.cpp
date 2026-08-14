#include "humanoid/Running.h"

#include "util/Name.h"

namespace RBX {

const char sRunning[] = "Running";

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sRunning>();
template void RBX::Name::callDoDeclare<RBX::sRunning>();
