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

// SIZE 0x4
class __declspec(novtable) ISelectionBase
{
public:
	virtual void VTable0x00(const SelectionChanged& event) = 0; // vtable+0x00
};

DECOMP_SIZE_ASSERT(ISelectionBase, 0x4)

extern const char sSelection[];
// SIZE 0x12c
class Selection : public DescribedCreatable<Selection, Instance, sSelection>,
				  public Notifier<Selection, SelectionChanged>,
				  public Listener<Instance, AncestorChanged>,
				  public Service
{
public:
	void raiseRemoved(boost::shared_ptr<Instance> item);

	void removeFromSelection(const Instance* instance);

	virtual void onEvent(const Instance* source, AncestorChanged event);

private:
	CopyOnWrite<std::vector<boost::shared_ptr<Instance> > > selection; // 0x114
	std::vector<ISelectionBase*> filteredSelections;                   // 0x11c
};

DECOMP_SIZE_ASSERT(Selection, 0x12c)

// clang-format off
// FUNCTION: WEBSERVICE 0x10047e10
// RBX::SelectionChanged::~SelectionChanged
// FUNCTION: WEBSERVICE 0x1005cf20
// RBX::SelectionChanged::SelectionChanged
// STUB: WEBSERVICE 0x1005d160
// RBX::Notifier<RBX::Selection,RBX::SelectionChanged>::raise
// FUNCTION: WEBSERVICE 0x100ddf60
// RBX::Notifier<RBX::Instance,RBX::AncestorChanged>::removeListener
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_SELECTION_H
