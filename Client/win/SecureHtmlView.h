#ifndef SECUREHTMLVIEW_H
#define SECUREHTMLVIEW_H

#include "placeholder.h"

#include <afxhtml.h>

class CSecureHtmlView : public CHtmlView
{
	DECLARE_DYNAMIC(CSecureHtmlView)

protected:
	CSecureHtmlView();

public:
	virtual ~CSecureHtmlView();

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
	undefined m_unk0xf0;
};

#endif // SECUREHTMLVIEW_H
