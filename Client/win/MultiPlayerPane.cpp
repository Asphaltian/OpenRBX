#include "MultiPlayerPane.h"

#include "ChatPrompt.h"

IMPLEMENT_DYNAMIC(CMultiPlayerPane, CWnd)

// FUNCTION: ROBLOX 0x00436fc0
CMultiPlayerPane::CMultiPlayerPane() : m_unk0x54(NULL)
{
}

// FUNCTION: ROBLOX 0x00436fe0
CMultiPlayerPane::~CMultiPlayerPane()
{
}

// FUNCTION: ROBLOX 0x00436ff0
BOOL CMultiPlayerPane::FUN_00436ff0(CWnd* parent, RBX::DataModel* dataModel)
{
	if (!CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), parent, 1002)) {
		return FALSE;
	}

	m_unk0x54 = static_cast<CChatPrompt*>(RUNTIME_CLASS(CChatPrompt)->CreateObject());
	if (!m_unk0x54->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1003)) {
		return FALSE;
	}

	m_unk0x54->FUN_0040f4d0(dataModel, parent);
	m_unk0x54->SendMessage(WM_INITIALUPDATE);
	return TRUE;
}
