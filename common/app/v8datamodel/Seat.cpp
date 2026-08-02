#include "v8datamodel/Seat.h"

namespace RBX {

char sSeat[] = "Seat";

} // namespace RBX

template class RBX::FactoryProduct<RBX::Seat, RBX::PartInstance, RBX::sSeat>;
template class RBX::Reflection::
	Described<RBX::Seat, RBX::sSeat, RBX::FactoryProduct<RBX::Seat, RBX::PartInstance, RBX::sSeat> >;
template class RBX::DescribedCreatable<RBX::Seat, RBX::PartInstance, RBX::sSeat>;
