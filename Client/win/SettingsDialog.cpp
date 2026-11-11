#include "SettingsDialog.h"

#include "v8datamodel/GlobalSettings.h"
#include "v8datamodel/Selection.h"
#include "v8tree/Service.h"

template <class T>
static T* createService(RBX::GlobalSettings* settings)
{
	return settings ? settings->create<T>() : NULL;
}

IMPLEMENT_DYNCREATE(CSettingsDialog, CDialog)
IMPLEMENT_DYNCREATE(CSettingsExplorer, CInstanceExplorer)

// FUNCTION: ROBLOX 0x0046b120
void CSettingsDialog::FUN_0046b120()
{
	AfxMessageBox(0x8f, MB_ICONINFORMATION);
	RBX::GlobalSettings::singleton()->eraseSettingsStore();
	EndDialog(IDCANCEL);
}

// FUNCTION: ROBLOX 0x0046b1d0
CSettingsDialog::~CSettingsDialog()
{
}

// FUNCTION: ROBLOX 0x0046b270
void CSettingsExplorer::onEvent(const RBX::Instance* source, RBX::DescendentAdded event)
{
	if (dynamic_cast<RBX::GlobalSettings::Item*>(event.instance.get())) {
		RBX::MarshaledListener<RBX::Instance, RBX::DescendentAdded, RBX::Instance, RBX::DescendentAdded>::onEvent(
			source, event
		);
	}
}

// FUNCTION: ROBLOX 0x0046b270 FOLDED
void CSettingsExplorer::onEvent(const RBX::Instance* source, RBX::DescendentRemoving event)
{
	if (dynamic_cast<RBX::GlobalSettings::Item*>(event.instance.get())) {
		RBX::MarshaledListener<RBX::Instance, RBX::DescendentRemoving, RBX::Instance, RBX::DescendentRemoving>::onEvent(
			source, event
		);
	}
}

// FUNCTION: ROBLOX 0x0046b350
CSettingsDialog::CSettingsDialog(CWnd* pParent) : CDialog(IDD, pParent), m_unk0x18c(NULL)
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

// STUB: ROBLOX 0x0046b590
BOOL CSettingsDialog::OnInitDialog()
{
	if (!CDialog::OnInitDialog()) {
		return FALSE;
	}

	if (!m_unk0x74.CreateStatic(this, 1, 2)) {
		return FALSE;
	}

	if (!m_unk0x74.CreateView(0, 0, RUNTIME_CLASS(CSettingsExplorer), CSize(200, 200), NULL)) {
		return FALSE;
	}

	m_unk0x18c = static_cast<CSettingsExplorer*>(m_unk0x74.GetPane(0, 0));
	m_unk0x18c->FUN_004212d0(RBX::GlobalSettings::singleton().get());

	m_unk0x190.Create(CRect(0, 0, 0, 0), &m_unk0x74, m_unk0x74.IdFromRowCol(0, 1));
	RBX::Selection* selection = createService<RBX::Selection>(RBX::GlobalSettings::singleton().get());
	m_unk0x190.FUN_00445580(selection);

	CRect rect;
	GetDlgItem(1001)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_unk0x74.MoveWindow(&rect);
	return TRUE;
}

// clang-format off
BEGIN_MESSAGE_MAP(CSettingsDialog, CDialog)
	ON_BN_CLICKED(1002, FUN_0046b120)
END_MESSAGE_MAP()
// clang-format on
