#ifndef MULTIPLAYERPANE_H
#define MULTIPLAYERPANE_H

#include "decomp.h"

class CChatPrompt;

namespace RBX {
class DataModel;
}

// SIZE 0x58
class CMultiPlayerPane : public CWnd
{
	DECLARE_DYNAMIC(CMultiPlayerPane)

	// SYNTHETIC: ROBLOX 0x00436fb0
	// ?GetThisClass@CMultiPlayerPane@@SGPAUCRuntimeClass@@XZ

public:
	CMultiPlayerPane();
	virtual ~CMultiPlayerPane();

	BOOL FUN_00436ff0(CWnd* parent, RBX::DataModel* dataModel);

	// SYNTHETIC: ROBLOX 0x00437100
	// CMultiPlayerPane::`scalar deleting destructor'

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

	// SYNTHETIC: ROBLOX 0x00437130
	// ?GetMessageMap@CMultiPlayerPane@@MBEPBUAFX_MSGMAP@@XZ

private:
	CChatPrompt* m_unk0x54; // 0x54
};

DECOMP_SIZE_ASSERT(CMultiPlayerPane, 0x58)

#endif // MULTIPLAYERPANE_H
