#include "InsertObjectDialog.h"

// FUNCTION: ROBLOX 0x0041ff10
CInsertObjectDialog::CInsertObjectDialog(CWnd* pParent) : CDialog(107, pParent), m_unk0x74("")
{
}

// FUNCTION: ROBLOX 0x0041ff80
CInsertObjectDialog::~CInsertObjectDialog()
{
}

// FUNCTION: ROBLOX 0x0041ffd0
void CInsertObjectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, 1000, m_unk0x74);
}

IMPLEMENT_DYNAMIC(CInsertObjectDialog, CDialog)

// clang-format off
BEGIN_MESSAGE_MAP(CInsertObjectDialog, CDialog)
END_MESSAGE_MAP()
// clang-format on
