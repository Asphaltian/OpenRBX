#include "reflection/function.h"
#include "reflection/object.h"

namespace RBX {
namespace Reflection {

extern template class MemberDescriptorContainer<FunctionDescriptor>;

// FUNCTION: WEBSERVICE 0x100713b0
// FUNCTION: ROBLOX 0x005a9d20
FunctionDescriptor::FunctionDescriptor(ClassDescriptor& classDescriptor, const char* name, Security security)
	: MemberDescriptor(classDescriptor, name, "Function"), security(security)
{
	classDescriptor.MemberDescriptorContainer<FunctionDescriptor>::declare(this);
}

} // namespace Reflection
} // namespace RBX
