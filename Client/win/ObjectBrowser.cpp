#include "ObjectBrowser.h"

#include "ClassTreeView.h"
#include "DeclarationView.h"
#include "MemberTreeView.h"
#include "ReflectionMetadata.h"
#include "StandardOutputView.h"
#include "reflection/function.h"
#include "reflection/property.h"
#include "reflection/signal.h"

#include <atlenc.h>
#include <boost/bind.hpp>
#include <boost/thread/recursive_mutex.hpp>

inline const RBX::Reflection::SignatureDescriptor& RBX::Reflection::FunctionDescriptor::getSignature() const
{
	return signature;
}

inline const RBX::Reflection::SignatureDescriptor& RBX::Reflection::SignalDescriptor::getSignature() const
{
	return signature;
}

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

CObjectBrowser::CObjectBrowser()
{
}

CObjectBrowser::~CObjectBrowser()
{
}

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

CMemberTreeView::CMemberTreeView() : m_unk0xa8(true), m_unk0xac(NULL)
{
}

CMemberTreeView::~CMemberTreeView()
{
}

void CMemberTreeView::OnInitialUpdate()
{
	CXTTreeView::OnInitialUpdate();
}

void CObjectBrowser::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}

BOOL CObjectBrowser::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs) != FALSE;
}

static boost::recursive_mutex& FUN_00437ab0()
{
	static boost::recursive_mutex g_unk0x0095395c;

	return g_unk0x0095395c;
}

CClassTreeView::CClassTreeView() : m_unk0xa8(NULL)
{
}

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

void CClassTreeView::FUN_00437f90(const RBX::Reflection::EnumDescriptor* descriptor)
{
	CString text;
	text.Format("%s", descriptor->name.c_str());

	HTREEITEM item = GetTreeCtrl().InsertItem(text, 9, 9);
	GetTreeCtrl().SetItemData(item, reinterpret_cast<DWORD_PTR>(descriptor));
}

void CMemberTreeView::FUN_004380e0(const RBX::Reflection::EnumDescriptor::Item* item)
{
	HTREEITEM treeItem = GetTreeCtrl().InsertItem(item->name.c_str(), 10, 10);
	GetTreeCtrl().SetItemData(treeItem, reinterpret_cast<DWORD_PTR>(item));
}

void CMemberTreeView::FUN_00438130(HTREEITEM item, RBX::Reflection::Metadata::Item* metadata)
{
	if (metadata) {
		if (metadata->deprecated) {
			CXTLogFont logFont;
			if (GetFont()->GetLogFont(&logFont)) {
				logFont.lfStrikeOut = TRUE;
				SetItemFont(item, logFont);
			}
		}

		if (metadata->preliminary) {
			CXTLogFont logFont;
			if (GetFont()->GetLogFont(&logFont)) {
				logFont.lfItalic = TRUE;
				SetItemFont(item, logFont);
			}
		}
	}
}

void CClassTreeView::FUN_00438130(HTREEITEM item, RBX::Reflection::Metadata::Item* metadata)
{
	if (metadata) {
		if (metadata->deprecated) {
			CXTLogFont logFont;
			if (GetFont()->GetLogFont(&logFont)) {
				logFont.lfStrikeOut = TRUE;
				SetItemFont(item, logFont);
			}
		}

		if (metadata->preliminary) {
			CXTLogFont logFont;
			if (GetFont()->GetLogFont(&logFont)) {
				logFont.lfItalic = TRUE;
				SetItemFont(item, logFont);
			}
		}
	}
}

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

void CMemberTreeView::FUN_00438300(const RBX::Reflection::PropertyDescriptor* descriptor)
{
	if (m_unk0xa8 || &descriptor->owner == m_unk0xac) {
		RBX::Reflection::Metadata::Member* metadata =
			RBX::Reflection::Metadata::Reflection::FUN_004480c0()->FUN_00446920(descriptor);
		if ((!metadata || metadata->browsable) && descriptor->isPublic()) {
			HTREEITEM item = GetTreeCtrl().InsertItem(descriptor->name.c_str(), 6, 6);
			GetTreeCtrl().SetItemData(item, reinterpret_cast<DWORD_PTR>(descriptor));
			FUN_00438130(item, metadata);
		}
	}
}

void CMemberTreeView::FUN_00438420(const RBX::Reflection::SignalDescriptor* descriptor)
{
	if (m_unk0xa8 || &descriptor->owner == m_unk0xac) {
		RBX::Reflection::Metadata::Member* metadata =
			RBX::Reflection::Metadata::Reflection::FUN_004480c0()->FUN_004469c0(descriptor);
		if (!metadata || metadata->browsable) {
			HTREEITEM item = GetTreeCtrl().InsertItem(descriptor->name.c_str(), 8, 8);
			GetTreeCtrl().SetItemData(item, reinterpret_cast<DWORD_PTR>(descriptor));
			FUN_00438130(item, metadata);
		}
	}
}

void CDeclarationView::FUN_00438530(
	RBX::Reflection::Metadata::Item* metadata, const char* declaration, const char* owner
)
{
	if (m_unk0xf4) {
		m_unk0xf4->put_innerHTML(CComBSTR(declaration ? declaration : ""));
	}

	if (m_unk0xf8) {
		CComPtr<IHTMLStyle> style;
		m_unk0xf8->get_style(&style);
		style->put_display(owner ? CComBSTR("block") : CComBSTR("none"));
		m_unk0xfc->put_innerHTML(CComBSTR(owner));
	}

	if (m_unk0x100) {
		CString summary(metadata ? metadata->summary.c_str() : "");
		CComPtr<IHTMLStyle> style;
		m_unk0x100->get_style(&style);
		style->put_display(!summary.IsEmpty() ? CComBSTR("block") : CComBSTR("none"));
		if (!summary.IsEmpty()) {
			m_unk0x104->put_innerHTML(CComBSTR(summary));
		}
	}

	if (m_unk0x108) {
		CComPtr<IHTMLStyle> style;
		m_unk0x108->get_style(&style);
		style->put_display(metadata && metadata->deprecated ? CComBSTR("block") : CComBSTR("none"));
	}

	if (m_unk0x10c) {
		CComPtr<IHTMLStyle> style;
		m_unk0x10c->get_style(&style);
		style->put_display(metadata && metadata->preliminary ? CComBSTR("block") : CComBSTR("none"));
	}
}

IMPLEMENT_DYNCREATE(CDeclarationView, CHtmlView)

BEGIN_MESSAGE_MAP(CDeclarationView, CHtmlView)
END_MESSAGE_MAP()

CDeclarationView::CDeclarationView()
{
}

void CDeclarationView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();
	LoadFromResource(210);
}

CDeclarationView::~CDeclarationView()
{
}

void CDeclarationView::OnDocumentComplete(LPCTSTR lpszURL)
{
	m_unk0xf0 = GetHtmlDocument();
	if (m_unk0xf0) {
		CComPtr<IHTMLElementCollection> all;
		m_unk0xf0->get_all(&all);

		CComPtr<IDispatch> dispatch;
		all->item(CComVariant("Declaration"), CComVariant(), &dispatch);
		m_unk0xf4 = dispatch;
		dispatch.Release();

		all->item(CComVariant("OwnerDiv"), CComVariant(), &dispatch);
		m_unk0xf8 = dispatch;
		dispatch.Release();

		all->item(CComVariant("Owner"), CComVariant(), &dispatch);
		m_unk0xfc = dispatch;
		dispatch.Release();

		all->item(CComVariant("DeprecatedElement"), CComVariant(), &dispatch);
		m_unk0x108 = dispatch;
		dispatch.Release();

		all->item(CComVariant("PreliminaryElement"), CComVariant(), &dispatch);
		m_unk0x10c = dispatch;
		dispatch.Release();

		all->item(CComVariant("SummaryDiv"), CComVariant(), &dispatch);
		m_unk0x100 = dispatch;
		dispatch.Release();

		all->item(CComVariant("Summary"), CComVariant(), &dispatch);
		m_unk0x104 = dispatch;
	}
	else {
		m_unk0xf4.Release();
		m_unk0xf8.Release();
		m_unk0xfc.Release();
		m_unk0x100.Release();
		m_unk0x104.Release();
		m_unk0x108.Release();
		m_unk0x10c.Release();
	}

	FUN_00438530(NULL, NULL, NULL);
}

void CClassTreeView::FUN_00439690(const RBX::Reflection::ClassDescriptor* descriptor)
{
	RBX::Reflection::Metadata::Class* metadata =
		RBX::Reflection::Metadata::Reflection::FUN_004480c0()->FUN_00446220(descriptor, false);
	if (!metadata || metadata->browsable) {
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

static void FUN_00439910(int* count, std::string* text, const RBX::Reflection::SignatureDescriptor::Item& argument)
{
	if (*count > 0) {
		*text += ", ";
	}

	*text += argument.name->c_str();
	++*count;
}

static void FUN_00439990(int* count, std::string* text, const RBX::Reflection::SignatureDescriptor::Item& argument)
{
	if (*count > 0) {
		*text += ", ";
	}

	*text += argument.type->name.c_str();
	*text += " ";
	*text += argument.name->c_str();
	if (&argument.defaultValue.type() == argument.type) {
		*text += " = ";
		*text += argument.defaultValue.get<std::string>();
	}

	++*count;
}

void CMemberTreeView::FUN_00439a70(const RBX::Reflection::FunctionDescriptor* descriptor)
{
	if (m_unk0xa8 || &descriptor->owner == m_unk0xac) {
		RBX::Reflection::Metadata::Member* metadata =
			RBX::Reflection::Metadata::Reflection::FUN_004480c0()->FUN_00446970(descriptor);
		if (!metadata || metadata->browsable) {
			std::string text = descriptor->name.toString();
			text += "(";

			int count = 0;
			std::for_each(
				descriptor->getSignature().arguments.begin(),
				descriptor->getSignature().arguments.end(),
				boost::bind(FUN_00439910, &count, &text, _1)
			);
			text += ")";

			int image = descriptor->security == RBX::Reflection::FunctionDescriptor::NeedTrustedCaller ? 5 : 4;
			HTREEITEM item = GetTreeCtrl().InsertItem(text.c_str(), image, image);
			GetTreeCtrl().SetItemData(item, reinterpret_cast<DWORD_PTR>(descriptor));
			FUN_00438130(item, metadata);
		}
	}
}

void CDeclarationView::FUN_00439c30(const RBX::Reflection::Descriptor* descriptor)
{
	if (!descriptor) {
		FUN_00438530(NULL, NULL, NULL);
		return;
	}

	std::string declaration;
	RBX::Reflection::Metadata::Item* metadata = NULL;
	const RBX::Reflection::ClassDescriptor* owner = NULL;

	const RBX::Reflection::FunctionDescriptor* function =
		dynamic_cast<const RBX::Reflection::FunctionDescriptor*>(descriptor);
	if (function) {
		declaration = function->getSignature().resultType->name.toString();
		declaration += " ";
		declaration += function->name.toString();
		declaration += "(";

		int count = 0;
		std::for_each(
			function->getSignature().arguments.begin(),
			function->getSignature().arguments.end(),
			boost::bind(FUN_00439990, &count, &declaration, _1)
		);
		declaration += ")";

		owner = &function->owner;
		metadata = RBX::Reflection::Metadata::Reflection::FUN_004480c0()->FUN_00446970(function);
	}
	else {
		const RBX::Reflection::PropertyDescriptor* property =
			dynamic_cast<const RBX::Reflection::PropertyDescriptor*>(descriptor);
		if (property) {
			declaration = property->type.name.toString();
			declaration += " ";
			declaration += property->name.toString();

			owner = &property->owner;
			metadata = RBX::Reflection::Metadata::Reflection::FUN_004480c0()->FUN_00446920(property);
		}
		else {
			const RBX::Reflection::SignalDescriptor* signal =
				dynamic_cast<const RBX::Reflection::SignalDescriptor*>(descriptor);
			if (signal) {
				declaration = "event ";
				declaration += signal->name.toString();
				declaration += "(";

				int count = 0;
				std::for_each(
					signal->getSignature().arguments.begin(),
					signal->getSignature().arguments.end(),
					boost::bind(FUN_00439990, &count, &declaration, _1)
				);
				declaration += ")";

				owner = &signal->owner;
				metadata = RBX::Reflection::Metadata::Reflection::FUN_004480c0()->FUN_004469c0(signal);
			}
			else {
				const RBX::Reflection::EnumDescriptor::Item* item =
					dynamic_cast<const RBX::Reflection::EnumDescriptor::Item*>(descriptor);
				if (item) {
					declaration = G3D::format("%d: %s", item->value, item->name.c_str());
				}
				else {
					const RBX::Reflection::ClassDescriptor* classDescriptor =
						dynamic_cast<const RBX::Reflection::ClassDescriptor*>(descriptor);
					if (classDescriptor) {
						declaration = classDescriptor->name.toString();
						metadata =
							RBX::Reflection::Metadata::Reflection::FUN_004480c0()->FUN_00446220(classDescriptor, false);
					}
					else {
						declaration = descriptor->name.toString();
					}
				}
			}
		}
	}

	FUN_00438530(metadata, declaration.c_str(), owner ? owner->name.c_str() : NULL);
}

void CMemberTreeView::FUN_0043a060(const RBX::Reflection::ClassDescriptor* descriptor)
{
	GetTreeCtrl().DeleteAllItems();
	m_unk0xac = descriptor;

	boost::recursive_mutex::scoped_lock lock(FUN_00437ab0());
	std::for_each(
		descriptor
			->RBX::Reflection::MemberDescriptorContainer<RBX::Reflection::PropertyDescriptor>::descriptors_begin(),
		descriptor->RBX::Reflection::MemberDescriptorContainer<RBX::Reflection::PropertyDescriptor>::descriptors_end(),
		boost::bind(&CMemberTreeView::FUN_00438300, this, _1)
	);
	std::for_each(
		descriptor
			->RBX::Reflection::MemberDescriptorContainer<RBX::Reflection::FunctionDescriptor>::descriptors_begin(),
		descriptor->RBX::Reflection::MemberDescriptorContainer<RBX::Reflection::FunctionDescriptor>::descriptors_end(),
		boost::bind(&CMemberTreeView::FUN_00439a70, this, _1)
	);
	std::for_each(
		descriptor->RBX::Reflection::MemberDescriptorContainer<RBX::Reflection::SignalDescriptor>::descriptors_begin(),
		descriptor->RBX::Reflection::MemberDescriptorContainer<RBX::Reflection::SignalDescriptor>::descriptors_end(),
		boost::bind(&CMemberTreeView::FUN_00438420, this, _1)
	);

	TVSORTCB sort;
	sort.hParent = TVI_ROOT;
	sort.lpfnCompare = FUN_004377d0;
	GetTreeCtrl().SortChildrenCB(&sort);
}

void CMemberTreeView::FUN_0043a240(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTREEVIEW* pNMTreeView = reinterpret_cast<NMTREEVIEW*>(pNMHDR);
	if (m_unk0xb0) {
		m_unk0xb0->FUN_00439c30(reinterpret_cast<const RBX::Reflection::Descriptor*>(pNMTreeView->itemNew.lParam));
	}

	*pResult = 0;
}

void CClassTreeView::FUN_0043a280(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTREEVIEW* pNMTreeView = reinterpret_cast<NMTREEVIEW*>(pNMHDR);
	const RBX::Reflection::Descriptor* descriptor =
		reinterpret_cast<const RBX::Reflection::Descriptor*>(pNMTreeView->itemNew.lParam);

	if (m_unk0xa8) {
		const RBX::Reflection::ClassDescriptor* classDescriptor =
			dynamic_cast<const RBX::Reflection::ClassDescriptor*>(descriptor);
		if (classDescriptor) {
			m_unk0xa8->FUN_0043a060(classDescriptor);
		}
		else {
			const RBX::Reflection::EnumDescriptor* enumDescriptor =
				dynamic_cast<const RBX::Reflection::EnumDescriptor*>(descriptor);
			if (enumDescriptor) {
				m_unk0xa8->FUN_004397f0(enumDescriptor);
			}
		}
	}

	if (m_unk0xac) {
		m_unk0xac->FUN_00439c30(descriptor);
	}

	*pResult = 0;
}

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

void COutputView::OnInitialUpdate()
{
	CSecureHtmlView::OnInitialUpdate();
	LoadFromResource(134);
}

void COutputView::OnDocumentComplete(LPCTSTR lpszURL)
{
	m_unk0x108 = GetHtmlDocument();
	if (m_unk0x108) {
		CComPtr<IHTMLElementCollection> all;
		m_unk0x108->get_all(&all);

		CComPtr<IDispatch> dispatch;
		all->item(CComVariant("output"), CComVariant(), &dispatch);
		m_unk0x10c = dispatch;
	}
	else {
		m_unk0x10c.Release();
	}
}

CStandardOutputView::~CStandardOutputView()
{
	boost::mutex::scoped_lock lock(RBX::StandardOut::singleton()->sync);
	RBX::stopListening<RBX::StandardOut, RBX::StandardOutMessage>(RBX::StandardOut::singleton(), this);
}

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

void COutputView::FUN_0043b300(IHTMLElement* element)
{
	CComQIPtr<IHTMLDOMNode> node(element);
	CComPtr<IHTMLDOMNode> appended;
	if (m_unk0x10c->appendChild(node, &appended) == S_OK) {
		element->scrollIntoView(CComVariant(VARIANT_FALSE));
		m_unk0xf4.push_back(node);
		if (m_unk0xf4.size() > 300) {
			CComPtr<IHTMLDOMNode> removed;
			m_unk0x10c->removeChild(m_unk0xf4.front(), &removed);
			m_unk0xf4.pop_front();
		}
	}
}

void COutputView::FUN_0043b490(RBX::MessageType type, const char* text, __time64_t time)
{
	CStringW message(text);
	CStringW escaped;
	int length = EscapeXML(message, message.GetLength(), NULL, 0);
	EscapeXML(message, message.GetLength(), escaped.GetBufferSetLength(length), length);
	escaped.ReleaseBuffer();

	CStringW html;
	if (type != RBX::MESSAGE_OUTPUT && time != 0) {
		char buffer[32];
		_ctime64_s(buffer, sizeof(buffer), &time);
		html = buffer;
		html += " - ";
	}

	switch (type) {
	case RBX::MESSAGE_INFO:
		html += "<FONT color=\"blue\">";
		html += escaped;
		html += "</FONT>";
		break;
	case RBX::MESSAGE_WARNING:
		html += "<STRONG><FONT color=\"orange\">";
		html += escaped;
		html += "</FONT>";
		break;
	case RBX::MESSAGE_ERROR:
		html += "<STRONG><FONT color=\"red\">";
		html += escaped;
		html += "</FONT>";
		break;
	case RBX::MESSAGE_OUTPUT:
		html += escaped;
		break;
	}

	CComPtr<IHTMLElement> element;
	if (m_unk0x108) {
		m_unk0x108->createElement(CComBSTR("DIV"), &element);
		element->put_innerHTML(CComBSTR(html));
		FUN_0043b300(element);
	}
}

CStandardOutputView::CStandardOutputView()
{
	m_unk0xf0 = 1;
	boost::mutex::scoped_lock lock(RBX::StandardOut::singleton()->sync);
	RBX::startListening<RBX::StandardOut, RBX::StandardOutMessage>(RBX::StandardOut::singleton(), this);
}

LRESULT CStandardOutputView::FUN_0043b8a0(WPARAM wParam, LPARAM lParam)
{
	while (true) {
		std::string message;
		RBX::MessageType type;
		__int64 time;
		{
			boost::mutex::scoped_lock lock(m_unk0x128);
			if (m_unk0x114.empty()) {
				break;
			}

			type = m_unk0x114.front().type;
			message = m_unk0x114.front().message;
			time = m_unk0x114.front().time;
			m_unk0x114.pop_front();
		}

		FUN_0043b490(type, message.c_str(), time);
	}

	return 0;
}
