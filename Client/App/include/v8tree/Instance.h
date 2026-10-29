#ifndef V8TREE_INSTANCE_H
#define V8TREE_INSTANCE_H

#include "decomp.h"
#include "reflection/property.h"
#include "reflection/reflection.h"
#include "reflection/signal.h"
#include "util/Association.h"
#include "util/Events.h"
#include "util/Guid.h"
#include "util/Utilities.h"
#include "util/object.h"
#include "v8xml/XmlElement.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace RBX {

using boost::shared_ptr;

class IReferenceBinder;
class Instance;
class Name;
class ServiceProvider;

// SIZE 0x8
struct ChildAdded
{
	const boost::shared_ptr<Instance> child; // 0x00

	ChildAdded(Instance* child);
	ChildAdded(const ChildAdded& other) : child(other.child) {}

private:
	ChildAdded& operator=(const ChildAdded& other);
};

// SIZE 0x8
struct ChildRemoved
{
	const boost::shared_ptr<Instance> child; // 0x00

	ChildRemoved(Instance* child);
	ChildRemoved(const ChildRemoved& other) : child(other.child) {}

private:
	ChildRemoved& operator=(const ChildRemoved& other);
};

// SIZE 0x10
struct DescendentAdded
{
	const boost::shared_ptr<Instance> instance; // 0x00
	const boost::shared_ptr<Instance> parent;   // 0x08

private:
	friend class Instance;
	friend class RaiseDescendentAdded2;

	// FUNCTION: WEBSERVICE 0x10049ad0
	// RBX::DescendentAdded::DescendentAdded

	DescendentAdded(Instance* instance, Instance* parent) : instance(shared_from(instance)), parent(shared_from(parent))
	{
	}
};

// SIZE 0x10
struct DescendentRemoving
{
	const boost::shared_ptr<Instance> instance; // 0x00
	const boost::shared_ptr<Instance> parent;   // 0x08

	DescendentRemoving(const boost::shared_ptr<Instance>& instance, const boost::shared_ptr<Instance>& parent)
		: instance(instance), parent(parent)
	{
	}
};
// SIZE 0xc
struct AncestorChanged
{
	Instance* const child;     // 0x00
	Instance* const oldParent; // 0x04
	Instance* const newParent; // 0x08

	AncestorChanged(Instance* child, Instance* oldParent, Instance* newParent)
		: child(child), oldParent(oldParent), newParent(newParent)
	{
	}
};
// SIZE 0x8
class PropertyChanged
{
public:
	PropertyChanged(const PropertyChanged& other) : property(other.property) {}

	const Reflection::Property& getProperty() const { return property; }

	const Reflection::PropertyDescriptor& getDescriptor() const { return property.getDescriptor(); }

private:
	friend class Instance;

	PropertyChanged(const Reflection::Property& property) : property(property) {}

	Reflection::Property property; // 0x00
};

DECOMP_SIZE_ASSERT(PropertyChanged, 0x8)

extern const char sInstance[];

class Instance : public boost::enable_shared_from_this<Instance>,
				 public GuidItem<Instance>,
				 public AbstractFactoryProduct<Instance>,
				 public Reflection::Described<Instance, sInstance, Reflection::DescribedBase>,
				 public Debugable,
				 public Notifier<Instance, ChildAdded>,
				 public Notifier<Instance, ChildRemoved>,
				 public Notifier<Instance, DescendentAdded>,
				 public Notifier<Instance, DescendentRemoving>,
				 public Notifier<Instance, AncestorChanged>,
				 public Notifier<Instance, PropertyChanged>,
				 public boost::noncopyable
{
protected:
	Instance();
	Instance(const char* name);

	virtual ~Instance(); // vtable+0x00

public:
	virtual void setName(const std::string& value); // vtable+0x04

protected:
	virtual bool askAddChild(const Instance* instance) const;  // vtable+0x08
	virtual bool askSetParent(const Instance* instance) const; // vtable+0x0c
	virtual void onAddListener(Listener<Instance, DescendentAdded>* listener) const;
	virtual void onAddListener(Listener<Instance, ChildAdded>* listener) const;
	virtual void onAncestorChanged(const AncestorChanged& event);            // vtable+0x10
	virtual void onDescendentAdded(Instance* instance);                      // vtable+0x14
	virtual void onDescendentRemoving(const shared_ptr<Instance>& instance); // vtable+0x18
	virtual void onChildAdded(Instance* child) {}                            // vtable+0x1c

	virtual void onChildRemoving(Instance* child) {} // vtable+0x20

	virtual void onChildRemoved(Instance* child) {} // vtable+0x24

private:
	virtual void onLastChildRemoved() {} // vtable+0x28

protected:
	virtual void readProperty(const XmlElement* element, IReferenceBinder& binder); // vtable+0x2c

public:
	virtual void onServiceProvider(
		const ServiceProvider* oldProvider, const ServiceProvider* newProvider
	);                                                               // vtable+0x30
	virtual shared_ptr<Instance> createChild(const Name& className); // vtable+0x34
	virtual XmlElement* write();                                     // vtable+0x38

protected:
	virtual void onChildChanged(Instance* child, const PropertyChanged& event); // vtable+0x3c

public:
	static Reflection::SignalDesc<Instance, void(shared_ptr<Instance>)> event_childAdded;
	static Reflection::SignalDesc<Instance, void(shared_ptr<Instance>)> event_childRemoved;
	static Reflection::SignalDesc<Instance, void(shared_ptr<Instance>)> event_descendentAdded;
	static Reflection::SignalDesc<Instance, void(shared_ptr<Instance>)> event_descendentRemoving;
	static Reflection::SignalDesc<Instance, void(shared_ptr<Instance>)> event_ancestryChanged;
	static Reflection::SignalDesc<Instance, void(const Reflection::PropertyDescriptor*)> event_propertyChanged;

	static Reflection::BoundProp<bool, 1> propArchivable;
	static const Reflection::PropDescriptor<Instance, std::string> desc_Name;
	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10047f40
	// ?raise@?$Notifier@VInstance@RBX@@UDescendentAdded@2@@RBX@@IBEXUDescendentAdded@2@PAV?$Listener@VInstance@RBX@@UDescendentAdded@2@@2@@Z
	// TEMPLATE: WEBSERVICE 0x100487e0
	// ?raise@?$Notifier@VInstance@RBX@@UDescendentRemoving@2@@RBX@@IBEXUDescendentRemoving@2@PAV?$Listener@VInstance@RBX@@UDescendentRemoving@2@@2@@Z
	// TEMPLATE: WEBSERVICE 0x100488f0
	// ?raise@?$Notifier@VInstance@RBX@@UAncestorChanged@2@@RBX@@IBEXUAncestorChanged@2@PAV?$Listener@VInstance@RBX@@UAncestorChanged@2@@2@@Z
	// TEMPLATE: WEBSERVICE 0x10048af0
	// ?raise@?$Notifier@VInstance@RBX@@UDescendentAdded@2@@RBX@@IBEXUDescendentAdded@2@@Z
	// TEMPLATE: WEBSERVICE 0x100493e0
	// ?raise@?$Notifier@VInstance@RBX@@UDescendentRemoving@2@@RBX@@IBEXUDescendentRemoving@2@@Z
	// TEMPLATE: WEBSERVICE 0x100494c0
	// ?raise@?$Notifier@VInstance@RBX@@UAncestorChanged@2@@RBX@@IBEXUAncestorChanged@2@@Z
	// SYNTHETIC: WEBSERVICE 0x10218450
	// `dynamic initializer for 'RBX::Instance::propArchivable''
	// SYNTHETIC: WEBSERVICE 0x10218480
	// RBX::`dynamic initializer for 'findFirstChild''
	// SYNTHETIC: WEBSERVICE 0x102184c0
	// RBX::`dynamic initializer for 'func_clone''
	// SYNTHETIC: WEBSERVICE 0x102184f0
	// RBX::`dynamic initializer for 'func_Remove''
	// SYNTHETIC: WEBSERVICE 0x10218520
	// RBX::`dynamic initializer for 'func_childrenOld''
	// SYNTHETIC: WEBSERVICE 0x10218550
	// RBX::`dynamic initializer for 'func_children''
	// SYNTHETIC: WEBSERVICE 0x10218580
	// RBX::`dynamic initializer for 'func_isDescendentOf''
	// SYNTHETIC: WEBSERVICE 0x102185b0
	// RBX::`dynamic initializer for 'func_isAncestorOf''
	// SYNTHETIC: WEBSERVICE 0x102185e0
	// RBX::`dynamic initializer for 'prop_className''
	// SYNTHETIC: WEBSERVICE 0x10218610
	// `dynamic initializer for 'RBX::Instance::desc_Name''
	// SYNTHETIC: WEBSERVICE 0x10218650
	// `dynamic initializer for 'RBX::Instance::propParent''
	// SYNTHETIC: WEBSERVICE 0x10218750
	// RBX::`dynamic initializer for 'nameName''
	// SYNTHETIC: WEBSERVICE 0x10218770
	// RBX::`dynamic initializer for 'nameParent''
	// SYNTHETIC: WEBSERVICE 0x102216a0
	// `dynamic atexit destructor for 'RBX::Instance::propParent''
	// SYNTHETIC: WEBSERVICE 0x102216c0
	// `dynamic atexit destructor for 'RBX::Instance::propArchivable''
	// SYNTHETIC: WEBSERVICE 0x102216e0
	// `dynamic atexit destructor for 'RBX::Instance::desc_Name''
	// SYNTHETIC: WEBSERVICE 0x10221700
	// RBX::`dynamic atexit destructor for 'prop_className''
	// SYNTHETIC: WEBSERVICE 0x10221720
	// RBX::`dynamic atexit destructor for 'func_children''
	// SYNTHETIC: WEBSERVICE 0x10221730
	// RBX::`dynamic atexit destructor for 'func_isAncestorOf''
	// SYNTHETIC: WEBSERVICE 0x10221740
	// RBX::`dynamic atexit destructor for 'func_clone''
	// SYNTHETIC: WEBSERVICE 0x10221750
	// RBX::`dynamic atexit destructor for 'func_Remove''
	// SYNTHETIC: WEBSERVICE 0x10221760
	// RBX::`dynamic atexit destructor for 'func_childrenOld''
	// SYNTHETIC: WEBSERVICE 0x10221770
	// RBX::`dynamic atexit destructor for 'func_isDescendentOf''
	// SYNTHETIC: WEBSERVICE 0x102217d0
	// RBX::`dynamic atexit destructor for 'findFirstChild''
	// clang-format on
	static const Reflection::RefPropDescriptor<Instance, Instance> propParent;

	const Association<Instance>& association() const { return assoc; }
	Association<Instance>& association() { return assoc; }

	Instance* getParent() const { return parent; }

	void setParent(Instance* newParent);

	// FUNCTION: WEBSERVICE 0x100ab0a0
	void setParent2(boost::shared_ptr<Instance> instance) { setParent(instance.get()); }

	Instance* getRootAncestor() { return parent != NULL ? parent->getRootAncestor() : this; }

	int findChildIndex(const Instance* instance) const;
	void removeAllChildren();

	Instance* findFirstChildByName(const std::string& findName) const;
	Instance* findFirstChildByNameRecursive(const std::string& findName) const;
	boost::shared_ptr<Instance> findFirstChildByName2(std::string findName, bool recursive);

	boost::shared_ptr<Instance> clone();
	static XmlElement* toNewXmlRoot(Instance* instance);
	static boost::shared_ptr<Instance> fromXmlRoot(XmlElement* root);
	void remove();

	template <class T>
	T* findFirstChildOfType() const;

	// FUNCTION: WEBSERVICE 0x10047ab0
	// RBX::Instance::getClassNameStr

	std::string getClassNameStr() const { return getClassName().name; }

	// FUNCTION: WEBSERVICE 0x10047ae0
	// RBX::Instance::getName

	const std::string& getName() const { return name; }

	const CopyOnWrite<std::vector<shared_ptr<Instance> > >& getChildren() const { return children; }

	// FUNCTION: WEBSERVICE 0x10048f00
	// RBX::Instance::getChildren2

	boost::shared_ptr<const std::vector<shared_ptr<Instance> > > getChildren2() { return children.read(); }

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1004a340
	// ??$for_eachChild@VRaiseDescendentAdded2@RBX@@@Instance@RBX@@QBEXVRaiseDescendentAdded2@1@@Z
	// TEMPLATE: WEBSERVICE 0x1005af10
	// ??$for_eachChild@P6AXPAVInstance@RBX@@@Z@Instance@RBX@@QBEXP6AXPAV01@@Z@Z
	// clang-format on
	template <class Func>
	void for_eachChild(Func func) const
	{
		if (children) {
			boost::shared_ptr<const std::vector<shared_ptr<Instance> > > c = children.read();
			std::vector<shared_ptr<Instance> >::const_iterator end = c->end();
			for (std::vector<shared_ptr<Instance> >::const_iterator iter = c->begin(); iter != end; ++iter) {
				func(iter->get());
			}
		}
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x100e9570
	// RBX::Instance::visitDescendents<boost::_bi::bind_t<void,void (__cdecl*)(boost::shared_ptr<RBX::Instance>,RBX::Adorn *,int),boost::_bi::list3<boost::arg<1>,boost::_bi::value<RBX::Adorn *>,boost::_bi::value<int> > > >
	// TEMPLATE: WEBSERVICE 0x100a1e70
	// RBX::Instance::visitDescendents<boost::_bi::bind_t<void,void (__cdecl*)(boost::shared_ptr<RBX::Instance>,std::vector<RBX::Primitive *,std::allocator<RBX::Primitive *> > &),boost::_bi::list2<boost::arg<1>,boost::reference_wrapper<std::vector<RBX::Primitive *,std::allocator<RBX::Primitive *> > > > > >
	// TEMPLATE: WEBSERVICE 0x1016a8f0
	// RBX::Instance::visitDescendents<boost::_bi::bind_t<void,boost::_mfi::mf1<void,RBX::View::View,boost::shared_ptr<RBX::Instance> >,boost::_bi::list2<boost::_bi::value<RBX::View::View *>,boost::arg<1> > > >
	// clang-format on
	template <class Func>
	void visitDescendents(Func func) const
	{
		if (children) {
			boost::shared_ptr<const std::vector<shared_ptr<Instance> > > c = children.read();
			std::vector<shared_ptr<Instance> >::const_iterator end = c->end();
			for (std::vector<shared_ptr<Instance> >::const_iterator iter = c->begin(); iter != end; ++iter) {
				func(*iter);
				(*iter)->visitDescendents(func);
			}
		}
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1016ee10
	// RBX::Instance::visitChildren<boost::_bi::bind_t<void,boost::_mfi::mf1<void,RBX::View::PartChunk,boost::shared_ptr<RBX::Instance> >,boost::_bi::list2<boost::_bi::value<RBX::View::PartChunk *>,boost::arg<1> > > >
	// TEMPLATE: WEBSERVICE 0x1019f290
	// RBX::Instance::visitChildren<boost::_bi::bind_t<void,boost::_mfi::mf1<void,RBX::Network::Replicator,boost::shared_ptr<RBX::Instance> >,boost::_bi::list2<boost::_bi::value<RBX::Network::Replicator *>,boost::arg<1> > > >
	// TEMPLATE: WEBSERVICE 0x1019f3a0
	// RBX::Instance::visitChildren<boost::_bi::bind_t<bool,boost::_mfi::mf1<bool,RBX::Network::Replicator,boost::shared_ptr<RBX::Instance> >,boost::_bi::list2<boost::_bi::value<RBX::Network::Replicator *>,boost::arg<1> > > >
	// clang-format on
	template <class Func>
	void visitChildren(Func func) const
	{
		if (children) {
			boost::shared_ptr<const std::vector<shared_ptr<Instance> > > c = children.read();
			std::vector<shared_ptr<Instance> >::const_iterator end = c->end();
			for (std::vector<shared_ptr<Instance> >::const_iterator iter = c->begin(); iter != end; ++iter) {
				func(*iter);
			}
		}
	}

	// FUNCTION: WEBSERVICE 0x10048320
	unsigned int numChildren() const { return children ? children->size() : 0; }

	const Instance* getChild(unsigned int index) const { return (*children)[index].get(); }
	Instance* getChild(unsigned int index) { return (*children)[index].get(); }

	// FUNCTION: WEBSERVICE 0x100469f0
	void raisePropertyChanged(const Reflection::PropertyDescriptor& descriptor)
	{
		Reflection::Property property(descriptor, this);
		PropertyChanged event(property);

		Notifier<Instance, PropertyChanged>::raise(property);

		if (!event_propertyChanged.empty(this)) {
			event_propertyChanged.fire(this, &descriptor);
		}

		Instance* parent = this->parent;

		if (parent != NULL) {
			parent->onChildChanged(this, event);
		}
	}

	void readProperties(const XmlElement* element, IReferenceBinder& binder);
	void read(const XmlElement* element, IReferenceBinder& binder);
	void readChildren(const XmlElement* element, IReferenceBinder& binder);
	void readChild(const XmlElement* childElement, IReferenceBinder& binder);
	void writeChildren(XmlElement* element);

	bool contains(const Instance* instance) const;

	bool canAddChild(const boost::shared_ptr<Instance>& instance) const;

	// FUNCTION: WEBSERVICE 0x100ab120
	bool canAddChild(const Instance* instance) const
	{
		if (instance->contains(this)) {
			return false;
		}
		if (instance->parent == this) {
			return false;
		}
		if (askAddChild(instance)) {
			return true;
		}
		if (instance->askSetParent(this)) {
			return true;
		}
		return false;
	}

	// FUNCTION: WEBSERVICE 0x100ab170
	bool canSetParent(const Instance* instance) const { return instance == NULL || instance->canAddChild(this); }

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10059dc0
	// RBX::Instance::getTypedRoot<RBX::PVInstance>
	// FUNCTION: WEBSERVICE 0x10005d00
	// RBX::Instance::getParent
	// clang-format on
	template <class T>
	const T* getTypedRoot() const
	{
		const Instance* root = this;
		const T* typedParent = dynamic_cast<const T*>(parent);

		while (typedParent != NULL) {
			root = typedParent;
			typedParent = dynamic_cast<const T*>(typedParent->parent);
		}

		return static_cast<const T*>(root);
	}

	// FUNCTION: WEBSERVICE 0x10047cd0
	bool isAncestorOf(const Instance* descendent) const
	{
		if (descendent == NULL) {
			return false;
		}

		descendent = descendent->parent;

		if (descendent == this) {
			return true;
		}

		return isAncestorOf(descendent);
	}

	// FUNCTION: WEBSERVICE 0x10047d00
	// RBX::Instance::isAncestorOf2

	bool isAncestorOf2(boost::shared_ptr<Instance> descendent) { return isAncestorOf(descendent.get()); }

	// FUNCTION: WEBSERVICE 0x10048290
	// RBX::Instance::isDescendentOf2

	bool isDescendentOf2(boost::shared_ptr<Instance> ancestor) { return isDescendentOf(ancestor.get()); }

	// FUNCTION: WEBSERVICE 0x10047d90
	bool isDescendentOf(const Instance* ancestor) const
	{
		if (ancestor == parent) {
			return true;
		}

		if (parent != NULL) {
			return parent->isDescendentOf(ancestor);
		}

		return false;
	}

private:
	friend class Creatable<Instance>::Deleter;
	friend class RaiseDescendentAdded2;

	void predelete();
	static void predelete(Instance* instance);

	static void signalDescendentAdded(Instance* instance, Instance* beginParent, Instance* oldParent);
	static void signalDescendentRemoving(
		const shared_ptr<Instance>& instance, Instance* beginParent, Instance* newParent
	);
	void writeProperties(XmlElement* container) const;

	Association<Instance> assoc;                                      // 0xbc
	Instance* parent;                                                 // 0xcc
	CopyOnWrite<std::vector<boost::shared_ptr<Instance> > > children; // 0xd0
	std::string name;                                                 // 0xd8
	bool archivable;                                                  // 0xf4
};

DECOMP_SIZE_ASSERT(Instance, 0xf8)

template <class T, class Base, const char* sName>
class DescribedNonCreatable : public Reflection::Described<T, sName, NonFactoryProduct<Base, sName> >
{
protected:
	DescribedNonCreatable() {}

	template <class U>
	DescribedNonCreatable(U arg0) : Reflection::Described<T, sName, NonFactoryProduct<Base, sName> >(arg0)
	{
	}
};

template <class T, class Base, const char* sName>
// clang-format off
	// TEMPLATE: WEBSERVICE 0x10007950
	// RBX::DescribedCreatable<ThumbnailGenerator,RBX::Instance,&sThumbnailGenerator>::DescribedCreatable<ThumbnailGenerator,RBX::Instance,&sThumbnailGenerator>
	// FUNCTION: WEBSERVICE 0x10043190
	// RBX::DescribedCreatable<RBX::Lighting,RBX::Instance,&RBX::sLighting>::~DescribedCreatable<RBX::Lighting,RBX::Instance,&RBX::sLighting>
	// FUNCTION: WEBSERVICE 0x10043270
	// RBX::Notifier<RBX::Instance,RBX::PropertyChanged>::raise
	// FUNCTION: WEBSERVICE 0x10043920
	// RBX::Notifier<RBX::Instance,RBX::PropertyChanged>::raise
	// FUNCTION: WEBSERVICE 0x100459e0
	// RBX::DescribedCreatable<RBX::Lighting,RBX::Instance,&RBX::sLighting>::DescribedCreatable<RBX::Lighting,RBX::Instance,&RBX::sLighting>
	// FUNCTION: WEBSERVICE 0x1004c040
	// RBX::Reflection::SignalDesc<RBX::Instance,void __cdecl(boost::shared_ptr<RBX::Instance>)>::SignalDesc<RBX::Instance,void __cdecl(boost::shared_ptr<RBX::Instance>)>
	// FUNCTION: WEBSERVICE 0x100543a0
	// RBX::DescribedCreatable<RBX::RunService,RBX::Instance,&RBX::sRunService>::~DescribedCreatable<RBX::RunService,RBX::Instance,&RBX::sRunService>
	// FUNCTION: WEBSERVICE 0x100563e0
	// RBX::DescribedCreatable<RBX::RunService,RBX::Instance,&RBX::sRunService>::DescribedCreatable<RBX::RunService,RBX::Instance,&RBX::sRunService>
	// TEMPLATE: WEBSERVICE 0x1005a880
	// RBX::DescribedCreatable<RBX::ModelInstance,RBX::PVInstance,&RBX::sModel>::~DescribedCreatable<RBX::ModelInstance,RBX::PVInstance,&RBX::sModel>
	// TEMPLATE: WEBSERVICE 0x1005bed0
	// RBX::DescribedCreatable<RBX::ModelInstance,RBX::PVInstance,&RBX::sModel>::DescribedCreatable<RBX::ModelInstance,RBX::PVInstance,&RBX::sModel><char const *>
	// FUNCTION: WEBSERVICE 0x1005cde0
	// RBX::Instance::getRootAncestor
	// FUNCTION: WEBSERVICE 0x1005d000
	// RBX::DescribedCreatable<RBX::Selection,RBX::Instance,&RBX::sSelection>::~DescribedCreatable<RBX::Selection,RBX::Instance,&RBX::sSelection>
	// FUNCTION: WEBSERVICE 0x1005e3f0
	// RBX::DescribedCreatable<RBX::Selection,RBX::Instance,&RBX::sSelection>::DescribedCreatable<RBX::Selection,RBX::Instance,&RBX::sSelection>
	// FUNCTION: WEBSERVICE 0x10061210
	// RBX::DescribedCreatable<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>::~DescribedCreatable<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>
	// TEMPLATE: WEBSERVICE 0x10065320
	// RBX::DescribedNonCreatable<RBX::Stats::Item,RBX::Instance,&RBX::Stats::sStatsItem>::DescribedNonCreatable<RBX::Stats::Item,RBX::Instance,&RBX::Stats::sStatsItem><char const *>
	// TEMPLATE: WEBSERVICE 0x100653c0
	// RBX::DescribedNonCreatable<RBX::Stats::StatsService,RBX::Instance,&RBX::Stats::sStats>::DescribedNonCreatable<RBX::Stats::StatsService,RBX::Instance,&RBX::Stats::sStats><char const *>
	// FUNCTION: WEBSERVICE 0x10065660
	// RBX::DescribedCreatable<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>::DescribedCreatable<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>
	// FUNCTION: WEBSERVICE 0x10068be0
	// RBX::DescribedCreatable<RBX::Script,RBX::Instance,&RBX::sScript>::~DescribedCreatable<RBX::Script,RBX::Instance,&RBX::sScript>
	// FUNCTION: WEBSERVICE 0x10068ff0
	// RBX::DescribedCreatable<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::~DescribedCreatable<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>
	// FUNCTION: WEBSERVICE 0x10069880
	// RBX::DescribedCreatable<RBX::Script,RBX::Instance,&RBX::sScript>::DescribedCreatable<RBX::Script,RBX::Instance,&RBX::sScript><char const *>
	// FUNCTION: WEBSERVICE 0x10069be0
	// RBX::DescribedCreatable<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::DescribedCreatable<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>
// FUNCTION: WEBSERVICE 0x1007d2e0
// RBX::DescribedCreatable<RBX::Soundscape::SoundChannel,RBX::Instance,&RBX::Soundscape::sSoundChannel>::`scalar deleting destructor'
// TEMPLATE: WEBSERVICE 0x1007daf0
// RBX::DescribedCreatable<RBX::Soundscape::SoundChannel,RBX::Instance,&RBX::Soundscape::sSoundChannel>::~DescribedCreatable<RBX::Soundscape::SoundChannel,RBX::Instance,&RBX::Soundscape::sSoundChannel>
	// TEMPLATE: WEBSERVICE 0x10082340
	// RBX::DescribedCreatable<RBX::Soundscape::SoundService,RBX::Instance,&RBX::Soundscape::sSoundService>::DescribedCreatable<RBX::Soundscape::SoundService,RBX::Instance,&RBX::Soundscape::sSoundService>
	// FUNCTION: WEBSERVICE 0x100826c0
	// RBX::DescribedCreatable<RBX::Soundscape::SoundChannel,RBX::Instance,&RBX::Soundscape::sSoundChannel>::DescribedCreatable<RBX::Soundscape::SoundChannel,RBX::Instance,&RBX::Soundscape::sSoundChannel>
	// TEMPLATE: WEBSERVICE 0x10083780
	// RBX::DescribedCreatable<StockSound,RBX::Soundscape::SoundChannel,&sStockSound>::DescribedCreatable<StockSound,RBX::Soundscape::SoundChannel,&sStockSound>
	// TEMPLATE: WEBSERVICE 0x1008c6f0
	// RBX::DescribedCreatable<RBX::Message,RBX::Instance,&RBX::sMessage>::~DescribedCreatable<RBX::Message,RBX::Instance,&RBX::sMessage>
	// FUNCTION: WEBSERVICE 0x1008cb80
	// RBX::DescribedCreatable<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::~DescribedCreatable<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>
	// FUNCTION: WEBSERVICE 0x1008de10
	// RBX::DescribedCreatable<RBX::Hint,RBX::Message,&RBX::sHint>::DescribedCreatable<RBX::Hint,RBX::Message,&RBX::sHint>
	// FUNCTION: WEBSERVICE 0x1008e2c0
	// RBX::DescribedCreatable<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::DescribedCreatable<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>
// FUNCTION: WEBSERVICE 0x10094680
// RBX::DescribedCreatable<RBX::Sky,RBX::Instance,&RBX::sSky>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100946f0
	// RBX::DescribedCreatable<RBX::Sky,RBX::Instance,&RBX::sSky>::~DescribedCreatable<RBX::Sky,RBX::Instance,&RBX::sSky>
	// TEMPLATE: WEBSERVICE 0x10095060
	// RBX::DescribedCreatable<RBX::Sky,RBX::Instance,&RBX::sSky>::DescribedCreatable<RBX::Sky,RBX::Instance,&RBX::sSky>
// FUNCTION: WEBSERVICE 0x1009ac30
// RBX::DescribedCreatable<RBX::PartInstance,RBX::PVInstance,&RBX::sPart>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x1009b4b0
	// RBX::DescribedCreatable<RBX::PartInstance,RBX::PVInstance,&RBX::sPart>::~DescribedCreatable<RBX::PartInstance,RBX::PVInstance,&RBX::sPart>
	// TEMPLATE: WEBSERVICE 0x1009ee20
	// RBX::DescribedCreatable<RBX::PartInstance,RBX::PVInstance,&RBX::sPart>::DescribedCreatable<RBX::PartInstance,RBX::PVInstance,&RBX::sPart><char const *>
	// TEMPLATE: WEBSERVICE 0x100a10f0
	// RBX::DescribedCreatable<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>::~DescribedCreatable<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>
	// FUNCTION: WEBSERVICE 0x100a3b60
	// RBX::DescribedCreatable<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>::DescribedCreatable<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>
	// FUNCTION: WEBSERVICE 0x100b3b30
	// RBX::DescribedCreatable<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::~DescribedCreatable<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>
	// FUNCTION: WEBSERVICE 0x100b4f20
	// RBX::DescribedCreatable<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::DescribedCreatable<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>
	// FUNCTION: WEBSERVICE 0x100be000
	// RBX::DescribedCreatable<RBX::Team,RBX::Instance,&RBX::sTeam>::~DescribedCreatable<RBX::Team,RBX::Instance,&RBX::sTeam>
	// FUNCTION: WEBSERVICE 0x100be560
	// RBX::DescribedCreatable<RBX::Team,RBX::Instance,&RBX::sTeam>::DescribedCreatable<RBX::Team,RBX::Instance,&RBX::sTeam>
	// FUNCTION: WEBSERVICE 0x100c2ee0
	// RBX::DescribedCreatable<RBX::Camera,RBX::Instance,&RBX::sCamera>::~DescribedCreatable<RBX::Camera,RBX::Instance,&RBX::sCamera>
	// FUNCTION: WEBSERVICE 0x100c4030
	// RBX::DescribedCreatable<RBX::Camera,RBX::Instance,&RBX::sCamera>::DescribedCreatable<RBX::Camera,RBX::Instance,&RBX::sCamera>
	// FUNCTION: WEBSERVICE 0x100c5b70
	// RBX::DescribedCreatable<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>::~DescribedCreatable<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>
	// FUNCTION: WEBSERVICE 0x100c6a90
	// RBX::DescribedCreatable<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>::DescribedCreatable<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>
	// TEMPLATE: WEBSERVICE 0x100c8770
	// RBX::DescribedNonCreatable<RBX::BackpackItem,RBX::Widget,&RBX::sBackpackItem>::DescribedNonCreatable<RBX::BackpackItem,RBX::Widget,&RBX::sBackpackItem>
	// TEMPLATE: WEBSERVICE 0x100c90f0
	// RBX::DescribedCreatable<RBX::StarterPackService,RBX::Hopper,&RBX::sStarterPackService>::DescribedCreatable<RBX::StarterPackService,RBX::Hopper,&RBX::sStarterPackService>
	// TEMPLATE: WEBSERVICE 0x100c91d0
	// RBX::DescribedCreatable<RBX::LegacyHopperService,RBX::Hopper,&RBX::sLegacyHopperService>::DescribedCreatable<RBX::LegacyHopperService,RBX::Hopper,&RBX::sLegacyHopperService>
	// TEMPLATE: WEBSERVICE 0x100c9ae0
	// RBX::DescribedCreatable<RBX::HopperBin,RBX::BackpackItem,&RBX::sHopperBin>::DescribedCreatable<RBX::HopperBin,RBX::BackpackItem,&RBX::sHopperBin>
	// SYNTHETIC: WEBSERVICE 0x100ca200
	// RBX::NonFactoryProduct<RBX::Instance,0>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100cbe00
	// RBX::DescribedNonCreatable<RBX::Mouse,RBX::Instance,&RBX::sMouse>::DescribedNonCreatable<RBX::Mouse,RBX::Instance,&RBX::sMouse>
	// TEMPLATE: WEBSERVICE 0x100ccc60
	// RBX::DescribedCreatable<RBX::Tool,RBX::BackpackItem,&RBX::sTool>::DescribedCreatable<RBX::Tool,RBX::BackpackItem,&RBX::sTool>
	// TEMPLATE: WEBSERVICE 0x100cecd0
	// RBX::DescribedCreatable<RBX::Flag,RBX::Tool,&RBX::sFlag>::DescribedCreatable<RBX::Flag,RBX::Tool,&RBX::sFlag>
	// TEMPLATE: WEBSERVICE 0x100d7050
	// RBX::DescribedCreatable<RBX::LocalBackpackItem,RBX::Widget,&RBX::sLocalBackpackItem>::DescribedCreatable<RBX::LocalBackpackItem,RBX::Widget,&RBX::sLocalBackpackItem>
	// TEMPLATE: WEBSERVICE 0x100d7170
	// RBX::DescribedCreatable<RBX::LocalBackpack,RBX::RelativePanel,&RBX::sLocalBackpack>::DescribedCreatable<RBX::LocalBackpack,RBX::RelativePanel,&RBX::sLocalBackpack>
	// SYNTHETIC: WEBSERVICE 0x100d8000
	// RBX::DescribedNonCreatable<RBX::AutoJoint,RBX::JointInstance,&RBX::sAutoJoint>::~DescribedNonCreatable<RBX::AutoJoint,RBX::JointInstance,&RBX::sAutoJoint>
	// FUNCTION: WEBSERVICE 0x100d87f0
	// RBX::DescribedCreatable<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>::~DescribedCreatable<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>
	// FUNCTION: WEBSERVICE 0x100d8800
	// RBX::DescribedCreatable<RBX::Motor,RBX::AutoJoint,&RBX::sMotor>::~DescribedCreatable<RBX::Motor,RBX::AutoJoint,&RBX::sMotor>
	// SYNTHETIC: WEBSERVICE 0x100d8dc0
	// RBX::DescribedNonCreatable<RBX::AutoJoint,RBX::JointInstance,&RBX::sAutoJoint>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100d9440
	// RBX::DescribedCreatable<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>::DescribedCreatable<RBX::Snap,RBX::AutoJoint,&RBX::sSnap><RBX::SnapJoint *>
	// FUNCTION: WEBSERVICE 0x100d94e0
	// RBX::DescribedCreatable<RBX::Weld,RBX::AutoJoint,&RBX::sWeld>::DescribedCreatable<RBX::Weld,RBX::AutoJoint,&RBX::sWeld><RBX::WeldJoint *>
	// FUNCTION: WEBSERVICE 0x100d95c0
	// RBX::DescribedCreatable<RBX::Glue,RBX::AutoJoint,&RBX::sGlue>::DescribedCreatable<RBX::Glue,RBX::AutoJoint,&RBX::sGlue><RBX::GlueJoint *>
	// FUNCTION: WEBSERVICE 0x100d96d0
	// RBX::DescribedCreatable<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate>::DescribedCreatable<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate><RBX::RotateJoint *>
	// FUNCTION: WEBSERVICE 0x100d9790
	// RBX::DescribedCreatable<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP>::DescribedCreatable<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP><RBX::RotatePJoint *>
	// FUNCTION: WEBSERVICE 0x100d9870
	// RBX::DescribedCreatable<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV>::DescribedCreatable<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV><RBX::RotateVJoint *>
	// FUNCTION: WEBSERVICE 0x100d9970
	// RBX::DescribedCreatable<RBX::Motor,RBX::AutoJoint,&RBX::sMotor>::DescribedCreatable<RBX::Motor,RBX::AutoJoint,&RBX::sMotor><RBX::MotorJoint *>
	// FUNCTION: WEBSERVICE 0x100da640
	// RBX::DescribedCreatable<RBX::Backpack,RBX::Hopper,&RBX::sBackpack>::DescribedCreatable<RBX::Backpack,RBX::Hopper,&RBX::sBackpack>
	// FUNCTION: WEBSERVICE 0x100da960
	// RBX::DescribedCreatable<RBX::Teams,RBX::Instance,&RBX::sTeams>::~DescribedCreatable<RBX::Teams,RBX::Instance,&RBX::sTeams>
	// FUNCTION: WEBSERVICE 0x100db0d0
	// RBX::DescribedCreatable<RBX::Teams,RBX::Instance,&RBX::sTeams>::DescribedCreatable<RBX::Teams,RBX::Instance,&RBX::sTeams>
	// FUNCTION: WEBSERVICE 0x100db930
	// RBX::DescribedCreatable<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>::~DescribedCreatable<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>
	// FUNCTION: WEBSERVICE 0x100dc280
	// RBX::DescribedCreatable<RBX::Hat,RBX::Accoutrement,&RBX::sHat>::~DescribedCreatable<RBX::Hat,RBX::Accoutrement,&RBX::sHat>
	// FUNCTION: WEBSERVICE 0x100dcc10
	// RBX::DescribedCreatable<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>::DescribedCreatable<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>
	// FUNCTION: WEBSERVICE 0x100dd680
	// RBX::DescribedCreatable<RBX::Hat,RBX::Accoutrement,&RBX::sHat>::DescribedCreatable<RBX::Hat,RBX::Accoutrement,&RBX::sHat>
	// FUNCTION: WEBSERVICE 0x100de230
	// RBX::DescribedCreatable<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::~DescribedCreatable<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>
	// FUNCTION: WEBSERVICE 0x100de4c0
	// RBX::DescribedCreatable<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::DescribedCreatable<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>
	// FUNCTION: WEBSERVICE 0x100e0920
	// RBX::DescribedCreatable<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>::~DescribedCreatable<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>
	// FUNCTION: WEBSERVICE 0x100e0a80
	// RBX::DescribedCreatable<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>::~DescribedCreatable<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>
	// FUNCTION: WEBSERVICE 0x100e0b50
	// RBX::DescribedCreatable<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>::~DescribedCreatable<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>
	// FUNCTION: WEBSERVICE 0x100e0d50
	// RBX::DescribedCreatable<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>::~DescribedCreatable<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>
	// FUNCTION: WEBSERVICE 0x100e11c0
	// RBX::DescribedCreatable<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>::DescribedCreatable<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>
	// FUNCTION: WEBSERVICE 0x100e12b0
	// RBX::DescribedCreatable<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>::DescribedCreatable<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>
	// FUNCTION: WEBSERVICE 0x100e13a0
	// RBX::DescribedCreatable<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>::DescribedCreatable<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>
	// FUNCTION: WEBSERVICE 0x100e1840
	// RBX::DescribedCreatable<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>::DescribedCreatable<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>
	// FUNCTION: WEBSERVICE 0x100e2100
	// RBX::DescribedCreatable<RBX::Explosion,RBX::Instance,&RBX::sExplosion>::~DescribedCreatable<RBX::Explosion,RBX::Instance,&RBX::sExplosion>
	// FUNCTION: WEBSERVICE 0x100e2c00
	// RBX::DescribedCreatable<RBX::Explosion,RBX::Instance,&RBX::sExplosion>::DescribedCreatable<RBX::Explosion,RBX::Instance,&RBX::sExplosion>
	// FUNCTION: WEBSERVICE 0x100e46f0
	// RBX::DescribedCreatable<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor>::~DescribedCreatable<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor>
	// FUNCTION: WEBSERVICE 0x100e4ea0
	// RBX::DescribedCreatable<RBX::Hole,RBX::Feature,&RBX::sHole>::~DescribedCreatable<RBX::Hole,RBX::Feature,&RBX::sHole>
	// FUNCTION: WEBSERVICE 0x100e4eb0
	// RBX::DescribedCreatable<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::~DescribedCreatable<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>
	// FUNCTION: WEBSERVICE 0x100e6c20
	// RBX::DescribedCreatable<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor>::DescribedCreatable<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor><RBX::MotorJoint *>
	// FUNCTION: WEBSERVICE 0x100e7730
	// RBX::DescribedCreatable<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::DescribedCreatable<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>
	// FUNCTION: WEBSERVICE 0x100e77d0
	// RBX::DescribedCreatable<RBX::Hole,RBX::Feature,&RBX::sHole>::DescribedCreatable<RBX::Hole,RBX::Feature,&RBX::sHole>
	// FUNCTION: WEBSERVICE 0x100e8050
	// RBX::DescribedCreatable<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>::~DescribedCreatable<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>
	// TEMPLATE: WEBSERVICE 0x100e8860
	// RBX::DescribedCreatable<RBX::FlagStand,RBX::PartInstance,&RBX::sFlagStand>::DescribedCreatable<RBX::FlagStand,RBX::PartInstance,&RBX::sFlagStand>
	// FUNCTION: WEBSERVICE 0x100e89f0
	// RBX::DescribedCreatable<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>::DescribedCreatable<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>
	// FUNCTION: WEBSERVICE 0x100e9460
	// RBX::DescribedCreatable<RBX::ForceField,RBX::Instance,&RBX::sForceField>::~DescribedCreatable<RBX::ForceField,RBX::Instance,&RBX::sForceField>
	// FUNCTION: WEBSERVICE 0x100e96e0
	// RBX::DescribedCreatable<RBX::ForceField,RBX::Instance,&RBX::sForceField>::DescribedCreatable<RBX::ForceField,RBX::Instance,&RBX::sForceField>
	// FUNCTION: WEBSERVICE 0x100e9d30
	// RBX::DescribedCreatable<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService>::DescribedCreatable<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService><char *>
	// FUNCTION: WEBSERVICE 0x100ec040
	// RBX::DescribedCreatable<RBX::Rocket,RBX::BodyMover,&RBX::sRocket>::~DescribedCreatable<RBX::Rocket,RBX::BodyMover,&RBX::sRocket>
	// FUNCTION: WEBSERVICE 0x100ec050
	// RBX::DescribedCreatable<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro>::~DescribedCreatable<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro>
	// FUNCTION: WEBSERVICE 0x100ece60
	// RBX::DescribedCreatable<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro>::DescribedCreatable<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro><char *>
	// FUNCTION: WEBSERVICE 0x100ecf60
	// RBX::DescribedCreatable<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition>::DescribedCreatable<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition><char *>
	// FUNCTION: WEBSERVICE 0x100ed010
	// RBX::DescribedCreatable<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity>::DescribedCreatable<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity><char *>
	// FUNCTION: WEBSERVICE 0x100ed1b0
	// RBX::DescribedCreatable<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce>::DescribedCreatable<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce><char *>
	// FUNCTION: WEBSERVICE 0x100ed380
	// RBX::DescribedCreatable<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust>::DescribedCreatable<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust><char *>
	// FUNCTION: WEBSERVICE 0x100eddb0
	// RBX::DescribedCreatable<RBX::Rocket,RBX::BodyMover,&RBX::sRocket>::DescribedCreatable<RBX::Rocket,RBX::BodyMover,&RBX::sRocket><char *>
	// FUNCTION: WEBSERVICE 0x100ee970
	// RBX::DescribedCreatable<RBX::Message,RBX::Instance,&RBX::sMessage>::DescribedCreatable<RBX::Message,RBX::Instance,&RBX::sMessage><char const *>
	// TEMPLATE: WEBSERVICE 0x100f3ea0
	// RBX::DescribedCreatable<RBX::Value<int,&RBX::sIntValue>,RBX::Instance,&RBX::sIntValue>::DescribedCreatable<RBX::Value<int,&RBX::sIntValue>,RBX::Instance,&RBX::sIntValue>
	// TEMPLATE: WEBSERVICE 0x100f3f50
	// RBX::DescribedCreatable<RBX::Value<bool,&RBX::sBoolValue>,RBX::Instance,&RBX::sBoolValue>::DescribedCreatable<RBX::Value<bool,&RBX::sBoolValue>,RBX::Instance,&RBX::sBoolValue>
	// TEMPLATE: WEBSERVICE 0x100f4010
	// RBX::DescribedCreatable<RBX::Value<double,&RBX::sDoubleValue>,RBX::Instance,&RBX::sDoubleValue>::DescribedCreatable<RBX::Value<double,&RBX::sDoubleValue>,RBX::Instance,&RBX::sDoubleValue>
	// TEMPLATE: WEBSERVICE 0x100f41f0
	// RBX::DescribedCreatable<RBX::Value<G3D::Vector3,&RBX::sVector3Value>,RBX::Instance,&RBX::sVector3Value>::DescribedCreatable<RBX::Value<G3D::Vector3,&RBX::sVector3Value>,RBX::Instance,&RBX::sVector3Value>
	// TEMPLATE: WEBSERVICE 0x100f4290
	// RBX::DescribedCreatable<RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>,RBX::Instance,&RBX::sCFrameValue>::DescribedCreatable<RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>,RBX::Instance,&RBX::sCFrameValue>
	// TEMPLATE: WEBSERVICE 0x100f4330
	// RBX::DescribedCreatable<RBX::Value<G3D::Color3,&RBX::sColor3Value>,RBX::Instance,&RBX::sColor3Value>::DescribedCreatable<RBX::Value<G3D::Color3,&RBX::sColor3Value>,RBX::Instance,&RBX::sColor3Value>
	// TEMPLATE: WEBSERVICE 0x100f4410
	// RBX::DescribedCreatable<RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>,RBX::Instance,&RBX::sBrickColorValue>::DescribedCreatable<RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>,RBX::Instance,&RBX::sBrickColorValue>
	// FUNCTION: WEBSERVICE 0x100f8d50
	// RBX::DescribedCreatable<RBX::Visit,RBX::Instance,&RBX::sVisit>::~DescribedCreatable<RBX::Visit,RBX::Instance,&RBX::sVisit>
	// FUNCTION: WEBSERVICE 0x100f94e0
	// RBX::DescribedCreatable<RBX::Visit,RBX::Instance,&RBX::sVisit>::DescribedCreatable<RBX::Visit,RBX::Instance,&RBX::sVisit>
	// FUNCTION: WEBSERVICE 0x100fa020
	// RBX::DescribedCreatable<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService>::~DescribedCreatable<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService>
	// FUNCTION: WEBSERVICE 0x100fac30
	// RBX::DescribedCreatable<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService>::DescribedCreatable<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService><char *>
	// FUNCTION: WEBSERVICE 0x100fb690
	// RBX::DescribedCreatable<RBX::TimerService,RBX::Instance,&RBX::sTimerService>::~DescribedCreatable<RBX::TimerService,RBX::Instance,&RBX::sTimerService>
	// FUNCTION: WEBSERVICE 0x100fbc60
	// RBX::DescribedCreatable<RBX::TimerService,RBX::Instance,&RBX::sTimerService>::DescribedCreatable<RBX::TimerService,RBX::Instance,&RBX::sTimerService>
	// FUNCTION: WEBSERVICE 0x100fc3f0
	// RBX::DescribedCreatable<RBX::Seat,RBX::PartInstance,&RBX::sSeat>::~DescribedCreatable<RBX::Seat,RBX::PartInstance,&RBX::sSeat>
	// FUNCTION: WEBSERVICE 0x100fcfd0
	// RBX::DescribedCreatable<RBX::Seat,RBX::PartInstance,&RBX::sSeat>::DescribedCreatable<RBX::Seat,RBX::PartInstance,&RBX::sSeat>
	// FUNCTION: WEBSERVICE 0x100fda80
	// RBX::DescribedCreatable<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>::~DescribedCreatable<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>
	// FUNCTION: WEBSERVICE 0x100feae0
	// RBX::DescribedCreatable<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>::DescribedCreatable<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>
// TEMPLATE: WEBSERVICE 0x100fecd0
// RBX::DescribedCreatable<RBX::SpawnLocation,RBX::PartInstance,&RBX::sSpawnLocation>::DescribedCreatable<RBX::SpawnLocation,RBX::PartInstance,&RBX::sSpawnLocation>
// TEMPLATE: WEBSERVICE 0x100ffa20
// RBX::DescribedCreatable<RBX::Decal,RBX::FaceInstance,&RBX::sDecal>::~DescribedCreatable<RBX::Decal,RBX::FaceInstance,&RBX::sDecal>
	// FUNCTION: WEBSERVICE 0x100ffb30
	// RBX::DescribedCreatable<RBX::Texture,RBX::Decal,&RBX::sTexture>::~DescribedCreatable<RBX::Texture,RBX::Decal,&RBX::sTexture>
	// TEMPLATE: WEBSERVICE 0x100ffe70
	// RBX::DescribedCreatable<RBX::Decal,RBX::FaceInstance,&RBX::sDecal>::DescribedCreatable<RBX::Decal,RBX::FaceInstance,&RBX::sDecal>
// FUNCTION: WEBSERVICE 0x100fff50
// RBX::DescribedCreatable<RBX::Decal,RBX::FaceInstance,&RBX::sDecal>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x101002f0
	// RBX::DescribedCreatable<RBX::Texture,RBX::Decal,&RBX::sTexture>::DescribedCreatable<RBX::Texture,RBX::Decal,&RBX::sTexture>
	// FUNCTION: WEBSERVICE 0x1012cc20
	// RBX::DescribedCreatable<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::~DescribedCreatable<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>
	// FUNCTION: WEBSERVICE 0x1012d890
	// RBX::DescribedCreatable<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::DescribedCreatable<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>
	// TEMPLATE: WEBSERVICE 0x1018c900
	// RBX::DescribedCreatable<RBX::Network::Player,RBX::Instance,&RBX::Network::sPlayer>::~DescribedCreatable<RBX::Network::Player,RBX::Instance,&RBX::Network::sPlayer>
	// TEMPLATE: WEBSERVICE 0x1018edc0
	// RBX::DescribedCreatable<RBX::Network::Player,RBX::Instance,&RBX::Network::sPlayer>::DescribedCreatable<RBX::Network::Player,RBX::Instance,&RBX::Network::sPlayer>
	// TEMPLATE: WEBSERVICE 0x10196c10
	// RBX::DescribedCreatable<RBX::Network::Client,RBX::Network::Peer,&RBX::Network::sClient>::~DescribedCreatable<RBX::Network::Client,RBX::Network::Peer,&RBX::Network::sClient>
	// TEMPLATE: WEBSERVICE 0x10197ca0
	// RBX::DescribedCreatable<RBX::Network::Client,RBX::Network::Peer,&RBX::Network::sClient>::DescribedCreatable<RBX::Network::Client,RBX::Network::Peer,&RBX::Network::sClient>
	// TEMPLATE: WEBSERVICE 0x101998f0
	// RBX::DescribedCreatable<RBX::Network::Server,RBX::Network::Peer,&RBX::Network::sServer>::~DescribedCreatable<RBX::Network::Server,RBX::Network::Peer,&RBX::Network::sServer>
	// TEMPLATE: WEBSERVICE 0x1019a990
	// RBX::DescribedCreatable<RBX::Network::Server,RBX::Network::Peer,&RBX::Network::sServer>::DescribedCreatable<RBX::Network::Server,RBX::Network::Peer,&RBX::Network::sServer>
// TEMPLATE: WEBSERVICE 0x1019b4a0
// RBX::DescribedCreatable<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings>::~DescribedCreatable<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings>
// TEMPLATE: WEBSERVICE 0x1019b8b0
// RBX::DescribedCreatable<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings>::DescribedCreatable<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings>
// clang-format on
class DescribedCreatable : public Reflection::Described<T, sName, FactoryProduct<T, Base, sName> >
{
protected:
	DescribedCreatable() {}

	template <class U>
	DescribedCreatable(U arg0) : Reflection::Described<T, sName, FactoryProduct<T, Base, sName> >(arg0)
	{
	}
};

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10218690
// `dynamic initializer for 'RBX::Instance::event_childAdded''
// SYNTHETIC: WEBSERVICE 0x102186b0
// `dynamic initializer for 'RBX::Instance::event_childRemoved''
// SYNTHETIC: WEBSERVICE 0x102186d0
// `dynamic initializer for 'RBX::Instance::event_descendentAdded''
// SYNTHETIC: WEBSERVICE 0x102186f0
// `dynamic initializer for 'RBX::Instance::event_descendentRemoving''
// SYNTHETIC: WEBSERVICE 0x10218710
// `dynamic initializer for 'RBX::Instance::event_ancestryChanged''
// SYNTHETIC: WEBSERVICE 0x10218730
// `dynamic initializer for 'RBX::Instance::event_propertyChanged''
// SYNTHETIC: WEBSERVICE 0x10221790
// `dynamic atexit destructor for 'RBX::Instance::event_descendentAdded''
// SYNTHETIC: WEBSERVICE 0x102217a0
// `dynamic atexit destructor for 'RBX::Instance::event_descendentRemoving''
// SYNTHETIC: WEBSERVICE 0x102217b0
// `dynamic atexit destructor for 'RBX::Instance::event_ancestryChanged''
// SYNTHETIC: WEBSERVICE 0x102217c0
// `dynamic atexit destructor for 'RBX::Instance::event_propertyChanged''
// SYNTHETIC: WEBSERVICE 0x10221780
// `dynamic atexit destructor for 'RBX::Instance::event_childRemoved''
// SYNTHETIC: WEBSERVICE 0x102217e0
// `dynamic atexit destructor for 'RBX::Instance::event_childAdded''
// clang-format on

// clang-format off
// TEMPLATE: WEBSERVICE 0x10043340
// RBX::Instance::findFirstChildOfType<RBX::Sky>
// TEMPLATE: WEBSERVICE 0x1005aff0
// RBX::Instance::findFirstChildOfType<RBX::Humanoid>
// TEMPLATE: WEBSERVICE 0x1006b680
// RBX::Instance::findFirstChildOfType<RBX::PartInstance>
// TEMPLATE: WEBSERVICE 0x1006b700
// RBX::Instance::findFirstChildOfType<RBX::ModelInstance>
// TEMPLATE: WEBSERVICE 0x100c79f0
// RBX::Instance::findFirstChildOfType<RBX::BackpackItem>
// TEMPLATE: WEBSERVICE 0x100ca6d0
// RBX::Instance::findFirstChildOfType<RBX::Tool>
// TEMPLATE: WEBSERVICE 0x100dbef0
// RBX::Instance::findFirstChildOfType<RBX::Accoutrement>
// TEMPLATE: WEBSERVICE 0x100e07a0
// RBX::Instance::findFirstChildOfType<RBX::Decal>
// TEMPLATE: WEBSERVICE 0x100e0820
// RBX::Instance::findFirstChildOfType<RBX::Shirt>
// TEMPLATE: WEBSERVICE 0x100e08a0
// RBX::Instance::findFirstChildOfType<RBX::Skin>
// TEMPLATE: WEBSERVICE 0x100e80a0
// RBX::Instance::findFirstChildOfType<RBX::Flag>
// TEMPLATE: WEBSERVICE 0x1018cbc0
// RBX::Instance::findFirstChildOfType<RBX::Backpack>
// clang-format on
template <class T>
T* Instance::findFirstChildOfType() const
{
	if (getChildren()) {
		std::vector<shared_ptr<Instance> >::const_iterator end = getChildren()->end();

		for (std::vector<shared_ptr<Instance> >::const_iterator iter = getChildren()->begin(); iter != end; ++iter) {
			T* child = dynamic_cast<T*>(iter->get());

			if (child != NULL) {
				return child;
			}
		}
	}

	return NULL;
}

} // namespace RBX

#endif // V8TREE_INSTANCE_H
