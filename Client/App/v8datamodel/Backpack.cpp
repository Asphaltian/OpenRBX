#include "v8datamodel/Backpack.h"

#include "network/Players.h"
#include "v8datamodel/Workspace.h"

namespace RBX {

const char sBackpack[] = "Backpack";

// FUNCTION: WEBSERVICE 0x100da590
// FUNCTION: ROBLOX 0x005e3fd0
IScriptOwner* Backpack::scriptShouldRun(Script* script)
{
	Workspace* workspace = ServiceProvider::find<Workspace>(this);
	if (workspace) {
		bool isLocalBackpack = getParent() == Network::Players::findLocalPlayer(this);
		bool isBackendProcessing = Network::Players::backendProcessing(this, true);
		bool isLocalScript = dynamic_cast<LocalScript*>(script) != NULL;
		bool inHopperBin = dynamic_cast<HopperBin*>(script->getParent()) != NULL;

		if ((!isLocalScript && ((inHopperBin && isLocalBackpack) || isBackendProcessing)) ||
			(isLocalScript && isLocalBackpack)) {
			return workspace;
		}
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x100da740
// FUNCTION: ROBLOX 0x005e4140
Backpack::Backpack()
{
	setName("Backpack");
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::Backpack, RBX::Hopper, RBX::sBackpack>;
template class RBX::DescribedCreatable<RBX::Backpack, RBX::Hopper, RBX::sBackpack>;
