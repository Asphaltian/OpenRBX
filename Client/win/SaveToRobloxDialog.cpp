#include "SaveToRobloxDialog.h"

#include <exdispid.h>

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

CSaveToRobloxDialog::CSaveToRobloxDialog(IDispatch* external, const char* url, CWnd* pParentWnd)
	: CRobloxDHtmlDialog(113, url, pParentWnd)
{
	SetExternalDispatch(external);
}

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
