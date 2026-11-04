#include "ObjectBrowser.h"

#include "ClassTreeView.h"
#include "DeclarationView.h"
#include "MemberTreeView.h"
#include "StandardOutputView.h"

IMPLEMENT_DYNCREATE(CClassTreeView, CXTTreeView)

BEGIN_MESSAGE_MAP(CClassTreeView, CXTTreeView)
ON_WM_CREATE()
ON_NOTIFY_REFLECT(TVN_SELCHANGED, FUN_0043a280)
END_MESSAGE_MAP()

// STUB: ROBLOX 0x00437590
CClassTreeView::~CClassTreeView()
{
}

IMPLEMENT_DYNCREATE(CObjectBrowser, CView)

BEGIN_MESSAGE_MAP(CObjectBrowser, CView)
ON_WM_SIZE()
ON_WM_CREATE()
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(CMemberTreeView, CXTTreeView)

BEGIN_MESSAGE_MAP(CMemberTreeView, CXTTreeView)
ON_WM_CREATE()
ON_NOTIFY_REFLECT(TVN_SELCHANGED, FUN_0043a240)
END_MESSAGE_MAP()

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
CClassTreeView::CClassTreeView() : m_unk0xa8(NULL)
{
}

// STUB: ROBLOX 0x00437c50
void CObjectBrowser::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	if (nType != SIZE_MINIMIZED && m_unk0x58.m_hWnd) {
		CRect rect;
		m_unk0x58.GetClientRect(&rect);
		int height = rect.Height();
		int delta = cy - height;
		m_unk0x58.MoveWindow(0, 0, cx, cy);
		if (m_unk0x170.m_hWnd) {
			int cyCur;
			int cyMin;
			m_unk0x170.GetRowInfo(0, cyCur, cyMin);
			int top = std::min<int>(cy - 150, cyCur + delta);
			m_unk0x170.SetRowInfo(0, std::max<int>(50, top), 0);
			m_unk0x170.RecalcLayout();
		}
	}
}

// FUNCTION: ROBLOX 0x00437e10
int CObjectBrowser::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}
	if (!m_unk0x58.CreateStatic(this, 1, 2)) {
		return -1;
	}
	m_unk0x58.SetSplitterStyle(XT_SPLIT_NOBORDER);

	CCreateContext context;
	context.m_pCurrentDoc = GetDocument();
	if (!m_unk0x58.CreateView(0, 0, RUNTIME_CLASS(CClassTreeView), CSize(200, 600), &context)) {
		return -1;
	}
	if (!m_unk0x170.CreateStatic(&m_unk0x58, 2, 1, WS_CHILD | WS_VISIBLE | WS_BORDER, m_unk0x58.IdFromRowCol(0, 1))) {
		return -1;
	}
	if (!m_unk0x170.CreateView(0, 0, RUNTIME_CLASS(CMemberTreeView), CSize(300, 200), &context)) {
		return -1;
	}
	if (!m_unk0x170.CreateView(1, 0, RUNTIME_CLASS(CDeclarationView), CSize(100, 190), &context)) {
		return -1;
	}

	CClassTreeView* classes = static_cast<CClassTreeView*>(m_unk0x58.GetPane(0, 0));
	CMemberTreeView* members = static_cast<CMemberTreeView*>(m_unk0x170.GetPane(0, 0));
	CDeclarationView* declaration = static_cast<CDeclarationView*>(m_unk0x170.GetPane(1, 0));
	classes->m_unk0xa8 = members;
	classes->m_unk0xac = declaration;
	members->m_unk0xb0 = declaration;
	return 0;
}

// STUB: ROBLOX 0x00438290
int CMemberTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	STUB(0x438290);
	return 0;
}

int CClassTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	STUB(0x438290);
	return 0;
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

// STUB: ROBLOX 0x0043a240
void CMemberTreeView::FUN_0043a240(NMHDR* pNMHDR, LRESULT* pResult)
{
	STUB(0x43a240);
}

// STUB: ROBLOX 0x0043a280
void CClassTreeView::FUN_0043a280(NMHDR* pNMHDR, LRESULT* pResult)
{
	STUB(0x43a280);
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
