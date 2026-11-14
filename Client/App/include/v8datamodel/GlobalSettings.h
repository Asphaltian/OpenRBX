#ifndef V8DATAMODEL_GLOBALSETTINGS_H
#define V8DATAMODEL_GLOBALSETTINGS_H

#include "placeholder.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace RBX {

extern const char sGlobalSettings[];

class GlobalSettings : public DescribedNonCreatable<GlobalSettings, ServiceProvider, sGlobalSettings>
{
public:
	GlobalSettings();

	static boost::recursive_mutex mutex;

	static boost::shared_ptr<GlobalSettings> singleton();
	void loadState();
	void saveState();
	void eraseSettingsStore();

	class Item : public NonFactoryProduct<Instance, static_cast<const char*>(0)>
	{
	protected:
		virtual bool askAddChild(const Instance* instance) const { return dynamic_cast<const Item*>(instance) != NULL; }
	};
};

template <class T, const char* sName>
class GlobalSettingsItem : public DescribedCreatable<T, GlobalSettings::Item, sName>, public Service
{
private:
	static GlobalSettingsItem* sing;

protected:
	GlobalSettingsItem()
	{
		setName(sName);
		sing = this;
	}

	virtual ~GlobalSettingsItem() { sing = NULL; }

public:
	static T& singleton()
	{
		boost::recursive_mutex::scoped_lock lock(GlobalSettings::mutex);
		if (sing == NULL) {
			boost::shared_ptr<T> s = Creatable<Instance>::create<T>();
			s->setParent(GlobalSettings::singleton().get());
		}
		return static_cast<T&>(*sing);
	}
};

template <class T, const char* sName>
GlobalSettingsItem<T, sName>* GlobalSettingsItem<T, sName>::sing = NULL;

} // namespace RBX

#endif // V8DATAMODEL_GLOBALSETTINGS_H
