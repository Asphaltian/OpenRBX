#ifndef MEMBERTREEVIEW_H
#define MEMBERTREEVIEW_H

#include "decomp.h"

// SIZE 0xb4
class CMemberTreeView : public CXTTreeView
{
	DECLARE_DYNCREATE(CMemberTreeView)

	// SYNTHETIC: ROBLOX 0x00437850
	// ?GetThisClass@CMemberTreeView@@SGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x00438050
	// ?CreateObject@CMemberTreeView@@SGPAVCObject@@XZ

protected:
	CMemberTreeView();

public:
	virtual ~CMemberTreeView();

	// SYNTHETIC: ROBLOX 0x004380c0
	// CMemberTreeView::`scalar deleting destructor'

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void FUN_0043a240(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	// SYNTHETIC: ROBLOX 0x0043a420
	// ?GetMessageMap@CMemberTreeView@@MBEPBUAFX_MSGMAP@@XZ

private:
	CImageList m_unk0xa0; // 0xa0
	undefined m_unk0xa8;  // 0xa8
	undefined4 m_unk0xac; // 0xac
	undefined4 m_unk0xb0; // 0xb0
};

DECOMP_SIZE_ASSERT(CMemberTreeView, 0xb4)

#endif // MEMBERTREEVIEW_H
