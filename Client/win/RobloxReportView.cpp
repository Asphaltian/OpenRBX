#include "RobloxReportView.h"

#include "ActiveDocView.h"
#include "RobloxDoc.h"
#include "util/Events.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Stats.h"
#include "v8tree/Service.h"

#include <boost/pointer_cast.hpp>

IMPLEMENT_DYNCREATE(CRobloxReportDocView, CRobloxReportView)
IMPLEMENT_DYNAMIC(CRobloxReportView, CXTPReportView)
IMPLEMENT_DYNCREATE(CRobloxReportPaneView, CRobloxReportView)

template <class T>
static T* createService(RBX::DataModel* dataModel)
{
	return dataModel ? dataModel->create<T>() : NULL;
}

template <class T>
static T* findService(RBX::DataModel* dataModel)
{
	return dataModel ? dataModel->find<T>() : NULL;
}

int CRobloxReportView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPReportView::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	CXTPReportControl& report = GetReportCtrl();
	report.ShowGroupBy(FALSE);
	report.GetReportHeader()->AllowColumnSort(FALSE);
	report.GetReportHeader()->AllowColumnRemove(FALSE);
	report.GetReportHeader()->SetAutoColumnSizing(TRUE);

	CXTPReportColumn* column = report.AddColumn(new CXTPReportColumn(0, "Name", 200));
	column->SetTreeColumn(TRUE);
	column->SetEditable(FALSE);

	report.AddColumn(new CXTPReportColumn(1, "Value", 200))->SetEditable(FALSE);
	return 0;
}

void CRobloxReportView::VTable0x08(const RBX::Instance* source, RBX::ChildRemoved event)
{
	CXTPReportRecords* records = GetReportCtrl().GetRecords();
	for (int i = records->GetCount() - 1; i >= 0; i--) {
		CStatsItemRecord* record = static_cast<CStatsItemRecord*>(records->GetAt(i));
		if (record->m_unk0x98.get() == event.child.get()) {
			records->RemoveAt(i);
			break;
		}
	}
}

void CRobloxReportDocView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (bActivate) {
		ActiveDocView::FUN_004054f0(true, static_cast<CRobloxDoc*>(GetDocument()));
	}
	else if (pActivateView != pDeactiveView) {
		ActiveDocView::FUN_004054f0(false, static_cast<CRobloxDoc*>(GetDocument()));
	}
}

CRobloxReportView::CStatsItemRecord::CNameItem::CNameItem(
	RBX::Stats::Item* item, boost::shared_ptr<RBX::DataModel> dataModel
)
	: MutexRef(dataModel), m_unk0x84(item)
{
}

CString CRobloxReportView::CStatsItemRecord::CNameItem::GetCaption(CXTPReportColumn* pColumn)
{
	RBX::DataModel::Lock lock(m_unk0x00);
	return m_unk0x84->getName().c_str();
}

CRobloxReportView::CStatsItemRecord::CValueItem::CValueItem(
	RBX::Stats::Item* item, boost::shared_ptr<RBX::DataModel> dataModel
)
	: MutexRef(dataModel), m_unk0x84(item)
{
}

CString CRobloxReportView::CStatsItemRecord::CValueItem::GetCaption(CXTPReportColumn* pColumn)
{
	RBX::DataModel::Lock lock(m_unk0x00);
	return m_unk0x84->getStringValue().c_str();
}

void CRobloxReportView::FUN_0045b450()
{
	RBX::DataModel::Lock lock(m_unk0x3a8);
	GetReportCtrl().Populate();
	GetReportCtrl().RedrawControl();
}

void CRobloxReportView::OnTimer(UINT_PTR nIDEvent)
{
	FUN_0045b450();
	CXTPReportView::OnTimer(nIDEvent);
}

CRobloxReportView::CRobloxReportView() : m_unk0x3a4(0)
{
}

CRobloxReportView::CStatsItemRecord::CStatsItemRecord(
	boost::shared_ptr<RBX::Stats::Item> item, boost::shared_ptr<RBX::DataModel> dataModel
)
	: MutexRef(dataModel), m_unk0x98(item)
{
	AddItem(new CNameItem(item.get(), dataModel));
	AddItem(new CValueItem(item.get(), dataModel));

	RBX::DataModel::Lock lock(dataModel);
	RBX::startListening<RBX::Instance, RBX::ChildAdded>(item, this);
	RBX::startListening<RBX::Instance, RBX::ChildRemoved>(item, this);
}

void CRobloxReportView::CStatsItemRecord::VTable0x08(const RBX::Instance* source, RBX::ChildRemoved event)
{
	for (int i = GetChilds()->GetCount() - 1; i >= 0; i--) {
		CStatsItemRecord* record = static_cast<CStatsItemRecord*>(GetChilds()->GetAt(i));
		if (record->m_unk0x98.get() == event.child.get()) {
			GetChilds()->RemoveAt(i);
			break;
		}
	}
}

CRobloxReportView::CStatsItemRecord::~CStatsItemRecord()
{
	RBX::DataModel::Lock lock(m_unk0x00);
	RBX::stopListening<RBX::Instance, RBX::ChildAdded>(m_unk0x98, this);
	RBX::stopListening<RBX::Instance, RBX::ChildRemoved>(m_unk0x98, this);
}

void CRobloxReportView::CStatsItemRecord::VTable0x08(const RBX::Instance* source, RBX::ChildAdded event)
{
	boost::shared_ptr<RBX::Stats::Item> item = boost::shared_polymorphic_downcast<RBX::Stats::Item>(event.child);
	GetChilds()->Add(new CStatsItemRecord(item, m_unk0x00));
}

void CRobloxReportView::VTable0x08(const RBX::Instance* source, RBX::ChildAdded event)
{
	CXTPReportControl& report = GetReportCtrl();
	CStatsItemRecord* record =
		new CStatsItemRecord(boost::shared_polymorphic_downcast<RBX::Stats::Item>(event.child), m_unk0x3a8);
	record->SetExpanded(TRUE);
	report.AddRecord(record);
}

void CRobloxReportView::FUN_0045cc90(boost::shared_ptr<RBX::DataModel> dataModel)
{
	if (dataModel == m_unk0x3a8) {
		return;
	}

	if (m_unk0x3a8) {
		RBX::DataModel::Lock lock(m_unk0x3a8);
		findService<RBX::RunService>(m_unk0x3a8.get());

		RBX::stopListening<RBX::Instance, RBX::ChildAdded>(m_unk0x3b0, this);
		RBX::stopListening<RBX::Instance, RBX::ChildRemoved>(m_unk0x3b0, this);
		RBX::MarshaledListener<RBX::Instance, RBX::ChildAdded, RBX::Instance, RBX::ChildAdded>::FUN_00420ac0();
		RBX::MarshaledListener<RBX::Instance, RBX::ChildRemoved, RBX::Instance, RBX::ChildRemoved>::FUN_00420ac0();

		GetReportCtrl().GetRecords()->RemoveAll();
	}

	GetReportCtrl().Populate();
	m_unk0x3b0.reset();
	m_unk0x3a8 = dataModel;

	if (m_unk0x3a8) {
		RBX::DataModel::Lock lock(dataModel);
		m_unk0x3b0 = shared_from(createService<RBX::Stats::StatsService>(m_unk0x3a8.get()));
		m_unk0x3a4 = SetTimer(1, 333, NULL);

		RBX::startListening<RBX::Instance, RBX::ChildRemoved>(m_unk0x3b0, this);
		RBX::startListening<RBX::Instance, RBX::ChildAdded>(m_unk0x3b0, this);
		createService<RBX::RunService>(m_unk0x3a8.get());
	}
	else if (m_hWnd) {
		KillTimer(m_unk0x3a4);
	}
}

void CRobloxReportView::OnInitialUpdate()
{
	CXTPReportView::OnInitialUpdate();

	if (GetDocument()) {
		FUN_0045cc90(static_cast<CRobloxDoc*>(GetDocument())->FUN_00403290());
	}
	else {
		FUN_0045cc90(boost::shared_ptr<RBX::DataModel>());
	}
}

void CRobloxReportPaneView::OnInitialUpdate()
{
	CRobloxReportView::OnInitialUpdate();
	FUN_004204e0();
}

void CRobloxReportPaneView::VTable0x00(
	boost::shared_ptr<RBX::DataModel> newDataModel, boost::shared_ptr<RBX::DataModel> oldDataModel
)
{
	FUN_0045cc90(newDataModel);
}

CRobloxReportView::~CRobloxReportView()
{
	FUN_0045cc90(boost::shared_ptr<RBX::DataModel>());
}

CRobloxReportPaneView::CRobloxReportPaneView()
{
}

// clang-format off
BEGIN_MESSAGE_MAP(CRobloxReportView, CXTPReportView)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CRobloxReportPaneView, CRobloxReportView)
END_MESSAGE_MAP()
// clang-format on
