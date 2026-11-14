#ifndef MULTIPLAYERPANE_H
#define MULTIPLAYERPANE_H

#include "placeholder.h"

class CChatPrompt;

namespace RBX {
class DataModel;
}

class CMultiPlayerPane : public CWnd
{
	DECLARE_DYNAMIC(CMultiPlayerPane)

public:
	CMultiPlayerPane();
	virtual ~CMultiPlayerPane();

	BOOL FUN_00436ff0(CWnd* parent, RBX::DataModel* dataModel);

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

private:
	CChatPrompt* m_unk0x54;
};

#endif // MULTIPLAYERPANE_H
