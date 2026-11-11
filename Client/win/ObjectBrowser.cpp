#include "ObjectBrowser.h"

#include "ClassTreeView.h"
#include "DeclarationView.h"
#include "MemberTreeView.h"
#include "ReflectionMetadata.h"
#include "StandardOutputView.h"
#include "reflection/function.h"
#include "reflection/property.h"
#include "reflection/signal.h"

#include <boost/bind.hpp>
#include <boost/thread/recursive_mutex.hpp>

inline std::vector<RBX::Reflection::ClassDescriptor*>::const_iterator RBX::Reflection::ClassDescriptor::
	derivedClasses_begin() const
{
	return derivedClasses.begin();
}

inline std::vector<RBX::Reflection::ClassDescriptor*>::const_iterator RBX::Reflection::ClassDescriptor::
	derivedClasses_end() const
{
	return derivedClasses.end();
}

IMPLEMENT_DYNCREATE(CClassTreeView, CXTTreeView)

BEGIN_MESSAGE_MAP(CClassTreeView, CXTTreeView)
ON_WM_CREATE()
ON_NOTIFY_REFLECT(TVN_SELCHANGED, FUN_0043a280)
END_MESSAGE_MAP()

// FUNCTION: ROBLOX 0x00437590
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

// FUNCTION: ROBLOX 0x00437700
static int FUN_00437700(const RBX::Reflection::Descriptor* descriptor)
{
	if (dynamic_cast<const RBX::Reflection::EnumDescriptor::Item*>(descriptor)) {
		return 5;
	}

	if (dynamic_cast<const RBX::Reflection::EnumDescriptor*>(descriptor)) {
		return 4;
	}

	if (dynamic_cast<const RBX::Reflection::SignalDescriptor*>(descriptor)) {
		return 3;
	}

	if (dynamic_cast<const RBX::Reflection::PropertyDescriptor*>(descriptor)) {
		return 2;
	}

	if (dynamic_cast<const RBX::Reflection::FunctionDescriptor*>(descriptor)) {
		return 1;
	}

	return dynamic_cast<const RBX::Reflection::ClassDescriptor*>(descriptor) ? 0 : 100;
}

// FUNCTION: ROBLOX 0x004377d0
static int CALLBACK FUN_004377d0(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	const RBX::Reflection::Descriptor* descriptor1 = reinterpret_cast<const RBX::Reflection::Descriptor*>(lParam1);
	const RBX::Reflection::Descriptor* descriptor2 = reinterpret_cast<const RBX::Reflection::Descriptor*>(lParam2);

	int rank1 = FUN_00437700(descriptor1);
	int rank2 = FUN_00437700(descriptor2);
	if (rank1 != rank2) {
		return rank1 - rank2;
	}

	return strcmp(descriptor1->name.c_str(), descriptor2->name.c_str());
}

// FUNCTION: ROBLOX 0x00437860
CMemberTreeView::CMemberTreeView() : m_unk0xa8(1), m_unk0xac(NULL)
{
}

// FUNCTION: ROBLOX 0x004378e0
CMemberTreeView::~CMemberTreeView()
{
}

// FUNCTION: ROBLOX 0x00437ab0
static boost::recursive_mutex& FUN_00437ab0()
{
	static boost::recursive_mutex g_unk0x0095395c;

	return g_unk0x0095395c;
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

// FUNCTION: ROBLOX 0x00437d30
static BOOL FUN_00437d30(CImageList& imageList, UINT nIDResource)
{
	if (!imageList.Create(16, 16, ILC_COLOR24 | ILC_MASK, 0, 1)) {
		return FALSE;
	}

	CBitmap bitmap;
	bitmap.LoadBitmap(nIDResource);
	imageList.Add(&bitmap, CXTPImageManager::GetBitmapMaskColor(bitmap, CPoint(0, 0)));
	return TRUE;
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

// FUNCTION: ROBLOX 0x00437f90
void CClassTreeView::FUN_00437f90(const RBX::Reflection::EnumDescriptor* descriptor)
{
	CString text;
	text.Format("%s", descriptor->name.c_str());

	HTREEITEM item = GetTreeCtrl().InsertItem(text, 9, 9);
	GetTreeCtrl().SetItemData(item, reinterpret_cast<DWORD_PTR>(descriptor));
}

// FUNCTION: ROBLOX 0x004380e0
void CMemberTreeView::FUN_004380e0(const RBX::Reflection::EnumDescriptor::Item* item)
{
	HTREEITEM treeItem = GetTreeCtrl().InsertItem(item->name.c_str(), 10, 10);
	GetTreeCtrl().SetItemData(treeItem, reinterpret_cast<DWORD_PTR>(item));
}

// FUNCTION: ROBLOX 0x00438130 FOLDED
void CMemberTreeView::FUN_00438130(HTREEITEM item, RBX::Reflection::Metadata::Item* metadata)
{
	if (metadata) {
		if (metadata->m_unk0xf9) {
			CXTLogFont logFont;
			if (GetFont()->GetLogFont(&logFont)) {
				logFont.lfStrikeOut = TRUE;
				SetItemFont(item, logFont);
			}
		}

		if (metadata->m_unk0xfa) {
			CXTLogFont logFont;
			if (GetFont()->GetLogFont(&logFont)) {
				logFont.lfItalic = TRUE;
				SetItemFont(item, logFont);
			}
		}
	}
}

// FUNCTION: ROBLOX 0x00438130 FOLDED
void CClassTreeView::FUN_00438130(HTREEITEM item, RBX::Reflection::Metadata::Item* metadata)
{
	if (metadata) {
		if (metadata->m_unk0xf9) {
			CXTLogFont logFont;
			if (GetFont()->GetLogFont(&logFont)) {
				logFont.lfStrikeOut = TRUE;
				SetItemFont(item, logFont);
			}
		}

		if (metadata->m_unk0xfa) {
			CXTLogFont logFont;
			if (GetFont()->GetLogFont(&logFont)) {
				logFont.lfItalic = TRUE;
				SetItemFont(item, logFont);
			}
		}
	}
}

// FUNCTION: ROBLOX 0x00438290
int CMemberTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTTreeView::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}
	if (!FUN_00437d30(m_unk0xa0, 196)) {
		return -1;
	}
	GetTreeCtrl().SetImageList(&m_unk0xa0, TVSIL_NORMAL);
	return 0;
}

// FUNCTION: ROBLOX 0x00438290 FOLDED
int CClassTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTTreeView::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}
	if (!FUN_00437d30(m_unk0xa0, 196)) {
		return -1;
	}
	GetTreeCtrl().SetImageList(&m_unk0xa0, TVSIL_NORMAL);
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

// FUNCTION: ROBLOX 0x00439690
void CClassTreeView::FUN_00439690(const RBX::Reflection::ClassDescriptor* descriptor)
{
	RBX::Reflection::Metadata::Class* metadata =
		RBX::Reflection::Metadata::Reflection::FUN_004480c0()->FUN_00446220(descriptor, false);
	if (!metadata || metadata->m_unk0xf8) {
		HTREEITEM item = GetTreeCtrl().InsertItem(descriptor->name.c_str(), 3, 3);
		GetTreeCtrl().SetItemData(item, reinterpret_cast<DWORD_PTR>(descriptor));
		FUN_00438130(item, metadata);
	}

	std::for_each(
		descriptor->derivedClasses_begin(),
		descriptor->derivedClasses_end(),
		boost::bind(&CClassTreeView::FUN_00439690, this, _1)
	);
}

// FUNCTION: ROBLOX 0x004397f0
void CMemberTreeView::FUN_004397f0(const RBX::Reflection::EnumDescriptor* descriptor)
{
	GetTreeCtrl().DeleteAllItems();
	m_unk0xac = descriptor;

	boost::recursive_mutex::scoped_lock lock(FUN_00437ab0());
	std::for_each(descriptor->begin(), descriptor->end(), boost::bind(&CMemberTreeView::FUN_004380e0, this, _1));

	TVSORTCB sort;
	sort.hParent = TVI_ROOT;
	sort.lpfnCompare = FUN_004377d0;
	GetTreeCtrl().SortChildrenCB(&sort);
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

// FUNCTION: ROBLOX 0x0043a320
void CClassTreeView::OnInitialUpdate()
{
	CXTTreeView::OnInitialUpdate();

	boost::recursive_mutex::scoped_lock lock(FUN_00437ab0());
	FUN_00439690(&RBX::Reflection::ClassDescriptor::rootDescriptor());
	std::for_each(
		RBX::Reflection::EnumDescriptor::enumsBegin(),
		RBX::Reflection::EnumDescriptor::enumsEnd(),
		boost::bind(&CClassTreeView::FUN_00437f90, this, _1)
	);

	TVSORTCB sort;
	sort.hParent = TVI_ROOT;
	sort.lpfnCompare = FUN_004377d0;
	GetTreeCtrl().SortChildrenCB(&sort);
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
