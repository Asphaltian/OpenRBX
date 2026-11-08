#include "Workspace.h"

#include "LogManager.h"
#include "v8xml/SerializerV2.h"
#include "v8xml/XmlSerializer.h"

#include <fstream>

// FUNCTION: ROBLOX 0x0046ee10
void CWorkspace::onEvent(const RBX::Workspace* source, RBX::DrawChanged event)
{
	m_unk0x2c->invalidateRunViews();
}

// FUNCTION: ROBLOX 0x0046ee30
CWorkspace::~CWorkspace()
{
}

// STUB: ROBLOX 0x0046ef00
void CWorkspace::FUN_0046ef00(std::istream& stream)
{
	RBX::DataModel::Lock lock(m_unk0x24);
	SerializerV2().load(stream, m_unk0x24.get());
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

// STUB: ROBLOX 0x004700b0
HRESULT CWorkspace::FUN_004700b0(const XmlElement* root, std::ostream& stream)
{
	TextXmlWriterWithEmbeddedContent writer(stream);
	writer.serialize(root);
	return S_OK;
}

// STUB: ROBLOX 0x00470120
STDMETHODIMP CWorkspace::SaveSelectionToFile(BSTR pathName)
{
	STUB(0x470120);
	return S_OK;
}

// STUB: ROBLOX 0x00470640
STDMETHODIMP CWorkspace::Write(IContent** result)
{
	STUB(0x470640);
	return S_OK;
}

// STUB: ROBLOX 0x004708b0
STDMETHODIMP CWorkspace::WriteSelection(IContent** result)
{
	STUB(0x4708b0);
	return S_OK;
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

// STUB: ROBLOX 0x00470f40
STDMETHODIMP CWorkspace::Insert(BSTR url)
{
	STUB(0x470f40);
	return S_OK;
}

OBJECT_ENTRY_AUTO(__uuidof(Workspace), CWorkspace)
