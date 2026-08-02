#include "script/Script.h"

#include "network/Player.h"
#include "util/Name.h"
#include "v8datamodel/Team.h"

namespace RBX {

char sScript[] = "Script";

char sLocalScript[] = "LocalScript";

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sTeam>();
template const RBX::Name& RBX::Name::doDeclare<RBX::Network::sPlayer>();
template void RBX::Name::callDoDeclare<RBX::sTeam>();
template void RBX::Name::callDoDeclare<RBX::Network::sPlayer>();

template class RBX::FactoryProduct<RBX::Script, RBX::Instance, RBX::sScript>;
template class RBX::Reflection::
	Described<RBX::Script, RBX::sScript, RBX::FactoryProduct<RBX::Script, RBX::Instance, RBX::sScript> >;
template class RBX::DescribedCreatable<RBX::Script, RBX::Instance, RBX::sScript>;

template class RBX::FactoryProduct<RBX::LocalScript, RBX::Script, RBX::sLocalScript>;
template class RBX::Reflection::Described<
	RBX::LocalScript,
	RBX::sLocalScript,
	RBX::FactoryProduct<RBX::LocalScript, RBX::Script, RBX::sLocalScript> >;
template class RBX::DescribedCreatable<RBX::LocalScript, RBX::Script, RBX::sLocalScript>;
