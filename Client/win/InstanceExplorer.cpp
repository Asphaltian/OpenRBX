#include "InstanceExplorer.h"

#include "util/Events.h"
#include "util/scopedassign.h"
#include "v8datamodel/Selection.h"
#include "v8tree/Service.h"

#include <vector>

IMPLEMENT_DYNCREATE(CInstanceExplorer, CXTPReportView)

// FUNCTION: ROBLOX 0x00420050
CString CInstanceRecord::CNameItem::GetCaption(CXTPReportColumn* pColumn)
{
	return m_unk0x7c->getName().c_str();
}

// FUNCTION: ROBLOX 0x004202e0
int CInstanceExplorer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPReportView::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	CXTPReportControl& report = GetReportCtrl();
	report.GetReportHeader()->AllowColumnSort(TRUE);
	report.GetReportHeader()->AllowColumnRemove(FALSE);
	report.GetReportHeader()->SetAutoColumnSizing(TRUE);

	CXTPReportColumn* column = report.AddColumn(new CXTPReportColumn(0, "Name", 300));
	column->SetTreeColumn(TRUE);
	column->SetEditable(FALSE);
	return 0;
}

// STUB: ROBLOX 0x00420530
void CInstanceExplorer::VTable0x08(const RBX::Instance* source, RBX::DescendentRemoving event)
{
	CXTPReportRecords* records = GetReportCtrl().GetRecords();
	for (int i = records->GetCount() - 1; i >= 0; i--) {
		if (static_cast<CInstanceRecord*>(records->GetAt(i))->m_unk0x54.get() == event.instance.get()) {
			records->RemoveAt(i);
			break;
		}
	}

	if (!m_unk0x3ac) {
		GetReportCtrl().Populate();
	}
}

// FUNCTION: ROBLOX 0x004206b0
CInstanceRecord::CInstanceRecord(boost::shared_ptr<RBX::Instance> instance) : m_unk0x54(instance)
{
	AddItem(new CNameItem(instance.get()));
	AddItem(new CXTPReportRecordItemText(instance->getClassName().toString().c_str()));
}

// FUNCTION: ROBLOX 0x004207f0
CInstanceRecord::~CInstanceRecord()
{
}

// STUB: ROBLOX 0x004208d0
void CInstanceExplorer::VTable0x08(const RBX::Instance* source, RBX::DescendentAdded event)
{
	CInstanceRecord* record = new CInstanceRecord(shared_from(event.instance.get()));
	GetReportCtrl().AddRecord(record);

	if (!m_unk0x3ac) {
		GetReportCtrl().Populate();
	}
}

// FUNCTION: ROBLOX 0x00421080
CInstanceExplorer::CInstanceExplorer() : m_unk0x3ac(false)
{
}

// FUNCTION: ROBLOX 0x004212d0
void CInstanceExplorer::FUN_004212d0(RBX::Instance* root)
{
	RBX::ScopedAssign<bool> assign(m_unk0x3ac, true);

	RBX::stopListening<RBX::Instance, RBX::DescendentAdded>(m_unk0x3a4, this);
	RBX::stopListening<RBX::Instance, RBX::DescendentRemoving>(m_unk0x3a4, this);

	GetReportCtrl().GetRecords()->RemoveAll();
	m_unk0x3a4 = shared_from(root);

	RBX::startListening<RBX::Instance, RBX::DescendentAdded>(m_unk0x3a4, this);
	RBX::startListening<RBX::Instance, RBX::DescendentRemoving>(m_unk0x3a4, this);

	GetReportCtrl().Populate();
}

// FUNCTION: ROBLOX 0x00421440
CInstanceExplorer::~CInstanceExplorer()
{
	FUN_004212d0(NULL);
}

// STUB: ROBLOX 0x00421580
void CInstanceExplorer::FUN_00421580(NMHDR* pNMHDR, LRESULT* pResult)
{
	RBX::Selection* selection = RBX::ServiceProvider::create<RBX::Selection>(m_unk0x3a4.get());
	std::vector<RBX::Instance*> instances;

	CXTPReportSelectedRows* rows = GetReportCtrl().GetSelectedRows();
	POSITION pos = rows->GetFirstSelectedRowPosition();
	while (pos) {
		CXTPReportRow* row = rows->GetNextSelectedRow(pos);
		RBX::Instance* instance = static_cast<CInstanceRecord*>(row->GetRecord())->m_unk0x54.get();
		instances.push_back(instance);
	}

	selection->setSelection(instances.begin(), instances.end());
}

// clang-format off
BEGIN_MESSAGE_MAP(CInstanceExplorer, CXTPReportView)
	ON_WM_CREATE()
	ON_NOTIFY(XTP_NM_REPORT_SELCHANGED, 100, FUN_00421580)
END_MESSAGE_MAP()
// clang-format on
