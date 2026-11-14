#ifndef V8TREE_INSTANCE_H
#define V8TREE_INSTANCE_H

#include "reflection/property.h"
#include "reflection/reflection.h"
#include "reflection/signal.h"
#include "placeholder.h"
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

struct ChildAdded
{
	const boost::shared_ptr<Instance> child;

	ChildAdded(Instance* child);
	ChildAdded(const ChildAdded& other);

private:
	ChildAdded& operator=(const ChildAdded& other);
};

struct ChildRemoved
{
	const boost::shared_ptr<Instance> child;

	ChildRemoved(Instance* child);
	ChildRemoved(const ChildRemoved& other);

private:
	ChildRemoved& operator=(const ChildRemoved& other);
};

struct DescendentAdded
{
	const boost::shared_ptr<Instance> instance;
	const boost::shared_ptr<Instance> parent;

private:
	friend class Instance;
	friend class RaiseDescendentAdded2;

	DescendentAdded(Instance* instance, Instance* parent) : instance(shared_from(instance)), parent(shared_from(parent))
	{
	}
};

struct DescendentRemoving
{
	const boost::shared_ptr<Instance> instance;
	const boost::shared_ptr<Instance> parent;

	DescendentRemoving(const boost::shared_ptr<Instance>& instance, const boost::shared_ptr<Instance>& parent)
		: instance(instance), parent(parent)
	{
	}
};
struct AncestorChanged
{
	Instance* const child;
	Instance* const oldParent;
	Instance* const newParent;

	AncestorChanged(Instance* child, Instance* oldParent, Instance* newParent)
		: child(child), oldParent(oldParent), newParent(newParent)
	{
	}
};
class PropertyChanged
{
public:
	PropertyChanged(const PropertyChanged& other) : property(other.property) {}

	const Reflection::Property& getProperty() const { return property; }
	bool getProperty(Reflection::Property*&);
	const Name& getName();

	const Reflection::PropertyDescriptor& getDescriptor() { return property.getDescriptor(); }

private:
	friend class Instance;

	PropertyChanged(const Reflection::Property& property) : property(property) {}

	Reflection::Property property;
};

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

	virtual ~Instance();

public:
	virtual void setName(const std::string& value);

protected:
	virtual bool askAddChild(const Instance* instance) const;
	virtual bool askSetParent(const Instance* instance) const;
	virtual void onAddListener(Listener<Instance, DescendentAdded>* listener) const;
	virtual void onAddListener(Listener<Instance, ChildAdded>* listener) const;
	virtual void onAncestorChanged(const AncestorChanged& event);
	virtual void onDescendentAdded(Instance* instance);
	virtual void onDescendentRemoving(const shared_ptr<Instance>& instance);
	virtual void onChildAdded(Instance* child) {}

	virtual void onChildRemoving(Instance* child) {}

	virtual void onChildRemoved(Instance* child) {}

private:
	virtual void onLastChildRemoved() {}

protected:
	virtual void readProperty(const XmlElement* element, IReferenceBinder& binder);

public:
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
	virtual shared_ptr<Instance> createChild(const Name& className);
	virtual XmlElement* write();

protected:
	virtual void onChildChanged(Instance* child, const PropertyChanged& event);
	void raiseChanged(const Reflection::PropertyDescriptor&);

public:
	static Reflection::SignalDesc<Instance, void(shared_ptr<Instance>)> event_childAdded;
	static Reflection::SignalDesc<Instance, void(shared_ptr<Instance>)> event_childRemoved;
	static Reflection::SignalDesc<Instance, void(shared_ptr<Instance>)> event_descendentAdded;
	static Reflection::SignalDesc<Instance, void(shared_ptr<Instance>)> event_descendentRemoving;
	static Reflection::SignalDesc<Instance, void(shared_ptr<Instance>)> event_ancestryChanged;
	static Reflection::SignalDesc<Instance, void(const Reflection::PropertyDescriptor*)> event_propertyChanged;

	static Reflection::BoundProp<bool, 1> propArchivable;
	static const Reflection::PropDescriptor<Instance, std::string> desc_Name;
	static const Reflection::RefPropDescriptor<Instance, Instance> propParent;

	const Association<Instance>& association() const { return assoc; }
	Association<Instance>& association() { return assoc; }

	Instance* getParent() const { return parent; }

	void setParent(Instance* newParent);

	void setParent2(boost::shared_ptr<Instance> instance) { setParent(instance.get()); }
	void promoteChildren();

	Instance* getRootAncestor() { return parent != NULL ? parent->getRootAncestor() : this; }
	const Instance* getRootAncestor() const;
	static const Instance* getRootAncestor(const Instance* instance)
	{
		return instance != NULL ? (instance->parent != NULL ? instance->parent->getRootAncestor() : instance) : NULL;
	}
	static Instance* getRootAncestor(Instance* instance)
	{
		return instance != NULL ? (instance->parent != NULL ? instance->parent->getRootAncestor() : instance) : NULL;
	}

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

	std::string getClassNameStr() const { return getClassName().name; }

	const std::string& getName() const { return name; }

	const CopyOnWrite<std::vector<shared_ptr<Instance> > >& getChildren() const { return children; }

	boost::shared_ptr<const std::vector<shared_ptr<Instance> > > getChildren2() { return children.read(); }

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

	template <class Iterator>
	bool FUN_00422f10(Iterator begin, Iterator end) const
	{
		for (Iterator iter = begin; iter != end; ++iter) {
			const Instance* child = iter->get();
			if (!child || child->contains(this) || child->getParent() == this) {
				return false;
			}
			if (!askAddChild(child) && !child->askSetParent(this)) {
				return false;
			}
		}
		return true;
	}

	unsigned int numChildren() const { return children ? children->size() : 0; }

	const Instance* getChild(unsigned int index) const { return (*children)[index].get(); }
	Instance* getChild(unsigned int index) { return (*children)[index].get(); }

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
	XmlElement* writeDelete();

	bool contains(const Instance* instance) const;

	bool canAddChild(const boost::shared_ptr<Instance>& instance) const;

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

	bool canSetParent(const Instance* instance) const { return instance == NULL || instance->canAddChild(this); }

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

	bool isAncestorOf2(boost::shared_ptr<Instance> descendent) { return isAncestorOf(descendent.get()); }

	bool isDescendentOf2(boost::shared_ptr<Instance> ancestor) { return isDescendentOf(ancestor.get()); }

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

	Association<Instance> assoc;
	Instance* parent;
	CopyOnWrite<std::vector<boost::shared_ptr<Instance> > > children;
	std::string name;
	bool archivable;
};

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
class DescribedCreatable : public Reflection::Described<T, sName, FactoryProduct<T, Base, sName> >
{
protected:
	DescribedCreatable() {}

	template <class U>
	DescribedCreatable(U arg0) : Reflection::Described<T, sName, FactoryProduct<T, Base, sName> >(arg0)
	{
	}
};

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
