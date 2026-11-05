#ifndef MULTIPLAYERPANE_H
#define MULTIPLAYERPANE_H

#include "decomp.h"

// SIZE 0x58
class CMultiPlayerPane : public CWnd
{
	DECLARE_DYNAMIC(CMultiPlayerPane)

	// SYNTHETIC: ROBLOX 0x00436fb0
	// ?GetThisClass@CMultiPlayerPane@@SGPAUCRuntimeClass@@XZ

public:
	CMultiPlayerPane();
	virtual ~CMultiPlayerPane();

	// SYNTHETIC: ROBLOX 0x00437100
	// CMultiPlayerPane::`scalar deleting destructor'

private:
	undefined4 m_unk0x54; // 0x54
};

DECOMP_SIZE_ASSERT(CMultiPlayerPane, 0x58)

#endif // MULTIPLAYERPANE_H
