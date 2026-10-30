#ifndef ROBLOXVIEW_H
#define ROBLOXVIEW_H

#include "decomp.h"

// SIZE 0x1c0
class CRobloxView : public CView
{
	DECLARE_DYNCREATE(CRobloxView)

	// SYNTHETIC: ROBLOX 0x0045d4e0
	// ?GetThisClass@CRobloxView@@SGPAUCRuntimeClass@@XZ

protected:
	CRobloxView();

public:
	virtual ~CRobloxView();

	virtual void OnDraw(CDC* pDC);

private:
	undefined m_unk0x58[0x1c0 - 0x58]; // 0x58
};

DECOMP_SIZE_ASSERT(CRobloxView, 0x1c0)

#endif // ROBLOXVIEW_H
