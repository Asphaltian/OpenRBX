#include "v8datamodel/Decal.h"

#include "reflection/property.h"

namespace RBX {

char sTexture[] = "Texture";

char sDecal[] = "Decal";

using namespace Reflection;

static PropDescriptor<Decal, float> prop_Specular;
static PropDescriptor<Decal, float> prop_Shiny;
static PropDescriptor<Texture, float> prop_StudsPerTileU;
static PropDescriptor<Texture, float> prop_StudsPerTileV;

// STUB: WEBSERVICE 0x101001f0
void Decal::setSpecular(float value)
{
	if (specular != value && value >= 0.0f) {
		specular = value;
		raisePropertyChanged(prop_Specular);
	}
}

// STUB: WEBSERVICE 0x10100230
void Decal::setShiny(float value)
{
	if (shiny != value && value > 0.0f) {
		shiny = value;
		raisePropertyChanged(prop_Shiny);
	}
}

// STUB: WEBSERVICE 0x10100270
void Texture::setStudsPerTileU(float value)
{
	if (studsPerTile.x != value && value > 0.0f) {
		studsPerTile.x = value;
		raisePropertyChanged(prop_StudsPerTileU);
	}
}

// STUB: WEBSERVICE 0x101002b0
void Texture::setStudsPerTileV(float value)
{
	if (studsPerTile.y != value && value > 0.0f) {
		studsPerTile.y = value;
		raisePropertyChanged(prop_StudsPerTileV);
	}
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::Texture, RBX::Decal, RBX::sTexture>;
template class RBX::Reflection::
	Described<RBX::Texture, RBX::sTexture, RBX::FactoryProduct<RBX::Texture, RBX::Decal, RBX::sTexture> >;
template class RBX::DescribedCreatable<RBX::Texture, RBX::Decal, RBX::sTexture>;
