#include "v8tree/Service.h"

#include <windows.h>

namespace RBX {

const char sServiceProvider[] = "ServiceProvider";

// FUNCTION: WEBSERVICE 0x10058c20
unsigned int ServiceProvider::newIndex()
{
	static long index = -1;

	return InterlockedIncrement(&index);
}

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

// FUNCTION: WEBSERVICE 0x10059270
boost::shared_ptr<Instance> ServiceProvider::findServiceByClassName(const Name& className) const
{
	std::map<const Name*, boost::shared_ptr<Instance> >::const_iterator iter = serviceMap.find(&className);

	if (iter != serviceMap.end()) {
		return iter->second;
	}

	return boost::shared_ptr<Instance>();
}

// FUNCTION: WEBSERVICE 0x10059320
void ServiceProvider::onChildRemoving(Instance* instance)
{
	std::map<const Name*, boost::shared_ptr<Instance> >::iterator iter = serviceMap.find(&instance->getClassName());

	if (iter != serviceMap.end()) {
		Notifier<ServiceProvider, ServiceRemoving>::raise(instance);
	}
}

// FUNCTION: WEBSERVICE 0x10059550
void ServiceProvider::onChildAdded(Instance* instance)
{
	if (dynamic_cast<Service*>(instance) != NULL) {
		const Name& nullName = Name::getNullName();

		if (&instance->getClassName() != &nullName) {
			serviceMap[&instance->getClassName()] = shared_from(instance);
		}

		Notifier<ServiceProvider, ServiceAdded>::raise(instance);
	}
}

// FUNCTION: WEBSERVICE 0x10059660
void ServiceProvider::clearServices()
{
	serviceArray.clear();
	serviceMap.clear();
}

// FUNCTION: WEBSERVICE 0x1007ab90
bool ServiceProvider::askAddChild(const Instance* instance) const
{
	return dynamic_cast<const Service*>(instance) != NULL;
}

} // namespace RBX
