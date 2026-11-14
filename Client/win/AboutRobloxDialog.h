#ifndef ABOUTROBLOXDIALOG_H
#define ABOUTROBLOXDIALOG_H

#include "Roblox.h"
#include "placeholder.h"

class CAboutRobloxDialog : public CRobloxDHtmlDialog
{
	DECLARE_DYNCREATE(CAboutRobloxDialog)

public:
	CAboutRobloxDialog(CWnd* pParentWnd = NULL);
	virtual ~CAboutRobloxDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);

	HRESULT OnButtonOK(IHTMLElement* pElement);

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};

#endif // ABOUTROBLOXDIALOG_H
