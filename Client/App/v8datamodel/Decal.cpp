#include "v8datamodel/Decal.h"

#include "reflection/property.h"
#include "util/Utilities.h"
#include "v8xml/XmlElement.h"

namespace RBX {

char sTexture[] = "Texture";

char sDecal[] = "Decal";

using namespace Reflection;

const PropDescriptor<Decal, TextureId> Decal::prop_Texture(
	"Texture",
	"Appearance",
	&RBX::Decal::getTexture,
	&RBX::Decal::setTexture
);

const PropDescriptor<Decal, float> Decal::prop_Specular(
	"Specular",
	"Appearance",
	&RBX::Decal::getSpecular,
	&RBX::Decal::setSpecular
);

const PropDescriptor<Decal, float> Decal::prop_Shiny(
	"Shiny",
	"Appearance",
	&RBX::Decal::getShiny,
	&RBX::Decal::setShiny
);

const PropDescriptor<Texture, float> Texture::prop_StudsPerTileU(
	"StudsPerTileU",
	"Appearance",
	&RBX::Texture::getStudsPerTileU,
	&RBX::Texture::setStudsPerTileU
);

const PropDescriptor<Texture, float> Texture::prop_StudsPerTileV(
	"StudsPerTileV",
	"Appearance",
	&RBX::Texture::getStudsPerTileV,
	&RBX::Texture::setStudsPerTileV
);

// FUNCTION: WEBSERVICE 0x100ff530 FOLDED
template <>
std::string StringConverter<TextureId>::convertToString(const TextureId& value)
{
	return value.toString();
}

// FUNCTION: WEBSERVICE 0x100ff580 FOLDED
template <>
bool StringConverter<TextureId>::convertToValue(const std::string& text, TextureId& value)
{
	value = TextureId(text);
	return true;
}

// FUNCTION: WEBSERVICE 0x10041750 FOLDED
template <>
void Reflection::TypedPropertyDescriptor<TextureId>::writeValue(
	const DescribedBase* instance,
	XmlElement* element
) const
{
	element->setValue(getValue(instance));
}

// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
template <>
bool Reflection::TypedPropertyDescriptor<TextureId>::hasStringValue() const
{
	return true;
}

// FUNCTION: WEBSERVICE 0x100417d0 FOLDED
template <>
std::string Reflection::TypedPropertyDescriptor<TextureId>::getStringValue(const DescribedBase* instance) const
{
	return StringConverter<TextureId>::convertToString(getValue(instance));
}

// FUNCTION: WEBSERVICE 0x100ff890
template <>
void Reflection::TypedPropertyDescriptor<TextureId>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const
{
	if (!element->isXsiNil()) {
		ContentId value;
		if (element->getValue(value)) {
			setValue(instance, TextureId(value));
		}
	}
}

// FUNCTION: WEBSERVICE 0x100ff960 FOLDED
template <>
bool Reflection::TypedPropertyDescriptor<TextureId>::setStringValue(
	DescribedBase* instance,
	const std::string& text
) const
{
	TextureId value;
	if (StringConverter<TextureId>::convertToValue(text, value)) {
		setValue(instance, value);
		return true;
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x100fff70
Decal::Decal() : specular(0.0f), shiny(20.0f)
{
	setName("Decal");
}

// FUNCTION: WEBSERVICE 0x10100170
void Decal::setTexture(TextureId value)
{
	if (texture != value) {
		texture = value;
		raisePropertyChanged(prop_Texture);
	}
}

// FUNCTION: WEBSERVICE 0x101001f0
void Decal::setSpecular(float value)
{
	if (specular != value && value >= 0.0f) {
		specular = value;
		raisePropertyChanged(prop_Specular);
	}
}

// FUNCTION: WEBSERVICE 0x10100230
void Decal::setShiny(float value)
{
	if (shiny != value && value > 0.0f) {
		shiny = value;
		raisePropertyChanged(prop_Shiny);
	}
}

// FUNCTION: WEBSERVICE 0x10100270
void Texture::setStudsPerTileU(float value)
{
	if (studsPerTile.x != value && value > 0.0f) {
		studsPerTile.x = value;
		raisePropertyChanged(prop_StudsPerTileU);
	}
}

// FUNCTION: WEBSERVICE 0x101002b0
void Texture::setStudsPerTileV(float value)
{
	if (studsPerTile.y != value && value > 0.0f) {
		studsPerTile.y = value;
		raisePropertyChanged(prop_StudsPerTileV);
	}
}

// FUNCTION: WEBSERVICE 0x101003c0
Texture::Texture() : studsPerTile(2.0f, 2.0f)
{
	setName("Texture");
}

namespace Reflection {

// FUNCTION: WEBSERVICE 0x1012dd60 FOLDED
template <>
const Type& Type::singleton<TextureId>()
{
	return singleton<ContentId>();
}

} // namespace Reflection

} // namespace RBX

template class RBX::FactoryProduct<RBX::Decal, RBX::FaceInstance, RBX::sDecal>;
template class RBX::Reflection::
	Described<RBX::Decal, RBX::sDecal, RBX::FactoryProduct<RBX::Decal, RBX::FaceInstance, RBX::sDecal> >;
template class RBX::DescribedCreatable<RBX::Decal, RBX::FaceInstance, RBX::sDecal>;
template class RBX::FactoryProduct<RBX::Texture, RBX::Decal, RBX::sTexture>;
template class RBX::Reflection::
	Described<RBX::Texture, RBX::sTexture, RBX::FactoryProduct<RBX::Texture, RBX::Decal, RBX::sTexture> >;
template class RBX::DescribedCreatable<RBX::Texture, RBX::Decal, RBX::sTexture>;
