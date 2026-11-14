#include "reflection/reflection.h"

namespace RBX {
namespace Reflection {

bool ClassDescriptor::lockedDown;

bool ClassDescriptor::operator==(const ClassDescriptor& other) const
{
	return this == &other;
}

bool ClassDescriptor::operator!=(const ClassDescriptor& other) const
{
	return this != &other;
}

ClassDescriptor::ClassDescriptor()
	: Descriptor("<<<ROOT>>>"), MemberDescriptorContainer<PropertyDescriptor>(NULL),
	  MemberDescriptorContainer<SignalDescriptor>(NULL), MemberDescriptorContainer<FunctionDescriptor>(NULL), base(NULL)
{
}

ClassDescriptor::ClassDescriptor(ClassDescriptor& base, const char* name)
	: Descriptor(name), MemberDescriptorContainer<PropertyDescriptor>(&base),
	  MemberDescriptorContainer<SignalDescriptor>(&base), MemberDescriptorContainer<FunctionDescriptor>(&base),
	  base(&base)
{
	boost::recursive_mutex::scoped_lock lock(sync());

	base.derivedClasses.push_back(this);
}

bool MemberDescriptor::isMemberOf(const ClassDescriptor& classDescriptor) const
{
	const ClassDescriptor* c = &classDescriptor;

	do {
		if (c == &owner) {
			return true;
		}
	} while ((c = c->getBase()) != &ClassDescriptor::rootDescriptor());

	return false;
}

bool MemberDescriptor::isMemberOf(const DescribedBase* instance) const
{
	return isMemberOf(instance->getDescriptor());
}

template class MemberDescriptorContainer<FunctionDescriptor>;
template class MemberDescriptorContainer<PropertyDescriptor>;

} // namespace Reflection
} // namespace RBX
