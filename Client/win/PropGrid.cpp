#include "PropGrid.h"

IMPLEMENT_DYNAMIC(CPropGrid, CXTPPropertyGrid)
IMPLEMENT_DYNAMIC(CSelectionPropGrid, CPropGrid)
IMPLEMENT_DYNAMIC(CDataModelPropGrid, CSelectionPropGrid)

// STUB: ROBLOX 0x0043df20
void CPropGrid::onEvent(const RBX::Instance* source, RBX::PropertyChanged event)
{
	STUB(0x43df20);
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
