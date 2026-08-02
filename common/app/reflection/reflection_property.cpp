#include "reflection/enumconverter.h"

namespace RBX {
namespace Reflection {

// STUB: WEBSERVICE 0x10094250
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

// STUB: WEBSERVICE 0x10094390
EnumDescriptor::EnumDescriptor(const char* name, const std::type_info& type)
	: Type(name, type, "token"), enumCount(0), enumCountMSB(0)
{
	allEnums().push_back(this);
}

} // namespace Reflection
} // namespace RBX
