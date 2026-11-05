#ifndef PROPGRID_H
#define PROPGRID_H

#include "ActiveDocView.h"
#include "decomp.h"
#include "util/Events.h"
#include "v8datamodel/Selection.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <map>
#include <set>
#include <vector>

// SIZE 0x1dc
class CPropGrid : public CXTPPropertyGrid, public RBX::Listener<RBX::Instance, RBX::PropertyChanged>
{
	DECLARE_DYNAMIC(CPropGrid)

	// SYNTHETIC: ROBLOX 0x0043bb90
	// ?_GetBaseClass@CPropGrid@@KGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x0043bba0
	// ?GetThisClass@CPropGrid@@SGPAUCRuntimeClass@@XZ

public:
	CPropGrid();
	virtual ~CPropGrid();

	// SYNTHETIC: ROBLOX 0x00445c20
	// CPropGrid::`scalar deleting destructor'

	// SIZE 0x18
	class Item
	{
	public:
		Item(const RBX::Reflection::PropertyDescriptor& descriptor, CPropGrid* grid);

		virtual void VTable0x00(
			const std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >&
				instances
		) = 0; // vtable+0x00

	protected:
		void FUN_0043e120(const RBX::Reflection::ClassDescriptor* classDescriptor);

		std::vector<const RBX::Reflection::ClassDescriptor*> m_unk0x04; // 0x04
		CPropGrid* m_unk0x14;                                           // 0x14
	};

protected:
	virtual void onEvent(const RBX::Instance* source, RBX::PropertyChanged event);

private:
	boost::recursive_mutex m_unk0x188;                                            // 0x188
	RBX::DataModel* m_unk0x194;                                                   // 0x194
	std::vector<undefined4*> m_unk0x198;                                          // 0x198
	std::map<const RBX::Reflection::PropertyDescriptor*, undefined4*> m_unk0x1a8; // 0x1a8
	std::map<undefined4, undefined4> m_unk0x1b4;                                  // 0x1b4
	bool m_unk0x1c0;                                                              // 0x1c0
	bool m_unk0x1c1;                                                              // 0x1c1
	std::set<undefined4*> m_unk0x1c4;                                             // 0x1c4
	std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >
		m_unk0x1d0; // 0x1d0
};

DECOMP_SIZE_ASSERT(CPropGrid, 0x1dc)

// SIZE 0x1e8
class CSelectionPropGrid : public CPropGrid, public RBX::Listener<RBX::Selection, RBX::SelectionChanged>
{
	DECLARE_DYNAMIC(CSelectionPropGrid)

	// SYNTHETIC: ROBLOX 0x0043bbb0
	// ?GetThisClass@CSelectionPropGrid@@SGPAUCRuntimeClass@@XZ

public:
	CSelectionPropGrid() {}

	// FUNCTION: ROBLOX 0x00434880
	virtual ~CSelectionPropGrid() { FUN_00445580(NULL); }

	void FUN_00445580(RBX::Selection* selection);

protected:
	virtual void onEvent(const RBX::Selection* source, RBX::SelectionChanged event);

private:
	boost::shared_ptr<RBX::Selection> m_unk0x1e0; // 0x1e0
};

DECOMP_SIZE_ASSERT(CSelectionPropGrid, 0x1e8)

// SIZE 0x1ec
class CDataModelPropGrid : public CSelectionPropGrid, public ActiveDocView
{
	DECLARE_DYNAMIC(CDataModelPropGrid)

	// SYNTHETIC: ROBLOX 0x0043bbc0
	// ?GetThisClass@CDataModelPropGrid@@SGPAUCRuntimeClass@@XZ

public:
	// FUNCTION: ROBLOX 0x00435320
	CDataModelPropGrid() {}

	// SYNTHETIC: ROBLOX 0x004353e0
	// CDataModelPropGrid::~CDataModelPropGrid

	// SYNTHETIC: ROBLOX 0x00435440
	// CDataModelPropGrid::`scalar deleting destructor'

protected:
	virtual void VTable0x00(
		boost::shared_ptr<RBX::DataModel> newDataModel, boost::shared_ptr<RBX::DataModel> oldDataModel
	);
};

DECOMP_SIZE_ASSERT(CDataModelPropGrid, 0x1ec)

#endif // PROPGRID_H
