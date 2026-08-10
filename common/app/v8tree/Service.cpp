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

// FUNCTION: WEBSERVICE 0x1007ab90
bool ServiceProvider::askAddChild(const Instance* instance) const
{
	return dynamic_cast<const Service*>(instance) != NULL;
}

} // namespace RBX
