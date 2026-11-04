#ifndef OBJECTBROWSER_H
#define OBJECTBROWSER_H

#include "decomp.h"

// SIZE 0x288
class CObjectBrowser : public CView
{
	DECLARE_DYNCREATE(CObjectBrowser)

	// SYNTHETIC: ROBLOX 0x00437610
	// ?GetThisClass@CObjectBrowser@@SGPAUCRuntimeClass@@XZ

protected:
	CObjectBrowser();

public:
	virtual ~CObjectBrowser();

	// SYNTHETIC: ROBLOX 0x00437c30
	// CObjectBrowser::`scalar deleting destructor'

	virtual void OnDraw(CDC* pDC);

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

	// SYNTHETIC: ROBLOX 0x00438cd0
	// ?GetMessageMap@CObjectBrowser@@MBEPBUAFX_MSGMAP@@XZ

private:
	CXTSplitterWnd m_unk0x58;  // 0x58
	CXTSplitterWnd m_unk0x170; // 0x170
};

DECOMP_SIZE_ASSERT(CObjectBrowser, 0x288)

#endif // OBJECTBROWSER_H
