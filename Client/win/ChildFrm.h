#ifndef CHILDFRM_H
#define CHILDFRM_H

#include "decomp.h"

// SIZE 0xdc
class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)

	// SYNTHETIC: ROBLOX 0x0040f820
	// ?GetThisClass@CChildFrame@@SGPAUCRuntimeClass@@XZ
	// SYNTHETIC: ROBLOX 0x0040fa00
	// ?GetMessageMap@CChildFrame@@MBEPBUAFX_MSGMAP@@XZ

public:
	CChildFrame();
	virtual ~CChildFrame();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs); // vtable+0x6c
	virtual void ActivateFrame(int nCmdShow = -1);  // vtable+0x154

	// SYNTHETIC: ROBLOX 0x0040fa10
	// ??_GCChildFrame@@UAEPAXI@Z

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

DECOMP_SIZE_ASSERT(CChildFrame, 0xdc)

// SIZE 0xdc
class CRbxChildFrame : public CChildFrame
{
	DECLARE_DYNCREATE(CRbxChildFrame)

	// SYNTHETIC: ROBLOX 0x0040f8b0
	// ?GetThisClass@CRbxChildFrame@@SGPAUCRuntimeClass@@XZ
	// SYNTHETIC: ROBLOX 0x0040fec0
	// ?GetMessageMap@CRbxChildFrame@@MBEPBUAFX_MSGMAP@@XZ

protected:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

	DECLARE_MESSAGE_MAP()
};

DECOMP_SIZE_ASSERT(CRbxChildFrame, 0xdc)

#endif // CHILDFRM_H
