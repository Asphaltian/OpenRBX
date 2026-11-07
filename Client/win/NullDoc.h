#ifndef NULLDOC_H
#define NULLDOC_H

#include "decomp.h"

// SIZE 0xd8
class CBrowserFrameWnd : public CXTPFrameWnd
{
	DECLARE_DYNCREATE(CBrowserFrameWnd)

	// SYNTHETIC: ROBLOX 0x00437150
	// ?GetThisClass@CBrowserFrameWnd@@SGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x00437440
	// ?CreateObject@CBrowserFrameWnd@@SGPAVCObject@@XZ

protected:
	CBrowserFrameWnd() {}

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

	// SYNTHETIC: ROBLOX 0x00437430
	// ?GetMessageMap@CBrowserFrameWnd@@MBEPBUAFX_MSGMAP@@XZ
};

DECOMP_SIZE_ASSERT(CBrowserFrameWnd, 0xd8)

// SIZE 0x54
class CNullDoc : public CDocument
{
	DECLARE_DYNCREATE(CNullDoc)

	// SYNTHETIC: ROBLOX 0x004374a0
	// ?GetThisClass@CNullDoc@@SGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x004374d0
	// ?CreateObject@CNullDoc@@SGPAVCObject@@XZ
	// SYNTHETIC: ROBLOX 0x00437530
	// ?GetMessageMap@CNullDoc@@MBEPBUAFX_MSGMAP@@XZ

protected:
	CNullDoc() {}

public:
	virtual BOOL IsModified() { return FALSE; }  // vtable+0x60
	virtual BOOL SaveModified() { return TRUE; } // vtable+0x98

	// SYNTHETIC: ROBLOX 0x0040a800
	// ??_GCNullDoc@@UAEPAXI@Z

protected:
	afx_msg void FUN_004374b0(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

DECOMP_SIZE_ASSERT(CNullDoc, 0x54)

#endif // NULLDOC_H
