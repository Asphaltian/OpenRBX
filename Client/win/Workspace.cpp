#include "Workspace.h"

#include "Content.h"
#include "InstanceExplorer.h"
#include "LogManager.h"
#include "LuaFunction.h"
#include "script/ScriptContext.h"
#include "security/SecurityContext.h"
#include "util/ContentProvider.h"
#include "v8datamodel/Commands.h"
#include "v8datamodel/Selection.h"
#include "v8xml/SerializerV2.h"
#include "v8xml/XmlSerializer.h"

#include <G3D/format.h>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/iostreams/copy.hpp>
#include <fstream>
#include <sstream>

#define CHECK_HRESULT(expr)                                                                                            \
	(void) ((expr) && (LogManager::ReportEvent(EVENTLOG_ERROR_TYPE, expr, __FILE__, __LINE__),                         \
					   FUN_0046f230(expr, CLSID_Workspace),                                                            \
					   1))

// FUNCTION: ROBLOX 0x0046edc0
boost::shared_ptr<RBX::DataModel> CWorkspace::FUN_0046edc0() const
{
	return m_unk0x24;
}

// FUNCTION: ROBLOX 0x0046edf0
RBX::Workspace* CWorkspace::FUN_0046edf0() const
{
	return m_unk0x24 ? m_unk0x24->getWorkspace() : NULL;
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

// FUNCTION: ROBLOX 0x0046f630
HRESULT CWorkspace::DoExecScript(
	LPCSTR script, VARIANT arg1, VARIANT arg2, VARIANT arg3, VARIANT arg4, SAFEARRAY** result
)
{
	try {
		RBX::DataModel::Lock lock(m_unk0x24);
		Unknown00431c00 arguments;
		arguments.FUN_00431c00(arg1);
		arguments.FUN_00431c00(arg2);
		arguments.FUN_00431c00(arg3);
		arguments.FUN_00431c00(arg4);

		std::auto_ptr<std::vector<RBX::Reflection::Value> > values =
			FUN_0046edc0()->create<RBX::ScriptContext>()->executeInNewThread(
				RBX::Security::TrustedCOM, script, NULL, arguments
			);
		if (result) {
			*result = FUN_00413500(*values);
		}

		return S_OK;
	}
	catch (std::exception& e) {
		std::string text(script);
		std::string message = G3D::format("DoExecScript failed because %s.  %s", e.what(), text.substr(0, 64).c_str());
		return LogManager::ReportCOMError(CLSID_Workspace, message.c_str(), S_OK);
	}
}

// FUNCTION: ROBLOX 0x0046f970
STDMETHODIMP CWorkspace::ExecUrlScript(
	BSTR url, VARIANT arg1, VARIANT arg2, VARIANT arg3, VARIANT arg4, SAFEARRAY** result
)
{
	try {
		AFX_MANAGE_STATE(AfxGetAppModuleState());
		std::ostringstream stream;

		try {
			CString text(url);
			std::auto_ptr<std::istream> content = RBX::ContentProvider::singleton().getContent(RBX::ContentId(text));
			boost::iostreams::copy(*content, stream, 0x1000);
		}
		catch (std::exception& e) {
			std::string message = G3D::format("Failed to get URL, %s", e.what());
			return LogManager::ReportCOMError(CLSID_Workspace, message.c_str(), S_OK);
		}

		return DoExecScript(stream.str().c_str(), arg1, arg2, arg3, arg4, result);
	}
	catch (CException* e) {
		char message[255];
		e->GetErrorMessage(message, 255, NULL);
		std::string text = G3D::format("CException %s", message);
		return LogManager::ReportCOMError(CLSID_Workspace, text.c_str(), S_OK);
	}
	catch (CAtlException e) {
		std::string message = G3D::format("CAtlException %d", e.m_hr);
		return LogManager::ReportCOMError(CLSID_Workspace, message.c_str(), S_OK);
	}
	catch (...) {
		return LogManager::ReportCOMError(CLSID_Workspace, "Unexpected exception", S_OK);
	}
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
HRESULT CWorkspace::FUN_004700b0(const XmlElement* root, std::ostream* stream)
{
	TextXmlWriterWithEmbeddedContent writer(*stream);
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
	FUN_004700b0(FUN_0046ffa0().get(), &stream);
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

// FUNCTION: ROBLOX 0x00470640
STDMETHODIMP CWorkspace::Write(IContent** result)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	try {
		CComObject<CContent>* content;
#line 304
		CHECK_HRESULT(CComObject<CContent>::CreateInstance(&content));
#line 308
		CHECK_HRESULT(FUN_004700b0(FUN_0046f040().get(), &content->m_unk0x0c));
#line 278

		CComPtr<IContent> pointer(content);
#line 312
		CHECK_HRESULT(pointer.CopyTo(result));
#line 283
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

// FUNCTION: ROBLOX 0x004708b0
STDMETHODIMP CWorkspace::WriteSelection(IContent** result)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	try {
		CComObject<CContent>* content;
#line 335
		CHECK_HRESULT(CComObject<CContent>::CreateInstance(&content));
#line 338
		CHECK_HRESULT(FUN_004700b0(FUN_0046ffa0().get(), &content->m_unk0x0c));
#line 306

		CComPtr<IContent> pointer(content);
#line 342
		CHECK_HRESULT(pointer.CopyTo(result));
#line 311
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

// FUNCTION: ROBLOX 0x00470b20
STDMETHODIMP CWorkspace::ExecScript(
	BSTR script, VARIANT arg1, VARIANT arg2, VARIANT arg3, VARIANT arg4, SAFEARRAY** result
)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	std::string text = (LPCTSTR) CString(script);
	if (RBX::ContentProvider::isHttpUrl(text)) {
		return ExecUrlScript(script, arg1, arg2, arg3, arg4, result);
	}

	return DoExecScript(text.c_str(), arg1, arg2, arg3, arg4, result);
}

// FUNCTION: ROBLOX 0x00470da0
STDMETHODIMP CWorkspace::SaveToFile(BSTR pathName)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	try {
		std::ofstream stream;
		stream.open(CString(pathName), std::ios_base::out | std::ios_base::binary);

		RBX::DataModel::Lock lock(m_unk0x24);
		return FUN_004700b0(FUN_0046f040().get(), &stream);
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
