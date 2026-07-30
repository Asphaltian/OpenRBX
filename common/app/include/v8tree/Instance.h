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

namespace RBX {

class Instance;

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
	virtual ~Instance();

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
