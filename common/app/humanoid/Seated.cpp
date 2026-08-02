#include "humanoid/Seated.h"

#include "util/Name.h"

namespace RBX {

const char sSeated[] = "Seated";

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sSeated>();
template void RBX::Name::callDoDeclare<RBX::sSeated>();
