#ifndef MAINFRM_H
#define MAINFRM_H

#include "decomp.h"
#include "util/Events.h"
#include "v8datamodel/BrickColor.h"

#include <boost/scoped_ptr.hpp>

namespace RBX {
class FillToolColor;
}

// SIZE 0xcc
class MyXTPCommandBars : public CXTPCommandBars
{
	DECLARE_DYNCREATE(MyXTPCommandBars)

	// SYNTHETIC: ROBLOX 0x00432540
	// ?GetThisClass@MyXTPCommandBars@@SGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x00433aa0
	// ?CreateObject@MyXTPCommandBars@@SGPAVCObject@@XZ

protected:
	// FUNCTION: ROBLOX 0x00432500
	virtual CXTPToolBar* AddCustomBar(LPCTSTR lpcstrCaption, UINT nID, BOOL bTearOffBar = FALSE)
	{
		CXTPToolBar* toolBar = CXTPCommandBars::AddCustomBar(lpcstrCaption, nID, bTearOffBar);

		if (nID == 1005) {
			toolBar->EnableDocking(xtpFlagFloating);
		}

		return toolBar;
	}

	// SYNTHETIC: ROBLOX 0x00433b00
	// ??_GMyXTPCommandBars@@UAEPAXI@Z
};

DECOMP_SIZE_ASSERT(MyXTPCommandBars, 0xcc)

// SIZE 0x1d8
class CPatchedControlComboBox : public CXTPControlComboBox
{
	DECLARE_XTP_CONTROL(CPatchedControlComboBox)

	// SYNTHETIC: ROBLOX 0x00432750
	// ?GetThisClass@CPatchedControlComboBox@@SGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x00433f20
	// ?CreateObject@CPatchedControlComboBox@@SGPAVCObject@@XZ

public:
	// FUNCTION: ROBLOX 0x00432720
	CPatchedControlComboBox() {}

	virtual BOOL Compare(CXTPControl* pOther); // vtable+0x88
};

// SIZE 0x14c
class CWrapperView : public CView
{
	DECLARE_DYNCREATE(CWrapperView)

	// SYNTHETIC: ROBLOX 0x00432970
	// ?GetThisClass@CWrapperView@@SGPAUCRuntimeClass@@XZ

protected:
	CWrapperView();

public:
	virtual ~CWrapperView();

	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo); // vtable+0x14
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);        // vtable+0x114
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);                                             // vtable+0x6c
	virtual void OnDraw(CDC* pDC);

private:
	undefined m_unk0x58[0x14c - 0x58]; // 0x58
};

DECOMP_SIZE_ASSERT(CWrapperView, 0x14c)

// SIZE 0x494
class CMainFrame : public CXTPMDIFrameWnd, public RBX::Listener<RBX::FillToolColor, RBX::BrickColor>
{
	DECLARE_DYNAMIC(CMainFrame)

	// SYNTHETIC: ROBLOX 0x004324f0
	// ?GetThisClass@CMainFrame@@SGPAUCRuntimeClass@@XZ
	// SYNTHETIC: ROBLOX 0x00436340
	// ?GetMessageMap@CMainFrame@@MBEPBUAFX_MSGMAP@@XZ

public:
	CMainFrame();
	virtual ~CMainFrame();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);                           // vtable+0x6c
	virtual void ActivateFrame(int nCmdShow = -1);                            // vtable+0x154
	virtual void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState); // vtable+0x158

	bool FUN_00432550(int nCmdShow);
	void FUN_00434b80();
	void FUN_00435230(bool p_unk0x04);

	// clang-format off
	// STUB: ROBLOX 0x00434730
	// ?raise@?$Notifier@VFillToolColor@RBX@@VBrickColor@2@@RBX@@IBEXVBrickColor@2@PAV?$Listener@VFillToolColor@RBX@@VBrickColor@2@@2@@Z
	// TEMPLATE: ROBLOX 0x00435100
	// ?raise@?$Notifier@VFillToolColor@RBX@@VBrickColor@2@@RBX@@IBEXVBrickColor@2@@Z
	// STUB: ROBLOX 0x0047d950
	// ?addListener@?$Notifier@VFillToolColor@RBX@@VBrickColor@2@@RBX@@QBEXPAV?$Listener@VFillToolColor@RBX@@VBrickColor@2@@2@@Z
	// clang-format on

	// SYNTHETIC: ROBLOX 0x00436320
	// CMainFrame::`scalar deleting destructor'

protected:
	virtual void onEvent(const RBX::FillToolColor* source, RBX::BrickColor event);

	void FUN_004325e0(UINT nID, CRect rect, XTPDockingPaneDirection direction);
	void FUN_00433b20(CXTPPropExchange* pPX, BOOL p_unk0x08);
	void RestoreResolution();
	void getMonitorInfo();
	bool FUN_00434970(bool p_unk0x04);
	void FUN_00435b60();
	void FUN_00435e00(bool p_unk0x04);

	afx_msg void FUN_00432650();
	afx_msg void FUN_00432680();
	afx_msg void FUN_00432980();
	afx_msg void FUN_00432990(CCmdUI* pCmdUI);
	afx_msg void FUN_004329b0(CCmdUI* pCmdUI);
	afx_msg void FUN_00432a80(CCmdUI* pCmdUI);
	afx_msg void FUN_00432b50();
	afx_msg void FUN_00432b60();
	afx_msg void FUN_00432b90(CCmdUI* pCmdUI);
	afx_msg void FUN_00432bc0(CCmdUI* pCmdUI);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void FUN_00433ce0();
	afx_msg void FUN_00433d10();
	afx_msg void FUN_00433d50();
	afx_msg int OnCreateCommandBar(LPCREATEBARSTRUCT lpCreatePopup);
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
	afx_msg void OnClose();
	afx_msg void FUN_00434d90(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void FUN_00434ea0();
	afx_msg void FUN_00435460(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void FUN_00435570(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void FUN_004357d0();
	afx_msg LRESULT FUN_004357f0(WPARAM wParam, LPARAM lParam);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void FUN_00435fa0();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void FUN_00453eb0(CCmdUI* pCmdUI);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

private:
	bool m_unk0xe0;                               // 0xe0
	bool m_unk0xe1;                               // 0xe1
	HMONITOR m_unk0xe4;                           // 0xe4
	CCmdTarget* m_unk0xe8;                        // 0xe8
	bool m_unk0xec;                               // 0xec
	bool m_unk0xed;                               // 0xed
	CString m_unk0xf0;                            // 0xf0
	CXTWindowPos m_unk0xf4;                       // 0xf4
	CXTPDockingPaneManager m_unk0x120;            // 0x120
	CXTPTabClientWnd m_unk0x26c;                  // 0x26c
	boost::scoped_ptr<CXTSplitterWnd> m_unk0x390; // 0x390
	boost::scoped_ptr<CWnd> m_unk0x394;           // 0x394
	boost::scoped_ptr<CWnd> m_unk0x398;           // 0x398
	CXTThemeManager m_unk0x39c;                   // 0x39c
	CXTPStatusBar m_unk0x3c8;                     // 0x3c8
};

#endif // MAINFRM_H
