#ifndef V8DATAMODEL_SELECTION_H
#define V8DATAMODEL_SELECTION_H

#include "decomp.h"
#include "util/Events.h"
#include "util/Handle.h"
#include "util/Utilities.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <boost/shared_ptr.hpp>
#include <vector>

namespace RBX {

// SIZE 0x10
class SelectionChanged
{
public:
	const boost::shared_ptr<Instance> addedItem;   // 0x00
	const boost::shared_ptr<Instance> removedItem; // 0x08

private:
	friend class Selection;

	SelectionChanged(boost::shared_ptr<Instance> addedItem, boost::shared_ptr<Instance> removedItem)
		: addedItem(addedItem), removedItem(removedItem)
	{
	}
};

DECOMP_SIZE_ASSERT(SelectionChanged, 0x10)

class ISelectionBase;

extern const char sSelection[];
// SIZE 0x12c
class Selection : public DescribedCreatable<Selection, Instance, sSelection>,
				  public Notifier<Selection, SelectionChanged>,
				  public Listener<Instance, AncestorChanged>,
				  public Service
{
public:
	void removeFromSelection(const Instance* instance);

	virtual void onEvent(const Instance* source, AncestorChanged event);

private:
	CopyOnWrite<std::vector<boost::shared_ptr<Instance> > > selection; // 0x114
	std::vector<ISelectionBase*> filteredSelections;                   // 0x11c
};

DECOMP_SIZE_ASSERT(Selection, 0x12c)

} // namespace RBX

#endif // V8DATAMODEL_SELECTION_H
