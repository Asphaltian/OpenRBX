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
	ServiceAdded(const ServiceAdded& other) : service(other.service) {}

private:
	ServiceAdded& operator=(const ServiceAdded& other);
};

DECOMP_SIZE_ASSERT(ServiceAdded, 0x8)

// SIZE 0x8
struct ServiceRemoving
{
	const boost::shared_ptr<Instance> service; // 0x00

	ServiceRemoving(Instance* service) : service(shared_from(service)) {}
	ServiceRemoving(const ServiceRemoving& other) : service(other.service) {}

private:
	ServiceRemoving& operator=(const ServiceRemoving& other);
};

DECOMP_SIZE_ASSERT(ServiceRemoving, 0x8)

// clang-format off
// TEMPLATE: WEBSERVICE 0x1000a7b0
// STUB: ROBLOX 0x00476c30
// RBX::ServiceProvider::create<RBX::Lighting>
// TEMPLATE: WEBSERVICE 0x1000bb10
// STUB: ROBLOX 0x00476730
// RBX::ServiceProvider::find<RBX::Lighting>
// TEMPLATE: WEBSERVICE 0x1000c2e0
// RBX::ServiceProvider::callDoGetClassIndex<RBX::Lighting>
// TEMPLATE: WEBSERVICE 0x1000c2f0
// RBX::ServiceProvider::doGetClassIndex<RBX::Lighting>
// TEMPLATE: WEBSERVICE 0x1000c360
// RBX::ServiceProvider::callDoGetClassIndex<RBX::Workspace>
// TEMPLATE: WEBSERVICE 0x1001e340
// RBX::ServiceProvider::callDoGetClassIndex<RBX::ScriptContext>
// TEMPLATE: WEBSERVICE 0x10042970
// RBX::ServiceProvider::doGetClassIndex<RBX::RunService>
// TEMPLATE: WEBSERVICE 0x10043180
// RBX::ServiceProvider::callDoGetClassIndex<RBX::RunService>
// TEMPLATE: WEBSERVICE 0x10045820
// STUB: ROBLOX 0x004580a0
// RBX::ServiceProvider::find<RBX::RunService>
// FUNCTION: WEBSERVICE 0x10058da0
// FUNCTION: ROBLOX 0x0058bea0
// RBX::Notifier<RBX::ServiceProvider,RBX::ServiceAdded>::raise
// FUNCTION: WEBSERVICE 0x10058ec0
// FUNCTION: ROBLOX 0x0058bfc0
// RBX::Notifier<RBX::ServiceProvider,RBX::ServiceRemoving>::raise
// FUNCTION: WEBSERVICE 0x10058fe0
// FUNCTION: ROBLOX 0x0058c0e0
// RBX::Notifier<RBX::ServiceProvider,RBX::ServiceAdded>::raise
// FUNCTION: WEBSERVICE 0x100590c0
// FUNCTION: ROBLOX 0x0058c1c0
// RBX::Notifier<RBX::ServiceProvider,RBX::ServiceRemoving>::raise
// TEMPLATE: WEBSERVICE 0x1005f5d0
// RBX::ServiceProvider::doGetClassIndex<RBX::Stats::StatsService>
// TEMPLATE: WEBSERVICE 0x10060a60
// RBX::ServiceProvider::callDoGetClassIndex<RBX::Stats::StatsService>
// TEMPLATE: WEBSERVICE 0x10064890
// STUB: ROBLOX 0x0045c0b0
// RBX::ServiceProvider::find<RBX::Stats::StatsService>
// TEMPLATE: WEBSERVICE 0x10064d40
// STUB: ROBLOX 0x00458420
// RBX::ServiceProvider::create<RBX::RunService>
// TEMPLATE: WEBSERVICE 0x10065ba0
// STUB: ROBLOX 0x0045cb90
// RBX::ServiceProvider::create<RBX::Stats::StatsService>
// TEMPLATE: WEBSERVICE 0x100666a0
// TEMPLATE: ROBLOX 0x00572f20
// RBX::ServiceProvider::create<RBX::Stats::StatsService>
// TEMPLATE: WEBSERVICE 0x1006a590
// RBX::ServiceProvider::doGetClassIndex<RBX::ControllerService>
// TEMPLATE: WEBSERVICE 0x1006a5f0
// RBX::ServiceProvider::doGetClassIndex<RBX::StarterPackService>
// TEMPLATE: WEBSERVICE 0x1006a650
// RBX::ServiceProvider::doGetClassIndex<RBX::Selection>
// TEMPLATE: WEBSERVICE 0x1006b170
// RBX::ServiceProvider::callDoGetClassIndex<RBX::ControllerService>
// TEMPLATE: WEBSERVICE 0x1006b180
// RBX::ServiceProvider::callDoGetClassIndex<RBX::StarterPackService>
// TEMPLATE: WEBSERVICE 0x1006b220
// RBX::ServiceProvider::callDoGetClassIndex<RBX::Selection>
// TEMPLATE: WEBSERVICE 0x1006d570
// STUB: ROBLOX 0x004aaae0
// RBX::ServiceProvider::find<RBX::StarterPackService>
// FUNCTION: WEBSERVICE 0x1006e4c0
// STUB: ROBLOX 0x00461fc0
// RBX::ServiceProvider::find<RBX::ControllerService>
// TEMPLATE: WEBSERVICE 0x1006e670
// STUB: ROBLOX 0x004cbe00
// RBX::ServiceProvider::create<RBX::StarterPackService>
// TEMPLATE: WEBSERVICE 0x1006e760
// STUB: ROBLOX 0x00462180
// RBX::ServiceProvider::create<RBX::ControllerService>
// TEMPLATE: WEBSERVICE 0x1006ea80
// TEMPLATE: ROBLOX 0x005b6ea0
// RBX::ServiceProvider::find<RBX::ControllerService>
// TEMPLATE: WEBSERVICE 0x1006eb20
// TEMPLATE: ROBLOX 0x005b6ef0
// RBX::ServiceProvider::create<RBX::ControllerService>
// FUNCTION: WEBSERVICE 0x10071ad0
// FUNCTION: ROBLOX 0x00581d20
// RBX::NonFactoryProduct<RBX::Instance,&RBX::sServiceProvider>::NonFactoryProduct<RBX::Instance,&RBX::sServiceProvider>
// TEMPLATE: WEBSERVICE 0x100722f0
// TEMPLATE: ROBLOX 0x0058efb0
// RBX::ServiceProvider::doGetClassIndex<RBX::LocalBackpack>
// TEMPLATE: WEBSERVICE 0x10072350
// RBX::ServiceProvider::doGetClassIndex<RBX::Soundscape::SoundService>
// TEMPLATE: WEBSERVICE 0x100723b0
// RBX::ServiceProvider::doGetClassIndex<RBX::JointsService>
// TEMPLATE: WEBSERVICE 0x10072450
// RBX::ServiceProvider::doGetClassIndex<RBX::Network::Players>
// TEMPLATE: WEBSERVICE 0x100724b0
// RBX::ServiceProvider::doGetClassIndex<RBX::Teams>
// TEMPLATE: WEBSERVICE 0x100731f0
// TEMPLATE: ROBLOX 0x00590180
// RBX::ServiceProvider::callDoGetClassIndex<RBX::LocalBackpack>
// TEMPLATE: WEBSERVICE 0x10073200
// RBX::ServiceProvider::callDoGetClassIndex<RBX::Soundscape::SoundService>
// TEMPLATE: WEBSERVICE 0x10073210
// RBX::ServiceProvider::callDoGetClassIndex<RBX::JointsService>
// TEMPLATE: WEBSERVICE 0x100733a0
// RBX::ServiceProvider::callDoGetClassIndex<RBX::Network::Players>
// TEMPLATE: WEBSERVICE 0x100733b0
// RBX::ServiceProvider::callDoGetClassIndex<RBX::Teams>
// TEMPLATE: WEBSERVICE 0x10077fb0
// TEMPLATE: ROBLOX 0x00591ee0
// RBX::ServiceProvider::find<RBX::LocalBackpack>
// TEMPLATE: WEBSERVICE 0x10078160
// RBX::ServiceProvider::find<RBX::Soundscape::SoundService>
// TEMPLATE: WEBSERVICE 0x10078310
// RBX::ServiceProvider::find<RBX::JointsService>
// FUNCTION: WEBSERVICE 0x100784c0
// STUB: ROBLOX 0x004b35a0
// RBX::ServiceProvider::find<RBX::Teams>
// FUNCTION: WEBSERVICE 0x10078670
// STUB: ROBLOX 0x0040f210
// RBX::ServiceProvider::find<RBX::Network::Players>
// TEMPLATE: WEBSERVICE 0x10078b10
// TEMPLATE: ROBLOX 0x00592190
// RBX::ServiceProvider::create<RBX::LocalBackpack>
// TEMPLATE: WEBSERVICE 0x10078cf0
// STUB: ROBLOX 0x004cbf00
// RBX::ServiceProvider::create<RBX::JointsService>
// TEMPLATE: WEBSERVICE 0x10078de0
// STUB: ROBLOX 0x0040f3d0
// RBX::ServiceProvider::create<RBX::Network::Players>
// FUNCTION: WEBSERVICE 0x10079520
// FUNCTION: ROBLOX 0x005822b0
// RBX::DescribedNonCreatable<RBX::ServiceProvider,RBX::Instance,&RBX::sServiceProvider>::DescribedNonCreatable<RBX::ServiceProvider,RBX::Instance,&RBX::sServiceProvider>
// SYNTHETIC: WEBSERVICE 0x10079940
// STUB: ROBLOX 0x00582350
// RBX::ServiceProvider::~ServiceProvider
// FUNCTION: WEBSERVICE 0x1007aa60
// FUNCTION: ROBLOX 0x00582480
// RBX::ServiceProvider::ServiceProvider
// TEMPLATE: WEBSERVICE 0x100a0ad0
// TEMPLATE: ROBLOX 0x005e9040
// RBX::ServiceProvider::doGetClassIndex<RBX::FilteredSelection<RBX::PartInstance> >
// TEMPLATE: WEBSERVICE 0x100a1050
// TEMPLATE: ROBLOX 0x005e9640
// RBX::ServiceProvider::callDoGetClassIndex<RBX::FilteredSelection<RBX::PartInstance> >
// TEMPLATE: WEBSERVICE 0x100a3410
// TEMPLATE: ROBLOX 0x005eba50
// RBX::ServiceProvider::find<RBX::FilteredSelection<RBX::PartInstance> >
// TEMPLATE: WEBSERVICE 0x100a41f0
// TEMPLATE: ROBLOX 0x004b3fc0
// RBX::ServiceProvider::find<RBX::Teams>
// TEMPLATE: WEBSERVICE 0x100bf1d0
// TEMPLATE: ROBLOX 0x00620e30
// RBX::ServiceProvider::doGetClassIndex<RBX::FilteredSelection<RBX::Instance> >
// TEMPLATE: WEBSERVICE 0x100bf300
// TEMPLATE: ROBLOX 0x00620f00
// RBX::ServiceProvider::callDoGetClassIndex<RBX::FilteredSelection<RBX::Instance> >
// TEMPLATE: WEBSERVICE 0x100c0280
// TEMPLATE: ROBLOX 0x00621ca0
// RBX::ServiceProvider::find<RBX::FilteredSelection<RBX::Instance> >
// TEMPLATE: WEBSERVICE 0x100cd240
// RBX::ServiceProvider::doGetClassIndex<RBX::TimerService>
// TEMPLATE: WEBSERVICE 0x100cd2a0
// TEMPLATE: ROBLOX 0x00622de0
// RBX::ServiceProvider::doGetClassIndex<RBX::FlagStandService>
// TEMPLATE: WEBSERVICE 0x100cd440
// RBX::ServiceProvider::callDoGetClassIndex<RBX::TimerService>
// TEMPLATE: WEBSERVICE 0x100cd450
// TEMPLATE: ROBLOX 0x00622fa0
// RBX::ServiceProvider::callDoGetClassIndex<RBX::FlagStandService>
// FUNCTION: WEBSERVICE 0x100cdf90
// RBX::ServiceProvider::find<RBX::TimerService>
// TEMPLATE: WEBSERVICE 0x100ce4f0
// STUB: ROBLOX 0x004abdb0
// RBX::ServiceProvider::create<RBX::TimerService>
// TEMPLATE: WEBSERVICE 0x100dac50
// TEMPLATE: ROBLOX 0x004abc70
// RBX::ServiceProvider::find<RBX::Network::Players>
// TEMPLATE: WEBSERVICE 0x100fd6c0
// STUB: ROBLOX 0x004a5940
// RBX::ServiceProvider::doGetClassIndex<RBX::SpawnerService>
// TEMPLATE: WEBSERVICE 0x100fd720
// TEMPLATE: ROBLOX 0x005e43f0
// RBX::ServiceProvider::doGetClassIndex<RBX::DebrisService>
// TEMPLATE: WEBSERVICE 0x100fda50
// TEMPLATE: ROBLOX 0x004a60f0
// RBX::ServiceProvider::callDoGetClassIndex<RBX::SpawnerService>
// TEMPLATE: WEBSERVICE 0x100fda60
// TEMPLATE: ROBLOX 0x005e46b0
// RBX::ServiceProvider::callDoGetClassIndex<RBX::DebrisService>
// TEMPLATE: WEBSERVICE 0x100fdf10
// TEMPLATE: ROBLOX 0x005e4ae0
// RBX::ServiceProvider::find<RBX::DebrisService>
// TEMPLATE: WEBSERVICE 0x100fe0c0
// STUB: ROBLOX 0x004aa920
// RBX::ServiceProvider::find<RBX::SpawnerService>
// TEMPLATE: WEBSERVICE 0x100feea0
// TEMPLATE: ROBLOX 0x005e58f0
// RBX::ServiceProvider::create<RBX::SpawnerService>
// TEMPLATE: WEBSERVICE 0x10106140
// TEMPLATE: ROBLOX 0x00640870
// RBX::ServiceProvider::doGetClassIndex<RBX::GeometryService>
// TEMPLATE: WEBSERVICE 0x10106670
// TEMPLATE: ROBLOX 0x00640ce0
// RBX::ServiceProvider::callDoGetClassIndex<RBX::GeometryService>
// FUNCTION: WEBSERVICE 0x10106790
// FUNCTION: ROBLOX 0x00640e10
// RBX::ServiceProvider::find<RBX::GeometryService>
// TEMPLATE: WEBSERVICE 0x10106940
// TEMPLATE: ROBLOX 0x00640fc0
// RBX::ServiceProvider::create<RBX::GeometryService>
// TEMPLATE: WEBSERVICE 0x10106a30
// TEMPLATE: ROBLOX 0x006410b0
// RBX::ServiceProvider::create<RBX::GeometryService>
// TEMPLATE: WEBSERVICE 0x1018e8f0
// TEMPLATE: ROBLOX 0x004abd60
// RBX::ServiceProvider::find<RBX::StarterPackService>
// TEMPLATE: WEBSERVICE 0x1018f3b0
// TEMPLATE: ROBLOX 0x004acc80
// RBX::ServiceProvider::create<RBX::TimerService>
// TEMPLATE: WEBSERVICE 0x10196910
// TEMPLATE: ROBLOX 0x004b8af0
// RBX::ServiceProvider::doGetClassIndex<RBX::Network::Client>
// TEMPLATE: WEBSERVICE 0x10196c00
// TEMPLATE: ROBLOX 0x004b8da0
// RBX::ServiceProvider::callDoGetClassIndex<RBX::Network::Client>
// TEMPLATE: WEBSERVICE 0x10197890
// TEMPLATE: ROBLOX 0x004b9b30
// RBX::ServiceProvider::find<RBX::Network::Client>
// TEMPLATE: WEBSERVICE 0x101993d0
// TEMPLATE: ROBLOX 0x004bb650
// RBX::ServiceProvider::doGetClassIndex<RBX::Network::Server>
// TEMPLATE: WEBSERVICE 0x101998e0
// TEMPLATE: ROBLOX 0x004bbad0
// RBX::ServiceProvider::callDoGetClassIndex<RBX::Network::Server>
// TEMPLATE: WEBSERVICE 0x1019a5e0
// TEMPLATE: ROBLOX 0x004bc7d0
// RBX::ServiceProvider::find<RBX::Network::Server>
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
	static Instance* create(Instance* context, const Name& name);

	// FUNCTION: WEBSERVICE 0x1007c960
	// FUNCTION: ROBLOX 0x004a4d40
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

DECOMP_SIZE_ASSERT(ServiceProvider, 0x15c)

// SIZE 0x0c
template <class T>
class ServiceClient
{
private:
	Instance* context;                    // 0x00
	mutable boost::shared_ptr<T> service; // 0x04

public:
	ServiceClient(Instance* context) : context(context) {}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1006f250
	// STUB: ROBLOX 0x0059a540
	// RBX::ServiceClient<RBX::Selection>::createService
	// TEMPLATE: WEBSERVICE 0x100a49c0
	// TEMPLATE: ROBLOX 0x005ed060
	// RBX::ServiceClient<RBX::FilteredSelection<RBX::PartInstance> >::createService
	// TEMPLATE: WEBSERVICE 0x100c0570
	// TEMPLATE: ROBLOX 0x00621f90
	// RBX::ServiceClient<RBX::FilteredSelection<RBX::Instance> >::createService
	// clang-format on
	operator T*() { return createService(true); }
	T* operator->() { return createService(true); }

private:
	T* createService(bool createIfNotFound) const
	{
		if (!service) {
			service = shared_from(ServiceProvider::create<T>(context));
		}

		return service.get();
	}
};

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10218870
// `dynamic initializer for 'RBX::ServiceProvider::func_service''
// SYNTHETIC: WEBSERVICE 0x102188a0
// `dynamic initializer for 'RBX::ServiceProvider::func_GetService''
// SYNTHETIC: WEBSERVICE 0x102218e0
// `dynamic atexit destructor for 'RBX::ServiceProvider::func_GetService''
// SYNTHETIC: WEBSERVICE 0x102218f0
// `dynamic atexit destructor for 'RBX::ServiceProvider::func_service''
// clang-format on

} // namespace RBX

#endif // V8TREE_SERVICE_H
