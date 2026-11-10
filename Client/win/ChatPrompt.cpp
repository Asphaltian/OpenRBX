#include "ChatPrompt.h"

IMPLEMENT_DYNCREATE(CChatPrompt, CEditView)

// FUNCTION: ROBLOX 0x0040d7a0
BOOL CChatPrompt::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CEditView::PreCreateWindow(cs)) {
		return FALSE;
	}

	cs.style &= ~(WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL);
	cs.style |= WS_DISABLED;
	return TRUE;
}

// FUNCTION: ROBLOX 0x0040e200
CChatPrompt::CChatPrompt() : m_unk0x88(NULL), m_unk0xa4(NULL)
{
}

// clang-format off
BEGIN_MESSAGE_MAP(CChatPrompt, CEditView)
END_MESSAGE_MAP()
// clang-format on

// STUB: ROBLOX 0x0040f4d0
void CChatPrompt::FUN_0040f4d0(RBX::DataModel* dataModel, CWnd* parent)
{
	STUB(0x40f4d0);
}

// STUB: ROBLOX 0x0040f6e0
CChatPrompt::~CChatPrompt()
{
	FUN_0040f4d0(NULL, NULL);
}
