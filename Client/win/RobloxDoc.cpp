#include "RobloxDoc.h"

#include "MainFrm.h"
#include "network/Players.h"
#include "v8datamodel/Selection.h"

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

// STUB: ROBLOX 0x004558a0
RBX::VerbContainer* CRobloxDoc::FUN_004558a0()
{
	STUB(0x4558a0);
	return NULL;
}

CRobloxDoc::~CRobloxDoc()
{
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

// FUNCTION: ROBLOX 0x00458620
bool ReportAbuseVerb::isEnabled() const
{
	RBX::DataModel::Lock lock(m_unk0x0c->FUN_00403290());
	return m_unk0x0c->FUN_00403290()->create<RBX::Network::Players>()->canReportAbuse();
}

// STUB: ROBLOX 0x00459950
BOOL CRobloxDoc::FUN_00459950(std::string p_unk0x04)
{
	STUB(0x459950);
	return FALSE;
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
