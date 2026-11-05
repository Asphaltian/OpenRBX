#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "SettingsPropGrid.h"
#include "decomp.h"

// SIZE 0x378
class CSettingsDialog : public CDialog
{
public:
	enum
	{
		IDD = 122
	};

	CSettingsDialog(CWnd* pParent);
	virtual ~CSettingsDialog();

	static void FUN_0046b3f0(CWnd* p_unk0x04);

	// SYNTHETIC: ROBLOX 0x0046b240
	// CSettingsDialog::`scalar deleting destructor'

private:
	CXTSplitterWnd m_unk0x74;     // 0x74
	undefined4 m_unk0x18c;        // 0x18c
	CSettingsPropGrid m_unk0x190; // 0x190
};

DECOMP_SIZE_ASSERT(CSettingsDialog, 0x378)

#endif // SETTINGSDIALOG_H
