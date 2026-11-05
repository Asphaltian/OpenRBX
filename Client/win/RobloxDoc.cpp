#include "RobloxDoc.h"

#include "MainFrm.h"
#include "network/Players.h"

IMPLEMENT_DYNCREATE(CRobloxDoc, CDocument)

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

// FUNCTION: ROBLOX 0x00456cb0
CRobloxDoc::CRobloxDoc() : m_unk0x74(0), m_unk0x78(NULL)
{
}

// FUNCTION: ROBLOX 0x00458620
bool ReportAbuseVerb::isEnabled() const
{
	RBX::DataModel::Lock lock(m_unk0x0c->FUN_00403290());
	return m_unk0x0c->FUN_00403290()->create<RBX::Network::Players>()->canReportAbuse();
}
