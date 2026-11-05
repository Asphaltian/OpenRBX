#include "PropGrid.h"

#include <algorithm>
#include <boost/bind.hpp>

inline std::vector<RBX::Reflection::ClassDescriptor*>::const_iterator RBX::Reflection::ClassDescriptor::
	derivedClasses_begin() const
{
	return derivedClasses.begin();
}

inline std::vector<RBX::Reflection::ClassDescriptor*>::const_iterator RBX::Reflection::ClassDescriptor::
	derivedClasses_end() const
{
	return derivedClasses.end();
}

IMPLEMENT_DYNAMIC(CPropGrid, CXTPPropertyGrid)
IMPLEMENT_DYNAMIC(CSelectionPropGrid, CPropGrid)
IMPLEMENT_DYNAMIC(CDataModelPropGrid, CSelectionPropGrid)

// STUB: ROBLOX 0x0043df20
void CPropGrid::onEvent(const RBX::Instance* source, RBX::PropertyChanged event)
{
	STUB(0x43df20);
}

// FUNCTION: ROBLOX 0x0043e120
void CPropGrid::Item::FUN_0043e120(const RBX::Reflection::ClassDescriptor* classDescriptor)
{
	m_unk0x04.push_back(classDescriptor);
	std::for_each(
		classDescriptor->derivedClasses_begin(),
		classDescriptor->derivedClasses_end(),
		boost::bind(&Item::FUN_0043e120, this, _1)
	);
}

// FUNCTION: ROBLOX 0x0043eba0
CPropGrid::Item::Item(const RBX::Reflection::PropertyDescriptor& descriptor, CPropGrid* grid) : m_unk0x14(grid)
{
	FUN_0043e120(&descriptor.owner);
}

// STUB: ROBLOX 0x00445430
void CSelectionPropGrid::onEvent(const RBX::Selection* source, RBX::SelectionChanged event)
{
	STUB(0x445430);
}

// STUB: ROBLOX 0x00445580
void CSelectionPropGrid::FUN_00445580(RBX::Selection* selection)
{
	STUB(0x445580);
}

// STUB: ROBLOX 0x00445830
void CDataModelPropGrid::VTable0x00(
	boost::shared_ptr<RBX::DataModel> newDataModel, boost::shared_ptr<RBX::DataModel> oldDataModel
)
{
	STUB(0x445830);
}

// FUNCTION: ROBLOX 0x00445960
CPropGrid::~CPropGrid()
{
}

// FUNCTION: ROBLOX 0x00445ae0
CPropGrid::CPropGrid() : m_unk0x194(NULL), m_unk0x1c0(false), m_unk0x1c1(false)
{
	m_bHelpVisible = FALSE;
}
