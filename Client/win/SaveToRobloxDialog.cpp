#include "SaveToRobloxDialog.h"

#include <exdispid.h>

// FUNCTION: ROBLOX 0x00464220
CSaveToRobloxDialog::~CSaveToRobloxDialog()
{
}

// clang-format off
BEGIN_DHTML_EVENT_MAP(CSaveToRobloxDialog)
END_DHTML_EVENT_MAP()

BEGIN_EVENTSINK_MAP(CSaveToRobloxDialog, CRobloxDHtmlDialog)
ON_EVENT(CSaveToRobloxDialog, AFX_IDC_BROWSER, DISPID_WINDOWCLOSING, OnWindowClosing, VTS_BOOL VTS_PBOOL)
END_EVENTSINK_MAP()

BEGIN_MESSAGE_MAP(CSaveToRobloxDialog, CRobloxDHtmlDialog)
END_MESSAGE_MAP()
// clang-format on

// FUNCTION: ROBLOX 0x00464280
CSaveToRobloxDialog::CSaveToRobloxDialog(IDispatch* external, const char* url, CWnd* pParentWnd)
	: CRobloxDHtmlDialog(113, url, pParentWnd)
{
	SetExternalDispatch(external);
}

// FUNCTION: ROBLOX 0x00464330
void CSaveToRobloxDialog::OnWindowClosing(BOOL isChildWindow, VARIANT_BOOL* cancel)
{
	*cancel = VARIANT_TRUE;

	CString result;
	DDX_DHtml_ElementText(_T("DialogResult"), DISPID_A_VALUE, result, TRUE);

	if (result == "") {
		OnCancel();
	}
	else {
		EndDialog(atoi(result));
	}
}
