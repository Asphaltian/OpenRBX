#include "ObjectBrowser.h"

#include "ClassTreeView.h"
#include "DeclarationView.h"
#include "MemberTreeView.h"
#include "StandardOutputView.h"

IMPLEMENT_DYNCREATE(CClassTreeView, CXTTreeView)

// STUB: ROBLOX 0x00437590
CClassTreeView::~CClassTreeView()
{
}

IMPLEMENT_DYNCREATE(CObjectBrowser, CView)
IMPLEMENT_DYNCREATE(CMemberTreeView, CXTTreeView)

// FUNCTION: ROBLOX 0x00437620
CObjectBrowser::CObjectBrowser()
{
}

// FUNCTION: ROBLOX 0x00437690
CObjectBrowser::~CObjectBrowser()
{
}

// FUNCTION: ROBLOX 0x00437860
CMemberTreeView::CMemberTreeView() : m_unk0xa8(1), m_unk0xac(0)
{
}

// STUB: ROBLOX 0x004378e0
CMemberTreeView::~CMemberTreeView()
{
}

// FUNCTION: ROBLOX 0x00437b30
CClassTreeView::CClassTreeView() : m_unk0xa8(0)
{
}

IMPLEMENT_DYNCREATE(CDeclarationView, CHtmlView)

BEGIN_MESSAGE_MAP(CDeclarationView, CHtmlView)
END_MESSAGE_MAP()

// FUNCTION: ROBLOX 0x00438b20
CDeclarationView::CDeclarationView()
{
}

// FUNCTION: ROBLOX 0x00438b70
void CDeclarationView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();
	LoadFromResource(210);
}

// FUNCTION: ROBLOX 0x00438b90
CDeclarationView::~CDeclarationView()
{
}

void CObjectBrowser::OnDraw(CDC* pDC)
{
}

IMPLEMENT_DYNAMIC(COutputView, CSecureHtmlView)
IMPLEMENT_DYNCREATE(CStandardOutputView, COutputView)

BEGIN_MESSAGE_MAP(CStandardOutputView, COutputView)
ON_MESSAGE(WM_USER + 101, FUN_0043b8a0)
END_MESSAGE_MAP()

// FUNCTION: ROBLOX 0x0043ae20
CStandardOutputView::~CStandardOutputView()
{
	boost::mutex::scoped_lock lock(RBX::StandardOut::singleton()->sync);
	RBX::stopListening<RBX::StandardOut, RBX::StandardOutMessage>(RBX::StandardOut::singleton(), this);
}

// FUNCTION: ROBLOX 0x0043b210
void CStandardOutputView::onEvent(const RBX::StandardOut* source, RBX::StandardOutMessage message)
{
	boost::mutex::scoped_lock lock(m_unk0x128);
	if (m_unk0x114.empty()) {
		PostMessage(WM_USER + 101, 0, 0);
	}
	m_unk0x114.push_back(message);
	if (m_unk0x114.size() > 300) {
		m_unk0x114.pop_front();
	}
}

// FUNCTION: ROBLOX 0x0043b720
CStandardOutputView::CStandardOutputView()
{
	m_unk0xf0 = 1;
	boost::mutex::scoped_lock lock(RBX::StandardOut::singleton()->sync);
	RBX::startListening<RBX::StandardOut, RBX::StandardOutMessage>(RBX::StandardOut::singleton(), this);
}

// STUB: ROBLOX 0x0043b8a0
LRESULT CStandardOutputView::FUN_0043b8a0(WPARAM wParam, LPARAM lParam)
{
	STUB(0x43b8a0);
	return 0;
}
