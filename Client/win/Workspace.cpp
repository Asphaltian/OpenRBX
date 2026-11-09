#include "Workspace.h"

#include "Content.h"
#include "InstanceExplorer.h"
#include "LogManager.h"
#include "util/ContentProvider.h"
#include "v8datamodel/Commands.h"
#include "v8datamodel/Selection.h"
#include "v8xml/SerializerV2.h"
#include "v8xml/XmlSerializer.h"

#include <algorithm>
#include <boost/bind.hpp>
#include <fstream>

#define CHECK_HRESULT(expr)                                                                                            \
	if (expr) {                                                                                                        \
		LogManager::ReportEvent(EVENTLOG_ERROR_TYPE, expr, __FILE__, __LINE__);                                        \
		FUN_0046f230(expr, CLSID_Workspace);                                                                           \
	}

// FUNCTION: ROBLOX 0x0046edc0
boost::shared_ptr<RBX::DataModel> CWorkspace::FUN_0046edc0() const
{
	return m_unk0x24;
}

// FUNCTION: ROBLOX 0x0046ee10
void CWorkspace::onEvent(const RBX::Workspace* source, RBX::DrawChanged event)
{
	m_unk0x2c->invalidateRunViews();
}

// FUNCTION: ROBLOX 0x0046ee30
CWorkspace::~CWorkspace()
{
}

// FUNCTION: ROBLOX 0x0046ef00
void CWorkspace::FUN_0046ef00(std::istream& stream)
{
	RBX::DataModel::Lock lock(m_unk0x24);
	SerializerV2 serializer;
	serializer.load(stream, m_unk0x24.get());
	FUN_0046edf0()->joinAllHack();
}

// FUNCTION: ROBLOX 0x0046efb0
void CWorkspace::onEvent(const RBX::Selection* source, RBX::SelectionChanged event)
{
	m_unk0x2c->invalidateRunViews();
}

// FUNCTION: ROBLOX 0x0046f010
CWorkspace::CWorkspace()
{
}

// FUNCTION: ROBLOX 0x0046f040
std::auto_ptr<XmlElement> CWorkspace::FUN_0046f040()
{
	std::auto_ptr<XmlElement> root(SerializerV2::newRootElement());
	RBX::DataModel::Lock lock(m_unk0x24);
	FUN_0046edc0()->writeChildren(root.get());
	return root;
}

// FUNCTION: ROBLOX 0x0046f230
void FUN_0046f230(HRESULT hr, const GUID& guid)
{
	COleException* exception = new COleException;
	exception->m_sc = E_NOTIMPL;
	char message[1024];
	exception->GetErrorMessage(message, 1024);
	AtlReportError(guid, message, 0, NULL, GUID_NULL, hr);
	throw exception;
}

// FUNCTION: ROBLOX 0x0046f420
void CWorkspace::FUN_0046f420()
{
	if (m_unk0x24) {
		{
			RBX::DataModel::Lock lock(m_unk0x24);
			RBX::stopListening<RBX::Selection, RBX::SelectionChanged>(m_unk0x24->find<RBX::Selection>(), this);
			RBX::stopListening<RBX::Workspace, RBX::DrawChanged>(m_unk0x24->getWorkspace(), this);
		}

		m_unk0x2c.reset();
		RBX::DataModel::closeDataModel(m_unk0x24, true);
		m_unk0x24.reset();
	}
}

// STUB: ROBLOX 0x0046f970
STDMETHODIMP CWorkspace::ExecUrlScript(
	BSTR url, VARIANT arg1, VARIANT arg2, VARIANT arg3, VARIANT arg4, SAFEARRAY** result
)
{
	STUB(0x46f970);
	return S_OK;
}

// FUNCTION: ROBLOX 0x0046fd00
STDMETHODIMP CWorkspace::Close()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	FUN_0046f420();
	return S_OK;
}

// FUNCTION: ROBLOX 0x0046fe10
HRESULT CWorkspace::FinalConstruct()
{
	m_unk0x24 = RBX::DataModel::createDataModel();

	RBX::DataModel::Lock lock(m_unk0x24);
	m_unk0x2c = RBX::shared_from(m_unk0x24->create<RBX::RunService>());
	RBX::startListening<RBX::Workspace, RBX::DrawChanged>(m_unk0x24->getWorkspace(), this);
	RBX::startListening<RBX::Selection, RBX::SelectionChanged>(m_unk0x24->create<RBX::Selection>(), this);
	return S_OK;
}

// FUNCTION: ROBLOX 0x0046ff90
void CWorkspace::FinalRelease()
{
	FUN_0046f420();
}

// FUNCTION: ROBLOX 0x0046ffa0
std::auto_ptr<XmlElement> CWorkspace::FUN_0046ffa0()
{
	std::auto_ptr<XmlElement> root(SerializerV2::newRootElement());
	RBX::DataModel::Lock lock(m_unk0x24);
	RBX::Selection* selection = RBX::ServiceProvider::create<RBX::Selection>(FUN_0046edf0());
	std::for_each(selection->begin(), selection->end(), boost::bind(&RBX::FUN_00597490, root.get(), _1));
	return root;
}

// STUB: ROBLOX 0x004700b0
HRESULT CWorkspace::FUN_004700b0(const XmlElement* root, std::ostream& stream)
{
	TextXmlWriterWithEmbeddedContent writer(stream);
	writer.serialize(root);
	return S_OK;
}

// FUNCTION: ROBLOX 0x00470120
STDMETHODIMP CWorkspace::SaveSelectionToFile(BSTR pathName)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	std::ofstream stream;
	stream.open(CString(pathName), std::ios_base::out);

	RBX::DataModel::Lock lock(m_unk0x24);
	FUN_004700b0(FUN_0046ffa0().get(), stream);
	return S_OK;
}

// FUNCTION: ROBLOX 0x004702b0
void CWorkspace::FUN_004702b0(RBX::ContentId contentId, bool p_unk0x20)
{
	if (m_unk0x24->findFirstChildByName("NetworkClient")) {
		return;
	}

	RBX::DataModel::Lock lock(m_unk0x24);
	RBX::Selection* selection = RBX::ServiceProvider::create<RBX::Selection>(FUN_0046edf0());
	RBX::Instance* parent = p_unk0x20 && selection->size() == 1 ? selection->front().get() : FUN_0046edf0();
	RBX::InsertMode insertMode = p_unk0x20 ? RBX::INSERT_TO_TREE : RBX::INSERT_TO_3D_VIEW;
	RBX::PromptMode promptMode = p_unk0x20 ? RBX::SUPPRESS_PROMPTS : RBX::ALLOW_PROMPTS;
	std::vector<boost::shared_ptr<RBX::Instance> > instances;

	try {
		{
			std::auto_ptr<std::istream> stream = RBX::ContentProvider::singleton().getContent(contentId);
			TextXmlParser machine(stream->rdbuf());
			std::auto_ptr<XmlElement> root = machine.parse();
			SerializerV2().loadInstances(root.get(), instances);
		}

		FUN_0046edf0()->insertInstances(instances, parent, insertMode, promptMode);

		if (m_unk0x2c->getRunState() == RBX::RS_NORMAL) {
			const std::vector<boost::shared_ptr<RBX::Instance> >& items = instances;
			selection->setSelection(items.begin(), items.end());
		}
	}
	catch (...) {
		std::for_each(
			instances.begin(),
			instances.end(),
			boost::bind(&RBX::Instance::setParent, _1, static_cast<RBX::Instance*>(NULL))
		);
		throw;
	}
}

// STUB: ROBLOX 0x00470640
STDMETHODIMP CWorkspace::Write(IContent** result)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	try {
		CComObject<CContent>* content = NULL;
		CHECK_HRESULT(CComObject<CContent>::CreateInstance(&content));
		CHECK_HRESULT(FUN_004700b0(FUN_0046f040().get(), content->m_unk0x0c));

		CComPtr<IContent> pointer(content);
		CHECK_HRESULT(pointer.CopyTo(result));
		return S_OK;
	}
	catch (const std::exception& e) {
		return LogManager::ReportCOMError(CLSID_Workspace, e.what(), S_OK);
	}
	catch (CException* e) {
		HRESULT hr = COleException::Process(e);
		e->Delete();
		return hr;
	}
}

// STUB: ROBLOX 0x004708b0
STDMETHODIMP CWorkspace::WriteSelection(IContent** result)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	try {
		CComObject<CContent>* content = NULL;
		CHECK_HRESULT(CComObject<CContent>::CreateInstance(&content));
		CHECK_HRESULT(FUN_004700b0(FUN_0046ffa0().get(), content->m_unk0x0c));

		CComPtr<IContent> pointer(content);
		CHECK_HRESULT(pointer.CopyTo(result));
		return S_OK;
	}
	catch (const std::exception& e) {
		return LogManager::ReportCOMError(CLSID_Workspace, e.what(), S_OK);
	}
	catch (CException* e) {
		HRESULT hr = COleException::Process(e);
		e->Delete();
		return hr;
	}
}

// STUB: ROBLOX 0x00470b20
STDMETHODIMP CWorkspace::ExecScript(
	BSTR script, VARIANT arg1, VARIANT arg2, VARIANT arg3, VARIANT arg4, SAFEARRAY** result
)
{
	STUB(0x470b20);
	return S_OK;
}

// FUNCTION: ROBLOX 0x00470da0
STDMETHODIMP CWorkspace::SaveToFile(BSTR pathName)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	try {
		std::ofstream stream;
		stream.open(CString(pathName), std::ios_base::out | std::ios_base::binary);

		RBX::DataModel::Lock lock(m_unk0x24);
		return FUN_004700b0(FUN_0046f040().get(), stream);
	}
	catch (const std::exception& e) {
		return LogManager::ReportCOMError(CLSID_Workspace, e.what(), S_OK);
	}
}

// FUNCTION: ROBLOX 0x00470f40
STDMETHODIMP CWorkspace::Insert(BSTR url)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	try {
		if (HRESULT hr = FUN_0041e630(url)) {
			CString message;
			message.Format("Untrusted Url: %s", CString(url));
			return LogManager::ReportCOMError(CLSID_Workspace, message, hr);
		}

		FUN_004702b0(RBX::ContentId(CString(url)), false);
		return S_OK;
	}
	catch (const std::exception& e) {
		return LogManager::ReportCOMError(CLSID_Workspace, e.what(), S_OK);
	}
}

OBJECT_ENTRY_AUTO(__uuidof(Workspace), CWorkspace)
