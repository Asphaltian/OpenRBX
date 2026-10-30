#ifndef ROBLOXREPORTVIEW_H
#define ROBLOXREPORTVIEW_H

#include "decomp.h"

// SIZE 0x3b8
class CRobloxReportView : public CXTPReportView
{
	DECLARE_DYNAMIC(CRobloxReportView)

protected:
	CRobloxReportView();

public:
	virtual ~CRobloxReportView();

private:
	undefined m_unk0x36c[0x3b8 - 0x36c]; // 0x36c
};

DECOMP_SIZE_ASSERT(CRobloxReportView, 0x3b8)

// SIZE 0x3b8
class CRobloxReportDocView : public CRobloxReportView
{
	DECLARE_DYNCREATE(CRobloxReportDocView)

	// SYNTHETIC: ROBLOX 0x0045ac00
	// ?GetThisClass@CRobloxReportDocView@@SGPAUCRuntimeClass@@XZ

protected:
	CRobloxReportDocView() {}
};

DECOMP_SIZE_ASSERT(CRobloxReportDocView, 0x3b8)

// SIZE 0x3bc
class CRobloxReportPaneView : public CRobloxReportView
{
	DECLARE_DYNCREATE(CRobloxReportPaneView)

protected:
	CRobloxReportPaneView() {}

private:
	undefined4 m_unk0x3b8; // 0x3b8
};

DECOMP_SIZE_ASSERT(CRobloxReportPaneView, 0x3bc)

#endif // ROBLOXREPORTVIEW_H
