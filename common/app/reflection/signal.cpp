#include "reflection/signal.h"

#include "reflection/object.h"

#include <cstddef>

namespace RBX {
namespace Reflection {

// STUB: WEBSERVICE 0x10095a50
SignalInstance* SignalDescriptor::findSignalInstance(const SignalSource* source) const
{
	STUB(0x10095a50);
	return NULL;
}

// FUNCTION: WEBSERVICE 0x10095b90
SignalDescriptor::SignalDescriptor(ClassDescriptor& classDescriptor, const char* name)
	: MemberDescriptor(classDescriptor, name, "Signals"), signalCreatedHook(NULL)
{
	classDescriptor.MemberDescriptorContainer<SignalDescriptor>::declare(*this);
}

} // namespace Reflection
} // namespace RBX
