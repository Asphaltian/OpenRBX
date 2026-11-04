#ifndef BROWSERVIEW_H
#define BROWSERVIEW_H

#include "NullDoc.h"
#include "SecureHtmlView.h"
#include "decomp.h"

#include <atlbase.h>
#include <list>
#include <string>

// SIZE 0x2bc
class CBrowserView : public CXTPCommandBarsSiteBase<CSecureHtmlView>
{
	DECLARE_DYNCREATE(CBrowserView)

	// SYNTHETIC: ROBLOX 0x0040d1d0
	// ?CreateObject@CBrowserView@@SGPAVCObject@@XZ

protected:
	CBrowserView();

public:
	virtual ~CBrowserView();

	// SYNTHETIC: ROBLOX 0x0040d320
	// ??_ECBrowserView@@UAEPAXI@Z

protected:
	virtual void OnInitialUpdate();
	virtual HRESULT OnGetHostInfo(DOCHOSTUIINFO* pInfo);
	virtual void OnCommandStateChange(long nCommand, BOOL bEnable);
	virtual void OnNavigateComplete2(LPCTSTR strURL);
	virtual void OnTitleChange(LPCTSTR lpszText);
	virtual void OnDocumentComplete(LPCTSTR lpszURL);

private:
	std::list<std::string> m_unk0xf8; // 0xf8
	CXTPControlComboBox* m_unk0x104;  // 0x104
	CXTPToolBar m_unk0x108;           // 0x108
	CComPtr<IUnknown> m_unk0x2b0;     // 0x2b0
	undefined m_unk0x2b4;             // 0x2b4
	undefined m_unk0x2b5;             // 0x2b5

protected:
	CString m_unk0x2b8; // 0x2b8
};

DECOMP_SIZE_ASSERT(CBrowserView, 0x2bc)

// SIZE 0x2bc
class CIDEBrowserView : public CBrowserView
{
	DECLARE_DYNCREATE(CIDEBrowserView)

	// SYNTHETIC: ROBLOX 0x0040a160
	// ?GetThisClass@CIDEBrowserView@@SGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x0040d240
	// ?CreateObject@CIDEBrowserView@@SGPAVCObject@@XZ

protected:
	CIDEBrowserView();
};

DECOMP_SIZE_ASSERT(CIDEBrowserView, 0x2bc)

// SIZE 0x2bc
class CPlayBrowserView : public CBrowserView
{
	DECLARE_DYNCREATE(CPlayBrowserView)

	// SYNTHETIC: ROBLOX 0x0040d2b0
	// ?CreateObject@CPlayBrowserView@@SGPAVCObject@@XZ

protected:
	CPlayBrowserView();
};

DECOMP_SIZE_ASSERT(CPlayBrowserView, 0x2bc)

// SIZE 0x58
class CBrowserDoc : public CNullDoc
{
	DECLARE_DYNCREATE(CBrowserDoc)

	// SYNTHETIC: ROBLOX 0x0040a1a0
	// ?GetThisClass@CBrowserDoc@@SGPAUCRuntimeClass@@XZ

protected:
	CBrowserDoc() {}

public:
	CBrowserView* m_unk0x54; // 0x54
};

DECOMP_SIZE_ASSERT(CBrowserDoc, 0x58)

#endif // BROWSERVIEW_H
