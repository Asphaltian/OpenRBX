#ifndef OBJECTBROWSER_H
#define OBJECTBROWSER_H

#include "placeholder.h"

class CObjectBrowser : public CView
{
	DECLARE_DYNCREATE(CObjectBrowser)

protected:
	CObjectBrowser();

public:
	virtual ~CObjectBrowser();

	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

private:
	CXTSplitterWnd m_unk0x58;
	CXTSplitterWnd m_unk0x170;
};

#endif // OBJECTBROWSER_H
