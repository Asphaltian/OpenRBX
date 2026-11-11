#ifndef SELECTIONTREECTRL_H
#define SELECTIONTREECTRL_H

#include "ActiveDocView.h"
#include "RobloxTreeCtrl.h"
#include "decomp.h"
#include "util/Events.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Selection.h"

#include <boost/shared_ptr.hpp>
#include <memory>
#include <set>

// SIZE 0xf4
class CSelectionTreeCtrl : public CRobloxTreeCtrl,
						   public ActiveDocView,
						   public RBX::Listener<RBX::Selection, RBX::SelectionChanged>
{
	DECLARE_DYNAMIC(CSelectionTreeCtrl)

	// SYNTHETIC: ROBLOX 0x00422190
	// ?GetThisClass@CSelectionTreeCtrl@@SGPAUCRuntimeClass@@XZ

public:
	CSelectionTreeCtrl();
	virtual ~CSelectionTreeCtrl();

	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	virtual RBX::Selection* VTable0x148();                         // vtable+0x148
	virtual boost::shared_ptr<RBX::DataModel> VTable0x14c() const; // vtable+0x14c
	virtual void VTable0x150(RBX::Instance* instance);             // vtable+0x150
	virtual void VTable0x154(CRobloxTreeCtrlNode* node);           // vtable+0x154
	virtual void VTable0x158(CRobloxTreeCtrlNode* node);           // vtable+0x158
	virtual void VTable0x15c();                                    // vtable+0x15c

	// SYNTHETIC: ROBLOX 0x00429010
	// CSelectionTreeCtrl::`scalar deleting destructor'

protected:
	virtual void VTable0x00(
		boost::shared_ptr<RBX::DataModel> newDataModel, boost::shared_ptr<RBX::DataModel> oldDataModel
	);
	virtual void onEvent(const RBX::Selection* source, RBX::SelectionChanged event);

	virtual void SelectAllIgnore(BOOL bSelect, HTREEITEM hIgnore);
	virtual BOOL OnButtonDown(BOOL bLeft, UINT nFlags, CPoint point);
	virtual void DoPreSelection(HTREEITEM hItem, BOOL bLeft, UINT nFlags);
	virtual void DoAction(HTREEITEM hItem, BOOL bLeft, UINT nFlags, CPoint point);
	virtual void DoBanding(UINT nFlags, CPoint point);
	virtual void UpdateSelectionForRect(LPCRECT pRect, UINT nFlags, CTypedPtrList<CPtrList, HTREEITEM>& list);

	afx_msg void FUN_00422230(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void FUN_00423630(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void FUN_004232d0(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

	// SYNTHETIC: ROBLOX 0x00429950
	// ?GetMessageMap@CSelectionTreeCtrl@@MBEPBUAFX_MSGMAP@@XZ

private:
	void FUN_00422270(CPoint point);
	void FUN_00423170(TVITEM* item, bool select);
	HTREEITEM FUN_00423cc0(HTREEITEM item);
	void FUN_00426f50();
	void FUN_004285a0(boost::shared_ptr<RBX::DataModel> dataModel);

	CMenu m_unk0xc4;                                       // 0xc4
	undefined m_unk0xcc;                                   // 0xcc
	undefined m_unk0xcd;                                   // 0xcd
	undefined m_unk0xce;                                   // 0xce
	std::set<boost::shared_ptr<RBX::Instance> > m_unk0xd0; // 0xd0
	std::set<CRobloxTreeCtrlNode*> m_unk0xdc;              // 0xdc
	boost::shared_ptr<RBX::DataModel> m_unk0xe8;           // 0xe8
	std::auto_ptr<CImageList> m_unk0xf0;                   // 0xf0
};

DECOMP_SIZE_ASSERT(CSelectionTreeCtrl, 0xf4)

#endif // SELECTIONTREECTRL_H
