#include "util/MeshId.h"

#include "reflection/property.h"
#include "util/Utilities.h"
#include "v8xml/XmlElement.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x100ff530 FOLDED
template <>
std::string StringConverter<MeshId>::convertToString(const MeshId& value)
{
	return value.toString();
}

// FUNCTION: WEBSERVICE 0x1007da70 FOLDED
template <>
bool StringConverter<MeshId>::convertToValue(const std::string& text, MeshId& value)
{
	value = MeshId(text);
	return true;
}

namespace Reflection {

// FUNCTION: WEBSERVICE 0x1012dd60 FOLDED
template <>
const Type& Type::singleton<MeshId>()
{
	return singleton<ContentId>();
}

// FUNCTION: WEBSERVICE 0x1012dd70 FOLDED
template <>
void TypedPropertyDescriptor<MeshId>::readValue(
	DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder
) const
{
	if (!element->isXsiNil()) {
		ContentId value;
		if (element->getValue(value)) {
			setValue(instance, MeshId(value));
		}
	}
}

// FUNCTION: WEBSERVICE 0x1007e3c0 FOLDED
template <>
void TypedPropertyDescriptor<MeshId>::writeValue(const DescribedBase* instance, XmlElement* element) const
{
	element->setValue(getValue(instance));
}

// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
template <>
bool TypedPropertyDescriptor<MeshId>::hasStringValue() const
{
	return true;
}

// FUNCTION: WEBSERVICE 0x1007e440 FOLDED
template <>
std::string TypedPropertyDescriptor<MeshId>::getStringValue(const DescribedBase* instance) const
{
	return StringConverter<MeshId>::convertToString(getValue(instance));
}

// FUNCTION: WEBSERVICE 0x1007e4c0 FOLDED
template <>
bool TypedPropertyDescriptor<MeshId>::setStringValue(DescribedBase* instance, const std::string& text) const
{
	MeshId value;
	if (StringConverter<MeshId>::convertToValue(text, value)) {
		setValue(instance, value);
		return true;
	}

	return false;
}

} // namespace Reflection
} // namespace RBX
