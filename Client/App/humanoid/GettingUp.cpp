#include "humanoid/GettingUp.h"

#include "util/Name.h"

namespace RBX {

const char sGettingUp[] = "GettingUp";

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sGettingUp>();
template void RBX::Name::callDoDeclare<RBX::sGettingUp>();
