#include "BrowserView.h"

IMPLEMENT_DYNCREATE(CIDEBrowserView, CBrowserView)
IMPLEMENT_DYNCREATE(CBrowserView, CSecureHtmlView)
IMPLEMENT_DYNCREATE(CPlayBrowserView, CBrowserView)
IMPLEMENT_DYNCREATE(CBrowserDoc, CNullDoc)

CBrowserView::CBrowserView()
{
}

CBrowserView::~CBrowserView()
{
}
