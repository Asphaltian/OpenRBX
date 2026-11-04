#include "BrowserView.h"

#include "MainFrm.h"
#include "Roblox.h"

IMPLEMENT_DYNCREATE(CIDEBrowserView, CBrowserView)
IMPLEMENT_DYNCREATE(CBrowserView, CSecureHtmlView)
IMPLEMENT_DYNCREATE(CPlayBrowserView, CBrowserView)
IMPLEMENT_DYNCREATE(CBrowserDoc, CNullDoc)

// FUNCTION: ROBLOX 0x0040ce30
CBrowserView::CBrowserView() : m_unk0x104(0), m_unk0x2b4(0), m_unk0x2b5(0)
{
	FUN_004334d0(XTPImageManager());
}

// FUNCTION: ROBLOX 0x0040cee0
CBrowserView::~CBrowserView()
{
}

// FUNCTION: ROBLOX 0x0040d090
CIDEBrowserView::CIDEBrowserView()
{
	m_unk0x2b8 = FUN_00407190() + "/IDE/Start.aspx";
}

// FUNCTION: ROBLOX 0x0040d130
CPlayBrowserView::CPlayBrowserView()
{
	m_unk0x2b8 = FUN_00407190() + "/Start.aspx";
}
