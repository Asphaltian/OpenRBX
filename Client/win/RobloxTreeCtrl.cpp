#include "RobloxTreeCtrl.h"

#include "RobloxTreeCtrlNode.h"
#include "SelectionTreeCtrl.h"
#include "util/scopedassign.h"

// SIZE 0x50
class RootNode : public CRobloxTreeCtrlNode
{
public:
	RootNode(const boost::shared_ptr<RBX::Instance>& instance, CRobloxTreeCtrl* tree);

	virtual void VTable0x08();
	virtual void VTable0x14(NMTVDISPINFO* dispInfo);
	virtual HTREEITEM VTable0x18();
};

DECOMP_SIZE_ASSERT(RootNode, 0x50)

// STUB: ROBLOX 0x00422ae0
void CRobloxTreeCtrl::VTable0x15c()
{
	STUB(0x422ae0);
}

// STUB: ROBLOX 0x004230c0
int FUN_004230c0(RBX::Instance* instance)
{
	STUB(0x4230c0);
	return 0;
}

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

// FUNCTION: ROBLOX 0x00424ca0
CRobloxTreeCtrl::CRobloxTreeCtrl() : m_unk0x94(NULL), m_unk0xa4(0)
{
}

// FUNCTION: ROBLOX 0x00424f00
CRobloxTreeCtrlNode::~CRobloxTreeCtrlNode()
{
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0x30->m_unk0xb0);

		if (m_unk0x34) {
			m_unk0x34->m_unk0x14.erase(m_unk0x0c.get());
		}

		m_unk0x30->VTable0x158(this);
		m_unk0x30->m_unk0x98.erase(m_unk0x0c.get());
	}

	RBX::DataModel::Lock lock(m_unk0x30->VTable0x14c());
	m_unk0x0c->RBX::Notifier<RBX::Instance, RBX::PropertyChanged>::removeListener(this);
	m_unk0x0c->RBX::Notifier<RBX::Instance, RBX::ChildAdded>::removeListener(this);
	m_unk0x0c->RBX::Notifier<RBX::Instance, RBX::ChildRemoved>::removeListener(this);
}

// FUNCTION: ROBLOX 0x00425150
void CRobloxTreeCtrlNode::VTable0x10(undefined4 p_unk0x04)
{
	if (m_unk0x2d) {
		m_unk0x2c_bit1 = 0;
	}
	else if (m_unk0x30->GetItemState(m_unk0x44, TVIF_STATE) | TVIS_EXPANDED) {
		m_unk0x2c_bit1 = 1;

		if (m_unk0x30->GetItemState(m_unk0x44, TVIF_STATE) | TVIS_EXPANDPARTIAL) {
			m_unk0x30->SetItemState(m_unk0x44, TVIS_EXPANDED, TVIF_STATE);
		}
	}
}

// STUB: ROBLOX 0x004252f0
HTREEITEM CRobloxTreeCtrlNode::VTable0x18()
{
	STUB(0x4252f0);
	return NULL;
}

// STUB: ROBLOX 0x004254b0
void CRobloxTreeCtrlNode::onEvent(const RBX::Instance* source, RBX::PropertyChanged event)
{
	STUB(0x4254b0);
}

// STUB: ROBLOX 0x004258b0
void CRobloxTreeCtrlNode::VTable0x14(NMTVDISPINFO* dispInfo)
{
	STUB(0x4258b0);
}

// FUNCTION: ROBLOX 0x00425ab0
void CRobloxTreeCtrlNode::VTable0x08()
{
	if (m_unk0x44) {
		if (m_unk0x30->VTable0x148() && m_unk0x30->VTable0x148()->isSelected(m_unk0x0c.get())) {
			for (CRobloxTreeCtrlNode* node = m_unk0x34; node; node = node->m_unk0x34) {
				RBX::ScopedAssign<undefined> assign(node->m_unk0x2d, true);
				node->m_unk0x30->Expand(node->m_unk0x44, TVE_EXPAND | TVE_EXPANDPARTIAL);
			}

			m_unk0x30->SelectItems(m_unk0x44, m_unk0x44, FALSE);
			m_unk0x30->EnsureVisible(m_unk0x44);
		}
		else {
			m_unk0x30->SetItemState(m_unk0x44, 0, TVIS_SELECTED);
		}
	}
}

// STUB: ROBLOX 0x00425b70
void CRobloxTreeCtrlNode::onEvent(const RBX::Instance* source, RBX::ChildAdded event)
{
	STUB(0x425b70);
}

// STUB: ROBLOX 0x00425d80
void CRobloxTreeCtrlNode::onEvent(const RBX::Instance* source, RBX::ChildRemoved event)
{
	STUB(0x425d80);
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

// STUB: ROBLOX 0x00426440
CRobloxTreeCtrlNode::CRobloxTreeCtrlNode(
	CRobloxTreeCtrlNode* parent, boost::shared_ptr<RBX::Instance> instance, CRobloxTreeCtrl* tree
)
	: m_unk0x0c(instance), m_unk0x2c_bit0(0), m_unk0x2c_bit1(0), m_unk0x2d(0), m_unk0x30(tree), m_unk0x34(parent),
	  m_unk0x44(NULL), m_unk0x48(FUN_004230c0(instance.get())), m_unk0x4c(&instance->getDescriptor())
{
	{
		boost::recursive_mutex::scoped_lock lock(tree->m_unk0xb0);
		tree->m_unk0x98[instance.get()] = this;

		if (parent) {
			parent->m_unk0x14[instance.get()] = this;
		}
	}

	RBX::DataModel::Lock lock(tree->VTable0x14c());
	instance->RBX::Notifier<RBX::Instance, RBX::PropertyChanged>::addListener(this);
}

// STUB: ROBLOX 0x004266a0
void CRobloxTreeCtrlNode::VTable0x0c()
{
	STUB(0x4266a0);
}

// STUB: ROBLOX 0x00426960
void CRobloxTreeCtrlNode::FUN_00426960()
{
	STUB(0x426960);
}

// FUNCTION: ROBLOX 0x00426c70
RootNode::RootNode(const boost::shared_ptr<RBX::Instance>& instance, CRobloxTreeCtrl* tree)
	: CRobloxTreeCtrlNode(NULL, instance, tree)
{
	m_unk0x2c_bit1 = 1;
}

// FUNCTION: ROBLOX 0x00426cd0
HTREEITEM RootNode::VTable0x18()
{
	return TVI_ROOT;
}

// STUB: ROBLOX 0x00426cf0
void CSelectionTreeCtrl::VTable0x15c()
{
	CRobloxTreeCtrl::VTable0x15c();

	if (m_unk0xe8 && m_hWnd) {
		m_unk0x94 = new RootNode(m_unk0xe8, this);
		m_unk0x94->VTable0x0c();
		m_unk0x94->FUN_00426960();
		Expand(m_unk0x94->m_unk0x44, TVE_EXPAND);
	}
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

void RootNode::VTable0x08()
{
}

void RootNode::VTable0x14(NMTVDISPINFO* dispInfo)
{
}
