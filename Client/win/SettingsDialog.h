#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "InstanceExplorer.h"
#include "SettingsPropGrid.h"
#include "placeholder.h"

class CSettingsExplorer : public CInstanceExplorer
{
	DECLARE_DYNCREATE(CSettingsExplorer)

protected:
	CSettingsExplorer() {}

	virtual void onEvent(const RBX::Instance* source, RBX::DescendentAdded event);
	virtual void onEvent(const RBX::Instance* source, RBX::DescendentRemoving event);
};

class CSettingsDialog : public CDialog
{
	DECLARE_DYNCREATE(CSettingsDialog)

public:
	enum
	{
		IDD = 122
	};

	CSettingsDialog(CWnd* pParent = NULL);
	virtual ~CSettingsDialog();

	static void FUN_0046b3f0(CWnd* p_unk0x04);

protected:
	virtual BOOL OnInitDialog();

	afx_msg void FUN_0046b120();

	DECLARE_MESSAGE_MAP()

private:
	CXTSplitterWnd m_unk0x74;
	CSettingsExplorer* m_unk0x18c;
	CSettingsPropGrid m_unk0x190;
};

#endif // SETTINGSDIALOG_H
