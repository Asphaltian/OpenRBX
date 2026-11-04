#ifndef CLASSTREEVIEW_H
#define CLASSTREEVIEW_H

#include "decomp.h"

// SIZE 0xb0
class CClassTreeView : public CXTTreeView
{
	DECLARE_DYNCREATE(CClassTreeView)

	// SYNTHETIC: ROBLOX 0x00438220
	// ?CreateObject@CClassTreeView@@SGPAVCObject@@XZ

protected:
	CClassTreeView();

public:
	virtual ~CClassTreeView();

	// SYNTHETIC: ROBLOX 0x00437ba0
	// CClassTreeView::`scalar deleting destructor'

private:
	CImageList m_unk0xa0; // 0xa0
	undefined4 m_unk0xa8; // 0xa8
	undefined4 m_unk0xac; // 0xac
};

DECOMP_SIZE_ASSERT(CClassTreeView, 0xb0)

#endif // CLASSTREEVIEW_H
