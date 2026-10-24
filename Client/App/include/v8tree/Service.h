#ifndef V8TREE_SERVICE_H
#define V8TREE_SERVICE_H

#include "decomp.h"
#include "util/Events.h"
#include "util/object.h"
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

	ServiceAdded(Instance* service) : service(shared_from(service)) {}

private:
	ServiceAdded& operator=(const ServiceAdded& other);
};

DECOMP_SIZE_ASSERT(ServiceAdded, 0x8)

// SIZE 0x8
struct ServiceRemoving
{
	const boost::shared_ptr<Instance> service; // 0x00

	ServiceRemoving(Instance* service) : service(shared_from(service)) {}

private:
	ServiceRemoving& operator=(const ServiceRemoving& other);
};

DECOMP_SIZE_ASSERT(ServiceRemoving, 0x8)

// clang-format off
// TEMPLATE: WEBSERVICE 0x10042970
// RBX::ServiceProvider::doGetClassIndex<RBX::RunService>
// FUNCTION: WEBSERVICE 0x10058da0
// RBX::Notifier<RBX::ServiceProvider,RBX::ServiceAdded>::raise
// FUNCTION: WEBSERVICE 0x10058ec0
// RBX::Notifier<RBX::ServiceProvider,RBX::ServiceRemoving>::raise
// FUNCTION: WEBSERVICE 0x10058fe0
// RBX::Notifier<RBX::ServiceProvider,RBX::ServiceAdded>::raise
// FUNCTION: WEBSERVICE 0x100590c0
// RBX::Notifier<RBX::ServiceProvider,RBX::ServiceRemoving>::raise
// TEMPLATE: WEBSERVICE 0x1006a590
// RBX::ServiceProvider::doGetClassIndex<RBX::ControllerService>
// TEMPLATE: WEBSERVICE 0x1006b170
// RBX::ServiceProvider::callDoGetClassIndex<RBX::ControllerService>
// STUB: WEBSERVICE 0x1006e4c0
// RBX::ServiceProvider::find<RBX::ControllerService>
// TEMPLATE: WEBSERVICE 0x1006e760
// RBX::ServiceProvider::create<RBX::ControllerService>
// TEMPLATE: WEBSERVICE 0x1006ea80
// RBX::ServiceProvider::find<RBX::ControllerService>
// TEMPLATE: WEBSERVICE 0x1006eb20
// RBX::ServiceProvider::create<RBX::ControllerService>
// FUNCTION: WEBSERVICE 0x10071ad0
// RBX::NonFactoryProduct<RBX::Instance,&RBX::sServiceProvider>::NonFactoryProduct<RBX::Instance,&RBX::sServiceProvider>
// TEMPLATE: WEBSERVICE 0x10072450
// RBX::ServiceProvider::doGetClassIndex<RBX::Network::Players>
// TEMPLATE: WEBSERVICE 0x100724b0
// RBX::ServiceProvider::doGetClassIndex<RBX::Teams>
// TEMPLATE: WEBSERVICE 0x100733a0
// RBX::ServiceProvider::callDoGetClassIndex<RBX::Network::Players>
// TEMPLATE: WEBSERVICE 0x100733b0
// RBX::ServiceProvider::callDoGetClassIndex<RBX::Teams>
// STUB: WEBSERVICE 0x100784c0
// RBX::ServiceProvider::find<RBX::Teams>
// STUB: WEBSERVICE 0x10078670
// RBX::ServiceProvider::find<RBX::Network::Players>
// FUNCTION: WEBSERVICE 0x10079520
// RBX::DescribedNonCreatable<RBX::ServiceProvider,RBX::Instance,&RBX::sServiceProvider>::DescribedNonCreatable<RBX::ServiceProvider,RBX::Instance,&RBX::sServiceProvider>
// FUNCTION: WEBSERVICE 0x1007aa60
// RBX::ServiceProvider::ServiceProvider
// TEMPLATE: WEBSERVICE 0x100a41f0
// RBX::ServiceProvider::find<RBX::Teams>
// TEMPLATE: WEBSERVICE 0x100cd240
// RBX::ServiceProvider::doGetClassIndex<RBX::TimerService>
// TEMPLATE: WEBSERVICE 0x100cd440
// RBX::ServiceProvider::callDoGetClassIndex<RBX::TimerService>
// STUB: WEBSERVICE 0x100cdf90
// RBX::ServiceProvider::find<RBX::TimerService>
// TEMPLATE: WEBSERVICE 0x100ce4f0
// RBX::ServiceProvider::create<RBX::TimerService>
// TEMPLATE: WEBSERVICE 0x100dac50
// RBX::ServiceProvider::find<RBX::Network::Players>
// TEMPLATE: WEBSERVICE 0x10106140
// RBX::ServiceProvider::doGetClassIndex<RBX::GeometryService>
// TEMPLATE: WEBSERVICE 0x10106670
// RBX::ServiceProvider::callDoGetClassIndex<RBX::GeometryService>
// TEMPLATE: WEBSERVICE 0x10043180
// RBX::ServiceProvider::callDoGetClassIndex<RBX::RunService>
// STUB: WEBSERVICE 0x10106790
// RBX::ServiceProvider::find<RBX::GeometryService>
// TEMPLATE: WEBSERVICE 0x10106940
// RBX::ServiceProvider::create<RBX::GeometryService>
// TEMPLATE: WEBSERVICE 0x10106a30
// RBX::ServiceProvider::create<RBX::GeometryService>
// SYNTHETIC: WEBSERVICE 0x10079940
// RBX::ServiceProvider::~ServiceProvider
// SYNTHETIC: WEBSERVICE 0x102218d0
// `RBX::Reflection::Described<RBX::ServiceProvider,&RBX::sServiceProvider,RBX::NonFactoryProduct<RBX::Instance,&RBX::sServiceProvider> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// clang-format on
// SIZE 0x15c
class ServiceProvider : public DescribedNonCreatable<ServiceProvider, Instance, sServiceProvider>,
						public Notifier<ServiceProvider, Closing>,
						public Notifier<ServiceProvider, ServiceAdded>,
						public Notifier<ServiceProvider, ServiceRemoving>
{
private:
	mutable std::vector<boost::shared_ptr<Instance> > serviceArray; // 0x140
	std::map<const Name*, boost::shared_ptr<Instance> > serviceMap; // 0x150

public:
	// FUNCTION: WEBSERVICE 0x1007c960
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
	Instance* findServiceByClassNameString(const std::string& className);
};

DECOMP_SIZE_ASSERT(ServiceProvider, 0x15c)

} // namespace RBX

#endif // V8TREE_SERVICE_H
