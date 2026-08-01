#include "v8tree/Instance.h"

#include "decomp.h"

#include <cstddef>

namespace RBX {

Instance::Instance() : parent(0), archivable(true)
{
}

Instance::~Instance()
{
}

// STUB: WEBSERVICE 0x10047af0
void Instance::onChildChanged(Instance* child, const PropertyChanged& event)
{
	if (parent != NULL) {
		parent->onChildChanged(child, event);
	}
}

// FUNCTION: WEBSERVICE 0x10047b20
bool Instance::contains(const Instance* instance) const
{
	while (instance != 0) {
		if (instance == this) {
			return true;
		}

		instance = instance->parent;
	}

	return false;
}

// STUB: WEBSERVICE 0x10049d40
XmlElement* Instance::write()
{
	STUB(0x10049d40);
	return NULL;
}

// STUB: WEBSERVICE 0x1004a010
void Instance::readProperty(const XmlElement* element, IReferenceBinder& binder)
{
	STUB(0x1004a010);
}

// STUB: WEBSERVICE 0x1004cdf0
shared_ptr<Instance> Instance::createChild(const Name& className)
{
	STUB(0x1004cdf0);
	return shared_ptr<Instance>();
}

// STUB: WEBSERVICE 0x1004ddb0
void Instance::onAncestorChanged(const AncestorChanged& event)
{
	STUB(0x1004ddb0);
}

// STUB: WEBSERVICE 0x1004dee0
void Instance::onDescendentAdded(Instance* instance)
{
	STUB(0x1004dee0);
}

// STUB: WEBSERVICE 0x1004df50
void Instance::onDescendentRemoving(const shared_ptr<Instance>& instance)
{
	STUB(0x1004df50);
}

// STUB: WEBSERVICE 0x1004e040
void Instance::setName(const std::string& value)
{
	STUB(0x1004e040);
}

// STUB: WEBSERVICE 0x1004e0f0
void Instance::predelete()
{
	STUB(0x1004e0f0);
}

} // namespace RBX
