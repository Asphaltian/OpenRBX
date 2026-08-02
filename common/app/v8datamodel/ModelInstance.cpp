#include "v8datamodel/ModelInstance.h"

#include "humanoid/Humanoid.h"
#include "util/Name.h"
#include "v8datamodel/PartInstance.h"

namespace RBX {

char sModel[] = "Model";

bool ModelInstance::showModelCoord;

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sPart>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sHumanoid>();
template void RBX::Name::callDoDeclare<RBX::sPart>();
template void RBX::Name::callDoDeclare<RBX::sHumanoid>();
