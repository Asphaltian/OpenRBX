#include "RobloxDoc.h"

#include "InstanceExplorer.h"
#include "LogManager.h"
#include "MainFrm.h"
#include "SaveToRobloxDialog.h"
#include "network/Players.h"
#include "script/ScriptContext.h"
#include "util/ContentProvider.h"
#include "util/RunStateOwner.h"
#include "util/StandardOut.h"
#include "v8datamodel/Selection.h"
#include "v8datamodel/TimeState.h"

#include <boost/iostreams/copy.hpp>
#include <shlobj.h>
#include <shlwapi.h>
#include <sstream>

IMPLEMENT_DYNCREATE(CRobloxDoc, CDocument)

// FUNCTION: ROBLOX 0x0040ffc0
void CutVerb::doIt(RBX::IDataState* dataState)
{
	getContainer()->getVerb("Copy")->doIt(dataState);
	RBX::DeleteBase::doIt(dataState);
}

// FUNCTION: ROBLOX 0x00412120
bool PasteVerb::isEnabled() const
{
	static UINT format = RegisterClipboardFormat("Roblox");

	if (IsClipboardFormatAvailable(format) != TRUE) {
		return false;
	}

	if (!m_unk0x10) {
		return true;
	}

	return (m_unk0x0c ? m_unk0x0c->create<RBX::Selection>() : NULL)->size() == 1;
}

// STUB: ROBLOX 0x004121a0
void PasteVerb::doIt(RBX::IDataState* dataState)
{
	STUB(0x4121a0);
}

// STUB: ROBLOX 0x004125e0
void CopyVerb::doIt(RBX::IDataState* dataState)
{
	STUB(0x4125e0);
}

// FUNCTION: ROBLOX 0x00454190
CPublishAsPlaceDialog::CPublishAsPlaceDialog(IDispatch* external, CWnd* pParentWnd)
	: CSaveToRobloxDialog(external, FUN_00407190() + "/IDE/Upload.aspx", pParentWnd)
{
}

// FUNCTION: ROBLOX 0x00454250
CUploadPlaceDialog::CUploadPlaceDialog(IDispatch* external, CWnd* pParentWnd)
	: CSaveToRobloxDialog(external, FUN_00407190() + "/Game/Upload.aspx", pParentWnd)
{
}

// FUNCTION: ROBLOX 0x004542f0
CSaveModelToRobloxDialog::CSaveModelToRobloxDialog(IDispatch* external, CWnd* pParentWnd)
	: CSaveToRobloxDialog(external, FUN_00407190() + "/UI/Save.aspx?type=Model", pParentWnd)
{
}

// FUNCTION: ROBLOX 0x00454390
void ReportAbuseVerb::doIt(RBX::IDataState* dataState)
{
	AfxGetApp()->m_pMainWnd->PostMessage(WM_COMMAND, 0x8100);
}

// FUNCTION: ROBLOX 0x004543c0
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

// FUNCTION: ROBLOX 0x00454920
void CRobloxDoc::FUN_00454920()
{
	CPublishAsPlaceDialog dialog(m_unk0x78, NULL);
	dialog.DoModal();
}

// FUNCTION: ROBLOX 0x004549c0
void CRobloxDoc::FUN_004549c0()
{
	CSaveModelToRobloxDialog dialog(m_unk0x78, NULL);
	dialog.DoModal();
}

// FUNCTION: ROBLOX 0x00454a60
void CRobloxDoc::FUN_00454a60()
{
	CRobloxDHtmlDialog dialog(123, FUN_00407190() + "/AbuseReport/InGameChat.aspx", NULL);
	dialog.SetExternalDispatch(m_unk0x78);
	dialog.DoModal();
}

// FUNCTION: ROBLOX 0x004558a0
RBX::VerbContainer* CRobloxDoc::FUN_004558a0()
{
	return FUN_00403290()->getWorkspace();
}

// FUNCTION: ROBLOX 0x00456b50
CRobloxDoc::~CRobloxDoc()
{
}

// FUNCTION: ROBLOX 0x00455ff0
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

// STUB: ROBLOX 0x00456c00
void CRobloxDoc::FUN_00456c00(int id, RBX::Verb* verb)
{
	const RBX::Name* name = verb ? &verb->getName() : &RBX::Name::getNullName();
	m_unk0x58[id] = name;
}

// STUB: ROBLOX 0x00456c50
void CRobloxDoc::FUN_00456c50(int id, RBX::Verb* verb)
{
	const RBX::Name* name;
	if (verb) {
		m_unk0x64.push_back(verb);
		name = &verb->getName();
	}
	else {
		name = &RBX::Name::getNullName();
	}
	m_unk0x58[id] = name;
}

// FUNCTION: ROBLOX 0x00456cb0
CRobloxDoc::CRobloxDoc() : m_unk0x78(NULL)
{
}

// STUB: ROBLOX 0x00456d30
void CRobloxDoc::FUN_00456d30()
{
	if (CComObject<CWorkspace>::CreateInstance(&m_unk0x78) != S_OK) {
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

	FUN_00456c00(0xe120, &m_unk0x74->m_unk0x28);
	FUN_00456c00(0xe12a, &m_unk0x74->m_unk0x50);
	FUN_00456c00(0x8005, &m_unk0x74->m_unk0xfc);
	FUN_00456c00(0x8011, &m_unk0x74->m_unk0x26c);
	FUN_00456c00(0x8010, &m_unk0x74->m_unk0x248);
	FUN_00456c00(0x80d8, &m_unk0x74->m_unk0x43c);
	FUN_00456c00(0x80d4, &m_unk0x74->m_unk0x420);
	FUN_00456c00(0x80d5, &m_unk0x74->m_unk0x42c);
	m_unk0x58[0x8039] = &RBX::Name::lookup("Group");
	m_unk0x58[0x803a] = &RBX::Name::lookup("UnGroup");
	m_unk0x58[0x8061] = &RBX::Name::lookup("WeldSelection");
	m_unk0x58[0x80a5] = &RBX::Name::lookup("SnapSelection");
	m_unk0x58[0x8043] = &RBX::Name::lookup("SelectionRotate");
	m_unk0x58[0x8044] = &RBX::Name::lookup("SelectionTilt");
	m_unk0x58[0x8057] = &RBX::Name::lookup("SelectionUpBrick");
	m_unk0x58[0x8059] = &RBX::Name::lookup("SelectionUpPlate");
	m_unk0x58[0x8058] = &RBX::Name::lookup("SelectionDown");
	FUN_00456c00(0x80aa, &m_unk0x74->m_unk0x59c);
	FUN_00456c00(0x80ab, &m_unk0x74->m_unk0x5b8);
	FUN_00456c50(0xe123, new CutVerb(dataModel.get()));
	FUN_00456c50(0xe122, new CopyVerb(dataModel.get()));
	FUN_00456c50(0xe125, new PasteVerb(dataModel.get(), false));
	FUN_00456c50(0x80f7, new PasteVerb(dataModel.get(), true));
	FUN_00456c50(0x8062, new InsertDecal(dataModel.get(), "InsertDecal"));
	FUN_00456c50(0x80ee, new InsertTexture(dataModel.get()));
	FUN_00456c50(0x807f, new InsertObject(dataModel.get()));
	FUN_00456c00(0x800d, &m_unk0x74->m_unk0x38c);
	FUN_00456c00(0x800e, &m_unk0x74->m_unk0x39c);
	m_unk0x58[0x8040] = &RBX::Name::lookup("FillTool");
	m_unk0x58[0x8041] = &RBX::Name::lookup("DropperTool");
	FUN_00456c00(0x8063, &m_unk0x74->m_unk0x44c);
	FUN_00456c00(0x8065, &m_unk0x74->m_unk0x468);
	FUN_00456c00(0x8067, &m_unk0x74->m_unk0x484);
	FUN_00456c00(0x8073, &m_unk0x74->m_unk0x5f0);
	FUN_00456c00(0x8074, &m_unk0x74->m_unk0x5d4);
	FUN_00456c00(0x8077, &m_unk0x74->m_unk0x4bc);
	FUN_00456c00(0x8078, &m_unk0x74->m_unk0x4a0);
	FUN_00456c00(0x8079, &m_unk0x74->m_unk0x4d8);
	FUN_00456c00(0x80a7, &m_unk0x74->m_unk0x4f4);
	FUN_00456c00(0x80a8, &m_unk0x74->m_unk0x510);
	FUN_00456c00(0x807b, &m_unk0x74->m_unk0x548);
	FUN_00456c00(0x807c, &m_unk0x74->m_unk0x52c);
}

// STUB: ROBLOX 0x00457910
BOOL CRobloxDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	char cachePath[MAX_PATH];
	if (SHGetFolderPath(NULL, CSIDL_INTERNET_CACHE, NULL, 0, cachePath) == S_OK) {
		CString relativePath;
		BOOL relative =
			PathRelativePathTo(relativePath.GetBuffer(MAX_PATH), cachePath, FILE_ATTRIBUTE_DIRECTORY, lpszPathName, 0);
		relativePath.ReleaseBuffer();

		if (relative && relativePath.Left(2).Compare(".\\") == 0) {
			return FALSE;
		}
	}

	try {
		DeleteContents();
		FUN_00456d30();
		SetModifiedFlag(TRUE);

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

// FUNCTION: ROBLOX 0x00457e30
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

// FUNCTION: ROBLOX 0x00458620
bool ReportAbuseVerb::isEnabled() const
{
	RBX::DataModel::Lock lock(m_unk0x0c->FUN_00403290());
	return m_unk0x0c->FUN_00403290()->create<RBX::Network::Players>()->canReportAbuse();
}

// FUNCTION: ROBLOX 0x00453eb0 FOLDED
void CRobloxDoc::FUN_00453eb0(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

// STUB: ROBLOX 0x004588a0
void CRobloxDoc::FUN_004588a0(NMHDR* pNMHDR, LRESULT* pResult)
{
	CXTPControlComboBox* pControl = (CXTPControlComboBox*) ((NMXTPCONTROL*) pNMHDR)->pControl;
	if (pControl->GetType() == xtpControlComboBox) {
		std::string text(pControl->GetEditText());

		try {
			if (pControl->FindStringExact(-1, text.c_str()) == -1) {
				pControl->InsertString(0, text.c_str());
				if (pControl->GetCount() > 10) {
					pControl->DeleteItem(pControl->GetCount() - 1);
				}
			}

			pControl->SetEnabled(FALSE);
			RBX::StandardOut::singleton()->print(RBX::MESSAGE_OUTPUT, "> %s", text.c_str());

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
		catch (...) {
		}

		pControl->SetEnabled(TRUE);
		pControl->GetEditCtrl()->SetSel(0, -1);
		pControl->GetEditCtrl()->SetFocus();
		*pResult = 1;
	}
}

// STUB: ROBLOX 0x004590a0
void CRobloxDoc::FUN_004590a0(CCmdUI* pCmdUI)
{
	if (FUN_00403290()) {
		RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
		if (FUN_00403290()->create<RBX::RunService>()->getRunState() == RBX::RS_RUNNING) {
			char text[32];
			sprintf(text, "%.3gfps", FUN_00403290()->timeState->simulationStopwatch.smoothFPS());
			pCmdUI->SetText(text);
		}
		else {
			pCmdUI->SetText("");
		}
	}
}

// STUB: ROBLOX 0x00459950
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
	}
	catch (...) {
		return FALSE;
	}

	return TRUE;
}

// STUB: ROBLOX 0x00459c80
void CRobloxDoc::FUN_00459c80()
{
	CString filter;
	filter.LoadString(208);

	CFileDialog dialog(TRUE, "lua", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
	if (dialog.DoModal() == IDOK) {
		FUN_00459950(static_cast<LPCTSTR>(dialog.GetPathName()));
	}
}

// FUNCTION: ROBLOX 0x0045a0b0
void CRobloxDoc::FUN_0045a0b0(CCmdUI* pCmdUI)
{
	RBX::DataModel::Lock lock(m_unk0x78->FUN_0046edc0());
	pCmdUI->Enable(FUN_00403290()->create<RBX::Network::Players>()->canReportAbuse() ? TRUE : FALSE);
}

// clang-format off
BEGIN_MESSAGE_MAP(CRobloxDoc, CDocument)
	ON_COMMAND(0x80b6, FUN_00454920)
	ON_UPDATE_COMMAND_UI(0x80b6, FUN_00453eb0)
	ON_COMMAND(0x80b7, FUN_004549c0)
	ON_NOTIFY(CBN_XTP_EXECUTE, 0x80c3, FUN_004588a0)
	ON_UPDATE_COMMAND_UI(0x80c3, FUN_00453eb0)
	ON_UPDATE_COMMAND_UI(0xbb, FUN_00455ff0)
	ON_UPDATE_COMMAND_UI(0xbe, FUN_004590a0)
	ON_COMMAND(0x80f1, FUN_00459c80)
	ON_UPDATE_COMMAND_UI(0x80f1, FUN_00453eb0)
	ON_COMMAND(0x8100, FUN_00454a60)
	ON_UPDATE_COMMAND_UI(0x8100, FUN_0045a0b0)
END_MESSAGE_MAP()
// clang-format on
