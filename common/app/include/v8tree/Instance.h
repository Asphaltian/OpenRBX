#ifndef V8TREE_INSTANCE_H
#define V8TREE_INSTANCE_H

#include "decomp.h"
#include "reflection/Reflection.h"
#include "util/Association.h"
#include "util/Events.h"
#include "util/Guid.h"
#include "util/Utilities.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

class XmlElement;

namespace RBX {

using boost::shared_ptr;

class IReferenceBinder;
class Instance;
class Name;
class ServiceProvider;

class ChildAdded
{
};
class ChildRemoved
{
};
class DescendentAdded
{
};
class DescendentRemoving
{
};
class AncestorChanged
{
};
class PropertyChanged
{
};

template <class T>
class AbstractFactoryProduct
{
public:
	virtual ~AbstractFactoryProduct() {}
};

extern const char* const sInstance;

class Instance : public GuidItem<Instance>,
				 public AbstractFactoryProduct<Instance>,
				 public Reflection::Described<Instance, sInstance, Reflection::DescribedBase>,
				 public Debugable,
				 public Notifier<Instance, ChildAdded>,
				 public Notifier<Instance, ChildRemoved>,
				 public Notifier<Instance, DescendentAdded>,
				 public Notifier<Instance, DescendentRemoving>,
				 public Notifier<Instance, AncestorChanged>,
				 public Notifier<Instance, PropertyChanged>,
				 public boost::enable_shared_from_this<Instance>,
				 public boost::noncopyable
{
public:
	Instance();
	virtual ~Instance(); // vtable+0x00

	virtual void setName(const std::string& value); // vtable+0x04
	// FUNCTION: WEBSERVICE 0x100c74e0 FOLDED
	virtual bool askAddChild(const Instance* instance) const { return false; } // vtable+0x08

	// FUNCTION: WEBSERVICE 0x100c74e0 FOLDED
	virtual bool askSetParent(const Instance* instance) const { return false; } // vtable+0x0c
	virtual void onAncestorChanged(const AncestorChanged& event);               // vtable+0x10
	virtual void onDescendentAdded(Instance* instance);                         // vtable+0x14
	virtual void onDescendentRemoving(const shared_ptr<Instance>& instance);    // vtable+0x18
	virtual void onChildAdded(Instance* child) {}                               // vtable+0x1c

	virtual void onChildRemoving(Instance* child) {} // vtable+0x20

	virtual void onChildRemoved(Instance* child) {} // vtable+0x24

	virtual void onLastChildRemoved() {}                                            // vtable+0x28
	virtual void readProperty(const XmlElement* element, IReferenceBinder& binder); // vtable+0x2c
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider) {
	} // vtable+0x30
	virtual shared_ptr<Instance> createChild(const Name& className);            // vtable+0x34
	virtual XmlElement* write();                                                // vtable+0x38
	virtual void onChildChanged(Instance* child, const PropertyChanged& event); // vtable+0x3c

	void predelete();

	void readProperties(const XmlElement* element, IReferenceBinder& binder);
	void writeChildren(XmlElement* element);

	bool contains(const Instance* instance) const;

	// FUNCTION: WEBSERVICE 0x10047cd0
	bool isAncestorOf(const Instance* instance) const
	{
		while (instance != 0) {
			instance = instance->parent;

			if (instance == this) {
				return true;
			}
		}

		return false;
	}

	// FUNCTION: WEBSERVICE 0x10047d90
	bool isDescendentOf(const Instance* instance) const
	{
		const Instance* walk = parent;

		while (instance != walk) {
			if (walk == 0) {
				return false;
			}

			walk = walk->parent;
		}

		return true;
	}

private:
	Association<Instance> assoc;                                      // 0xbc
	Instance* parent;                                                 // 0xcc
	CopyOnWrite<std::vector<boost::shared_ptr<Instance> > > children; // 0xd0
	std::string name;                                                 // 0xd8
	bool archivable;                                                  // 0xf4
};

DECOMP_SIZE_ASSERT(Instance, 0xf8)

} // namespace RBX

#endif // V8TREE_INSTANCE_H
