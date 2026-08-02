#include "v8datamodel/GlobalSettings.h"

#include "util/Name.h"

namespace RBX {

const char sGlobalSettings[] = "GlobalSettings";

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sGlobalSettings>();
template void RBX::Name::callDoDeclare<RBX::sGlobalSettings>();
