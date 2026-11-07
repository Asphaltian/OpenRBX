#ifndef SECUREHTMLVIEW_H
#define SECUREHTMLVIEW_H

#include "decomp.h"

#include <afxhtml.h>

// SIZE 0xf4
class CSecureHtmlView : public CHtmlView
{
	DECLARE_DYNAMIC(CSecureHtmlView)

	// SYNTHETIC: ROBLOX 0x0046a0b0
	// ?GetRuntimeClass@CSecureHtmlView@@UBEPAUCRuntimeClass@@XZ

protected:
	CSecureHtmlView();

public:
	virtual ~CSecureHtmlView();

	// SYNTHETIC: ROBLOX 0x0046a1f0
	// CSecureHtmlView::`scalar deleting destructor'

protected:
	virtual BOOL OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	virtual void OnBeforeNavigate2(
		LPCTSTR lpszURL,
		DWORD nFlags,
		LPCTSTR lpszTargetFrameName,
		CByteArray& baPostedData,
		LPCTSTR lpszHeaders,
		BOOL* pbCancel
	);

protected:
	undefined m_unk0xf0; // 0xf0
};

DECOMP_SIZE_ASSERT(CSecureHtmlView, 0xf4)

#endif // SECUREHTMLVIEW_H
