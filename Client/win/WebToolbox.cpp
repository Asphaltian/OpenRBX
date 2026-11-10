#include "WebToolbox.h"

#include "Roblox.h"

IMPLEMENT_DYNCREATE(CWebToolbox, CSecureHtmlView)

// FUNCTION: ROBLOX 0x0046ea00
void CWebToolbox::FUN_0046ea00(int category)
{
	CString url;
	url.Format("%s/IDE/ClientToolbox.aspx?Category=%d", static_cast<LPCTSTR>(FUN_00407190()), category);
	Navigate(url);
}
