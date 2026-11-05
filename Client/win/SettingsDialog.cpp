#include "SettingsDialog.h"

#include "v8datamodel/GlobalSettings.h"

// FUNCTION: ROBLOX 0x0046b1d0
CSettingsDialog::~CSettingsDialog()
{
}

// FUNCTION: ROBLOX 0x0046b350
CSettingsDialog::CSettingsDialog(CWnd* pParent) : CDialog(IDD, pParent), m_unk0x18c(0)
{
}

// FUNCTION: ROBLOX 0x0046b3f0
void CSettingsDialog::FUN_0046b3f0(CWnd* p_unk0x04)
{
	if (AfxMessageBox(0x88, MB_OKCANCEL | MB_ICONEXCLAMATION) == IDOK) {
		CSettingsDialog dialog(p_unk0x04);

		if (dialog.DoModal() == IDOK) {
			AfxMessageBox(0x8e, MB_ICONINFORMATION);
			RBX::GlobalSettings::singleton()->saveState();
		}
	}
}
