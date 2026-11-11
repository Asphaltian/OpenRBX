#ifndef INSERTOBJECTDIALOG_H
#define INSERTOBJECTDIALOG_H

#include "decomp.h"

// SIZE 0x78
class CInsertObjectDialog : public CDialog
{
	DECLARE_DYNAMIC(CInsertObjectDialog)

	// SYNTHETIC: ROBLOX 0x0041ff00
	// ?GetThisClass@CInsertObjectDialog@@SGPAUCRuntimeClass@@XZ

public:
	CInsertObjectDialog(CWnd* pParent = NULL);
	virtual ~CInsertObjectDialog();

	// SYNTHETIC: ROBLOX 0x0041ffa0
	// CInsertObjectDialog::`scalar deleting destructor'

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
	CString m_unk0x74; // 0x74
};

DECOMP_SIZE_ASSERT(CInsertObjectDialog, 0x78)

#endif // INSERTOBJECTDIALOG_H
