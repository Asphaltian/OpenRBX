#include "RobloxDoc.h"

#include "ActiveDocView.h"
#include "InstanceExplorer.h"
#include "LDrawParser.h"
#include "LogManager.h"
#include "MainFrm.h"
#include "SaveToRobloxDialog.h"
#include "network/Players.h"
#include "script/LuaMemory.h"
#include "script/ScriptContext.h"
#include "util/ContentProvider.h"
#include "util/FileSystem.h"
#include "util/Log.h"
#include "util/RunStateOwner.h"
#include "util/Sound.h"
#include "util/StandardOut.h"
#include "v8datamodel/Selection.h"
#include "v8datamodel/TimeState.h"
#include "v8datamodel/Visit.h"
#include "v8datamodel/Workspace.h"
#include "v8xml/SerializerV2.h"
#include "v8xml/XmlSerializer.h"

#include <boost/iostreams/copy.hpp>
#include <shlobj.h>
#include <shlwapi.h>
#include <sstream>

IMPLEMENT_DYNCREATE(CRobloxDoc, CDocument)

template <class T>
static T* createService(RBX::DataModel* dataModel)
{
	return dataModel ? dataModel->create<T>() : NULL;
}

static bool canSave(RBX::DataModel* dataModel)
{
	bool result = !RBX::Network::Players::clientIsPresent(dataModel, true) ||
				  RBX::Network::Players::serverIsPresent(dataModel, true);
	return result;
}

template <class T>
static T* findService(RBX::DataModel* dataModel)
{
	return dataModel ? dataModel->find<T>() : NULL;
}

void CRobloxDoc::OnCloseDocument()
{
	CDocument::OnCloseDocument();
}

CPublishAsPlaceDialog::CPublishAsPlaceDialog(IDispatch* external, CWnd* pParentWnd)
	: CSaveToRobloxDialog(external, FUN_00407190() + "/IDE/Upload.aspx", pParentWnd)
{
}

CUploadPlaceDialog::CUploadPlaceDialog(IDispatch* external, CWnd* pParentWnd)
	: CSaveToRobloxDialog(external, FUN_00407190() + "/Game/Upload.aspx", pParentWnd)
{
}

CSaveModelToRobloxDialog::CSaveModelToRobloxDialog(IDispatch* external, CWnd* pParentWnd)
	: CSaveToRobloxDialog(external, FUN_00407190() + "/UI/Save.aspx?type=Model", pParentWnd)
{
}

void ReportAbuseVerb::doIt(RBX::IDataState* dataState)
{
	AfxGetApp()->m_pMainWnd->PostMessage(WM_COMMAND, 0x8100);
}

void ExitCommand::doIt(RBX::IDataState* dataState)
{
	CMainFrame* mainFrame = static_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd);

	if (mainFrame->m_unk0xec) {
		mainFrame->PostMessage(WM_CLOSE);
		return;
	}

	POSITION pos = m_unk0x0c->GetFirstViewPosition();
	if (pos) {
		m_unk0x0c->GetNextView(pos)->PostMessage(WM_COMMAND, ID_FILE_CLOSE);
	}
}

void CRobloxDoc::FUN_00454430()
{
	CString filter;
	filter.LoadString(184);

	CFileDialog dialog(FALSE, "rbxm", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
	if (dialog.DoModal() == IDOK) {
		CComBSTR pathName(dialog.GetPathName());
		m_unk0x78->SaveSelectionToFile(pathName);
	}
}

BOOL CRobloxDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	if (!canSave(m_unk0x78->FUN_0046edc0().get())) {
		return FALSE;
	}

	return CDocument::DoSave(lpszPathName, bReplace);
}

BOOL CRobloxDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	if (!canSave(m_unk0x78->FUN_0046edc0().get())) {
		return FALSE;
	}

	if (m_unk0x78->SaveToFile(CComBSTR(lpszPathName)) != S_OK) {
		return FALSE;
	}

	SetModifiedFlag(FALSE);
	return TRUE;
}

void CRobloxDoc::SetModifiedFlag(BOOL bModified)
{
	m_bModified = bModified;
	if (m_unk0x78) {
		m_unk0x78->FUN_0046edc0()->setDirty(bModified == TRUE);
	}
}

void CRobloxDoc::SetTitle(LPCTSTR lpszTitle)
{
	CDocument::SetTitle(lpszTitle);
	if (m_unk0x78) {
		m_unk0x78->FUN_0046edc0()->setName(lpszTitle);
	}
}

void CRobloxDoc::FUN_00454920()
{
	CPublishAsPlaceDialog dialog(m_unk0x78, NULL);
	dialog.DoModal();
}

void CRobloxDoc::FUN_004549c0()
{
	CSaveModelToRobloxDialog dialog(m_unk0x78, NULL);
	dialog.DoModal();
}

void CRobloxDoc::FUN_00454a60()
{
	CRobloxDHtmlDialog dialog(123, FUN_00407190() + "/AbuseReport/InGameChat.aspx", NULL);
	dialog.SetExternalDispatch(m_unk0x78);
	dialog.DoModal();
}

CString CRobloxDoc::FUN_00454b50(CRobloxDoc* doc, const char* name)
{
	CString path;
	GetShortPathName(
		RBX::FileSystem::getUserAppDataDirectory(true, "Test").c_str(), path.GetBuffer(MAX_PATH), MAX_PATH
	);
	path.ReleaseBuffer();
	path += name;
	path += ".rbxl";

	if (doc->m_unk0x78->SaveToFile(CComBSTR(path)) != S_OK) {
		throw std::runtime_error("Failed to copy document");
	}

	return path;
}

static void FUN_00454cb0(const char* file, const char* arguments)
{
	CString folder;
	if (SHGetFolderPath(NULL, CSIDL_COMMON_DOCUMENTS, NULL, SHGFP_TYPE_CURRENT, folder.GetBuffer(MAX_PATH))) {
		throw std::runtime_error("Unable to get common document folder");
	}
	folder.ReleaseBuffer();

	CString module;
	AfxGetModuleShortFileName(AfxGetApp()->m_hInstance, module);

	CString parameters;
	if (file) {
		parameters += file;
	}
	if (arguments) {
		parameters += arguments;
	}

	if ((UINT_PTR) ShellExecute(AfxGetMainWnd()->m_hWnd, "open", module, parameters, folder, SW_SHOW) <= 32) {
		throw std::runtime_error("ShellExecute failed");
	}
}

void CRobloxDoc::FUN_00454e50()
{
	try {
		FUN_00454cb0(FUN_00454b50(this, "Visit"), " -script " + FUN_00407190() + "/game/visit.ashx");
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what());
	}
}

void CRobloxDoc::FUN_00454f50()
{
	try {
		FUN_00454cb0(FUN_00454b50(this, "Server"), " -script " + FUN_00407190() + "/game/gameserver.ashx");
		FUN_00454cb0(NULL, " -script " + FUN_00407190() + "/game/join.ashx");
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what());
	}
}

void CRobloxDoc::FUN_004550b0()
{
	try {
		FUN_00454cb0(FUN_00454b50(this, "Server"), " -script " + FUN_00407190() + "/game/gameserver.ashx");
		FUN_00454cb0(NULL, " -script " + FUN_00407190() + "/game/join.ashx");
		FUN_00454cb0(NULL, " -script " + FUN_00407190() + "/game/join.ashx?UserID=0&port=4344");
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what());
	}
}

RBX::VerbContainer* CRobloxDoc::FUN_004558a0()
{
	return FUN_00403290()->getWorkspace();
}

void CRobloxDoc::FUN_00455900()
{
	RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
	m_unk0x74->m_unk0x120.doIt(m_unk0x78->FUN_0046edc0().get());
}

void CRobloxDoc::FUN_00455a30(CCmdUI* pCmdUI)
{
	boost::shared_ptr<RBX::DataModel> dataModel = m_unk0x78->FUN_0046edc0();
	std::string name;
	{
		RBX::DataModel::Lock lock(dataModel);
		pCmdUI->Enable(dataModel->canUndo(name));
	}

	CString text;
	text.Format(130, name.c_str());
	pCmdUI->SetText(text);
}

void CRobloxDoc::FUN_00455bb0()
{
	RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
	m_unk0x74->m_unk0x130.doIt(m_unk0x78->FUN_0046edc0().get());
}

void CRobloxDoc::FUN_00455ce0(CCmdUI* pCmdUI)
{
	boost::shared_ptr<RBX::DataModel> dataModel = m_unk0x78->FUN_0046edc0();
	std::string name;
	{
		RBX::DataModel::Lock lock(dataModel);
		pCmdUI->Enable(dataModel->canRedo(name));
	}

	CString text;
	text.Format(131, name.c_str());
	pCmdUI->SetText(text);
}

BOOL CRobloxDoc::IsModified()
{
	if (FUN_00403290()) {
		RBX::DataModel::Lock lock(FUN_00403290());
		return FUN_00403290()->isDirty();
	}

	return FALSE;
}

void CRobloxDoc::FUN_004561a0()
{
	CString filter;
	filter.LoadString(184);

	CFileDialog dialog(TRUE, "rbxm", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
	if (dialog.DoModal() == IDOK) {
		RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
		m_unk0x78->FUN_004702b0(RBX::ContentId::fromFile(static_cast<LPCTSTR>(dialog.GetPathName())), true);
	}
}

void CRobloxDoc::DeleteContents()
{
	{
		RBX::DataModel::Lock lock(FUN_00403290());
		std::vector<RBX::Verb*>::iterator it = m_unk0x64.begin();
		std::vector<RBX::Verb*>::iterator end = m_unk0x64.end();
		while (it != end) {
			delete *it;
			++it;
		}
		m_unk0x64.clear();
		m_unk0x74.reset();
	}

	if (m_unk0x78) {
		m_unk0x78->FUN_0046f420();
		m_unk0x78->Release();
		m_unk0x78 = NULL;
	}

	if (ActiveDocView::g_unk0x00952d8c == this) {
		ActiveDocView::FUN_004054f0(true, this);
	}

	CDocument::DeleteContents();
}

CRobloxDoc::~CRobloxDoc()
{
}

void CRobloxDoc::FUN_00455ff0(CCmdUI* pCmdUI)
{
	if (FUN_00403290()) {
		char text[32];
		{
			RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
			sprintf(text, "t %.4g", FUN_00403290()->getSimTime());
		}
		pCmdUI->SetText(text);
	}
}

void CRobloxDoc::FUN_00456c50(int id, RBX::Verb* verb)
{
	if (verb) {
		m_unk0x64.push_back(verb);
	}

	bind(id, verb);
}

CRobloxDoc::CRobloxDoc() : m_unk0x78(NULL)
{
}

void CRobloxDoc::FUN_00456d30()
{
	HRESULT hr = CComObject<CWorkspace>::CreateInstance(&m_unk0x78);
	if (hr == S_OK) {
	}
	else {
		throw std::runtime_error("Unable to create CWorkspace");
	}

	m_unk0x78->AddRef();

	boost::shared_ptr<RBX::DataModel> dataModel = m_unk0x78->FUN_0046edc0();
	RBX::DataModel::Lock lock(dataModel);

	dataModel->setName((LPCTSTR) GetTitle());

	m_unk0x74.reset(new RBX::Unknown005d2700(dataModel.get()));

	m_unk0x64.push_back(new ReportAbuseVerb(dataModel.get(), this));
	m_unk0x64.push_back(new ExitCommand(dataModel.get(), this));
	m_unk0x64.push_back(new RBX::FixedCameraCommand(m_unk0x78->FUN_0046edf0()));
	m_unk0x64.push_back(new RBX::CameraPanLeftCommand(m_unk0x78->FUN_0046edf0()));
	m_unk0x64.push_back(new RBX::CameraPanRightCommand(m_unk0x78->FUN_0046edf0()));
	m_unk0x64.push_back(new RBX::CameraTiltUpCommand(m_unk0x78->FUN_0046edf0()));
	m_unk0x64.push_back(new RBX::CameraTiltDownCommand(m_unk0x78->FUN_0046edf0()));
	m_unk0x64.push_back(new RBX::CameraZoomInCommand(m_unk0x78->FUN_0046edf0()));
	m_unk0x64.push_back(new RBX::CameraZoomOutCommand(m_unk0x78->FUN_0046edf0()));
	m_unk0x64.push_back(new RBX::CameraCenterCommand(m_unk0x78->FUN_0046edf0()));
	m_unk0x64.push_back(new RBX::CameraZoomExtentsCommand(m_unk0x78->FUN_0046edf0()));
	m_unk0x64.push_back(new RBX::CameraModelViewCommand(m_unk0x78->FUN_0046edf0()));

	bind(0xe120, &m_unk0x74->m_unk0x28);
	bind(0xe12a, &m_unk0x74->m_unk0x50);
	bind(0x8005, &m_unk0x74->m_unk0xfc);
	bind(0x8011, &m_unk0x74->m_unk0x26c);
	bind(0x8010, &m_unk0x74->m_unk0x248);
	bind(0x80d8, &m_unk0x74->m_unk0x43c);
	bind(0x80d4, &m_unk0x74->m_unk0x420);
	bind(0x80d5, &m_unk0x74->m_unk0x42c);
	bind(0x8039, RBX::Name::lookup("Group"));
	bind(0x803a, RBX::Name::lookup("UnGroup"));
	bind(0x8061, RBX::Name::lookup("WeldSelection"));
	bind(0x80a5, RBX::Name::lookup("SnapSelection"));
	bind(0x8043, RBX::Name::lookup("SelectionRotate"));
	bind(0x8044, RBX::Name::lookup("SelectionTilt"));
	bind(0x8057, RBX::Name::lookup("SelectionUpBrick"));
	bind(0x8059, RBX::Name::lookup("SelectionUpPlate"));
	bind(0x8058, RBX::Name::lookup("SelectionDown"));
	bind(0x80aa, &m_unk0x74->m_unk0x59c);
	bind(0x80ab, &m_unk0x74->m_unk0x5b8);
	FUN_00456c50(0xe123, new CutVerb(dataModel.get()));
	FUN_00456c50(0xe122, new CopyVerb(dataModel.get()));
	FUN_00456c50(0xe125, new PasteVerb(dataModel.get(), false));
	FUN_00456c50(0x80f7, new PasteVerb(dataModel.get(), true));
	FUN_00456c50(0x8062, new InsertDecal(dataModel.get(), "InsertDecal"));
	FUN_00456c50(0x80ee, new InsertTexture(dataModel.get()));
	FUN_00456c50(0x807f, new InsertObject(dataModel.get()));
	bind(0x800d, &m_unk0x74->m_unk0x38c);
	bind(0x800e, &m_unk0x74->m_unk0x39c);
	bind(0x8040, RBX::Name::lookup("FillTool"));
	bind(0x8041, RBX::Name::lookup("DropperTool"));
	bind(0x8063, &m_unk0x74->m_unk0x44c);
	bind(0x8065, &m_unk0x74->m_unk0x468);
	bind(0x8067, &m_unk0x74->m_unk0x484);
	bind(0x8073, &m_unk0x74->m_unk0x5f0);
	bind(0x8074, &m_unk0x74->m_unk0x5d4);
	bind(0x8077, &m_unk0x74->m_unk0x4bc);
	bind(0x8078, &m_unk0x74->m_unk0x4a0);
	bind(0x8079, &m_unk0x74->m_unk0x4d8);
	bind(0x80a7, &m_unk0x74->m_unk0x4f4);
	bind(0x80a8, &m_unk0x74->m_unk0x510);
	bind(0x807b, &m_unk0x74->m_unk0x548);
	bind(0x807c, &m_unk0x74->m_unk0x52c);
}

BOOL CRobloxDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	char cachePath[MAX_PATH];
	if (SHGetFolderPath(NULL, CSIDL_INTERNET_CACHE, NULL, 0, cachePath) == S_OK) {
		CString relativePath;
		LPTSTR buffer = relativePath.GetBuffer(MAX_PATH);
		BOOL relative = PathRelativePathTo(buffer, cachePath, FILE_ATTRIBUTE_DIRECTORY, lpszPathName, 0);
		relativePath.ReleaseBuffer();

		if (relative) {
			if (relativePath.Left(2).Compare(".\\") == 0) {
				return FALSE;
			}
		}
	}

	DeleteContents();
	FUN_00456d30();
	SetModifiedFlag(TRUE);

	try {
		std::auto_ptr<std::istream> stream =
			RBX::ContentProvider::singleton().getContent(RBX::ContentId::fromFile(lpszPathName));
		m_unk0x78->FUN_0046ef00(*stream);

		SetModifiedFlag(FALSE);
	}
	catch (std::exception& e) {
		FUN_0042b3c0(e);
		ReportSaveLoadException(lpszPathName, NULL, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}

	return TRUE;
}

BOOL CRobloxDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument()) {
		return FALSE;
	}

	try {
		FUN_00456d30();
	}
	catch (std::exception& e) {
		FUN_0042b3c0(e);
		AfxMessageBox("An error occured while creating the document", MB_ICONEXCLAMATION);
		return FALSE;
	}

	SetModifiedFlag(FALSE);
	return TRUE;
}

bool ReportAbuseVerb::isEnabled() const
{
	RBX::DataModel::Lock lock(m_unk0x0c->FUN_00403290());
	return m_unk0x0c->FUN_00403290()->create<RBX::Network::Players>()->canReportAbuse();
}

void CRobloxDoc::FUN_00453eb0(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CRobloxDoc::FUN_00458750(CCmdUI* pCmdUI)
{
	RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
	RBX::Selection* selection = createService<RBX::Selection>(m_unk0x78->FUN_0046edc0().get());
	pCmdUI->Enable(selection->size() > 0);
}

void CRobloxDoc::FUN_004588a0(NMHDR* pNMHDR, LRESULT* pResult)
{
	CXTPControlComboBox* pControl = (CXTPControlComboBox*) ((NMXTPCONTROL*) pNMHDR)->pControl;
	if (pControl->GetType() == xtpControlComboBox) {
		std::string text(pControl->GetEditText());

		if (pControl->FindString(-1, text.c_str()) == -1) {
			pControl->InsertString(0, text.c_str());
			if (pControl->GetCount() > 10) {
				pControl->DeleteItem(pControl->GetCount() - 1);
			}
		}

		pControl->SetEnabled(FALSE);
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_OUTPUT, "> %s", text.c_str());

		try {
			if (RBX::ContentProvider::isHttpUrl(text)) {
				std::auto_ptr<std::istream> stream = RBX::ContentProvider::singleton().getContent(RBX::ContentId(text));
				std::ostringstream script;
				boost::iostreams::copy(*stream, script, 0x1000);
				text = script.str();
			}

			RBX::DataModel::Lock lock(FUN_00403290());
			m_unk0x78->FUN_0046edc0()->create<RBX::ScriptContext>()->executeInNewThread(
				RBX::Security::CmdLine, text.c_str(), "Cmd"
			);
		}
		catch (std::exception&) {
		}

		pControl->SetEnabled(TRUE);
		pControl->GetEditCtrl()->SetSel(0, -1);
		pControl->GetEditCtrl()->SetFocus();
		*pResult = 1;
	}
}

void CRobloxDoc::FUN_00458c80(CCmdUI* pCmdUI)
{
	RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
	pCmdUI->Enable(FUN_00403290()->create<RBX::RunService>()->getRunState() == RBX::RS_RUNNING);
}

void CRobloxDoc::FUN_00458dc0(CCmdUI* pCmdUI)
{
	RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
	RBX::RunState runState = FUN_00403290()->create<RBX::RunService>()->getRunState();
	bool hasLocalPlayer = RBX::Network::Players::findLocalPlayer(FUN_00403290().get()) != NULL;
	pCmdUI->Enable(runState != RBX::RS_NORMAL && !hasLocalPlayer);
}

void CRobloxDoc::FUN_00458f60(CCmdUI* pCmdUI)
{
	RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
	RBX::RunService* runService = FUN_00403290()->create<RBX::RunService>();
	pCmdUI->Enable(!runService->runDisabled && !runService->isRunState());
}

void CRobloxDoc::FUN_004590a0(CCmdUI* pCmdUI)
{
	if (FUN_00403290()) {
		RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
		RBX::RunState runState = FUN_00403290()->create<RBX::RunService>()->getRunState();
		if (runState != RBX::RS_RUNNING) {
			pCmdUI->SetText("");
		}
		else {
			char text[32];
			sprintf(text, "%.3gfps", FUN_00403290()->timeState->simulationStopwatch.smoothFPS());
			pCmdUI->SetText(text);
		}
	}
}

void CRobloxDoc::FUN_004592c0(CCmdUI* pCmdUI)
{
	RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
	RBX::ScriptContext* scriptContext = findService<RBX::ScriptContext>(FUN_00403290().get());
	pCmdUI->Enable(scriptContext && scriptContext->hasState());
}

void CRobloxDoc::FUN_00459400()
{
	RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
	RBX::ScriptContext* scriptContext = findService<RBX::ScriptContext>(FUN_00403290().get());
	if (scriptContext && scriptContext->allocator) {
		scriptContext->gc();

		unsigned int heapSize;
		unsigned int heapCount;
		scriptContext->allocator->getHeapStats(heapSize, heapCount);

		std::string heap = RBX::Log::formatMem(heapSize);
		unsigned int threadCount = scriptContext->getThreadCount();
		RBX::StandardOut::singleton()->print(
			RBX::MESSAGE_INFO, "Script heap %s (%d blocks), %d threads", heap.c_str(), heapCount, threadCount
		);
	}
}

void CRobloxDoc::FUN_00459600(CCmdUI* pCmdUI)
{
	RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
	RBX::ScriptContext* scriptContext = findService<RBX::ScriptContext>(FUN_00403290().get());
	if (scriptContext && scriptContext->allocator) {
		char text[32];
		sprintf(text, "%d threads", scriptContext->getThreadCount());
		pCmdUI->SetText(text);
		pCmdUI->Enable(TRUE);
	}
	else {
		pCmdUI->SetText("");
		pCmdUI->Enable(FALSE);
	}
}

void CRobloxDoc::FUN_00459790(CCmdUI* pCmdUI)
{
	RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
	RBX::ScriptContext* scriptContext = findService<RBX::ScriptContext>(FUN_00403290().get());
	if (scriptContext && scriptContext->allocator) {
		unsigned int heapSize;
		unsigned int heapCount;
		scriptContext->allocator->getHeapStats(heapSize, heapCount);

		std::string heap = RBX::Log::formatMem(heapSize);
		pCmdUI->SetText(heap.c_str());
		pCmdUI->Enable(TRUE);
	}
	else {
		pCmdUI->SetText("");
		pCmdUI->Enable(FALSE);
	}
}

BOOL CRobloxDoc::FUN_00459950(std::string fileName)
{
	if (RBX::ContentProvider::isHttpUrl(fileName.c_str())) {
		fileName = RBX::ContentProvider::singleton().getFile(RBX::ContentId(fileName.c_str()));
	}

	CString script;
	{
		CFile file(fileName.c_str(), CFile::modeRead);
		char buffer[0x1000];
		UINT count;
		do {
			count = file.Read(buffer, sizeof(buffer) - 1);
			buffer[count] = 0;
			script += buffer;
		} while (count != 0);
	}

	RBX::DataModel::Lock lock(FUN_00403290());
	std::vector<RBX::Reflection::Value> arguments;
	try {
		FUN_00403290()->create<RBX::ScriptContext>()->executeInNewThread(
			RBX::Security::LocalGUI, script, fileName.c_str(), arguments
		);
		return TRUE;
	}
	catch (std::exception&) {
		return FALSE;
	}
}

void CRobloxDoc::FUN_00459c80()
{
	CString filter;
	filter.LoadString(208);

	CFileDialog dialog(TRUE, "lua", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
	if (dialog.DoModal() == IDOK) {
		FUN_00459950(static_cast<LPCTSTR>(dialog.GetPathName()));
	}
}

void CRobloxDoc::FUN_00459da0()
{
	CString filter;
	filter.LoadString(141);

	CFileDialog dialog(TRUE, "ldr", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
	if (dialog.DoModal() == IDOK) {
		RBX::LDraw2Lua::LDrawParser parser(dialog.GetPathName());
		std::ostringstream stream;
		parser.FUN_00473a80(stream);
		stream.flush();

		try {
			RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
			std::vector<RBX::Reflection::Value> arguments;
			std::string script = stream.str();
			FUN_00403290()->create<RBX::ScriptContext>()->executeInNewThread(
				RBX::Security::LocalGUI, script.c_str(), "LDraw", arguments
			);
		}
		catch (std::exception& e) {
			AfxMessageBox(e.what(), MB_ICONERROR);
		}
	}
}

void CRobloxDoc::FUN_0045a0b0(CCmdUI* pCmdUI)
{
	RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
	pCmdUI->Enable(FUN_00403290()->create<RBX::Network::Players>()->canReportAbuse() ? TRUE : FALSE);
}

BOOL CRobloxDoc::SaveModified()
{
	if (!m_unk0x78->FUN_0046edc0()) {
		return TRUE;
	}

	if (!IsModified()) {
		return TRUE;
	}

	RBX::DataModel::Lock lock(FUN_00403290());
	bool wasRunning = createService<RBX::RunService>(FUN_00403290().get())->isRunState();
	createService<RBX::RunService>(FUN_00403290().get())->pause();
	RBX::Visit* visit = findService<RBX::Visit>(FUN_00403290().get());

	BOOL result;
	if (visit == NULL || visit->getUploadUrl() == "NoVisit") {
		SetForegroundWindow(AfxGetMainWnd()->m_hWnd);
		result = CDocument::SaveModified();
	}
	else {
		if (visit->getUploadUrl().size() == 0) {
			return TRUE;
		}

		FlashWindow(AfxGetMainWnd()->m_hWnd, FALSE);

		CUploadPlaceDialog dialog(m_unk0x78, NULL);
		result = dialog.DoModal() == IDOK;
		if (!result && wasRunning) {
			createService<RBX::RunService>(m_unk0x78->FUN_0046edc0().get())->run();
		}
	}

	return result;
}

void CRobloxDoc::FUN_0045a600()
{
	RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
	createService<RBX::RunService>(FUN_00403290().get())->pause();
}

void CRobloxDoc::FUN_0045a730()
{
	RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
	if (FUN_00403290()->create<RBX::RunService>()->getRunState() != RBX::RS_NORMAL) {
		createService<RBX::RunService>(FUN_00403290().get())->reset();
		FUN_00403290()->clearContents();
		SerializerV2::load(m_unk0x60.get(), FUN_00403290().get());
		FUN_00403290()->getWorkspace()->joinAllHack();
		m_unk0x60.reset();
	}
}

void CRobloxDoc::FUN_0045a9e0()
{
	RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
	if (FUN_00403290()->create<RBX::RunService>()->getRunState() == RBX::RS_NORMAL) {
		m_unk0x60.reset(SerializerV2::newRootElement());
		FUN_00403290()->writeChildren(m_unk0x60.get());
	}

	createService<RBX::RunService>(FUN_00403290().get())->run();
}

// clang-format off
BEGIN_MESSAGE_MAP(CRobloxDoc, CDocument)
	ON_COMMAND(0x80b6, FUN_00454920)
	ON_UPDATE_COMMAND_UI(0x80b6, FUN_00453eb0)
	ON_COMMAND(0x80b7, FUN_004549c0)
	ON_UPDATE_COMMAND_UI(0x80b7, FUN_00458750)
	ON_NOTIFY(CBN_XTP_EXECUTE, 0x80c3, FUN_004588a0)
	ON_UPDATE_COMMAND_UI(0x80c3, FUN_00453eb0)
	ON_UPDATE_COMMAND_UI(0xbb, FUN_00455ff0)
	ON_UPDATE_COMMAND_UI(0xbe, FUN_004590a0)
	ON_COMMAND(0x80f1, FUN_00459c80)
	ON_UPDATE_COMMAND_UI(0x80f1, FUN_00453eb0)
	ON_COMMAND(0x80f8, FUN_00459da0)
	ON_UPDATE_COMMAND_UI(0x80f8, FUN_00453eb0)
	ON_COMMAND(0x8100, FUN_00454a60)
	ON_UPDATE_COMMAND_UI(0x8100, FUN_0045a0b0)
	ON_COMMAND(0x8104, FUN_00454e50)
	ON_UPDATE_COMMAND_UI(0x8104, FUN_00453eb0)
	ON_UPDATE_COMMAND_UI(0x8105, FUN_00453eb0)
	ON_COMMAND(0x8105, FUN_00454f50)
	ON_COMMAND(0x8106, FUN_004550b0)
	ON_UPDATE_COMMAND_UI(0x8106, FUN_00453eb0)
	ON_COMMAND(0x8021, FUN_004561a0)
	ON_COMMAND(0x8025, FUN_00454430)
	ON_UPDATE_COMMAND_UI(0x8025, FUN_00458750)
	ON_COMMAND(ID_EDIT_UNDO, FUN_00455900)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, FUN_00455a30)
	ON_COMMAND(ID_EDIT_REDO, FUN_00455bb0)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, FUN_00455ce0)
	ON_COMMAND(0x80c7, FUN_0045a600)
	ON_UPDATE_COMMAND_UI(0x80c7, FUN_00458c80)
	ON_COMMAND(0x80c8, FUN_0045a730)
	ON_UPDATE_COMMAND_UI(0x80c8, FUN_00458dc0)
	ON_COMMAND(0x80c6, FUN_0045a9e0)
	ON_UPDATE_COMMAND_UI(0x80c6, FUN_00458f60)
	ON_UPDATE_COMMAND_UI(0x80da, FUN_004592c0)
	ON_COMMAND(0x80da, FUN_00459400)
	ON_UPDATE_COMMAND_UI(0x80f6, FUN_00459600)
	ON_UPDATE_COMMAND_UI(0x80db, FUN_00459790)
END_MESSAGE_MAP()
// clang-format on

template const RBX::FactoryProduct<RBX::Visit, RBX::Instance, RBX::sVisit>::Creator
	RBX::FactoryProduct<RBX::Visit, RBX::Instance, RBX::sVisit>::creator;

template RBX::Soundscape::SoundService* RBX::ServiceProvider::create<RBX::Soundscape::SoundService>() const;
