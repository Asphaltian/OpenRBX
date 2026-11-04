#include "RobloxTreeCtrl.h"

#include "ReflectionMetadata.h"
#include "RobloxTreeCtrlNode.h"
#include "SelectionTreeCtrl.h"
#include "util/scopedassign.h"

#include <algorithm>
#include <boost/bind.hpp>
#include <vector>

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

// FUNCTION: ROBLOX 0x004221a0
void CRobloxTreeCtrlNode::FUN_004221a0()
{
	if (m_unk0x2c_bit0) {
		if (m_unk0x44 && m_unk0x44 != TVI_ROOT) {
			CRect rect;
			if (m_unk0x30->GetItemRect(m_unk0x44, &rect, FALSE)) {
				m_unk0x30->InvalidateRect(&rect);
			}
		}

		m_unk0x2c_bit0 = 0;
	}
}

// FUNCTION: ROBLOX 0x00422ae0
void CRobloxTreeCtrl::VTable0x15c()
{
	RBX::ScopedAssign<undefined> clearing(m_unk0xa4, true);
	if (m_hWnd) {
		DeleteAllItems();
		if (m_unk0x94) {
			delete m_unk0x94;
			m_unk0x94 = NULL;
		}
	}
}

// FUNCTION: ROBLOX 0x004230c0
int FUN_004230c0(RBX::Instance* instance)
{
	boost::shared_ptr<RBX::Reflection::Metadata::Reflection> reflection =
		RBX::Reflection::Metadata::Reflection::FUN_004480c0();
	RBX::Reflection::Metadata::Class* metadata = reflection->FUN_00446220(&instance->getDescriptor(), true);
	return metadata ? metadata->FUN_00422100() : 0;
}

// FUNCTION: ROBLOX 0x004248f0
void CRobloxTreeCtrlNode::FUN_004248f0(const boost::shared_ptr<RBX::Instance>& instance)
{
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0x30->m_unk0xb0);
		m_unk0x38.insert(instance);
	}
	m_unk0x30->VTable0x154(this);
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

// FUNCTION: ROBLOX 0x004254b0
void CRobloxTreeCtrlNode::onEvent(const RBX::Instance* source, RBX::PropertyChanged event)
{
	bool isName;
	{
		RBX::DataModel::Lock lock(m_unk0x30->VTable0x14c());
		isName = event.getDescriptor() == RBX::Instance::desc_Name;
	}

	if (isName) {
		m_unk0x2c_bit0 = 1;
		m_unk0x30->VTable0x154(this);
	}
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

// FUNCTION: ROBLOX 0x00425b70
void CRobloxTreeCtrlNode::onEvent(const RBX::Instance* source, RBX::ChildAdded event)
{
	if (m_unk0x2c_bit1) {
		boost::recursive_mutex::scoped_lock lock(m_unk0x30->m_unk0xb0);
		m_unk0x38.insert(event.child);
	}

	m_unk0x30->VTable0x154(this);
}

// FUNCTION: ROBLOX 0x00425c40
void CRobloxTreeCtrlNode::FUN_00425c40()
{
	std::vector<CRobloxTreeCtrlNode*> nodes;

	{
		boost::recursive_mutex::scoped_lock lock(m_unk0x30->m_unk0xb0);
		std::for_each(
			m_unk0x20.begin(), m_unk0x20.end(), boost::bind(&std::vector<CRobloxTreeCtrlNode*>::push_back, &nodes, _1)
		);
		m_unk0x20.clear();
	}

	for (std::vector<CRobloxTreeCtrlNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		m_unk0x30->DeleteItem((*it)->m_unk0x44);
	}
}

// FUNCTION: ROBLOX 0x00425d80
void CRobloxTreeCtrlNode::onEvent(const RBX::Instance* source, RBX::ChildRemoved event)
{
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0x30->m_unk0xb0);
		m_unk0x30->m_unk0x98.erase(event.child.get());

		std::map<RBX::Instance*, CRobloxTreeCtrlNode*>::iterator it = m_unk0x14.find(event.child.get());
		if (it != m_unk0x14.end()) {
			m_unk0x20.insert(it->second);
		}

		m_unk0x38.erase(event.child);
	}

	m_unk0x30->VTable0x154(this);
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

// FUNCTION: ROBLOX 0x00426440
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

// FUNCTION: ROBLOX 0x004267a0
CRobloxTreeCtrlNode* CRobloxTreeCtrlNode::FUN_004267a0(const boost::shared_ptr<RBX::Instance>& instance)
{
	boost::shared_ptr<RBX::Reflection::Metadata::Reflection> reflection =
		RBX::Reflection::Metadata::Reflection::FUN_004480c0();
	RBX::Reflection::Metadata::Class* metadata = reflection->FUN_00446220(&instance->getDescriptor(), true);
	if (!metadata || metadata->FUN_00422100() < 0) {
		return NULL;
	}

	{
		boost::recursive_mutex::scoped_lock lock(m_unk0x30->m_unk0xb0);
		std::map<RBX::Instance*, CRobloxTreeCtrlNode*>::iterator it = m_unk0x14.find(instance.get());
		if (it != m_unk0x14.end()) {
			return it->second;
		}
	}

	CRobloxTreeCtrlNode* node = new CRobloxTreeCtrlNode(this, instance, m_unk0x30);
	node->VTable0x0c();
	return node;
}

// FUNCTION: ROBLOX 0x00426960
void CRobloxTreeCtrlNode::FUN_00426960()
{
	std::vector<boost::shared_ptr<RBX::Instance> > children;

	{
		RBX::DataModel::Lock lock(m_unk0x30->VTable0x14c());
		if (m_unk0x0c->getChildren()) {
			children = *m_unk0x0c->getChildren();
		}
	}

	std::for_each(children.begin(), children.end(), boost::bind(&CRobloxTreeCtrlNode::FUN_004248f0, this, _1));
}

// FUNCTION: ROBLOX 0x00426b00
void CRobloxTreeCtrlNode::FUN_00426b00()
{
	for (int i = 0; i < 10; i++) {
		boost::shared_ptr<RBX::Instance> instance;
		{
			boost::recursive_mutex::scoped_lock lock(m_unk0x30->m_unk0xb0);
			std::set<boost::shared_ptr<RBX::Instance> >::iterator it = m_unk0x38.begin();
			if (it == m_unk0x38.end()) {
				return;
			}
			instance = *it;
			m_unk0x38.erase(it);
		}
		FUN_004267a0(instance);
	}
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

// FUNCTION: ROBLOX 0x00426cf0
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

// FUNCTION: ROBLOX 0x00426e00
CRobloxTreeCtrlNode* CRobloxTreeCtrl::FUN_00426e00(const boost::shared_ptr<RBX::Instance>& instance)
{
	if (!instance) {
		return NULL;
	}

	{
		boost::recursive_mutex::scoped_lock lock(m_unk0xb0);
		std::map<RBX::Instance*, CRobloxTreeCtrlNode*>::iterator it = m_unk0x98.find(instance.get());
		if (it != m_unk0x98.end()) {
			return it->second;
		}
	}

	CRobloxTreeCtrlNode* parent = FUN_00426e00(RBX::shared_from(instance->getParent()));
	if (!parent) {
		return NULL;
	}
	return parent->FUN_004267a0(instance);
}

// FUNCTION: ROBLOX 0x00426f50
void CSelectionTreeCtrl::FUN_00426f50()
{
	for (int i = 0; i < 10; i++) {
		boost::shared_ptr<RBX::Instance> instance;
		{
			boost::recursive_mutex::scoped_lock lock(m_unk0xb0);
			std::set<boost::shared_ptr<RBX::Instance> >::iterator it = m_unk0xd0.begin();
			if (it == m_unk0xd0.end()) {
				return;
			}
			instance = *it;
			m_unk0xd0.erase(it);
		}
		FUN_00426e00(instance);
	}
}

// FUNCTION: ROBLOX 0x004270b0
void CSelectionTreeCtrl::OnTimer(UINT_PTR nIDEvent)
{
	CRobloxTreeCtrlNode* node = NULL;
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0xb0);
		if (m_unk0xdc.size() > 0) {
			node = *m_unk0xdc.begin();
		}
	}

	if (node) {
		RBX::ScopedAssign<undefined> updating(m_unk0xcd, true);
		node->FUN_004221a0();
		node->VTable0x08();
		{
			RBX::ScopedAssign<undefined> selecting(m_unk0xcc, true);
			node->FUN_00425c40();
		}
		node->FUN_00426b00();

		boost::recursive_mutex::scoped_lock lock(m_unk0xb0);
		if (node->m_unk0x38.empty()) {
			VTable0x158(node);
		}
	}

	FUN_00426f50();
	CXTTreeCtrl::OnTimer(nIDEvent);
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
