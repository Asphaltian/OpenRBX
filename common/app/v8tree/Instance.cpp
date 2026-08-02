#include "v8tree/Instance.h"

#include "decomp.h"

#include <cstddef>

namespace RBX {

const char sInstance[] = "Instance";

Reflection::SignalDesc<Instance, void(const Reflection::PropertyDescriptor*)> Instance::event_propertyChanged;

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

// FUNCTION: WEBSERVICE 0x10048340
void Instance::readProperties(const XmlElement* element, IReferenceBinder& binder)
{
	for (XmlElement* child = element->firstChild(); child != NULL; child = child->nextSibling()) {
		readProperty(child, binder);
	}
}

// FUNCTION: WEBSERVICE 0x10048370
void Instance::writeChildren(XmlElement* element)
{
	const std::vector<shared_ptr<Instance> >* kids = children.read();

	if (kids != NULL) {
		for (std::vector<shared_ptr<Instance> >::const_iterator it = kids->begin(); it != kids->end(); ++it) {
			XmlElement* child = (*it)->write();

			if (child != NULL) {
				element->addChild(child);
			}
		}
	}
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

// STUB: WEBSERVICE 0x1004eb10
Instance::Instance() : parent(0), archivable(true)
{
}

} // namespace RBX
