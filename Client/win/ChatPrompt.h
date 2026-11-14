#ifndef CHATPROMPT_H
#define CHATPROMPT_H

#include "placeholder.h"

#include <boost/shared_ptr.hpp>

class ChatEnter;

namespace RBX {
class DataModel;

namespace Network {
class Players;
}
} // namespace RBX

class CChatPrompt : public CEditView
{
	DECLARE_DYNCREATE(CChatPrompt)

protected:
	CChatPrompt();

public:
	virtual ~CChatPrompt();

	void FUN_0040f4d0(RBX::DataModel* dataModel, CWnd* parent);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void FUN_0040dbe0();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void FUN_00751160();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT FUN_0045f4d0(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_unk0x88;
	boost::shared_ptr<ChatEnter> m_unk0x8c;
	CBrush m_unk0x94;
	boost::shared_ptr<RBX::DataModel> m_unk0x9c;
	RBX::Network::Players* m_unk0xa4;
};

#endif // CHATPROMPT_H
