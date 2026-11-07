#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "InstanceExplorer.h"
#include "SettingsPropGrid.h"
#include "decomp.h"

// SIZE 0x3b0
class CSettingsExplorer : public CInstanceExplorer
{
	DECLARE_DYNCREATE(CSettingsExplorer)

	// SYNTHETIC: ROBLOX 0x0046b070
	// ?_GetBaseClass@CSettingsExplorer@@KGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x0046b080
	// ?GetThisClass@CSettingsExplorer@@SGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x0046b0a0
	// ?CreateObject@CSettingsExplorer@@SGPAVCObject@@XZ

protected:
	CSettingsExplorer() {}

	virtual void onEvent(const RBX::Instance* source, RBX::DescendentAdded event);
	virtual void onEvent(const RBX::Instance* source, RBX::DescendentRemoving event);
};

DECOMP_SIZE_ASSERT(CSettingsExplorer, 0x3b0)

// SIZE 0x378
class CSettingsDialog : public CDialog
{
	DECLARE_DYNCREATE(CSettingsDialog)

	// SYNTHETIC: ROBLOX 0x0046b050
	// ?_GetBaseClass@CSettingsDialog@@KGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x0046b060
	// ?GetThisClass@CSettingsDialog@@SGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x0046b4f0
	// ?CreateObject@CSettingsDialog@@SGPAVCObject@@XZ

public:
	enum
	{
		IDD = 122
	};

	CSettingsDialog(CWnd* pParent = NULL);
	virtual ~CSettingsDialog();

	static void FUN_0046b3f0(CWnd* p_unk0x04);

	// SYNTHETIC: ROBLOX 0x0046b240
	// CSettingsDialog::`scalar deleting destructor'

protected:
	virtual BOOL OnInitDialog();

	afx_msg void FUN_0046b120();

	DECLARE_MESSAGE_MAP()

	// SYNTHETIC: ROBLOX 0x0046b340
	// ?GetMessageMap@CSettingsDialog@@MBEPBUAFX_MSGMAP@@XZ

private:
	CXTSplitterWnd m_unk0x74;      // 0x74
	CSettingsExplorer* m_unk0x18c; // 0x18c
	CSettingsPropGrid m_unk0x190;  // 0x190
};

DECOMP_SIZE_ASSERT(CSettingsDialog, 0x378)

#endif // SETTINGSDIALOG_H
