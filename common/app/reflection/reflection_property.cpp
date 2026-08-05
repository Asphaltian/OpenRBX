#include "reflection/enumconverter.h"
#include "reflection/object.h"
#include "reflection/property.h"

namespace RBX {
namespace Reflection {

// FUNCTION: WEBSERVICE 0x10094120
void PropertyDescriptor::read(DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder) const
{
	readValue(instance, element, binder);
}

// FUNCTION: WEBSERVICE 0x10094130
template <>
const Type& Type::singleton<const PropertyDescriptor*>()
{
	static Type type("Property", typeid(const PropertyDescriptor*));

	return type;
}

// FUNCTION: WEBSERVICE 0x10094250
std::vector<const EnumDescriptor*>& EnumDescriptor::allEnums()
{
	static std::vector<const EnumDescriptor*> s;

	return s;
}

// STUB: WEBSERVICE 0x100942b0
EnumDescriptor::~EnumDescriptor()
{
	STUB(0x100942b0);
}

// FUNCTION: WEBSERVICE 0x10094390
EnumDescriptor::EnumDescriptor(const char* typeName, const std::type_info& type)
	: Type(typeName, type, "token"), enumCount(0), enumCountMSB(0)
{
	allEnums().push_back(this);
}

// FUNCTION: WEBSERVICE 0x10094410
PropertyDescriptor::PropertyDescriptor(
	ClassDescriptor& classDescriptor,
	const Type& type,
	const char* name,
	const char* category,
	Functionality flags
)
	: MemberDescriptor(classDescriptor, name, category), type(type), bIsPublic(flags), bCanStreamWrite(flags >> 2)
{
	classDescriptor.MemberDescriptorContainer<PropertyDescriptor>::declare(*this);
}

} // namespace Reflection
} // namespace RBX
