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

protected:
	virtual void onEvent(const RBX::Instance* source, RBX::PropertyChanged event);

private:
	boost::recursive_mutex m_unk0x188;                                            // 0x188
	undefined4 m_unk0x194;                                                        // 0x194
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

private:
	undefined m_unk0x1e0[0x1e8 - 0x1e0]; // 0x1e0
};

DECOMP_SIZE_ASSERT(CSelectionPropGrid, 0x1e8)

// SIZE 0x1ec
class CDataModelPropGrid : public CSelectionPropGrid, public ActiveDocView
{
	DECLARE_DYNAMIC(CDataModelPropGrid)

	// SYNTHETIC: ROBLOX 0x0043bbc0
	// ?GetThisClass@CDataModelPropGrid@@SGPAUCRuntimeClass@@XZ
};

DECOMP_SIZE_ASSERT(CDataModelPropGrid, 0x1ec)

#endif // PROPGRID_H
