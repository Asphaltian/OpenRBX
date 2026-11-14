#ifndef ROBLOXREPORTVIEW_H
#define ROBLOXREPORTVIEW_H

#include "ActiveDocView.h"
#include "MarshaledListener.h"
#include "placeholder.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Stats.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>

class MutexRef
{
public:
	MutexRef(boost::shared_ptr<RBX::DataModel> dataModel) : m_unk0x00(dataModel) {}

	boost::shared_ptr<RBX::DataModel> m_unk0x00;
};

class CRobloxReportView
	: public CXTPReportView,
	  public RBX::MarshaledListener<RBX::Instance, RBX::ChildAdded, RBX::Instance, RBX::ChildAdded>,
	  public RBX::MarshaledListener<RBX::Instance, RBX::ChildRemoved, RBX::Instance, RBX::ChildRemoved>
{
	DECLARE_DYNAMIC(CRobloxReportView)

public:
	class CStatsItemRecord
		: public CXTPReportRecord,
		  public MutexRef,
		  public RBX::MarshaledListener<RBX::Instance, RBX::ChildAdded, RBX::Instance, RBX::ChildAdded>,
		  public RBX::MarshaledListener<RBX::Instance, RBX::ChildRemoved, RBX::Instance, RBX::ChildRemoved>
	{
	public:
		class CNameItem : public MutexRef, public CXTPReportRecordItem
		{
		public:
			CNameItem(RBX::Stats::Item* item, boost::shared_ptr<RBX::DataModel> dataModel);

			virtual CString GetCaption(CXTPReportColumn* pColumn);

		private:
			RBX::Stats::Item* m_unk0x84;
		};

		class CValueItem : public MutexRef, public CXTPReportRecordItem
		{
		public:
			CValueItem(RBX::Stats::Item* item, boost::shared_ptr<RBX::DataModel> dataModel);

			virtual CString GetCaption(CXTPReportColumn* pColumn);

		private:
			RBX::Stats::Item* m_unk0x84;
		};

		CStatsItemRecord(boost::shared_ptr<RBX::Stats::Item> item, boost::shared_ptr<RBX::DataModel> dataModel);
		virtual ~CStatsItemRecord();

	protected:
		virtual void VTable0x08(const RBX::Instance* source, RBX::ChildRemoved event);
		virtual void VTable0x08(const RBX::Instance* source, RBX::ChildAdded event);

	public:
		undefined4 m_unk0x94;
		boost::shared_ptr<RBX::Stats::Item> m_unk0x98;
	};

protected:
	CRobloxReportView();

public:
	virtual ~CRobloxReportView();

	virtual void OnInitialUpdate();

	void FUN_0045b450();
	void FUN_0045cc90(boost::shared_ptr<RBX::DataModel> dataModel);

protected:
	virtual void VTable0x08(const RBX::Instance* source, RBX::ChildRemoved event);
	virtual void VTable0x08(const RBX::Instance* source, RBX::ChildAdded event);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

private:
	UINT_PTR m_unk0x3a4;
	boost::shared_ptr<RBX::DataModel> m_unk0x3a8;
	boost::shared_ptr<RBX::Stats::StatsService> m_unk0x3b0;
};

class CRobloxReportDocView : public CRobloxReportView
{
	DECLARE_DYNCREATE(CRobloxReportDocView)

protected:
	CRobloxReportDocView() {}

	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
};

class CRobloxReportPaneView : public CRobloxReportView, public ActiveDocView
{
	DECLARE_DYNCREATE(CRobloxReportPaneView)

protected:
	CRobloxReportPaneView();

public:
	virtual void OnInitialUpdate();

protected:
	virtual void VTable0x00(
		boost::shared_ptr<RBX::DataModel> newDataModel, boost::shared_ptr<RBX::DataModel> oldDataModel
	);

	DECLARE_MESSAGE_MAP()
};

#endif // ROBLOXREPORTVIEW_H
