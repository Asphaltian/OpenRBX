#ifndef V8DATAMODEL_SELECTION_H
#define V8DATAMODEL_SELECTION_H

#include "compat.h"
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

	unsigned int size() const;
	boost::shared_ptr<Instance> front() const;
	boost::shared_ptr<Instance> back() const;

	template <class _InIt>
	void setSelection(_InIt _First, _InIt _Last)
	{
		clearSelection();
		for (; _First != _Last; ++_First) {
			addToSelection(*_First);
		}
	}

	// FUNCTION: ROBLOX 0x00410aa0
	std::vector<boost::shared_ptr<Instance> >::const_iterator begin() const { return selection->begin(); }

	// FUNCTION: ROBLOX 0x00410ad0
	std::vector<boost::shared_ptr<Instance> >::const_iterator end() const { return selection->end(); }
	const CopyOnWrite<std::vector<boost::shared_ptr<Instance> > >& getSelection() const;

	void clearSelection();

	// FUNCTION: WEBSERVICE 0x1005d130
	boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > getSelection2() { return selection.read(); }
	// FUNCTION: WEBSERVICE 0x1005e920
	// FUNCTION: ROBLOX 0x00568950
	void setSelection(boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > selection)
	{
		setSelection(selection->begin(), selection->end());
	}
	void setSelection(Instance* instance);

	void addToSelection(const boost::shared_ptr<Instance>& instance) { addToSelection(instance.get()); }
	void addToSelection(Instance* instance);
	void toggleSelection(Instance* instance);
	void removeFromSelection(const Instance* instance);

	// FUNCTION: WEBSERVICE 0x100bfd90
	// STUB: ROBLOX 0x004259e0
	bool isSelected(const Instance* instance) const
	{
		return std::find(selection->begin(), selection->end(), shared_from(instance)) != selection->end();
	}

	// SIZE 0x8
	class AddIterator : public std::_Outit
	{
	private:
		Selection* selection; // 0x4

	public:
		AddIterator(Selection* selection) : selection(selection) {}

		AddIterator& operator*() { return *this; }
		AddIterator& operator++() { return *this; }
		AddIterator operator++(int) { return *this; }

		template <class T>
		AddIterator& operator=(T* instance)
		{
			selection->addToSelection(instance);
			return *this;
		}
	};

	// SIZE 0x8
	class RemoveIterator : public std::_Outit
	{
	private:
		Selection* selection; // 0x4

	public:
		RemoveIterator(Selection* selection) : selection(selection) {}

		RemoveIterator& operator*() { return *this; }
		RemoveIterator& operator++() { return *this; }
		RemoveIterator operator++(int) { return *this; }

		template <class T>
		RemoveIterator& operator=(T* instance)
		{
			selection->removeFromSelection(instance);
			return *this;
		}
	};

	// SIZE 0x8
	class ToggleIterator : public std::_Outit
	{
	private:
		Selection* selection; // 0x4

	public:
		ToggleIterator(Selection* selection) : selection(selection) {}

		ToggleIterator& operator*() { return *this; }
		ToggleIterator& operator++() { return *this; }
		ToggleIterator operator++(int) { return *this; }

		template <class T>
		ToggleIterator& operator=(T* instance)
		{
			selection->toggleSelection(instance);
			return *this;
		}
	};

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
DECOMP_SIZE_ASSERT(Selection::AddIterator, 0x8)
DECOMP_SIZE_ASSERT(Selection::RemoveIterator, 0x8)
DECOMP_SIZE_ASSERT(Selection::ToggleIterator, 0x8)

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

	const std::vector<T*>& items() const { return filteredSelection; }
	unsigned int size() const;

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
					if (T* c = dynamic_cast<T*>(iter->get())) {
						filteredSelection.push_back(c);
					}
				}

				rootSelection->addFilteredSelection(this);
			}
		}
	}

	virtual void onSelectionChanged(const SelectionChanged& event)
	{
		if (T* c = dynamic_cast<T*>(event.addedItem.get())) {
			filteredSelection.push_back(c);
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
	// TEMPLATE: ROBLOX 0x005e9e40
	// RBX::FilteredSelection<RBX::PartInstance>::FilteredSelection<RBX::PartInstance>
	// TEMPLATE: WEBSERVICE 0x100a1bd0
	// RBX::FilteredSelection<RBX::PartInstance>::~FilteredSelection<RBX::PartInstance>
	// SYNTHETIC: WEBSERVICE 0x100a1ce0
	// RBX::FilteredSelection<RBX::PartInstance>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100a32e0
	// STUB: ROBLOX 0x005eb920
	// RBX::FilteredSelection<RBX::PartInstance>::onSelectionChanged
	// TEMPLATE: WEBSERVICE 0x100a4a80
	// STUB: ROBLOX 0x005ed120
	// RBX::FilteredSelection<RBX::PartInstance>::onAncestorChanged
	// TEMPLATE: WEBSERVICE 0x100bf860
	// TEMPLATE: ROBLOX 0x006212e0
	// RBX::FilteredSelection<RBX::Instance>::FilteredSelection<RBX::Instance>
	// TEMPLATE: WEBSERVICE 0x100bfa30
	// RBX::FilteredSelection<RBX::Instance>::~FilteredSelection<RBX::Instance>
	// SYNTHETIC: WEBSERVICE 0x100bfb40
	// RBX::FilteredSelection<RBX::Instance>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100c01e0
	// STUB: ROBLOX 0x00621c00
	// RBX::FilteredSelection<RBX::Instance>::onSelectionChanged
	// TEMPLATE: WEBSERVICE 0x100c0630
	// STUB: ROBLOX 0x00622050
	// RBX::FilteredSelection<RBX::Instance>::onAncestorChanged
	// clang-format on

private:
	boost::shared_ptr<Selection> rootSelection; // 0xfc
	std::vector<T*> filteredSelection;          // 0x104
};

// clang-format off
// TEMPLATE: WEBSERVICE 0x1005e8c0
// TEMPLATE: ROBLOX 0x005688f0
// RBX::Selection::setSelection<std::_Vector_const_iterator<boost::shared_ptr<RBX::Instance>,std::allocator<boost::shared_ptr<RBX::Instance> > > >
// FUNCTION: WEBSERVICE 0x10047e10
// FUNCTION: ROBLOX 0x00568f50 FOLDED
// RBX::DescendentRemoving::~DescendentRemoving
// FUNCTION: WEBSERVICE 0x1005cf20
// FUNCTION: ROBLOX 0x00566fc0
// RBX::SelectionChanged::SelectionChanged
// TEMPLATE: WEBSERVICE 0x1005d010
// STUB: ROBLOX 0x005670b0
// ?raise@?$Notifier@VSelection@RBX@@VSelectionChanged@2@@RBX@@IBEXVSelectionChanged@2@PAV?$Listener@VSelection@RBX@@VSelectionChanged@2@@2@@Z
// TEMPLATE: WEBSERVICE 0x1005d160
// STUB: ROBLOX 0x00567200
// ?raise@?$Notifier@VSelection@RBX@@VSelectionChanged@2@@RBX@@IBEXVSelectionChanged@2@@Z
// FUNCTION: WEBSERVICE 0x100d6c30
// ?addListener@?$Notifier@VRunService@RBX@@VStepped@2@@RBX@@QBEXPAV?$Listener@VRunService@RBX@@VStepped@2@@2@@Z
// FUNCTION: WEBSERVICE 0x100ddf60
// FUNCTION: ROBLOX 0x0060d390
// ?removeListener@?$Notifier@VRunService@RBX@@VStepped@2@@RBX@@QBEXPAV?$Listener@VRunService@RBX@@VStepped@2@@2@@Z
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
