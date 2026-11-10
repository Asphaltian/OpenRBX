#ifndef CHATPROMPT_H
#define CHATPROMPT_H

#include "decomp.h"

#include <boost/shared_ptr.hpp>

class ChatEnter;

namespace RBX {
class DataModel;

namespace Network {
class Players;
}
} // namespace RBX

// SIZE 0xa8
class CChatPrompt : public CEditView
{
	DECLARE_DYNCREATE(CChatPrompt)

	// SYNTHETIC: ROBLOX 0x0040d790
	// ?GetThisClass@CChatPrompt@@SGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x0040e520
	// ?CreateObject@CChatPrompt@@SGPAVCObject@@XZ

protected:
	CChatPrompt();

public:
	virtual ~CChatPrompt();

	// SYNTHETIC: ROBLOX 0x0040f7d0
	// CChatPrompt::`scalar deleting destructor'

	void FUN_0040f4d0(RBX::DataModel* dataModel, CWnd* parent);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void FUN_0040dbe0();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT FUN_0045f4d0(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	// SYNTHETIC: ROBLOX 0x0040e730
	// ?GetMessageMap@CChatPrompt@@MBEPBUAFX_MSGMAP@@XZ

private:
	CWnd* m_unk0x88;                             // 0x88
	boost::shared_ptr<ChatEnter> m_unk0x8c;      // 0x8c
	CBrush m_unk0x94;                            // 0x94
	boost::shared_ptr<RBX::DataModel> m_unk0x9c; // 0x9c
	RBX::Network::Players* m_unk0xa4;            // 0xa4
};

DECOMP_SIZE_ASSERT(CChatPrompt, 0xa8)

#endif // CHATPROMPT_H
