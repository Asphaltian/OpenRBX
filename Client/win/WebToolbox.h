#ifndef WEBTOOLBOX_H
#define WEBTOOLBOX_H

#include "Roblox_h.h"
#include "SecureHtmlView.h"
#include "decomp.h"

#include <atlbase.h>

class CRobloxWnd;

// SIZE 0xfc
class CWebToolbox : public CSecureHtmlView
{
	DECLARE_DYNCREATE(CWebToolbox)

	// SYNTHETIC: ROBLOX 0x0046e9e0
	// ?_GetBaseClass@CWebToolbox@@KGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x0046e9f0
	// ?GetThisClass@CWebToolbox@@SGPAUCRuntimeClass@@XZ

protected:
	CWebToolbox() {}

public:
	virtual ~CWebToolbox() {}

	// SYNTHETIC: ROBLOX 0x0046ec50
	// CWebToolbox::`scalar deleting destructor'

	// SYNTHETIC: ROBLOX 0x0046ecd0
	// ?CreateObject@CWebToolbox@@SGPAVCObject@@XZ

private:
	friend class CRobloxView;

	CRobloxWnd* m_unk0xf4;         // 0xf4
	CComPtr<IWorkspace> m_unk0xf8; // 0xf8
};

DECOMP_SIZE_ASSERT(CWebToolbox, 0xfc)

#endif // WEBTOOLBOX_H
