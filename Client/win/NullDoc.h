#ifndef NULLDOC_H
#define NULLDOC_H

#include "placeholder.h"

class CBrowserFrameWnd : public CXTPFrameWnd
{
	DECLARE_DYNCREATE(CBrowserFrameWnd)

protected:
	CBrowserFrameWnd() {}

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

class CNullDoc : public CDocument
{
	DECLARE_DYNCREATE(CNullDoc)

protected:
	CNullDoc() {}

public:
	virtual BOOL IsModified() { return FALSE; }
	virtual BOOL SaveModified() { return TRUE; }

protected:
	afx_msg void FUN_004374b0(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

#endif // NULLDOC_H
