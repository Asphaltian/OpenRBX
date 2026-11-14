#ifndef V8TREE_SERVICE_H
#define V8TREE_SERVICE_H

#include "placeholder.h"
#include "util/Events.h"
#include "util/object.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>
#include <map>
#include <vector>

namespace RBX {

extern const char sServiceProvider[];

class Service
{
};

class Closing
{
};

struct ServiceAdded
{
	const boost::shared_ptr<Instance> service;

	ServiceAdded(Instance* service) : service(shared_from(service)) {}
	ServiceAdded(const ServiceAdded& other) : service(other.service) {}

private:
	ServiceAdded& operator=(const ServiceAdded& other);
};

struct ServiceRemoving
{
	const boost::shared_ptr<Instance> service;

	ServiceRemoving(Instance* service) : service(shared_from(service)) {}
	ServiceRemoving(const ServiceRemoving& other) : service(other.service) {}

private:
	ServiceRemoving& operator=(const ServiceRemoving& other);
};

class ServiceProvider : public DescribedNonCreatable<ServiceProvider, Instance, sServiceProvider>,
						public Notifier<ServiceProvider, Closing>,
						public Notifier<ServiceProvider, ServiceAdded>,
						public Notifier<ServiceProvider, ServiceRemoving>
{
private:
	mutable std::vector<boost::shared_ptr<Instance> > serviceArray;
	std::map<const Name*, boost::shared_ptr<Instance> > serviceMap;

public:
	static Instance* create(Instance* context, const Name& name);

	static const ServiceProvider* findServiceProvider(const Instance* context)
	{
		while (context != NULL) {
			const ServiceProvider* serviceProvider = dynamic_cast<const ServiceProvider*>(context);

			if (serviceProvider != NULL) {
				return serviceProvider;
			}

			context = context->getParent();
		}

		return NULL;
	}

	template <class T>
	T* find() const
	{
		unsigned int index = getClassIndex<T>();

		if (index + 1 > serviceArray.size()) {
			serviceArray.resize(index + 1);
		}
		else if (Instance* service = serviceArray[index].get()) {
			return static_cast<T*>(service);
		}

		static const bool isNullClassName = &T::className() == &Name::getNullName();
		if (isNullClassName) {
			return NULL;
		}

		boost::shared_ptr<Instance> i = findServiceByClassName(T::className());
		if (!i) {
			return NULL;
		}

		T* service = static_cast<T*>(i.get());
		serviceArray[index] = i;
		return service;
	}

	template <class T>
	T* create() const
	{
		T* t = find<T>();
		if (t == NULL) {
			boost::shared_ptr<T> s = Creatable<Instance>::create<T>();

			t = s.get();

			unsigned int index = getClassIndex<T>();
			serviceArray[index] = s;
			t->setParent(const_cast<ServiceProvider*>(this));
		}

		return t;
	}

	template <class T>
	static T* find(const Instance* context)
	{
		const ServiceProvider* provider;

		while (true) {
			if (context == NULL) {
				return NULL;
			}

			provider = dynamic_cast<const ServiceProvider*>(context);

			if (provider != NULL) {
				break;
			}

			context = context->getParent();
		}

		return provider->find<T>();
	}

	template <class T>
	static T* create(const Instance* context)
	{
		const ServiceProvider* provider;

		while (true) {
			if (context == NULL) {
				return NULL;
			}

			provider = dynamic_cast<const ServiceProvider*>(context);

			if (provider != NULL) {
				break;
			}

			context = context->getParent();
		}

		return provider->create<T>();
	}

protected:
	virtual shared_ptr<Instance> createChild(const Name& className);
	virtual void onAddListener(Listener<ServiceProvider, ServiceAdded>* listener) const;
	virtual bool askAddChild(const Instance* instance) const;
	virtual void onChildAdded(Instance* instance);
	virtual void onChildRemoving(Instance* instance);
	virtual void onDescendentAdded(Instance* instance);
	virtual void onDescendentRemoving(const shared_ptr<Instance>& instance);

	void clearServices();

private:
	static unsigned int newIndex();

	template <class T>
	static unsigned int doGetClassIndex()
	{
		static unsigned int index = newIndex();

		return index;
	}

	template <class T>
	static void callDoGetClassIndex()
	{
		doGetClassIndex<T>();
	}

	template <class T>
	static unsigned int getClassIndex()
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;

		boost::call_once(&callDoGetClassIndex<T>, flag);

		return doGetClassIndex<T>();
	}

	boost::shared_ptr<Instance> findServiceByClassName(const Name& className) const;
	boost::shared_ptr<Instance> findServiceByClassNameString(std::string sName);

	static Reflection::BoundFuncDesc<ServiceProvider, boost::shared_ptr<Instance>(std::string), 1> func_service;
	static Reflection::BoundFuncDesc<ServiceProvider, boost::shared_ptr<Instance>(std::string), 1> func_GetService;
};

template <class T>
class ServiceClient
{
private:
	Instance* context;
	mutable boost::shared_ptr<T> service;

public:
	ServiceClient(Instance* context) : context(context) {}

	operator T*() { return createService(true); }
	operator const T*() const { return createService(true); }
	T* operator->() { return createService(true); }
	const T* operator->() const { return createService(true); }

private:
	T* createService(bool createIfNotFound) const
	{
		if (!service) {
			service = shared_from(ServiceProvider::create<T>(context));
		}

		return service.get();
	}
};

} // namespace RBX

#endif // V8TREE_SERVICE_H
