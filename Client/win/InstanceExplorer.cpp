#include "InstanceExplorer.h"

#include "InsertObjectDialog.h"
#include "util/Events.h"
#include "util/Http.h"
#include "util/scopedassign.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Selection.h"
#include "v8datamodel/custommesh.h"
#include "v8tree/Service.h"

#include <fstream>
#include <urlmon.h>
#include <vector>

IMPLEMENT_DYNCREATE(CInstanceExplorer, CXTPReportView)

// FUNCTION: ROBLOX 0x0041e610
HRESULT FUN_0041e610(LPCTSTR url)
{
	return RBX::Http::trustCheck(url) ? S_OK : E_FAIL;
}

// FUNCTION: ROBLOX 0x0041e630
HRESULT FUN_0041e630(BSTR url)
{
	return FUN_0041e610(CString(url));
}

// FUNCTION: ROBLOX 0x0041e6b0
HRESULT FUN_0041e6b0(BSTR url, DWORD zone)
{
	if (zone == URLZONE_UNTRUSTED) {
		return E_FAIL;
	}

	return FUN_0041e630(url);
}

// FUNCTION: ROBLOX 0x0041e6d0
InsertDecal::InsertDecal(RBX::DataModel* dataModel, const char* name) : RBX::EditSelectionVerb(name, dataModel)
{
}

// FUNCTION: ROBLOX 0x0041e900
RBX::ContentId FUN_0041e900(const char* path)
{
	std::ifstream stream(path, std::ios_base::in | std::ios_base::binary);
	return RBX::ContentProvider::singleton().registerContent(stream, FUN_0043c120(path));
}

// FUNCTION: ROBLOX 0x0041ed90
boost::shared_ptr<RBX::Decal> InsertDecal::VTable0x14()
{
	return RBX::Creatable<RBX::Instance>::create<RBX::Decal>();
}

// FUNCTION: ROBLOX 0x0041edb0
boost::shared_ptr<RBX::Decal> InsertTexture::VTable0x14()
{
	return RBX::Creatable<RBX::Instance>::create<RBX::Texture>();
}

// FUNCTION: ROBLOX 0x0041f500
RBX::Instance* InsertDecal::FUN_0041f500() const
{
	RBX::Selection* selection = dataModel ? dataModel->create<RBX::Selection>() : NULL;
	if (selection->size() == 1) {
		RBX::Instance* instance = selection->front().get();
		if (dynamic_cast<RBX::PartInstance*>(instance)) {
			return instance;
		}

		if (dynamic_cast<RBX::SpecialShape*>(instance)) {
			return instance;
		}
	}

	return NULL;
}

// FUNCTION: ROBLOX 0x0041f5c0
void InsertDecal::doIt(RBX::IDataState* dataState)
{
	CString filter;
	filter.LoadString(132);

	CFileDialog dialog(TRUE, NULL, NULL, 0, filter);
	if (dialog.DoModal() == IDOK) {
		RBX::ContentId id = FUN_0041e900(dialog.GetPathName());
		boost::shared_ptr<RBX::Decal> decal = VTable0x14();
		decal->setTexture(id);
		decal->setName(std::string(dialog.GetFileTitle()));
		decal->setParent(FUN_0041f500());

		RBX::Selection* selection = dataModel ? dataModel->create<RBX::Selection>() : NULL;
		selection->setSelection(decal.get());

		dataState->clearStack();
	}
}

// FUNCTION: ROBLOX 0x0041f810
bool InsertDecal::isEnabled() const
{
	return RBX::EditSelectionVerb::isEnabled() && FUN_0041f500();
}

// FUNCTION: ROBLOX 0x0041f880
InsertObject::InsertObject(RBX::DataModel* dataModel) : RBX::RunStateVerb("Insert Object", dataModel)
{
}

// FUNCTION: ROBLOX 0x0041fb40
RBX::Instance* InsertObject::FUN_0041fb40() const
{
	RBX::Selection* selection = dataModel ? dataModel->create<RBX::Selection>() : NULL;
	size_t count = selection->size();
	if (count == 1) {
		return selection->front().get();
	}

	if (count == 0) {
		return dataModel->getWorkspace();
	}

	return NULL;
}

// FUNCTION: ROBLOX 0x0041fbe0
void InsertObject::doIt(RBX::IDataState* dataState)
{
	CInsertObjectDialog dialog;
	dialog.m_unk0x74 = AfxGetApp()->GetProfileString("InsertObject", "ClassName").GetBuffer();

	if (dialog.DoModal() == IDOK) {
		AfxGetApp()->WriteProfileString("InsertObject", "ClassName", dialog.m_unk0x74);

		const RBX::Name& className = RBX::Name::lookup(dialog.m_unk0x74);
		boost::shared_ptr<RBX::Instance> instance = RBX::AbstractFactoryProduct<RBX::Instance>::create(className);
		if (!instance) {
			CString message;
			message.Format(133, dialog.m_unk0x74);
			AfxMessageBox(message);
		}
		else if (dynamic_cast<RBX::Service*>(instance.get())) {
			if (!RBX::ServiceProvider::create(dataModel, className)) {
				CString message;
				message.Format(137, dialog.m_unk0x74);
				AfxMessageBox(message);
			}
		}
		else {
			if (!instance->canSetParent(FUN_0041fb40())) {
				throw std::runtime_error(
					G3D::format(
						"%s cannot be a child of %s",
						instance->getClassNameStr().c_str(),
						FUN_0041fb40()->getName().c_str()
					)
				);
			}

			instance->setParent(FUN_0041fb40());

			RBX::Selection* selection = dataModel ? dataModel->create<RBX::Selection>() : NULL;
			selection->setSelection(instance.get());

			dataState->clearStack();
		}
	}
}

// FUNCTION: ROBLOX 0x0041fef0
bool InsertObject::isEnabled() const
{
	return FUN_0041fb40() != NULL;
}

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
