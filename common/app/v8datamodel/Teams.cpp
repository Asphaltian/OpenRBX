#include "v8datamodel/Teams.h"

namespace RBX {

const char sTeams[] = "Teams";

} // namespace RBX

template class RBX::FactoryProduct<RBX::Teams, RBX::Instance, RBX::sTeams>;
template class RBX::Reflection::
	Described<RBX::Teams, RBX::sTeams, RBX::FactoryProduct<RBX::Teams, RBX::Instance, RBX::sTeams> >;
template class RBX::DescribedCreatable<RBX::Teams, RBX::Instance, RBX::sTeams>;
