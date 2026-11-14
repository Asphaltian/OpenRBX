#ifndef MAINFRM_H
#define MAINFRM_H

#include "SelectionTreeCtrl.h"
#include "placeholder.h"
#include "util/Events.h"
#include "v8datamodel/BrickColor.h"

#include <boost/scoped_ptr.hpp>

class CDataModelPropGrid;
class CSelectionCaption;

namespace RBX {
class FillToolColor;
}

void FUN_004334d0(CXTPImageManager* imageManager);

class MyXTPCommandBars : public CXTPCommandBars
{
	DECLARE_DYNCREATE(MyXTPCommandBars)

protected:
	virtual CXTPToolBar* AddCustomBar(LPCTSTR lpcstrCaption, UINT nID, BOOL bTearOffBar = FALSE)
	{
		CXTPToolBar* toolBar = CXTPCommandBars::AddCustomBar(lpcstrCaption, nID, bTearOffBar);

		if (nID == 1005) {
			toolBar->EnableDocking(xtpFlagFloating);
		}

		return toolBar;
	}
};

class CPatchedControlComboBox : public CXTPControlComboBox
{
	DECLARE_XTP_CONTROL(CPatchedControlComboBox)

public:
	CPatchedControlComboBox() {}

	virtual BOOL Compare(CXTPControl* pOther);
};

class CWrapperView : public CView
{
	DECLARE_DYNCREATE(CWrapperView)

protected:
	CWrapperView();

public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDraw(CDC* pDC);

private:
	CSelectionTreeCtrl m_unk0x58;
};

class CMainFrame : public CXTPMDIFrameWnd, public RBX::Listener<RBX::FillToolColor, RBX::BrickColor>
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame();
	virtual ~CMainFrame();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);

	bool FUN_00432550(int nCmdShow);
	void FUN_00434b80();
	void FUN_00435230(bool p_unk0x04);

protected:
	virtual void onEvent(const RBX::FillToolColor* source, RBX::BrickColor event);

	void FUN_004325e0(UINT nID, CRect rect, XTPDockingPaneDirection direction);
	void FUN_00433b20(CXTPPropExchange* pPX, BOOL p_unk0x08);
	void RestoreResolution();
	void getMonitorInfo();
	bool FUN_00434970(bool p_unk0x04);
	void ChangeResolution();
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

	friend class CRbxChildFrame;
	friend class DxUserInput;
	friend class ExitCommand;
	friend class ToggleFullscreenVerb;
	friend class ToggleIDEModeVerb;

private:
	bool m_unk0xe0;
	bool m_unk0xe1;
	HMONITOR m_unk0xe4;
	CCmdTarget* m_unk0xe8;
	bool m_unk0xec;
	bool m_unk0xed;
	CString m_unk0xf0;
	CXTWindowPos m_unk0xf4;
	CXTPDockingPaneManager m_unk0x120;
	CXTPTabClientWnd m_unk0x26c;
	boost::scoped_ptr<CXTSplitterWnd> m_unk0x390;
	boost::scoped_ptr<CDataModelPropGrid> m_unk0x394;
	boost::scoped_ptr<CSelectionCaption> m_unk0x398;
	CXTThemeManager m_unk0x39c;
	CXTPStatusBar m_unk0x3c8;
};

#endif // MAINFRM_H
