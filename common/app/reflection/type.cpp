#include "reflection/type.h"

namespace RBX {
namespace Reflection {

// FUNCTION: WEBSERVICE 0x1003e950
Type::Type(const char* name, const std::type_info& type) : Descriptor(name), type(type), tag(Name::lookup(name))
{
}

// STUB: WEBSERVICE 0x1003e9c0
Type::Type(const char* name, const std::type_info& type, const char* tag)
	: Descriptor(name), type(type), tag(Name::lookup(tag))
{
}

} // namespace Reflection
} // namespace RBX
