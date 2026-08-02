#include "humanoid/Jumping.h"

#include "util/Name.h"

namespace RBX {

const char sJumping[] = "Jumping";

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sJumping>();
template void RBX::Name::callDoDeclare<RBX::sJumping>();
