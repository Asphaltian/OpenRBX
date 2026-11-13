#ifndef WEBTOOLBOX_H
#define WEBTOOLBOX_H

#include "Roblox_h.h"
#include "SecureHtmlView.h"
#include "decomp.h"

#include <atlbase.h>

class CRobloxWnd;

// SIZE 0xfc
class CWebToolbox : public CSecureHtmlView
{
	DECLARE_DYNCREATE(CWebToolbox)

	// SYNTHETIC: ROBLOX 0x0046e9e0
	// ?_GetBaseClass@CWebToolbox@@KGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x0046e9f0
	// ?GetThisClass@CWebToolbox@@SGPAUCRuntimeClass@@XZ

protected:
	CWebToolbox() {}

public:
	virtual ~CWebToolbox() {}

	void FUN_0046ea00(int category);

	virtual void OnInitialUpdate();
	virtual HRESULT OnGetExternal(LPDISPATCH* lppDispatch);
	virtual HRESULT OnDocWindowActivate(BOOL fActivate);
	virtual HRESULT OnFrameWindowActivate(BOOL fActivate);
	virtual HRESULT OnGetHostInfo(DOCHOSTUIINFO* pInfo);

	// SYNTHETIC: ROBLOX 0x0046ec50
	// CWebToolbox::`scalar deleting destructor'

	// SYNTHETIC: ROBLOX 0x0046ecd0
	// ?CreateObject@CWebToolbox@@SGPAVCObject@@XZ

protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	DECLARE_MESSAGE_MAP()

	// SYNTHETIC: ROBLOX 0x0046ed50
	// ?GetMessageMap@CWebToolbox@@MBEPBUAFX_MSGMAP@@XZ

private:
	friend class CRobloxView;

	CRobloxWnd* m_unk0xf4;         // 0xf4
	CComPtr<IWorkspace> m_unk0xf8; // 0xf8
};

DECOMP_SIZE_ASSERT(CWebToolbox, 0xfc)

#endif // WEBTOOLBOX_H
