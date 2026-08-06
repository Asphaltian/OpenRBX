#include "reflection/signal.h"

#include "reflection/object.h"

#include <cstddef>

namespace RBX {
namespace Reflection {

// FUNCTION: WEBSERVICE 0x10095790
SignalInstance::~SignalInstance()
{
}

// FUNCTION: WEBSERVICE 0x10095a50
SignalInstance* SignalDescriptor::findSignalInstance(const SignalSource* source) const
{
	if (source == NULL) {
		return NULL;
	}

	if (source->signals.get() == NULL) {
		return NULL;
	}

	SignalSource::SignalMap::const_iterator it = source->signals->find(this);

	if (it == source->signals->end()) {
		return NULL;
	}

	return it->second.get();
}

// FUNCTION: WEBSERVICE 0x10095b90
SignalDescriptor::SignalDescriptor(ClassDescriptor& classDescriptor, const char* name)
	: MemberDescriptor(classDescriptor, name, "Signals"), signalCreatedHook(NULL)
{
	classDescriptor.MemberDescriptorContainer<SignalDescriptor>::declare(this);
}

// STUB: WEBSERVICE 0x10095e80
SignalSource::~SignalSource()
{
}

// STUB: WEBSERVICE 0x10095ed0
void SignalSource::disconnect_all_slots()
{
	delete signals.release();
}

} // namespace Reflection
} // namespace RBX
