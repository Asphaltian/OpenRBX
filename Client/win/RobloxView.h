#ifndef ROBLOXVIEW_H
#define ROBLOXVIEW_H

#include "MultiPlayerPane.h"
#include "RobloxWnd.h"
#include "VerbBinder.h"
#include "WebToolbox.h"
#include "decomp.h"
#include "network/Players.h"
#include "network/SuperSafeChanged.h"
#include "util/Events.h"
#include "v8datamodel/DataModel.h"
#include "v8tree/Service.h"
#include "v8tree/Verb.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

class CRobloxView;

// VTABLE: ROBLOX 0x0080c8b8
// SIZE 0x20
class InsertModelFromRobloxVerb : public RBX::Verb
{
public:
	InsertModelFromRobloxVerb(CRobloxView* view, RBX::DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual bool isChecked() const;
	virtual void doIt(RBX::IDataState* dataState);

private:
	CRobloxView* m_unk0x0c;    // 0x0c
	RBX::DataModel* m_unk0x10; // 0x10
	double m_unk0x18;          // 0x18
};

DECOMP_SIZE_ASSERT(InsertModelFromRobloxVerb, 0x20)

// VTABLE: ROBLOX 0x0080c8ec
// SIZE 0x18
class ToggleIDEModeVerb : public RBX::Verb
{
public:
	ToggleIDEModeVerb(RBX::VerbContainer* container);

	virtual bool isEnabled() const;
	virtual bool isChecked() const;
	virtual void doIt(RBX::IDataState* dataState);

	// SYNTHETIC: ROBLOX 0x00410420
	// ExitCommand::`scalar deleting destructor'

private:
	double m_unk0x10; // 0x10
};

DECOMP_SIZE_ASSERT(ToggleIDEModeVerb, 0x18)

// VTABLE: ROBLOX 0x0080c914
// SIZE 0x0c
class LockPlayModeVerb : public RBX::Verb
{
public:
	LockPlayModeVerb(RBX::VerbContainer* container) : RBX::Verb(container, "LockPlayMode") {}

	virtual void doIt(RBX::IDataState* dataState);
};

DECOMP_SIZE_ASSERT(LockPlayModeVerb, 0x0c)

// VTABLE: ROBLOX 0x0080c93c
// SIZE 0x0c
class ToggleFullscreenVerb : public RBX::Verb
{
public:
	ToggleFullscreenVerb(RBX::VerbContainer* container) : RBX::Verb(container, "ToggleFullScreen") {}

	virtual bool isChecked() const;
	virtual void doIt(RBX::IDataState* dataState);
};

DECOMP_SIZE_ASSERT(ToggleFullscreenVerb, 0x0c)

// VTABLE: ROBLOX 0x0080c968
// SIZE 0x10
class HelpCommand : public RBX::Verb
{
public:
	HelpCommand(RBX::VerbContainer* container, CRobloxView* view) : RBX::Verb(container, "Help"), m_unk0x0c(view) {}

	virtual void doIt(RBX::IDataState* dataState);

private:
	CRobloxView* m_unk0x0c; // 0x0c
};

DECOMP_SIZE_ASSERT(HelpCommand, 0x10)

// SIZE 0x1c0
class CRobloxView : public VerbBinder<CRobloxView, CView>,
					public RBX::VerbContainer,
					public RBX::Listener<RBX::ServiceProvider, RBX::ServiceAdded>,
					public RBX::Listener<RBX::ServiceProvider, RBX::ServiceRemoving>,
					public RBX::Listener<RBX::Network::Players, RBX::SuperSafeChanged>
{
	DECLARE_DYNCREATE(CRobloxView)
	DECLARE_MESSAGE_MAP()

	// SYNTHETIC: ROBLOX 0x0045d4e0
	// ?GetThisClass@CRobloxView@@SGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x0045e5c0
	// ?CreateObject@CRobloxView@@SGPAVCObject@@XZ

protected:
	CRobloxView();

public:
	virtual ~CRobloxView();

	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	void FUN_0045d920();
	void FUN_0045da90(int p_unk0x04);
	afx_msg void FUN_0045dbe0();
	afx_msg void FUN_0045dbf0();
	void FUN_0045dd10(bool show, bool activate);
	afx_msg LRESULT FUN_0045de20(WPARAM wParam, LPARAM lParam);
	afx_msg void FUN_0045de50();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

	boost::shared_ptr<RBX::DataModel> FUN_00403290() const;
	void FUN_0045e900();
	RBX::VerbContainer* FUN_004558a0() { return this; }

protected:
	virtual void onEvent(const RBX::ServiceProvider* source, RBX::ServiceAdded event);
	virtual void onEvent(const RBX::ServiceProvider* source, RBX::ServiceRemoving event);
	virtual void onEvent(const RBX::Network::Players* source, RBX::SuperSafeChanged event);

private:
	friend class InsertModelFromRobloxVerb;

	undefined4 m_unk0x84;                         // 0x84
	CRobloxWnd m_unk0x88;                         // 0x88
	boost::shared_ptr<RBX::DataModel> m_unk0x140; // 0x140
	CMenu m_unk0x148;                             // 0x148
	CMultiPlayerPane m_unk0x150;                  // 0x150
	CWebToolbox* m_unk0x1a8;                      // 0x1a8
	boost::scoped_ptr<RBX::Verb> m_unk0x1ac;      // 0x1ac
	boost::scoped_ptr<RBX::Verb> m_unk0x1b0;      // 0x1b0
	boost::scoped_ptr<RBX::Verb> m_unk0x1b4;      // 0x1b4
	boost::scoped_ptr<RBX::Verb> m_unk0x1b8;      // 0x1b8
	boost::scoped_ptr<RBX::Verb> m_unk0x1bc;      // 0x1bc
};

DECOMP_SIZE_ASSERT(CRobloxView, 0x1c0)

#endif // ROBLOXVIEW_H
