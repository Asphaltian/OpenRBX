#ifndef OBJECTBROWSER_H
#define OBJECTBROWSER_H

#include "decomp.h"

// SIZE 0x288
class CObjectBrowser : public CView
{
	DECLARE_DYNCREATE(CObjectBrowser)

	// SYNTHETIC: ROBLOX 0x00437610
	// ?GetThisClass@CObjectBrowser@@SGPAUCRuntimeClass@@XZ

protected:
	CObjectBrowser();

public:
	virtual ~CObjectBrowser();

	virtual void OnDraw(CDC* pDC);

private:
	undefined m_unk0x58[0x288 - 0x58]; // 0x58
};

DECOMP_SIZE_ASSERT(CObjectBrowser, 0x288)

#endif // OBJECTBROWSER_H
