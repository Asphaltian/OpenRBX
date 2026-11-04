#include "PropGrid.h"

IMPLEMENT_DYNAMIC(CPropGrid, CXTPPropertyGrid)
IMPLEMENT_DYNAMIC(CSelectionPropGrid, CPropGrid)
IMPLEMENT_DYNAMIC(CDataModelPropGrid, CSelectionPropGrid)

// STUB: ROBLOX 0x0043df20
void CPropGrid::onEvent(const RBX::Instance* source, RBX::PropertyChanged event)
{
	STUB(0x43df20);
}

// FUNCTION: ROBLOX 0x00445960
CPropGrid::~CPropGrid()
{
}

// FUNCTION: ROBLOX 0x00445ae0
CPropGrid::CPropGrid() : m_unk0x194(0), m_unk0x1c0(false), m_unk0x1c1(false)
{
	m_bHelpVisible = FALSE;
}
