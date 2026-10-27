#ifndef V8DATAMODEL_SELECTION_H
#define V8DATAMODEL_SELECTION_H

#include "decomp.h"
#include "util/Events.h"
#include "util/Handle.h"
#include "util/Utilities.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <algorithm>
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
private:
	friend class Selection;

	virtual void onSelectionChanged(const SelectionChanged& event) = 0; // vtable+0x00
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
	Selection();
	virtual ~Selection();

	template <class _InIt>
	void setSelection(_InIt _First, _InIt _Last)
	{
		clearSelection();
		for (; _First != _Last; ++_First) {
			addToSelection(*_First);
		}
	}

	std::vector<boost::shared_ptr<Instance> >::const_iterator begin() const { return selection->begin(); }
	std::vector<boost::shared_ptr<Instance> >::const_iterator end() const { return selection->end(); }

	void clearSelection();

	// FUNCTION: WEBSERVICE 0x1005d130
	boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > getSelection2() { return selection.read(); }
	// FUNCTION: WEBSERVICE 0x1005e920
	void setSelection(boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > selection)
	{
		setSelection(selection->begin(), selection->end());
	}
	void setSelection(Instance* instance);

	void addToSelection(const boost::shared_ptr<Instance>& instance) { addToSelection(instance.get()); }
	void addToSelection(Instance* instance);
	void toggleSelection(Instance* instance);
	void removeFromSelection(const Instance* instance);

	virtual void onEvent(const Instance* source, AncestorChanged event);

	void addFilteredSelection(ISelectionBase* fs);
	void removeFilteredSelection(ISelectionBase* fs);

private:
	void raiseAdded(boost::shared_ptr<Instance> instance);
	void raiseRemoved(boost::shared_ptr<Instance> source);

	CopyOnWrite<std::vector<boost::shared_ptr<Instance> > > selection; // 0x114
	std::vector<ISelectionBase*> filteredSelections;                   // 0x11c
};

DECOMP_SIZE_ASSERT(Selection, 0x12c)

// SIZE 0x114
template <class T>
class FilteredSelection : public NonFactoryProduct<Instance, static_cast<const char*>(0)>,
						  public Service,
						  public ISelectionBase
{
public:
	FilteredSelection() { setName("FilteredSelection"); }
	virtual ~FilteredSelection()
	{
		if (rootSelection) {
			rootSelection->removeFilteredSelection(this);
		}
	}

	typename std::vector<T*>::const_iterator begin() const { return filteredSelection.begin(); }
	typename std::vector<T*>::const_iterator end() const { return filteredSelection.end(); }

protected:
	virtual void onAncestorChanged(const AncestorChanged& event)
	{
		Instance::onAncestorChanged(event);

		if (!rootSelection) {
			rootSelection = shared_from(ServiceProvider::create<Selection>(event.newParent));

			if (rootSelection) {
				for (std::vector<boost::shared_ptr<Instance> >::const_iterator iter = rootSelection->begin();
					 iter != rootSelection->end();
					 ++iter) {
					if (T* item = dynamic_cast<T*>(iter->get())) {
						filteredSelection.push_back(item);
					}
				}

				rootSelection->addFilteredSelection(this);
			}
		}
	}

	virtual void onSelectionChanged(const SelectionChanged& event)
	{
		if (T* item = dynamic_cast<T*>(event.addedItem.get())) {
			filteredSelection.push_back(item);
		}

		if (event.removedItem) {
			typename std::vector<T*>::iterator iter =
				std::find(filteredSelection.begin(), filteredSelection.end(), event.removedItem.get());
			if (iter != filteredSelection.end()) {
				filteredSelection.erase(iter);
			}
		}
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x100a1850
	// RBX::FilteredSelection<RBX::PartInstance>::FilteredSelection<RBX::PartInstance>
	// TEMPLATE: WEBSERVICE 0x100a1bd0
	// RBX::FilteredSelection<RBX::PartInstance>::~FilteredSelection<RBX::PartInstance>
	// SYNTHETIC: WEBSERVICE 0x100a1ce0
	// RBX::FilteredSelection<RBX::PartInstance>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100a32e0
	// RBX::FilteredSelection<RBX::PartInstance>::onSelectionChanged
	// TEMPLATE: WEBSERVICE 0x100a4a80
	// RBX::FilteredSelection<RBX::PartInstance>::onAncestorChanged
	// clang-format on

private:
	boost::shared_ptr<Selection> rootSelection; // 0xfc
	std::vector<T*> filteredSelection;          // 0x104
};

// clang-format off
// TEMPLATE: WEBSERVICE 0x1005e8c0
// RBX::Selection::setSelection<std::_Vector_const_iterator<boost::shared_ptr<RBX::Instance>,std::allocator<boost::shared_ptr<RBX::Instance> > > >
// FUNCTION: WEBSERVICE 0x10047e10
// RBX::DescendentRemoving::~DescendentRemoving
// FUNCTION: WEBSERVICE 0x1005cf20
// RBX::SelectionChanged::SelectionChanged
// TEMPLATE: WEBSERVICE 0x1005d010
// ?raise@?$Notifier@VSelection@RBX@@VSelectionChanged@2@@RBX@@IBEXVSelectionChanged@2@PAV?$Listener@VSelection@RBX@@VSelectionChanged@2@@2@@Z
// TEMPLATE: WEBSERVICE 0x1005d160
// ?raise@?$Notifier@VSelection@RBX@@VSelectionChanged@2@@RBX@@IBEXVSelectionChanged@2@@Z
// FUNCTION: WEBSERVICE 0x100d6c30
// RBX::Notifier<RBX::RunService,RBX::Stepped>::addListener
// FUNCTION: WEBSERVICE 0x100ddf60
// RBX::Notifier<RBX::RunService,RBX::Stepped>::removeListener
// SYNTHETIC: WEBSERVICE 0x10218b30
// `dynamic initializer for 'func_getSelection''
// SYNTHETIC: WEBSERVICE 0x10218b60
// `dynamic initializer for 'func_setSelection''
// SYNTHETIC: WEBSERVICE 0x10221a40
// `dynamic atexit destructor for 'func_setSelection''
// SYNTHETIC: WEBSERVICE 0x10221a50
// `dynamic atexit destructor for 'func_getSelection''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_SELECTION_H
