#include "RobloxTreeCtrl.h"

#include "MarshaledListener.h"
#include "ReflectionMetadata.h"
#include "Roblox.h"
#include "RobloxTreeCtrlNode.h"
#include "SelectionTreeCtrl.h"
#include "script/Script.h"
#include "util/scopedassign.h"
#include "v8datamodel/Workspace.h"

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

// FUNCTION: ROBLOX 0x004221f0
void CRobloxTreeCtrl::FUN_004221f0(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTREEVIEW* treeView = reinterpret_cast<NMTREEVIEW*>(pNMHDR);
	reinterpret_cast<CRobloxTreeCtrlNode*>(treeView->itemNew.lParam)->VTable0x10(treeView->action);
	*pResult = 0;
}

// FUNCTION: ROBLOX 0x00422210
void CRobloxTreeCtrl::FUN_00422210(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTVDISPINFO* dispInfo = reinterpret_cast<NMTVDISPINFO*>(pNMHDR);
	reinterpret_cast<CRobloxTreeCtrlNode*>(dispInfo->item.lParam)->VTable0x14(dispInfo);
	*pResult = 0;
}

// FUNCTION: ROBLOX 0x00422230
void CSelectionTreeCtrl::FUN_00422230(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = m_unk0xcc != 0;
}

// VTABLE: ROBLOX 0x008024f0
// SIZE 0x2c
class CEnabledCmdUI : public CCmdUI
{
public:
	CEnabledCmdUI() : m_unk0x28(FALSE) {}

	// FUNCTION: ROBLOX 0x00422250
	virtual void Enable(BOOL bOn = TRUE)
	{
		m_unk0x28 = bOn;
		m_bEnableChanged = TRUE;
	}

	virtual void SetCheck(int nCheck = 1) {}
	virtual void SetRadio(BOOL bOn = TRUE) {}
	virtual void SetText(LPCTSTR lpszText) {}

	BOOL m_unk0x28; // 0x28
};

DECOMP_SIZE_ASSERT(CEnabledCmdUI, 0x2c)

// FUNCTION: ROBLOX 0x00422270
void CSelectionTreeCtrl::FUN_00422270(CPoint point)
{
	CMenu* popup = m_unk0xc4.GetSubMenu(0);
	if (popup) {
		for (UINT i = 0; i < popup->GetMenuItemCount(); i++) {
			CEnabledCmdUI state;
			state.m_nID = popup->GetMenuItemID(i);
			if (state.m_nID == 0) {
				continue;
			}
			if (state.m_nID == (UINT) -1) {
				popup->GetSubMenu(i);
				continue;
			}

			AfxGetMainWnd()->OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL);
			popup->EnableMenuItem(i, MF_BYPOSITION | (state.m_unk0x28 ? MF_ENABLED : MF_DISABLED));
		}

		if (point.x == -1 && point.y == -1) {
			CRect rect;
			GetClientRect(&rect);
			point = rect.CenterPoint();
			ClientToScreen(&point);
			CXTPCommandBars::TrackPopupMenu(
				popup, TPM_CENTERALIGN | TPM_VCENTERALIGN, point.x, point.y, GetParentFrame()
			);
		}
		else {
			CXTPCommandBars::TrackPopupMenu(popup, 0, point.x, point.y, GetParentFrame());
		}
	}
}

// FUNCTION: ROBLOX 0x00422400
void CSelectionTreeCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
{
	FUN_00422270(point);
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

// FUNCTION: ROBLOX 0x00422b80
void CRobloxTreeCtrl::FUN_00422b80(NMHDR* pNMHDR, LRESULT* pResult)
{
	RBX::ScopedAssign<undefined> deleting(m_unk0xa4, true);
	delete reinterpret_cast<CRobloxTreeCtrlNode*>(reinterpret_cast<NMTREEVIEW*>(pNMHDR)->itemOld.lParam);
	*pResult = 0;
}

// FUNCTION: ROBLOX 0x00422cc0
void CRobloxTreeCtrl::OnDestroy()
{
	CXTTreeCtrl::OnDestroy();

	RBX::ScopedAssign<undefined> clearing(m_unk0xa4, true);
	DeleteAllItems();
	if (m_unk0x94) {
		delete m_unk0x94;
		m_unk0x94 = NULL;
	}
}

// FUNCTION: ROBLOX 0x00422d60
BOOL CRobloxTreeCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~TVS_DISABLEDRAGDROP;
	cs.style |= TVS_HASBUTTONS | TVS_LINESATROOT | TVS_EDITLABELS | TVS_SHOWSELALWAYS;
	return CXTTreeCtrl::PreCreateWindow(cs);
}

// FUNCTION: ROBLOX 0x00422da0
void CSelectionTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	CXTTreeCtrl::OnRButtonDown(nFlags, point);
	ClientToScreen(&point);
	FUN_00422270(point);
}

// FUNCTION: ROBLOX 0x00422de0
void CRobloxTreeCtrl::FUN_00422de0(NMHDR* pNMHDR, LRESULT* pResult)
{
	GetEditControl()->LimitText(32);
	*pResult = 0;
}

// FUNCTION: ROBLOX 0x00422e20
void CRobloxTreeCtrl::OnPaint()
{
	COLORREF face = XTPColorManager()->GetColor(COLOR_3DFACE);
	COLORREF highlight = XTPColorManager()->GetColor(COLOR_HIGHLIGHT);
	XTPColorManager()->SetColor(
		COLOR_3DFACE,
		RGB((GetRValue(face) + GetRValue(highlight)) / 2,
			(GetGValue(face) + GetGValue(highlight)) / 2,
			(GetBValue(face) + GetBValue(highlight)) / 2)
	);
	CXTTreeCtrl::OnPaint();
	XTPColorManager()->SetColor(COLOR_3DFACE, face);
}

// FUNCTION: ROBLOX 0x00422ec0
void CRobloxTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_F2) {
		EditLabel(GetSelectedItem());
	}
	CXTTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

// FUNCTION: ROBLOX 0x004230c0
int FUN_004230c0(RBX::Instance* instance)
{
	boost::shared_ptr<RBX::Reflection::Metadata::Reflection> reflection =
		RBX::Reflection::Metadata::Reflection::FUN_004480c0();
	RBX::Reflection::Metadata::Class* metadata = reflection->FUN_00446220(&instance->getDescriptor(), true);
	return metadata ? metadata->FUN_00422100() : 0;
}

// FUNCTION: ROBLOX 0x00423170
void CSelectionTreeCtrl::FUN_00423170(TVITEM* item, bool select)
{
	if (item->hItem && item->lParam) {
		boost::shared_ptr<RBX::Instance> instance = reinterpret_cast<CRobloxTreeCtrlNode*>(item->lParam)->m_unk0x0c;
		if (instance) {
			RBX::DataModel::Lock lock(VTable0x14c());
			RBX::Selection* selection = VTable0x148();
			RBX::ScopedAssign<undefined> updating(m_unk0xce, true);
			if (select) {
				selection->addToSelection(instance.get());
			}
			else {
				selection->removeFromSelection(instance.get());
			}
		}
	}
}

// FUNCTION: ROBLOX 0x004232d0
void CSelectionTreeCtrl::FUN_004232d0(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	if (!m_unk0xcd && !m_unk0xa4 && m_unk0xe8) {
		NMTREEVIEW* treeView = reinterpret_cast<NMTREEVIEW*>(pNMHDR);
		if (treeView->itemNew.hItem) {
			FUN_00423170(&treeView->itemNew, true);
		}
		if (treeView->itemOld.hItem && treeView->action) {
			FUN_00423170(&treeView->itemOld, false);
		}
	}
}

// STUB: ROBLOX 0x00423330
int CRobloxTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTTreeCtrl::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	Init();

	BOOL alpha = FALSE;
	CBitmap bitmap;
	bitmap.Attach(CXTPImageManagerIcon::LoadBitmapFromResource(MAKEINTRESOURCE(0x85), &alpha));

	if (alpha) {
		if (!m_unk0xa8.Create(16, 16, ILC_COLOR32, 0, 0)) {
			return -1;
		}
		if (m_unk0xa8.Add(&bitmap, (CBitmap*) NULL) == -1) {
			return -1;
		}
	}
	else {
		if (!m_unk0xa8.Create(16, 16, ILC_COLORDDB | ILC_MASK, 0, 0)) {
			return -1;
		}
		if (m_unk0xa8.Add(&bitmap, RGB(255, 0, 255)) == -1) {
			return -1;
		}
	}

	SetImageList(&m_unk0xa8, TVSIL_NORMAL);
	EnableMultiSelect(TRUE);
	VTable0x15c();
	return 0;
}

// FUNCTION: ROBLOX 0x004234f0
void CRobloxTreeCtrl::FUN_004234f0(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTVDISPINFO* dispInfo = reinterpret_cast<NMTVDISPINFO*>(pNMHDR);
	if (dispInfo->item.pszText) {
		CRobloxTreeCtrlNode* node = reinterpret_cast<CRobloxTreeCtrlNode*>(dispInfo->item.lParam);
		RBX::DataModel::Lock lock(VTable0x14c());
		node->m_unk0x0c->setName(dispInfo->item.pszText);
		*pResult = 1;
	}
	else {
		*pResult = 0;
	}
}

// STUB: ROBLOX 0x00423630
void CSelectionTreeCtrl::FUN_00423630(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTREEVIEW* treeView = reinterpret_cast<NMTREEVIEW*>(pNMHDR);
	m_unk0xf0.reset(CreateDragImage(treeView->itemNew.hItem));
	if (m_unk0xf0.get()) {
		CRect rect;
		GetItemRect(treeView->itemNew.hItem, &rect, TRUE);
		int indent = GetIndent();
		CPoint point = treeView->ptDrag;
		m_unk0xf0->BeginDrag(0, CPoint(point.x - rect.left + indent, point.y - rect.top));
		ClientToScreen(&point);
		CImageList::DragEnter(NULL, point);
		SetCapture();
	}
	*pResult = 0;
}

// FUNCTION: ROBLOX 0x00423730
int CSelectionTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CRobloxTreeCtrl::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	FUN_004204e0();
	SetTimer(0, 200, NULL);
	return 0;
}

// FUNCTION: ROBLOX 0x004239a0
void CRobloxTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	Default();

	if (nFlags & (MK_SHIFT | MK_CONTROL)) {
		return;
	}

	HTREEITEM item = HitTest(point, &nFlags);
	if (item && (nFlags & TVHT_ONITEM)) {
		CRobloxTreeCtrlNode* node = reinterpret_cast<CRobloxTreeCtrlNode*>(GetItemData(item));
		if (node) {
			boost::shared_ptr<RBX::Script> script = boost::shared_dynamic_cast<RBX::Script>(node->m_unk0x0c);
			if (script) {
				RBX::DataModel::Lock lock(VTable0x14c());
				static_cast<CRobloxApp*>(AfxGetApp())->FUN_0044ef90(script);
			}
		}
	}
}

// FUNCTION: ROBLOX 0x00423cc0
HTREEITEM CSelectionTreeCtrl::FUN_00423cc0(HTREEITEM item)
{
	if (item) {
		CRobloxTreeCtrlNode* node = reinterpret_cast<CRobloxTreeCtrlNode*>(GetItemData(item));
		if (node) {
			boost::shared_ptr<RBX::Instance> instance = node->m_unk0x0c;
			if (instance && instance->FUN_00422f10(VTable0x148()->begin(), VTable0x148()->end())) {
				return item;
			}
		}
	}
	return NULL;
}

// FUNCTION: ROBLOX 0x00423dc0
void CSelectionTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_unk0xf0.get()) {
		CPoint screen = point;
		ClientToScreen(&screen);
		CImageList::DragMove(screen);

		UINT flags;
		HTREEITEM target = HitTest(point, &flags);
		if (target) {
			if (flags & TVHT_ONITEM) {
				target = FUN_00423cc0(target);
			}
			else {
				target = NULL;
			}
		}

		CImageList::DragShowNolock(FALSE);
		SelectDropTarget(target);
		CImageList::DragShowNolock(TRUE);
	}
	CXTTreeCtrl::OnMouseMove(nFlags, point);
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

// FUNCTION: ROBLOX 0x00425240
int FUN_00425240(const RBX::Reflection::ClassDescriptor* descriptor)
{
	boost::shared_ptr<RBX::Reflection::Metadata::Reflection> reflection =
		RBX::Reflection::Metadata::Reflection::FUN_004480c0();
	RBX::Reflection::Metadata::Class* metadata = reflection->FUN_00446220(descriptor, true);
	return metadata ? metadata->FUN_004251c0() : 0;
}

// FUNCTION: ROBLOX 0x004252f0
HTREEITEM CRobloxTreeCtrlNode::VTable0x18()
{
	TVINSERTSTRUCT insert;
	insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;
	insert.item.pszText = LPSTR_TEXTCALLBACK;
	insert.item.cChildren = I_CHILDRENCALLBACK;
	insert.item.iImage = insert.item.iSelectedImage = FUN_00425240(m_unk0x4c);
	insert.item.lParam = reinterpret_cast<LPARAM>(this);
	insert.hParent = m_unk0x34 ? m_unk0x34->m_unk0x44 : TVI_ROOT;

	HTREEITEM after = TVI_FIRST;
	for (HTREEITEM item = m_unk0x30->GetChildItem(insert.hParent); item;
		 item = m_unk0x30->GetNextItem(item, TVGN_NEXT)) {
		CRobloxTreeCtrlNode* sibling = reinterpret_cast<CRobloxTreeCtrlNode*>(m_unk0x30->GetItemData(item));
		if (m_unk0x48 > sibling->m_unk0x48) {
			after = item;
		}
		else if (m_unk0x48 == sibling->m_unk0x48) {
			RBX::DataModel::Lock lock(m_unk0x30->VTable0x14c());
			if (!(m_unk0x0c->getName() >= sibling->m_unk0x0c->getName())) {
				break;
			}
			after = item;
		}
		else {
			break;
		}
	}

	insert.hInsertAfter = after;
	return m_unk0x30->InsertItem(&insert);
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

// FUNCTION: ROBLOX 0x004255f0
bool CRobloxTreeCtrlNode::FUN_004255f0()
{
	RBX::DataModel::Lock lock(m_unk0x30->VTable0x14c());
	boost::shared_ptr<const std::vector<boost::shared_ptr<RBX::Instance> > > children = m_unk0x0c->getChildren().read();
	if (!children) {
		return false;
	}

	std::vector<boost::shared_ptr<RBX::Instance> >::const_iterator end = children->end();
	for (std::vector<boost::shared_ptr<RBX::Instance> >::const_iterator it = children->begin(); it != end; ++it) {
		boost::shared_ptr<RBX::Reflection::Metadata::Reflection> reflection =
			RBX::Reflection::Metadata::Reflection::FUN_004480c0();
		RBX::Reflection::Metadata::Class* metadata = reflection->FUN_00446220(&(*it)->getDescriptor(), true);
		if (metadata && metadata->FUN_00422100() >= 0) {
			return true;
		}
	}
	return false;
}

// FUNCTION: ROBLOX 0x004258b0
void CRobloxTreeCtrlNode::VTable0x14(NMTVDISPINFO* dispInfo)
{
	try {
		if (dispInfo->item.mask & TVIF_TEXT) {
			RBX::DataModel::Lock lock(m_unk0x30->VTable0x14c());
			strncpy(dispInfo->item.pszText, m_unk0x0c->getName().c_str(), dispInfo->item.cchTextMax);
		}
		if (dispInfo->item.mask & TVIF_CHILDREN) {
			dispInfo->item.cChildren = FUN_004255f0() ? 1 : 0;
		}
	}
	catch (std::exception&) {
		strncpy(dispInfo->item.pszText, "", dispInfo->item.cchTextMax);
	}
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

// FUNCTION: ROBLOX 0x00425ef0
void CSelectionTreeCtrl::onEvent(const RBX::Selection* source, RBX::SelectionChanged event)
{
	if (!m_unk0xce) {
		if (event.addedItem) {
			boost::recursive_mutex::scoped_lock lock(m_unk0xb0);
			std::map<RBX::Instance*, CRobloxTreeCtrlNode*>::iterator it = m_unk0x98.find(event.addedItem.get());
			if (it != m_unk0x98.end()) {
				VTable0x154(it->second);
			}
			else {
				m_unk0xd0.insert(RBX::shared_from(event.addedItem.get()));
			}
		}

		if (event.removedItem) {
			boost::recursive_mutex::scoped_lock lock(m_unk0xb0);
			m_unk0xd0.erase(event.removedItem);
			std::map<RBX::Instance*, CRobloxTreeCtrlNode*>::iterator it = m_unk0x98.find(event.removedItem.get());
			if (it != m_unk0x98.end()) {
				VTable0x154(it->second);
			}
		}
	}
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

// FUNCTION: ROBLOX 0x004266a0
void CRobloxTreeCtrlNode::VTable0x0c()
{
	m_unk0x44 = VTable0x18();
	VTable0x08();

	RBX::DataModel::Lock lock(m_unk0x30->VTable0x14c());
	m_unk0x0c->RBX::Notifier<RBX::Instance, RBX::ChildAdded>::addListener(this);
	m_unk0x0c->RBX::Notifier<RBX::Instance, RBX::ChildRemoved>::addListener(this);
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

// STUB: ROBLOX 0x00428720
void CRobloxTreeCtrl::FUN_00428720(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTREEVIEW* treeView = reinterpret_cast<NMTREEVIEW*>(pNMHDR);
	if (treeView->action & TVE_EXPAND) {
		CRobloxTreeCtrlNode* node = reinterpret_cast<CRobloxTreeCtrlNode*>(treeView->itemNew.lParam);
		if (!node->m_unk0x2d) {
			node->FUN_00426960();
			node->FUN_00426b00();
		}
	}
	*pResult = 0;
}

// STUB: ROBLOX 0x00428770
static void FUN_00428770(RBX::Instance* parent, boost::shared_ptr<RBX::Instance> child)
{
	if (child->getParent() == parent) {
		return;
	}

	RBX::Workspace* workspace = RBX::Workspace::findWorkspace(parent);
	if (workspace) {
		bool descendent = child->isDescendentOf(workspace);
		if (!descendent) {
			std::vector<boost::shared_ptr<RBX::Instance> > instances;
			instances.push_back(child);
			workspace->insertInstances(instances, workspace, RBX::INSERT_TO_TREE, RBX::SUPPRESS_PROMPTS);
		}
	}

	child->setParent(parent);
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

template class RBX::MarshaledListener<RBX::Instance, RBX::DescendentAdded, RBX::Instance, RBX::DescendentAdded>;
template class RBX::MarshaledListener<RBX::Instance, RBX::DescendentRemoving, RBX::Instance, RBX::DescendentRemoving>;

// FUNCTION: ROBLOX 0x00429040
void CSelectionTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	Default();

	if (m_unk0xf0.get()) {
		CImageList::DragLeave(this);
		CImageList::EndDrag();
		ReleaseCapture();
		m_unk0xf0.reset();

		HTREEITEM target = GetDropHilightItem();
		if (target) {
			CRobloxTreeCtrlNode* node = reinterpret_cast<CRobloxTreeCtrlNode*>(GetItemData(target));
			if (node) {
				boost::shared_ptr<RBX::Instance> instance = node->m_unk0x0c;
				RBX::DataModel::Lock lock(m_unk0xe8);
				if (instance) {
					std::for_each(
						VTable0x148()->begin(), VTable0x148()->end(), boost::bind(&FUN_00428770, instance.get(), _1)
					);
				}
				m_unk0xe8->clearStack();
				m_unk0xe8->setDirty(true);
			}
		}

		SelectDropTarget(NULL);
	}
}

// clang-format off
BEGIN_MESSAGE_MAP(CRobloxTreeCtrl, CXTTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_GETDISPINFO, FUN_00422210)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, FUN_00428720)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, FUN_004221f0)
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, FUN_00422b80)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, FUN_00422de0)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, FUN_004234f0)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
	ON_WM_CREATE()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CSelectionTreeCtrl, CRobloxTreeCtrl)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, FUN_004232d0)
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, FUN_00422230)
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, FUN_00423630)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()
// clang-format on
