#ifndef BROWSERVIEW_H
#define BROWSERVIEW_H

#include "NullDoc.h"
#include "SecureHtmlView.h"
#include "placeholder.h"

#include <atlbase.h>
#include <list>
#include <string>

class CBrowserView : public CXTPCommandBarsSiteBase<CSecureHtmlView>
{
	DECLARE_DYNCREATE(CBrowserView)

protected:
	CBrowserView();

public:
	virtual ~CBrowserView();

	static CBrowserView* FUN_0040b6d0(CRect rect, CWnd* parent, CString url, IDispatch* external);

protected:
	virtual void OnInitialUpdate();
	virtual HRESULT OnGetExternal(LPDISPATCH* lppDispatch);
	virtual HRESULT OnGetHostInfo(DOCHOSTUIINFO* pInfo);
	virtual void OnCommandStateChange(long nCommand, BOOL bEnable);
	virtual void OnNavigateComplete2(LPCTSTR strURL);
	virtual void OnBeforeNavigate2(
		LPCTSTR lpszURL,
		DWORD nFlags,
		LPCTSTR lpszTargetFrameName,
		CByteArray& baPostedData,
		LPCTSTR lpszHeaders,
		BOOL* pbCancel
	);
	virtual void OnTitleChange(LPCTSTR lpszText);
	virtual void OnDocumentComplete(LPCTSTR lpszURL);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void FUN_0040a320(CCmdUI* pCmdUI);
	afx_msg void FUN_0040a340(CCmdUI* pCmdUI);
	afx_msg void FUN_0040a360(CCmdUI* pCmdUI);
	afx_msg void FUN_0040a380(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
	afx_msg void FUN_0040aa90();
	afx_msg void FUN_0040aad0();
	afx_msg void FUN_0040aae0();
	afx_msg void FUN_0040aaf0();
	afx_msg void FUN_0040ac30();
	afx_msg void FUN_0040ac40();
	afx_msg void FUN_0040ac50(CCmdUI* pCmdUI);
	afx_msg void FUN_0040b0f0(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void FUN_00453eb0(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

private:
	std::list<std::string> m_unk0xf8;
	CXTPControlComboBox* m_unk0x104;
	CXTPToolBar m_unk0x108;
	CComPtr<IDispatch> m_unk0x2b0;
	undefined m_unk0x2b4;
	undefined m_unk0x2b5;

protected:
	CString m_unk0x2b8;
};

class CIDEBrowserView : public CBrowserView
{
	DECLARE_DYNCREATE(CIDEBrowserView)

protected:
	CIDEBrowserView();
};

class CPlayBrowserView : public CBrowserView
{
	DECLARE_DYNCREATE(CPlayBrowserView)

protected:
	CPlayBrowserView();
};

class CBrowserDoc : public CNullDoc
{
	DECLARE_DYNCREATE(CBrowserDoc)

protected:
	CBrowserDoc() : m_unk0x54(NULL) {}

public:
	CBrowserView* m_unk0x54;
};

#endif // BROWSERVIEW_H
