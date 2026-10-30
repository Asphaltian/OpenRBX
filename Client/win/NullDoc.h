#ifndef NULLDOC_H
#define NULLDOC_H

#include "decomp.h"

// SIZE 0x54
class CNullDoc : public CDocument
{
	DECLARE_DYNCREATE(CNullDoc)

	// SYNTHETIC: ROBLOX 0x004374a0
	// ?GetThisClass@CNullDoc@@SGPAUCRuntimeClass@@XZ

	// STUB: ROBLOX 0x004374d0
	// ?CreateObject@CNullDoc@@SGPAVCObject@@XZ
	// SYNTHETIC: ROBLOX 0x00437530
	// ?GetMessageMap@CNullDoc@@MBEPBUAFX_MSGMAP@@XZ

protected:
	CNullDoc() {}

public:
	virtual BOOL IsModified() { return FALSE; } // vtable+0x60

	// SYNTHETIC: ROBLOX 0x0040a800
	// ??_GCNullDoc@@UAEPAXI@Z

protected:
	afx_msg void FUN_004374b0(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

DECOMP_SIZE_ASSERT(CNullDoc, 0x54)

#endif // NULLDOC_H
