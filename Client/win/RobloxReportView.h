#ifndef ROBLOXREPORTVIEW_H
#define ROBLOXREPORTVIEW_H

#include "ActiveDocView.h"
#include "MarshaledListener.h"
#include "decomp.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Stats.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>

// SIZE 0x8
class MutexRef
{
public:
	MutexRef(boost::shared_ptr<RBX::DataModel> dataModel) : m_unk0x00(dataModel) {}

	boost::shared_ptr<RBX::DataModel> m_unk0x00; // 0x00
};

DECOMP_SIZE_ASSERT(MutexRef, 0x8)

// SIZE 0x3b8
class CRobloxReportView
	: public CXTPReportView,
	  public RBX::MarshaledListener<RBX::Instance, RBX::ChildAdded, RBX::Instance, RBX::ChildAdded>,
	  public RBX::MarshaledListener<RBX::Instance, RBX::ChildRemoved, RBX::Instance, RBX::ChildRemoved>
{
	DECLARE_DYNAMIC(CRobloxReportView)

public:
	// SIZE 0xa0
	class CStatsItemRecord
		: public CXTPReportRecord,
		  public MutexRef,
		  public RBX::MarshaledListener<RBX::Instance, RBX::ChildAdded, RBX::Instance, RBX::ChildAdded>,
		  public RBX::MarshaledListener<RBX::Instance, RBX::ChildRemoved, RBX::Instance, RBX::ChildRemoved>
	{
	public:
		// SIZE 0x88
		class CNameItem : public MutexRef, public CXTPReportRecordItem
		{
		public:
			CNameItem(RBX::Stats::Item* item, boost::shared_ptr<RBX::DataModel> dataModel);

			virtual CString GetCaption(CXTPReportColumn* pColumn);

		private:
			RBX::Stats::Item* m_unk0x84; // 0x84
		};

		// SIZE 0x88
		class CValueItem : public MutexRef, public CXTPReportRecordItem
		{
		public:
			CValueItem(RBX::Stats::Item* item, boost::shared_ptr<RBX::DataModel> dataModel);

			virtual CString GetCaption(CXTPReportColumn* pColumn);

		private:
			RBX::Stats::Item* m_unk0x84; // 0x84
		};

		CStatsItemRecord(boost::shared_ptr<RBX::Stats::Item> item, boost::shared_ptr<RBX::DataModel> dataModel);
		virtual ~CStatsItemRecord();

	protected:
		virtual void VTable0x08(const RBX::Instance* source, RBX::ChildRemoved event);
		virtual void VTable0x08(const RBX::Instance* source, RBX::ChildAdded event);

	public:
		undefined4 m_unk0x94;                          // 0x94
		boost::shared_ptr<RBX::Stats::Item> m_unk0x98; // 0x98
	};

protected:
	CRobloxReportView();

public:
	virtual ~CRobloxReportView();

	virtual void OnInitialUpdate(); // vtable+0x164

	void FUN_0045b450();
	void FUN_0045cc90(boost::shared_ptr<RBX::DataModel> dataModel);

protected:
	virtual void VTable0x08(const RBX::Instance* source, RBX::ChildRemoved event);
	virtual void VTable0x08(const RBX::Instance* source, RBX::ChildAdded event);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

private:
	UINT_PTR m_unk0x3a4;                                    // 0x3a4
	boost::shared_ptr<RBX::DataModel> m_unk0x3a8;           // 0x3a8
	boost::shared_ptr<RBX::Stats::StatsService> m_unk0x3b0; // 0x3b0
};

DECOMP_SIZE_ASSERT(CRobloxReportView::CStatsItemRecord::CNameItem, 0x88)
DECOMP_SIZE_ASSERT(CRobloxReportView::CStatsItemRecord::CValueItem, 0x88)
DECOMP_SIZE_ASSERT(CRobloxReportView::CStatsItemRecord, 0xa0)
DECOMP_SIZE_ASSERT(CRobloxReportView, 0x3b8)

// SIZE 0x3b8
class CRobloxReportDocView : public CRobloxReportView
{
	DECLARE_DYNCREATE(CRobloxReportDocView)

protected:
	CRobloxReportDocView() {}

	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView); // vtable+0x168
};

DECOMP_SIZE_ASSERT(CRobloxReportDocView, 0x3b8)

// SIZE 0x3bc
class CRobloxReportPaneView : public CRobloxReportView, public ActiveDocView
{
	DECLARE_DYNCREATE(CRobloxReportPaneView)

protected:
	CRobloxReportPaneView();

public:
	virtual void OnInitialUpdate(); // vtable+0x164

protected:
	virtual void VTable0x00(
		boost::shared_ptr<RBX::DataModel> newDataModel,
		boost::shared_ptr<RBX::DataModel> oldDataModel
	); // vtable+0x00

	DECLARE_MESSAGE_MAP()
};

DECOMP_SIZE_ASSERT(CRobloxReportPaneView, 0x3bc)

// clang-format off
// SYNTHETIC: ROBLOX 0x0045ac00
// ?GetThisClass@CRobloxReportDocView@@SGPAUCRuntimeClass@@XZ
// SYNTHETIC: ROBLOX 0x0045ac10
// ?_GetBaseClass@CRobloxReportDocView@@KGPAUCRuntimeClass@@XZ
// SYNTHETIC: ROBLOX 0x0045ac20
// ?GetRuntimeClass@CRobloxReportPaneView@@UBEPAUCRuntimeClass@@XZ
// SYNTHETIC: ROBLOX 0x0045b180
// CRobloxReportView::CStatsItemRecord::CNameItem::~CNameItem
// SYNTHETIC: ROBLOX 0x0045b5d0
// CRobloxReportView::CStatsItemRecord::CValueItem::`scalar deleting destructor'
// SYNTHETIC: ROBLOX 0x0045b620
// ?GetMessageMap@CRobloxReportView@@MBEPBUAFX_MSGMAP@@XZ
// SYNTHETIC: ROBLOX 0x0045b630
// ?GetMessageMap@CRobloxReportPaneView@@MBEPBUAFX_MSGMAP@@XZ
// SYNTHETIC: ROBLOX 0x0045c720
// CRobloxReportView::CStatsItemRecord::`scalar deleting destructor'
// SYNTHETIC: ROBLOX 0x0045d350
// CRobloxReportPaneView::~CRobloxReportPaneView
// SYNTHETIC: ROBLOX 0x0045d3b0
// CRobloxReportDocView::`scalar deleting destructor'
// SYNTHETIC: ROBLOX 0x0045d3d0
// ?CreateObject@CRobloxReportDocView@@SGPAVCObject@@XZ
// SYNTHETIC: ROBLOX 0x0045d450
// ?CreateObject@CRobloxReportPaneView@@SGPAVCObject@@XZ
// SYNTHETIC: ROBLOX 0x0045d4c0
// CRobloxReportPaneView::`scalar deleting destructor'
// clang-format on

#endif // ROBLOXREPORTVIEW_H
