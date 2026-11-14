#include "reflection/enumconverter.h"
#include "reflection/object.h"
#include "reflection/property.h"
#include "v8xml/XmlElement.h"

namespace RBX {
namespace Reflection {

extern template class MemberDescriptorContainer<PropertyDescriptor>;

void PropertyDescriptor::read(DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder) const
{
	readValue(instance, element, binder);
}

template <>
const Type& Type::singleton<const PropertyDescriptor*>()
{
	static Type type("Property", typeid(const PropertyDescriptor*));

	return type;
}

XmlElement* PropertyDescriptor::write(const DescribedBase* instance, bool ignoreWriteProtection) const
{
	if (!ignoreWriteProtection) {
		if (isReadOnly()) {
			return NULL;
		}

		if (!canStreamWrite()) {
			return NULL;
		}
	}

	XmlElement* element = new XmlElement(type.tag);

	element->addAttribute<const RBX::Name*>(name_name, &name);

	writeValue(instance, element);

	return element;
}

std::vector<const EnumDescriptor*>& EnumDescriptor::allEnums()
{
	static std::vector<const EnumDescriptor*> s;

	return s;
}

std::vector<const EnumDescriptor*>::const_iterator EnumDescriptor::enumsBegin()
{
	return allEnums().begin();
}

std::vector<const EnumDescriptor*>::const_iterator EnumDescriptor::enumsEnd()
{
	return allEnums().end();
}

EnumDescriptor::~EnumDescriptor()
{
	allEnums().erase(std::find(allEnums().begin(), allEnums().end(), this));
}

EnumDescriptor::EnumDescriptor(const char* typeName, const std::type_info& type)
	: Type(typeName, type, "token"), enumCount(0), enumCountMSB(0)
{
	allEnums().push_back(this);
}

PropertyDescriptor::PropertyDescriptor(
	ClassDescriptor& classDescriptor, const Type& type, const char* name, const char* category, Functionality flags
)
	: MemberDescriptor(classDescriptor, name, category), type(type), bIsPublic(flags), bCanStreamWrite(flags >> 2)
{
	classDescriptor.MemberDescriptorContainer<PropertyDescriptor>::declare(this);
}

} // namespace Reflection
} // namespace RBX
