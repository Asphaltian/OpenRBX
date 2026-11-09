#ifndef SAVETOROBLOXDIALOG_H
#define SAVETOROBLOXDIALOG_H

#include "Roblox.h"
#include "decomp.h"

// SIZE 0x130
class CSaveToRobloxDialog : public CRobloxDHtmlDialog
{
	// SYNTHETIC: ROBLOX 0x00464240
	// ?GetDHtmlEventMap@CSaveToRobloxDialog@@MAEPBUDHtmlEventMapEntry@@XZ

	// SYNTHETIC: ROBLOX 0x00464270
	// ?GetEventSinkMap@CSaveToRobloxDialog@@MBEPBUAFX_EVENTSINKMAP@@XZ

public:
	CSaveToRobloxDialog(IDispatch* external, const char* url, CWnd* pParentWnd);
	virtual ~CSaveToRobloxDialog();

	// SYNTHETIC: ROBLOX 0x00464300
	// ??_GCSaveToRobloxDialog@@UAEPAXI@Z

protected:
	void OnWindowClosing(BOOL isChildWindow, VARIANT_BOOL* cancel);

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
	DECLARE_DHTML_EVENT_MAP()
};

DECOMP_SIZE_ASSERT(CSaveToRobloxDialog, 0x130)

// SIZE 0x130
class CPublishAsPlaceDialog : public CSaveToRobloxDialog
{
public:
	CPublishAsPlaceDialog(IDispatch* external, CWnd* pParentWnd);

	// SYNTHETIC: ROBLOX 0x00454230 FOLDED
	// ??_GCPublishAsPlaceDialog@@UAEPAXI@Z
};

DECOMP_SIZE_ASSERT(CPublishAsPlaceDialog, 0x130)

// SIZE 0x130
class CUploadPlaceDialog : public CSaveToRobloxDialog
{
public:
	CUploadPlaceDialog(IDispatch* external, CWnd* pParentWnd);

	// SYNTHETIC: ROBLOX 0x00454230 FOLDED
	// ??_GCUploadPlaceDialog@@UAEPAXI@Z
};

DECOMP_SIZE_ASSERT(CUploadPlaceDialog, 0x130)

// SIZE 0x130
class CSaveModelToRobloxDialog : public CSaveToRobloxDialog
{
public:
	CSaveModelToRobloxDialog(IDispatch* external, CWnd* pParentWnd);

	// SYNTHETIC: ROBLOX 0x00454230 FOLDED
	// ??_GCSaveModelToRobloxDialog@@UAEPAXI@Z
};

DECOMP_SIZE_ASSERT(CSaveModelToRobloxDialog, 0x130)

#endif // SAVETOROBLOXDIALOG_H
