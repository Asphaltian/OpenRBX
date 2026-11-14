#ifndef CHILDFRM_H
#define CHILDFRM_H

#include "placeholder.h"

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)

public:
	CChildFrame();
	virtual ~CChildFrame();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

class CRbxChildFrame : public CChildFrame
{
	DECLARE_DYNCREATE(CRbxChildFrame)

protected:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

	DECLARE_MESSAGE_MAP()
};

#endif // CHILDFRM_H
