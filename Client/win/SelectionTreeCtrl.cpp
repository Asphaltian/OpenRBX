#include "SelectionTreeCtrl.h"

// FUNCTION: ROBLOX 0x00424980
void CSelectionTreeCtrl::VTable0x150(RBX::Instance* instance)
{
	boost::recursive_mutex::scoped_lock lock(m_unk0xb0);

	std::map<RBX::Instance*, CRobloxTreeCtrlNode*>::iterator it = m_unk0x98.find(instance);
	if (it != m_unk0x98.end()) {
		m_unk0xdc.insert(it->second);
	}
}

// FUNCTION: ROBLOX 0x00424a50
void CSelectionTreeCtrl::VTable0x154(CRobloxTreeCtrlNode* node)
{
	boost::recursive_mutex::scoped_lock lock(m_unk0xb0);
	m_unk0xdc.insert(node);
}

// FUNCTION: ROBLOX 0x00424ad0
void CSelectionTreeCtrl::VTable0x158(CRobloxTreeCtrlNode* node)
{
	boost::recursive_mutex::scoped_lock lock(m_unk0xb0);
	m_unk0xdc.erase(node);
}

// STUB: ROBLOX 0x00425ef0
void CSelectionTreeCtrl::onEvent(const RBX::Selection* source, RBX::SelectionChanged event)
{
	STUB(0x425ef0);
}

// FUNCTION: ROBLOX 0x004262b0
CSelectionTreeCtrl::CSelectionTreeCtrl() : m_unk0xcc(0), m_unk0xcd(0), m_unk0xce(0)
{
	m_unk0xc4.LoadMenu(103);
}

// FUNCTION: ROBLOX 0x00426400
boost::shared_ptr<RBX::DataModel> CSelectionTreeCtrl::VTable0x14c() const
{
	return m_unk0xe8;
}

// STUB: ROBLOX 0x00426cf0
void CSelectionTreeCtrl::VTable0x15c()
{
	CRobloxTreeCtrl::VTable0x15c();
	STUB(0x426cf0);
}

// FUNCTION: ROBLOX 0x00428580
RBX::Selection* CSelectionTreeCtrl::VTable0x148()
{
	return m_unk0xe8 ? m_unk0xe8->create<RBX::Selection>() : NULL;
}

// FUNCTION: ROBLOX 0x004285a0
void CSelectionTreeCtrl::FUN_004285a0(boost::shared_ptr<RBX::DataModel> dataModel)
{
	if (m_unk0xe8) {
		RBX::DataModel::Lock lock(m_unk0xe8);
		RBX::stopListening<RBX::Selection, RBX::SelectionChanged>(m_unk0xe8->find<RBX::Selection>(), this);
	}

	m_unk0xe8 = dataModel;

	if (m_unk0xe8) {
		RBX::DataModel::Lock lock(m_unk0xe8);
		RBX::startListening<RBX::Selection, RBX::SelectionChanged>(m_unk0xe8->create<RBX::Selection>(), this);
	}
}

// FUNCTION: ROBLOX 0x00428b40
CSelectionTreeCtrl::~CSelectionTreeCtrl()
{
	FUN_004285a0(boost::shared_ptr<RBX::DataModel>());
}

// FUNCTION: ROBLOX 0x00428cb0
void CSelectionTreeCtrl::VTable0x00(
	boost::shared_ptr<RBX::DataModel> newDataModel, boost::shared_ptr<RBX::DataModel> oldDataModel
)
{
	FUN_004285a0(newDataModel);
	VTable0x15c();
}
