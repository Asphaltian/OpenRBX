#ifndef SAVETOROBLOXDIALOG_H
#define SAVETOROBLOXDIALOG_H

#include "Roblox.h"
#include "placeholder.h"

class CSaveToRobloxDialog : public CRobloxDHtmlDialog
{

public:
	CSaveToRobloxDialog(IDispatch* external, const char* url, CWnd* pParentWnd);
	virtual ~CSaveToRobloxDialog();

protected:
	void OnWindowClosing(BOOL isChildWindow, VARIANT_BOOL* cancel);

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
	DECLARE_DHTML_EVENT_MAP()
};

class CPublishAsPlaceDialog : public CSaveToRobloxDialog
{
public:
	CPublishAsPlaceDialog(IDispatch* external, CWnd* pParentWnd);
};

class CUploadPlaceDialog : public CSaveToRobloxDialog
{
public:
	CUploadPlaceDialog(IDispatch* external, CWnd* pParentWnd);
};

class CSaveModelToRobloxDialog : public CSaveToRobloxDialog
{
public:
	CSaveModelToRobloxDialog(IDispatch* external, CWnd* pParentWnd);
};

#endif // SAVETOROBLOXDIALOG_H
