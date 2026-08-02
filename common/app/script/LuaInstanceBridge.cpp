#include "util/Name.h"
#include "v8datamodel/Decal.h"

template const RBX::Name& RBX::Name::doDeclare<RBX::sDecal>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sTexture>();
template void RBX::Name::callDoDeclare<RBX::sDecal>();
template void RBX::Name::callDoDeclare<RBX::sTexture>();
