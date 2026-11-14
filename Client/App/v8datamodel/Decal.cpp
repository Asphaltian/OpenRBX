#include "v8datamodel/Decal.h"

#include "reflection/property.h"
#include "util/Utilities.h"
#include "v8xml/XmlElement.h"

namespace RBX {

char sTexture[] = "Texture";

char sDecal[] = "Decal";

using namespace Reflection;

const PropDescriptor<Decal, TextureId> Decal::prop_Texture(
	"Texture", "Appearance", &Decal::getTexture, &Decal::setTexture
);

const PropDescriptor<Decal, float> Decal::prop_Specular(
	"Specular", "Appearance", &Decal::getSpecular, &Decal::setSpecular
);

const PropDescriptor<Decal, float> Decal::prop_Shiny("Shiny", "Appearance", &Decal::getShiny, &Decal::setShiny);

const PropDescriptor<Texture, float> Texture::prop_StudsPerTileU(
	"StudsPerTileU", "Appearance", &Texture::getStudsPerTileU, &Texture::setStudsPerTileU
);

const PropDescriptor<Texture, float> Texture::prop_StudsPerTileV(
	"StudsPerTileV", "Appearance", &Texture::getStudsPerTileV, &Texture::setStudsPerTileV
);

template <>
std::string StringConverter<TextureId>::convertToString(const TextureId& value)
{
	return value.toString();
}

template <>
bool StringConverter<TextureId>::convertToValue(const std::string& text, TextureId& value)
{
	value = TextureId(text);
	return true;
}

template <>
void Reflection::TypedPropertyDescriptor<TextureId>::writeValue(
	const DescribedBase* instance, XmlElement* element
) const
{
	element->setValue(getValue(instance));
}

template <>
bool Reflection::TypedPropertyDescriptor<TextureId>::hasStringValue() const
{
	return true;
}

template <>
std::string Reflection::TypedPropertyDescriptor<TextureId>::getStringValue(const DescribedBase* instance) const
{
	return StringConverter<TextureId>::convertToString(getValue(instance));
}

template <>
void Reflection::TypedPropertyDescriptor<TextureId>::readValue(
	DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder
) const
{
	if (!element->isXsiNil()) {
		ContentId value;
		if (element->getValue(value)) {
			setValue(instance, TextureId(value));
		}
	}
}

template <>
bool Reflection::TypedPropertyDescriptor<TextureId>::setStringValue(
	DescribedBase* instance, const std::string& text
) const
{
	TextureId value;
	if (StringConverter<TextureId>::convertToValue(text, value)) {
		setValue(instance, value);
		return true;
	}

	return false;
}

Decal::Decal() : specular(0.0f), shiny(20.0f)
{
	setName("Decal");
}

void Decal::setTexture(TextureId value)
{
	if (texture != value) {
		texture = value;
		raisePropertyChanged(prop_Texture);
	}
}

void Decal::setSpecular(float value)
{
	if (specular != value && value >= 0.0f) {
		specular = value;
		raisePropertyChanged(prop_Specular);
	}
}

void Decal::setShiny(float value)
{
	if (shiny != value && value > 0.0f) {
		shiny = value;
		raisePropertyChanged(prop_Shiny);
	}
}

void Texture::setStudsPerTileU(float value)
{
	if (studsPerTile.x != value && value > 0.0f) {
		studsPerTile.x = value;
		raisePropertyChanged(prop_StudsPerTileU);
	}
}

void Texture::setStudsPerTileV(float value)
{
	if (studsPerTile.y != value && value > 0.0f) {
		studsPerTile.y = value;
		raisePropertyChanged(prop_StudsPerTileV);
	}
}

Texture::Texture() : studsPerTile(2.0f, 2.0f)
{
	setName("Texture");
}

namespace Reflection {

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
