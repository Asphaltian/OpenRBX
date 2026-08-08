#ifndef V8TREE_SERVICE_H
#define V8TREE_SERVICE_H

#include "decomp.h"
#include "util/Events.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>
#include <map>
#include <vector>

namespace RBX {

extern const char sServiceProvider[];

// SIZE 0x1
class Service
{
};

DECOMP_SIZE_ASSERT(Service, 0x1)

// SIZE 0x1
class Closing
{
};

DECOMP_SIZE_ASSERT(Closing, 0x1)

// SIZE 0x8
struct ServiceAdded
{
	const boost::shared_ptr<Instance> service; // 0x00

	ServiceAdded(boost::shared_ptr<Instance> service) : service(service) {}

private:
	ServiceAdded& operator=(const ServiceAdded& other);
};

DECOMP_SIZE_ASSERT(ServiceAdded, 0x8)

// SIZE 0x8
struct ServiceRemoving
{
	const boost::shared_ptr<Instance> service; // 0x00

	ServiceRemoving(boost::shared_ptr<Instance> service) : service(service) {}

private:
	ServiceRemoving& operator=(const ServiceRemoving& other);
};

DECOMP_SIZE_ASSERT(ServiceRemoving, 0x8)

// SIZE 0x15c
class ServiceProvider : public DescribedNonCreatable<ServiceProvider, Instance, sServiceProvider>,
						public Notifier<ServiceProvider, Closing>,
						public Notifier<ServiceProvider, ServiceAdded>,
						public Notifier<ServiceProvider, ServiceRemoving>
{
private:
	std::vector<boost::shared_ptr<Instance> > serviceArray;         // 0x140
	std::map<const Name*, boost::shared_ptr<Instance> > serviceMap; // 0x150

public:
	static ServiceProvider* findServiceProvider(Instance* instance);

	template <class T>
	T* find()
	{
		return dynamic_cast<T*>(findServiceByClassName(T::className()));
	}

	template <class T>
	static T* find(Instance* context)
	{
		ServiceProvider* provider;

		while (true) {
			if (context == NULL) {
				return NULL;
			}

			provider = dynamic_cast<ServiceProvider*>(context);

			if (provider != NULL) {
				break;
			}

			context = context->getParent();
		}

		return provider->find<T>();
	}

protected:
	virtual bool askAddChild(const Instance* instance) const;
	virtual void onChildAdded(Instance* child);
	virtual void onChildRemoving(Instance* child);
	virtual void onDescendentAdded(Instance* instance);
	virtual void onDescendentRemoving(const shared_ptr<Instance>& instance);

	void clearServices();

private:
	Instance* findServiceByClassName(const Name& className) const;
	Instance* findServiceByClassNameString(const std::string& className);
};

DECOMP_SIZE_ASSERT(ServiceProvider, 0x15c)

} // namespace RBX

#endif // V8TREE_SERVICE_H
