#ifndef ROBLOXVIEW_H
#define ROBLOXVIEW_H

#include "decomp.h"
#include "v8datamodel/DataModel.h"

#include <boost/shared_ptr.hpp>

// SIZE 0x1c0
class CRobloxView : public CView
{
	DECLARE_DYNCREATE(CRobloxView)

	// SYNTHETIC: ROBLOX 0x0045d4e0
	// ?GetThisClass@CRobloxView@@SGPAUCRuntimeClass@@XZ

protected:
	CRobloxView();

public:
	virtual ~CRobloxView();

	virtual void OnDraw(CDC* pDC);

	boost::shared_ptr<RBX::DataModel> FUN_0045d8e0() const;

private:
	undefined m_unk0x58[0x140 - 0x58];            // 0x58
	boost::shared_ptr<RBX::DataModel> m_unk0x140; // 0x140
	undefined m_unk0x148[0x1c0 - 0x148];          // 0x148
};

DECOMP_SIZE_ASSERT(CRobloxView, 0x1c0)

#endif // ROBLOXVIEW_H
