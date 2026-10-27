#include "v8datamodel/Hopper.h"

#include "util/Name.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x100c70f0
const TextureId BackpackItem::getTextureId() const
{
	return textureId;
}

// FUNCTION: WEBSERVICE 0x100c7120
bool Hopper::askAddChild(const Instance* instance) const
{
	return dynamic_cast<const BackpackItem*>(instance) != NULL;
}

// FUNCTION: WEBSERVICE 0x100c7870
Hopper::Hopper()
{
	yLocation = Rect::BOTTOM;
}

// STUB: WEBSERVICE 0x100c7b90
void StarterPackService::render2d(Adorn* adorn)
{
	STUB(0x100c7b90);
}

// FUNCTION: WEBSERVICE 0x100c9520
StarterPackService::StarterPackService()
{
	setName(sStarterPackService);
}

// FUNCTION: WEBSERVICE 0x100e44b0 FOLDED
bool Hopper::askSetParent(const Instance* instance) const
{
	return true;
}

const char sLegacyHopperService[] = "Hopper";

const char sStarterPackService[] = "StarterPack";

const char sHopperBin[] = "HopperBin";

const char sBackpackItem[] = "BackpackItem";

} // namespace RBX

template class RBX::FactoryProduct<RBX::StarterPackService, RBX::Hopper, RBX::sStarterPackService>;
template class RBX::Reflection::Described<
	RBX::StarterPackService,
	RBX::sStarterPackService,
	RBX::FactoryProduct<RBX::StarterPackService, RBX::Hopper, RBX::sStarterPackService> >;
template class RBX::DescribedCreatable<RBX::StarterPackService, RBX::Hopper, RBX::sStarterPackService>;

template const RBX::Name& RBX::Name::doDeclare<RBX::sBackpackItem>();
template void RBX::Name::callDoDeclare<RBX::sBackpackItem>();
