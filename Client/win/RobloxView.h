#ifndef ROBLOXVIEW_H
#define ROBLOXVIEW_H

#include "MultiPlayerPane.h"
#include "RobloxWnd.h"
#include "VerbBinder.h"
#include "decomp.h"
#include "network/Players.h"
#include "network/SuperSafeChanged.h"
#include "util/Events.h"
#include "v8datamodel/DataModel.h"
#include "v8tree/Service.h"
#include "v8tree/Verb.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

// SIZE 0x1c0
class CRobloxView : public VerbBinder<CRobloxView, CView>,
					public RBX::VerbContainer,
					public RBX::Listener<RBX::ServiceProvider, RBX::ServiceAdded>,
					public RBX::Listener<RBX::ServiceProvider, RBX::ServiceRemoving>,
					public RBX::Listener<RBX::Network::Players, RBX::SuperSafeChanged>
{
	DECLARE_DYNCREATE(CRobloxView)

	// SYNTHETIC: ROBLOX 0x0045d4e0
	// ?GetThisClass@CRobloxView@@SGPAUCRuntimeClass@@XZ

protected:
	CRobloxView();

public:
	virtual ~CRobloxView();

	virtual void OnDraw(CDC* pDC);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

	boost::shared_ptr<RBX::DataModel> FUN_00403290() const;
	RBX::VerbContainer* FUN_004558a0() { return this; }

protected:
	virtual void onEvent(const RBX::ServiceProvider* source, RBX::ServiceAdded event);
	virtual void onEvent(const RBX::ServiceProvider* source, RBX::ServiceRemoving event);
	virtual void onEvent(const RBX::Network::Players* source, RBX::SuperSafeChanged event);

private:
	undefined4 m_unk0x84;                         // 0x84
	CRobloxWnd m_unk0x88;                         // 0x88
	boost::shared_ptr<RBX::DataModel> m_unk0x140; // 0x140
	CMenu m_unk0x148;                             // 0x148
	CMultiPlayerPane m_unk0x150;                  // 0x150
	undefined4 m_unk0x1a8;                        // 0x1a8
	boost::scoped_ptr<RBX::Verb> m_unk0x1ac;      // 0x1ac
	boost::scoped_ptr<RBX::Verb> m_unk0x1b0;      // 0x1b0
	boost::scoped_ptr<RBX::Verb> m_unk0x1b4;      // 0x1b4
	boost::scoped_ptr<RBX::Verb> m_unk0x1b8;      // 0x1b8
	boost::scoped_ptr<RBX::Verb> m_unk0x1bc;      // 0x1bc
};

DECOMP_SIZE_ASSERT(CRobloxView, 0x1c0)

#endif // ROBLOXVIEW_H
