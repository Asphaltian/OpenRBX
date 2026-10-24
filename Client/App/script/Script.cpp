#include "script/Script.h"

#include "network/Player.h"
#include "util/ContentProvider.h"
#include "util/Name.h"
#include "v8datamodel/Team.h"

namespace RBX {

char sScript[] = "Script";

char sLocalScript[] = "LocalScript";

// STUB: WEBSERVICE 0x10068bf0
Script::~Script()
{
	STUB(0x10068bf0);
}

// FUNCTION: WEBSERVICE 0x10068d50
boost::shared_ptr<const std::string> Script::requestCode()
{
	if (scriptId.isNull()) {
		return embeddedSource;
	}

	return ContentProvider::singleton().requestContentString(scriptId);
}

// STUB: WEBSERVICE 0x100699a0
Script::Script()
{
	STUB(0x100699a0);
}

// STUB: WEBSERVICE 0x10069cd0
LocalScript::LocalScript()
{
	STUB(0x10069cd0);
}

// STUB: WEBSERVICE 0x10069e10
LocalScript::~LocalScript()
{
	STUB(0x10069e10);
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sTeam>();
template const RBX::Name& RBX::Name::doDeclare<RBX::Network::sPlayer>();
template void RBX::Name::callDoDeclare<RBX::sTeam>();
template void RBX::Name::callDoDeclare<RBX::Network::sPlayer>();

template const RBX::FactoryProduct<RBX::Network::Player, RBX::Instance, RBX::Network::sPlayer>::Creator
	RBX::FactoryProduct<RBX::Network::Player, RBX::Instance, RBX::Network::sPlayer>::creator;

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
