#include "reflection/object.h"

namespace RBX {
namespace Reflection {

// GLOBAL: WEBSERVICE 0x102f59a1
bool ClassDescriptor::lockedDown;

// STUB: WEBSERVICE 0x10005bd0
ClassDescriptor::~ClassDescriptor()
{
	STUB(0x10005bd0);
}

// FUNCTION: WEBSERVICE 0x10047430
bool ClassDescriptor::operator==(const ClassDescriptor& other) const
{
	return this == &other;
}

// STUB: WEBSERVICE 0x10047800
ClassDescriptor::ClassDescriptor()
	: Descriptor("Object"), MemberDescriptorContainer<PropertyDescriptor>(NULL),
	  MemberDescriptorContainer<SignalDescriptor>(NULL), MemberDescriptorContainer<FunctionDescriptor>(NULL), base(NULL)
{
	STUB(0x10047800);
}

// STUB: WEBSERVICE 0x100478c0
ClassDescriptor::ClassDescriptor(ClassDescriptor& base, const char* name)
	: Descriptor(name), MemberDescriptorContainer<PropertyDescriptor>(&base),
	  MemberDescriptorContainer<SignalDescriptor>(&base), MemberDescriptorContainer<FunctionDescriptor>(&base),
	  base(&base)
{
	STUB(0x100478c0);
}

} // namespace Reflection
} // namespace RBX
