#ifndef ABOUTROBLOXDIALOG_H
#define ABOUTROBLOXDIALOG_H

#include "Roblox.h"
#include "decomp.h"

// SIZE 0x130
class CAboutRobloxDialog : public CRobloxDHtmlDialog
{
	DECLARE_DYNCREATE(CAboutRobloxDialog)

	// SYNTHETIC: ROBLOX 0x00401040
	// ?GetRuntimeClass@CAboutRobloxDialog@@UBEPAUCRuntimeClass@@XZ

public:
	CAboutRobloxDialog(CWnd* pParentWnd = NULL);
	virtual ~CAboutRobloxDialog();

	// SYNTHETIC: ROBLOX 0x004012e0
	// ??_GCAboutRobloxDialog@@UAEPAXI@Z

	// SYNTHETIC: ROBLOX 0x00401310
	// ?GetMessageMap@CAboutRobloxDialog@@MBEPBUAFX_MSGMAP@@XZ

	// SYNTHETIC: ROBLOX 0x00401320
	// ?GetDHtmlEventMap@CAboutRobloxDialog@@MAEPBUDHtmlEventMapEntry@@XZ

	// SYNTHETIC: ROBLOX 0x00401380
	// ?CreateObject@CAboutRobloxDialog@@SGPAVCObject@@XZ

protected:
	virtual void DoDataExchange(CDataExchange* pDX);                  // vtable+0xfc
	virtual BOOL OnInitDialog();                                      // vtable+0x150
	virtual void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl); // vtable+0x170

	HRESULT OnButtonOK(IHTMLElement* pElement);

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};

DECOMP_SIZE_ASSERT(CAboutRobloxDialog, 0x130)

#endif // ABOUTROBLOXDIALOG_H
