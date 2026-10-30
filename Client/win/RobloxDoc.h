#ifndef ROBLOXDOC_H
#define ROBLOXDOC_H

#include "decomp.h"

// SIZE 0x7c
class CRobloxDoc : public CDocument
{
	DECLARE_DYNCREATE(CRobloxDoc)

	// SYNTHETIC: ROBLOX 0x00453ea0
	// ?GetThisClass@CRobloxDoc@@SGPAUCRuntimeClass@@XZ

protected:
	CRobloxDoc();

public:
	virtual ~CRobloxDoc();

private:
	undefined m_unk0x54[0x7c - 0x54]; // 0x54
};

DECOMP_SIZE_ASSERT(CRobloxDoc, 0x7c)

#endif // ROBLOXDOC_H
