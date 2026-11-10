#include "ChatPrompt.h"

#include "ChatEnter.h"
#include "network/Players.h"
#include "util/object.h"
#include "v8datamodel/DataModel.h"

IMPLEMENT_DYNCREATE(CChatPrompt, CEditView)

// GLOBAL: ROBLOX 0x00913b50
const char* g_unk0x00913b50 = "To chat click here or press the \"/\" key";

// FUNCTION: ROBLOX 0x0040d730
RBX::GuiResponse ChatEnter::process(const RBX::GuiEvent& event)
{
	if (event.isKeyPressedEvent(SDLK_SLASH)) {
		m_unk0x10c->PostMessage(0x468);
		return RBX::GuiResponse::used();
	}

	return RBX::GuiResponse::notUsed();
}

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

// FUNCTION: ROBLOX 0x0040d7d0
HBRUSH CChatPrompt::CtlColor(CDC* pDC, UINT nCtlColor)
{
	if (GetFocus() == this) {
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(230, 230, 250));
	}
	else {
		pDC->SetTextColor(RGB(255, 255, 200));
		pDC->SetBkColor(RGB(64, 64, 64));
	}

	return m_unk0x94;
}

// FUNCTION: ROBLOX 0x0040dbe0
void CChatPrompt::FUN_0040dbe0()
{
	CString text;
	GetWindowText(text);
	if (text.Compare(g_unk0x00913b50) == 0) {
		SetWindowText("");
	}
}

// FUNCTION: ROBLOX 0x0040dc60
int CChatPrompt::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_unk0x94.CreateSolidBrush(RGB(64, 64, 64));

	if (CEditView::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	SetFont(XTPPaintManager()->GetRegularBoldFont());
	SetWindowText(g_unk0x00913b50);
	return 0;
}

// FUNCTION: ROBLOX 0x0040dd10
ChatEnter::ChatEnter()
{
}

// FUNCTION: ROBLOX 0x0040e200
CChatPrompt::CChatPrompt() : m_unk0x88(NULL), m_unk0xa4(NULL)
{
}

// clang-format off
BEGIN_MESSAGE_MAP(CChatPrompt, CEditView)
	ON_CONTROL_REFLECT(EN_CHANGE, FUN_00751160)
	ON_WM_CHAR()
	ON_WM_SYSCHAR()
	ON_MESSAGE(0x468, FUN_0045f4d0)
	ON_CONTROL_REFLECT(EN_SETFOCUS, FUN_0040dbe0)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()
// clang-format on

// FUNCTION: ROBLOX 0x0040e250
void CChatPrompt::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {
	case VK_RETURN: {
		CString text;
		GetWindowText(text);
		if (m_unk0xa4 && !text.IsEmpty()) {
			try {
				{
					RBX::DataModel::Lock lock(m_unk0x9c);
					m_unk0xa4->chat(static_cast<LPCTSTR>(text));
				}
				SetWindowText(g_unk0x00913b50);
			}
			catch (std::exception&) {
			}
		}
		m_unk0x88->SetFocus();
		break;
	}
	case VK_ESCAPE:
		SetWindowText(g_unk0x00913b50);
		m_unk0x88->SetFocus();
		break;
	default:
		Default();
		break;
	}
}

// FUNCTION: ROBLOX 0x0040f4d0
void CChatPrompt::FUN_0040f4d0(RBX::DataModel* dataModel, CWnd* parent)
{
	m_unk0x9c = RBX::shared_from(dataModel);
	RBX::DataModel::Lock lock(m_unk0x9c);

	if (m_unk0x8c) {
		m_unk0x8c->m_unk0x10c = NULL;
		m_unk0x8c->setParent(NULL);
		m_unk0x8c.reset();
	}

	m_unk0x88 = parent;
	m_unk0xa4 = dataModel ? dataModel->create<RBX::Network::Players>() : NULL;
	if (dataModel) {
		m_unk0x8c = RBX::Creatable<RBX::Instance>::create<ChatEnter>();
		m_unk0x8c->m_unk0x10c = this;
		m_unk0x8c->setParent(dataModel->getGuiHooks());
	}

	if (m_hWnd) {
		EnableWindow(m_unk0xa4 != NULL);
	}
}

// FUNCTION: ROBLOX 0x0040f6e0
CChatPrompt::~CChatPrompt()
{
	FUN_0040f4d0(NULL, NULL);
}

// FUNCTION: ROBLOX 0x00751160 FOLDED
void CChatPrompt::FUN_00751160()
{
}

// FUNCTION: ROBLOX 0x0045f4d0 FOLDED
LRESULT CChatPrompt::FUN_0045f4d0(WPARAM wParam, LPARAM lParam)
{
	SetFocus();
	return 0;
}
