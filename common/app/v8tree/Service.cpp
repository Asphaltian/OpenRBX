#include "v8tree/Service.h"

namespace RBX {

const char sServiceProvider[] = "ServiceProvider";

// FUNCTION: WEBSERVICE 0x10058c30
void ServiceProvider::onDescendentAdded(Instance* instance)
{
	Instance::onDescendentAdded(instance);
	instance->onServiceProvider(NULL, this);
}

// FUNCTION: WEBSERVICE 0x10058c90
void ServiceProvider::onDescendentRemoving(const shared_ptr<Instance>& instance)
{
	instance->onServiceProvider(this, NULL);
	Instance::onDescendentRemoving(instance);
}

// STUB: WEBSERVICE 0x10059270
Instance* ServiceProvider::findServiceByClassName(const Name& className) const
{
	STUB(0x10059270);

	return NULL;
}

// STUB: WEBSERVICE 0x10059320
void ServiceProvider::onChildRemoving(Instance* child)
{
	const Name* className = &child->getClassName();
	std::map<const Name*, boost::shared_ptr<Instance> >::iterator iter = serviceMap.find(className);

	if (iter != serviceMap.end()) {
		Notifier<ServiceProvider, ServiceRemoving>::raise(ServiceRemoving(shared_from(child)));
	}
}

// STUB: WEBSERVICE 0x10059550
void ServiceProvider::onChildAdded(Instance* child)
{
	if (dynamic_cast<Service*>(child) != NULL) {
		if (&child->getClassName() != &Name::getNullName()) {
			serviceMap[&child->getClassName()] = shared_from(child);
		}

		Notifier<ServiceProvider, ServiceAdded>::raise(ServiceAdded(shared_from(child)));
	}
}

// FUNCTION: WEBSERVICE 0x1007ab90
bool ServiceProvider::askAddChild(const Instance* instance) const
{
	return dynamic_cast<const Service*>(instance) != NULL;
}

} // namespace RBX
