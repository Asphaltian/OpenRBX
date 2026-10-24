#include "reflection/enumconverter.h"
#include "reflection/property.h"
#include "util/ContentProvider.h"
#include "util/NormalId.h"
#include "util/Utilities.h"
#include "v8datamodel/BrickColor.h"
#include "v8tree/Instance.h"
#include "v8xml/XmlElement.h"

#include <G3D/Color3.h>
#include <G3D/Vector3.h>
#include <string>
#include <vector>

namespace RBX {
namespace Reflection {

// FUNCTION: WEBSERVICE 0x1003ef60
template <>
const Type& Type::singleton<boost::shared_ptr<DescribedBase> >()
{
	static Type type("Object", typeid(boost::shared_ptr<DescribedBase>));

	return type;
}

// FUNCTION: WEBSERVICE 0x1003efd0
template <>
const Type& Type::singleton<boost::shared_ptr<Instance> >()
{
	static Type type("Instance", typeid(boost::shared_ptr<Instance>));

	return type;
}

// FUNCTION: WEBSERVICE 0x1003f040
template <>
const Type& Type::singleton<boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > >()
{
	static Type type("Objects", typeid(boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > >));

	return type;
}

// FUNCTION: WEBSERVICE 0x1003f0b0
template <>
const Type& Type::singleton<int>()
{
	static Type type("int", typeid(int));

	return type;
}

// FUNCTION: WEBSERVICE 0x1003f120
template <>
const Type& Type::singleton<bool>()
{
	static Type type("bool", typeid(bool));

	return type;
}

// FUNCTION: WEBSERVICE 0x1003f190
template <>
const Type& Type::singleton<float>()
{
	static Type type("float", typeid(float));

	return type;
}

// FUNCTION: WEBSERVICE 0x1003f200
template <>
const Type& Type::singleton<double>()
{
	static Type type("double", typeid(double));

	return type;
}

// FUNCTION: WEBSERVICE 0x1003f270
template <>
const Type& Type::singleton<ContentId>()
{
	static Type type("Content", typeid(ContentId));

	return type;
}

// FUNCTION: WEBSERVICE 0x1003f2e0
template <>
const Type& Type::singleton<std::string>()
{
	static Type type("string", typeid(std::string));

	return type;
}

// FUNCTION: WEBSERVICE 0x1003f350
template <>
const Type& Type::singleton<G3D::Vector3>()
{
	static Type type("Vector3", typeid(G3D::Vector3));

	return type;
}

// FUNCTION: WEBSERVICE 0x1003f3c0
template <>
const Type& Type::singleton<G3D::Color3>()
{
	static Type type("Color3", typeid(G3D::Color3));

	return type;
}

// FUNCTION: WEBSERVICE 0x1003f430
template <>
const Type& Type::singleton<std::vector<Value> >()
{
	static Type type("Table", typeid(std::vector<Value>));

	return type;
}

// FUNCTION: WEBSERVICE 0x1003f5c0
template <>
const Type& Type::singleton<BrickColor>()
{
	static Type type("BrickColor", typeid(BrickColor), "int");

	return type;
}

// FUNCTION: WEBSERVICE 0x10040410
template <>
std::string TypedPropertyDescriptor<std::string>::getStringValue(const DescribedBase* instance) const
{
	return getValue(instance);
}

// FUNCTION: WEBSERVICE 0x10040430
template <>
bool TypedPropertyDescriptor<std::string>::setStringValue(DescribedBase* instance, const std::string& text) const
{
	setValue(instance, text);
	return true;
}

// FUNCTION: WEBSERVICE 0x10040450
template <>
std::string TypedPropertyDescriptor<bool>::getStringValue(const DescribedBase* instance) const
{
	return StringConverter<bool>::convertToString(getValue(instance));
}

// FUNCTION: WEBSERVICE 0x10040490
template <>
bool TypedPropertyDescriptor<bool>::setStringValue(DescribedBase* instance, const std::string& text) const
{
	bool value;
	if (StringConverter<bool>::convertToValue(text, value)) {
		setValue(instance, value);
		return true;
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x100404d0
template <>
std::string TypedPropertyDescriptor<float>::getStringValue(const DescribedBase* instance) const
{
	return StringConverter<float>::convertToString(getValue(instance));
}

// FUNCTION: WEBSERVICE 0x10040510
template <>
bool TypedPropertyDescriptor<float>::setStringValue(DescribedBase* instance, const std::string& text) const
{
	float value;
	if (StringConverter<float>::convertToValue(text, value)) {
		setValue(instance, value);
		return true;
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x10040550
template <>
std::string TypedPropertyDescriptor<double>::getStringValue(const DescribedBase* instance) const
{
	return StringConverter<double>::convertToString(getValue(instance));
}

// FUNCTION: WEBSERVICE 0x10040590
template <>
bool TypedPropertyDescriptor<double>::setStringValue(DescribedBase* instance, const std::string& text) const
{
	double value;
	if (StringConverter<double>::convertToValue(text, value)) {
		setValue(instance, value);
		return true;
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x100405e0
template <>
std::string TypedPropertyDescriptor<int>::getStringValue(const DescribedBase* instance) const
{
	return StringConverter<int>::convertToString(getValue(instance));
}

// FUNCTION: WEBSERVICE 0x10040620
template <>
bool TypedPropertyDescriptor<int>::setStringValue(DescribedBase* instance, const std::string& text) const
{
	int value;
	if (StringConverter<int>::convertToValue(text, value)) {
		setValue(instance, value);
		return true;
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x10040660
template <>
std::string TypedPropertyDescriptor<G3D::Vector3>::getStringValue(const DescribedBase* instance) const
{
	return StringConverter<G3D::Vector3>::convertToString(getValue(instance));
}

// FUNCTION: WEBSERVICE 0x100406a0
template <>
bool TypedPropertyDescriptor<G3D::Vector3>::setStringValue(DescribedBase* instance, const std::string& text) const
{
	G3D::Vector3 value;
	if (StringConverter<G3D::Vector3>::convertToValue(text, value)) {
		setValue(instance, value);
		return true;
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x100406f0
template <>
void TypedPropertyDescriptor<std::string>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const
{
	if (!element->isXsiNil()) {
		std::string value;
		if (element->getValue(value)) {
			setValue(instance, value);
		}
	}
}

// FUNCTION: WEBSERVICE 0x10040780
template <>
void TypedPropertyDescriptor<std::string>::writeValue(const DescribedBase* instance, XmlElement* element) const
{
	element->setValue(getValue(instance));
}

// FUNCTION: WEBSERVICE 0x100407b0
template <>
void TypedPropertyDescriptor<float>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const
{
	if (!element->isXsiNil()) {
		float value;
		if (element->getValue(value)) {
			setValue(instance, value);
		}
	}
}

// FUNCTION: WEBSERVICE 0x100407f0
template <>
void TypedPropertyDescriptor<float>::writeValue(const DescribedBase* instance, XmlElement* element) const
{
	element->setValue(getValue(instance));
}

// FUNCTION: WEBSERVICE 0x10040830
template <>
void TypedPropertyDescriptor<bool>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const
{
	if (!element->isXsiNil()) {
		bool value;
		if (element->getValue(value)) {
			setValue(instance, value);
		}
	}
}

// FUNCTION: WEBSERVICE 0x10040870
template <>
void TypedPropertyDescriptor<double>::writeValue(const DescribedBase* instance, XmlElement* element) const
{
	element->setValue(getValue(instance));
}

// FUNCTION: WEBSERVICE 0x100408b0
template <>
void TypedPropertyDescriptor<double>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const
{
	if (!element->isXsiNil()) {
		double value;
		if (element->getValue(value)) {
			setValue(instance, value);
		}
	}
}

// FUNCTION: WEBSERVICE 0x10040900
template <>
void TypedPropertyDescriptor<bool>::writeValue(const DescribedBase* instance, XmlElement* element) const
{
	element->setValue(getValue(instance));
}

// FUNCTION: WEBSERVICE 0x10040930
template <>
void TypedPropertyDescriptor<int>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const
{
	if (!element->isXsiNil()) {
		int value;
		if (element->getValue(value)) {
			setValue(instance, value);
		}
	}
}

// FUNCTION: WEBSERVICE 0x10040970
template <>
void TypedPropertyDescriptor<int>::writeValue(const DescribedBase* instance, XmlElement* element) const
{
	element->setValue(getValue(instance));
}

// FUNCTION: WEBSERVICE 0x100409a0
template <>
void TypedPropertyDescriptor<G3D::Vector3>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const
{
	if (!element->isXsiNil()) {
		const XmlElement* x = element->findFirstChildByTag(tag_X);
		if (x) {
			const XmlElement* y = element->findFirstChildByTag(tag_Y);
			const XmlElement* z = element->findFirstChildByTag(tag_Z);

			G3D::Vector3 v;
			x->getValue(v.x);
			y->getValue(v.y);
			z->getValue(v.z);
			setValue(instance, v);
		}
	}
}

// FUNCTION: WEBSERVICE 0x10040a50
template <>
void TypedPropertyDescriptor<G3D::Vector3>::writeValue(const DescribedBase* instance, XmlElement* element) const
{
	XmlElement* x = element->addChild(tag_X);
	XmlElement* y = element->addChild(tag_Y);
	XmlElement* z = element->addChild(tag_Z);

	G3D::Vector3 v = getValue(instance);
	x->setValue(v.x);
	y->setValue(v.y);
	z->setValue(v.z);
}

// FUNCTION: WEBSERVICE 0x100421f0
template <>
EnumDesc<NormalId>::EnumDesc() : EnumDescriptor("NormalId", typeid(NormalId))
{
	addPair(NORM_Y, "Top");
	addPair(NORM_Y_NEG, "Bottom");
	addPair(NORM_Z, "Back");
	addPair(NORM_Z_NEG, "Front");
	addPair(NORM_X, "Right");
	addPair(NORM_X_NEG, "Left");
}

} // namespace Reflection
} // namespace RBX
