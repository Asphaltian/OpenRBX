#include "v8datamodel/Explosion.h"

namespace RBX {

char sExplosion[] = "Explosion";

} // namespace RBX

template class RBX::FactoryProduct<RBX::Explosion, RBX::Instance, RBX::sExplosion>;
template class RBX::Reflection::
	Described<RBX::Explosion, RBX::sExplosion, RBX::FactoryProduct<RBX::Explosion, RBX::Instance, RBX::sExplosion> >;
template class RBX::DescribedCreatable<RBX::Explosion, RBX::Instance, RBX::sExplosion>;
