#include "v8datamodel/Flag.h"

#include "reflection/property.h"

namespace RBX {

const char sFlag[] = "Flag";

static Reflection::PropDescriptor<Flag, BrickColor> prop_Color;

// STUB: WEBSERVICE 0x100cef60
void Flag::setTeamColor(BrickColor color)
{
	teamColor = color;
	raisePropertyChanged(prop_Color);
}

} // namespace RBX
