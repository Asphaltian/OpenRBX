#include "InsertObjectDialog.h"

CInsertObjectDialog::CInsertObjectDialog(CWnd* pParent) : CDialog(107, pParent), m_unk0x74("")
{
}

CInsertObjectDialog::~CInsertObjectDialog()
{
}

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
