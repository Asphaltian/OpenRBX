#include "ChatPrompt.h"

#include "ChatEnter.h"
#include "network/Players.h"
#include "util/object.h"
#include "v8datamodel/DataModel.h"

IMPLEMENT_DYNCREATE(CChatPrompt, CEditView)

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
END_MESSAGE_MAP()
// clang-format on

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
