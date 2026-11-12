#include "PropGrid.h"

#include "Roblox.h"
#include "script/Script.h"
#include "util/ContentProvider.h"
#include "util/Sound.h"
#include "v8datamodel/BrickColor.h"
#include "v8datamodel/Commands.h"
#include "v8datamodel/DataModel.h"

#include <G3D/GImage.h>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <cstdio>
#include <fstream>
#include <string>

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

IMPLEMENT_DYNAMIC(CPropGrid, CXTPPropertyGrid)
IMPLEMENT_DYNAMIC(CSelectionPropGrid, CPropGrid)
IMPLEMENT_DYNAMIC(CDataModelPropGrid, CSelectionPropGrid)

// FUNCTION: ROBLOX 0x0043bbd0
template <>
bool XItem<bool, CXTPPropertyGridItemBool, RBX::Reflection::TypedPropertyDescriptor<bool> >::VTable0xe4()
{
	return GetBool() ? true : false;
}

// FUNCTION: ROBLOX 0x0043bbe0
template <>
void XItem<bool, CXTPPropertyGridItemBool, RBX::Reflection::TypedPropertyDescriptor<bool> >::VTable0xe8(
	const bool& value
)
{
	SetBool(value);
}

// FUNCTION: ROBLOX 0x0043bc00
template <>
void XItem<float, CXTPPropertyGridItem, RBX::Reflection::TypedPropertyDescriptor<float> >::VTable0xe8(
	const float& value
)
{
	char buffer[32];
	sprintf(buffer, "%.3g", value);
	SetValue(buffer);
}

// FUNCTION: ROBLOX 0x0043bc60
template <>
void XItem<int, CXTPPropertyGridItem, RBX::Reflection::TypedPropertyDescriptor<int> >::VTable0xe8(const int& value)
{
	char buffer[32];
	sprintf(buffer, "%d", value);
	SetValue(buffer);
}

// FUNCTION: ROBLOX 0x0043bcc0
template <>
G3D::Color3 XItem<G3D::Color3, CXTPPropertyGridItemColor, RBX::Reflection::TypedPropertyDescriptor<G3D::Color3> >::
	VTable0xe4()
{
	COLORREF color = GetColor();
	return G3D::Color3(GetRValue(color) / 255.0, GetGValue(color) / 255.0, GetBValue(color) / 255.0);
}

// FUNCTION: ROBLOX 0x0043bd10
template <>
void XItem<G3D::Color3, CXTPPropertyGridItemColor, RBX::Reflection::TypedPropertyDescriptor<G3D::Color3> >::VTable0xe8(
	const G3D::Color3& value
)
{
	SetColor(RGB(value.r * 255.0, value.g * 255.0, value.b * 255.0));
}

// FUNCTION: ROBLOX 0x0043bfd0
template <>
RBX::BrickColor XItem<
	RBX::BrickColor,
	CPropertyGridItemBrickColor,
	RBX::Reflection::TypedPropertyDescriptor<RBX::BrickColor> >::VTable0xe4()
{
	COLORREF color = GetColor();
	return RBX::BrickColor::closest(
		G3D::Color3(GetRValue(color) / 255.0, GetGValue(color) / 255.0, GetBValue(color) / 255.0)
	);
}

// FUNCTION: ROBLOX 0x0043c030
template <>
void XItem<RBX::BrickColor, CPropertyGridItemBrickColor, RBX::Reflection::TypedPropertyDescriptor<RBX::BrickColor> >::
	VTable0xe8(const RBX::BrickColor& value)
{
	G3D::Color4uint8 color = value.color4uint8();
	SetColor(RGB(color.r, color.g, color.b));
}

// FUNCTION: ROBLOX 0x0043bdc0
CString FUN_0043bdc0(COLORREF color)
{
	RBX::BrickColor brickColor = RBX::BrickColor::closest(
		G3D::Color3(GetRValue(color) / 255.0, GetGValue(color) / 255.0, GetBValue(color) / 255.0)
	);
	return brickColor.name().c_str();
}

// FUNCTION: ROBLOX 0x0043be40
void CPropertyGridItemBrickColor::SetColor(COLORREF clr)
{
	m_clrValue = clr;
	CXTPPropertyGridItem::SetValue(FUN_0043bdc0(clr));
}

// FUNCTION: ROBLOX 0x0043be70
CPropertyGridItemBrickColor::CPropertyGridItemBrickColor(CString strCaption, COLORREF clr)
	: CXTPPropertyGridItemColor(strCaption, clr)
{
	m_nFlags = xtpGridItemHasComboButton;
	SetColor(clr);
	m_strDefaultValue = m_strValue;
}

// FUNCTION: ROBLOX 0x0043bf30
static COLORREF FUN_0043bf30(CString text)
{
	G3D::Color3uint8 color = RBX::BrickColor::parse(text).color3uint8();
	return RGB(color.r, color.g, color.b);
}

// FUNCTION: ROBLOX 0x0043c070
template <>
void XItem<G3D::Vector3, CXTPPropertyGridItem, RBX::Reflection::TypedPropertyDescriptor<G3D::Vector3> >::VTable0xe8(
	const G3D::Vector3& value
)
{
	char buffer[256];
	sprintf(buffer, "%.3g, %.3g, %.3g", value.x, value.y, value.z);
	SetValue(buffer);
}

// FUNCTION: ROBLOX 0x0043c0f0 FOLDED
template <>
void XItem<std::string, CXTPPropertyGridItem, RBX::Reflection::PropertyDescriptor>::VTable0xe8(const std::string& value)
{
	SetValue(value.c_str());
}

// FUNCTION: ROBLOX 0x0043c0f0 FOLDED
template <>
void XItem<
	RBX::Soundscape::SoundId,
	CXTPPropertyGridItem,
	RBX::Reflection::TypedPropertyDescriptor<RBX::Soundscape::SoundId> >::
	VTable0xe8(const RBX::Soundscape::SoundId& value)
{
	SetValue(value.c_str());
}

// FUNCTION: ROBLOX 0x0043c0f0 FOLDED
template <>
void XItem<RBX::TextureId, CXTPPropertyGridItem, RBX::Reflection::TypedPropertyDescriptor<RBX::TextureId> >::VTable0xe8(
	const RBX::TextureId& value
)
{
	SetValue(value.c_str());
}

// FUNCTION: ROBLOX 0x0043c120
const RBX::Name& FUN_0043c120(const char* path)
{
	switch (G3D::GImage::resolveFormat(path)) {
	case G3D::GImage::JPEG:
		return RBX::Name::declare("image/jpeg", -1);
	case G3D::GImage::BMP:
		return RBX::Name::declare("image/bmp", -1);
	case G3D::GImage::ICO:
		return RBX::Name::declare("image/vnd.microsoft.icon", -1);
	case G3D::GImage::PNG:
		return RBX::Name::declare("image/png", -1);
	default:
		return RBX::Name::getNullName();
	}
}

// FUNCTION: ROBLOX 0x0043c240
template <>
int XItem<int, CXTPPropertyGridItemEnum, RBX::Reflection::EnumPropertyDescriptor>::VTable0xe4()
{
	return GetEnum();
}

// FUNCTION: ROBLOX 0x0043c250
template <>
void XItem<int, CXTPPropertyGridItemEnum, RBX::Reflection::EnumPropertyDescriptor>::VTable0xe8(const int& value)
{
	SetEnum(value);
}

// FUNCTION: ROBLOX 0x0043c270
int CPropGrid::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPPropertyGrid::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	SetTheme(xtpGridThemeWhidbey);
	XTPImageManager()->InternalAddRef();
	SetImageManager(XTPImageManager());
	return 0;
}

// FUNCTION: ROBLOX 0x0043c580
static boost::recursive_mutex& FUN_0043c580()
{
	static boost::recursive_mutex mutex;
	return mutex;
}

// FUNCTION: ROBLOX 0x0043c740
void CPropertyGridItemBrickColor::SetValue(CString strValue)
{
	if (strValue != "") {
		SetColor(FUN_0043bf30(strValue));
	}
	else {
		m_clrValue = (COLORREF) -1;
		CXTPPropertyGridItem::SetValue("");
	}
}

// FUNCTION: ROBLOX 0x0043c7f0
void CPropertyGridItemBrickColor::OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton)
{
	if (m_pGrid->SendNotifyMessage(XTP_PGN_INPLACEBUTTONDOWN, (LPARAM) pButton) == TRUE) {
		return;
	}

	if (!OnRequestEdit()) {
		return;
	}

	CRect rect = GetItemRect();
	m_pGrid->ClientToScreen(&rect);
	rect.left = rect.right - 158;

	CXTPPopupToolBar* popup = CXTPPopupToolBar::CreatePopupToolBar(NULL);
	CRobloxControlColorSelector* selector = new CRobloxControlColorSelector();
	popup->GetControls()->Add(selector, 0);
	if (GetValue() != "") {
		selector->SetColor(GetColor());
	}

	if (CXTPCommandBars::TrackPopupMenu(
			popup, TPM_RIGHTALIGN | TPM_NONOTIFY, rect.right, rect.bottom, AfxGetMainWnd()
		) &&
		selector->m_unk0x174) {
		OnValueChanged(FUN_0043bdc0(selector->GetColor()));
	}

	popup->InternalRelease();
}

// FUNCTION: ROBLOX 0x0043c990
template <>
std::string XItem<std::string, CXTPPropertyGridItem, RBX::Reflection::PropertyDescriptor>::VTable0xe4()
{
	return (LPCTSTR) GetValue();
}

// FUNCTION: ROBLOX 0x0043cad0 FOLDED
template <>
RBX::Soundscape::SoundId XItem<
	RBX::Soundscape::SoundId,
	CXTPPropertyGridItem,
	RBX::Reflection::TypedPropertyDescriptor<RBX::Soundscape::SoundId> >::VTable0xe4()
{
	return (LPCTSTR) GetValue();
}

// FUNCTION: ROBLOX 0x0043cad0 FOLDED
template <>
RBX::TextureId XItem<RBX::TextureId, CXTPPropertyGridItem, RBX::Reflection::TypedPropertyDescriptor<RBX::TextureId> >::
	VTable0xe4()
{
	return (LPCTSTR) GetValue();
}

// FUNCTION: ROBLOX 0x0043cba0
CXTPPropertyGridItem* CPropGrid::FUN_0043cba0(const RBX::Name& category)
{
	CXTPPropertyGridItem* item = FindItem(category.toString().c_str());
	if (!item) {
		item = AddCategory(category.toString().c_str());
		item->Expand();
	}
	return item;
}

// FUNCTION: ROBLOX 0x0043db00
void CPropGrid::FUN_0043db00(const RBX::Reflection::ClassDescriptor* classDescriptor)
{
	boost::recursive_mutex::scoped_lock lock(FUN_0043c580());
	std::for_each(
		classDescriptor->derivedClasses_begin(),
		classDescriptor->derivedClasses_end(),
		boost::bind(&CPropGrid::FUN_0043db00, this, _1)
	);
	std::for_each(
		classDescriptor
			->RBX::Reflection::MemberDescriptorContainer<RBX::Reflection::PropertyDescriptor>::descriptors_begin(),
		classDescriptor
			->RBX::Reflection::MemberDescriptorContainer<RBX::Reflection::PropertyDescriptor>::descriptors_end(),
		boost::bind(&CPropGrid::FUN_00442e20, this, classDescriptor, _1)
	);
}

// STUB: ROBLOX 0x0043df20
void CPropGrid::onEvent(const RBX::Instance* source, RBX::PropertyChanged event)
{
	boost::recursive_mutex::scoped_lock lock(m_unk0x188);
	if (!m_unk0x1c1) {
		std::map<const RBX::Reflection::PropertyDescriptor*, Item*>::iterator iter =
			m_unk0x1a8.find(&event.getDescriptor());
		if (iter != m_unk0x1a8.end()) {
			m_unk0x1c4.insert(iter->second);
		}

		boost::recursive_mutex::scoped_lock lock(m_unk0x188);
		if (!m_unk0x1c0) {
			if (m_hWnd) {
				::PostMessage(m_hWnd, WM_USER + 101, 0, 0);
			}
			m_unk0x1c0 = true;
		}
	}
}

// FUNCTION: ROBLOX 0x0043e120
void CPropGrid::Item::FUN_0043e120(const RBX::Reflection::ClassDescriptor* classDescriptor)
{
	m_unk0x04.push_back(classDescriptor);
	std::for_each(
		classDescriptor->derivedClasses_begin(),
		classDescriptor->derivedClasses_end(),
		boost::bind(&Item::FUN_0043e120, this, _1)
	);
}

// FUNCTION: ROBLOX 0x0043e170
void CPropGrid::Unk0x1d0::FUN_0043e170(RBX::Instance* instance)
{
	iterator iter = find(&instance->getDescriptor());
	if (iter != end()) {
		std::vector<boost::shared_ptr<RBX::Instance> >::iterator found =
			std::find(iter->second.begin(), iter->second.end(), shared_from(instance));
		if (found != iter->second.end()) {
			iter->second.erase(found);
		}
	}
}

template <class T>
void CPropGrid::FUN_0043e6b0(T* item)
{
	CXTPPropertyGridItem* category = FUN_0043cba0(item->m_unk0x124->category);

	int i = 0;
	while (i < category->GetChilds()->GetCount()) {
		CXTPPropertyGridItem* child = category->GetChilds()->GetAt(i);
		if (item->GetCaption().Compare(child->GetCaption()) < 0) {
			break;
		}
		i++;
	}

	category->InsertChildItem(i, item);
	m_unk0x1a8[item->getDescriptor()] = item;
	m_unk0x198.push_back(item);
}

// FUNCTION: ROBLOX 0x0043e7f0
void CPropGrid::FUN_0043e7f0(boost::shared_ptr<RBX::Instance> instance)
{
	RBX::stopListening<RBX::Instance, RBX::PropertyChanged>(instance, this);
	m_unk0x1d0.FUN_0043e170(instance.get());
}

// FUNCTION: ROBLOX 0x0043e8a0
LRESULT CPropGrid::FUN_0043e8a0(WPARAM wParam, LPARAM lParam)
{
	if (m_unk0x198.empty()) {
		CXTPPropertyGridUpdateContext context;
		BeginUpdate(context);
		FUN_0043db00(&RBX::Reflection::ClassDescriptor::rootDescriptor());
		EndUpdate(context);
	}

	std::vector<Item*> items;
	std::set<Item*> dirty;
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0x188);
		if (m_unk0x1c1) {
			items = m_unk0x198;
			m_unk0x1c1 = false;
		}
		else {
			dirty = m_unk0x1c4;
			m_unk0x1c4.clear();
		}
		m_unk0x1c0 = false;
	}

	{
		RBX::DataModel::Lock lock(m_unk0x194);
		std::for_each(items.begin(), items.end(), boost::bind(&Item::VTable0x00, _1, boost::cref(m_unk0x1d0)));
		std::for_each(dirty.begin(), dirty.end(), boost::bind(&Item::VTable0x00, _1, boost::cref(m_unk0x1d0)));
	}

	for (int i = 0; i < GetCategories()->GetCount(); i++) {
		CXTPPropertyGridItem* category = GetCategories()->GetAt(i);
		CXTPPropertyGridItems* childs = category->GetChilds();
		bool hidden = true;
		for (int j = 0; j < childs->GetCount(); j++) {
			if (!childs->GetAt(j)->IsHidden()) {
				hidden = false;
				break;
			}
		}
		category->SetHidden(hidden);
	}

	Refresh();
	return 0;
}

// FUNCTION: ROBLOX 0x0043eba0
CPropGrid::Item::Item(const RBX::Reflection::PropertyDescriptor& descriptor, CPropGrid* grid) : m_unk0x14(grid)
{
	FUN_0043e120(&descriptor.owner);
}

template <class T, class Base, class Descriptor>
void XItem<T, Base, Descriptor>::VTable0x00(
	const std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >& instances
)
{
	T first;
	bool hasValue = false;
	std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >::const_iterator
		objectsEnd = instances.end();
	for (std::vector<const RBX::Reflection::ClassDescriptor*>::const_iterator iter = m_unk0x04.begin(),
																			  end = m_unk0x04.end();
		 iter != end;
		 ++iter) {
		const RBX::Reflection::ClassDescriptor* descriptor = *iter;
		std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >::
			const_iterator found = instances.find(descriptor);
		if (found != objectsEnd) {
			for (std::vector<boost::shared_ptr<RBX::Instance> >::const_iterator instance = found->second.begin(),
																				instancesEnd = found->second.end();
				 instance != instancesEnd;
				 ++instance) {
				if (!hasValue) {
					first = FUN_0043f1f0(instance->get());
					VTable0xe8(first);
				}
				else if (!VTable0xec(first, FUN_0043f1f0(instance->get()))) {
					CXTPPropertyGridItem::SetValue("");
					SetHidden(FALSE);
					return;
				}
				hasValue = true;
			}
		}
	}
	SetHidden(!hasValue);
}

template <class T, class Base, class Descriptor>
T XItem<T, Base, Descriptor>::FUN_0043f1f0(RBX::Instance* instance)
{
	RBX::DataModel::Lock lock(m_unk0x14->m_unk0x194);
	return m_unk0x124->getValue(instance);
}

template <>
int XItem<int, CXTPPropertyGridItemEnum, RBX::Reflection::EnumPropertyDescriptor>::FUN_0043f1f0(RBX::Instance* instance)
{
	RBX::DataModel::Lock lock(m_unk0x14->m_unk0x194);
	return m_unk0x124->getEnumValue(instance);
}

template <>
std::string XItem<std::string, CXTPPropertyGridItem, RBX::Reflection::PropertyDescriptor>::FUN_0043f1f0(
	RBX::Instance* instance
)
{
	RBX::DataModel::Lock lock(m_unk0x14->m_unk0x194);
	return m_unk0x124->getStringValue(instance);
}

template <class T, class Base, class Descriptor>
void XItem<T, Base, Descriptor>::FUN_00443670(const T& value)
{
	try {
		std::auto_ptr<RBX::XmlState> undoState(new RBX::UndoState());
		std::auto_ptr<RBX::XmlState> redoState(new RBX::RedoState());

		std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >::iterator
			objectsEnd = m_unk0x14->m_unk0x1d0.end();
		std::vector<const RBX::Reflection::ClassDescriptor*>::const_iterator end = m_unk0x04.end();
		for (std::vector<const RBX::Reflection::ClassDescriptor*>::const_iterator iter = m_unk0x04.begin(); iter != end;
			 ++iter) {
			const RBX::Reflection::ClassDescriptor* descriptor = *iter;
			CPropGrid::Unk0x1d0& objects = m_unk0x14->m_unk0x1d0;
			std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >::iterator
				found = objects.find(descriptor);
			if (found != objectsEnd) {
				for (std::vector<boost::shared_ptr<RBX::Instance> >::iterator instance = found->second.begin(),
																			  instancesEnd = found->second.end();
					 instance != instancesEnd;
					 ++instance) {
					undoState->addProperty(RBX::Reflection::Property(*m_unk0x124, instance->get()));
					VTable0xf0(instance->get(), value);
					redoState->addProperty(RBX::Reflection::Property(*m_unk0x124, instance->get()));
				}
			}
		}

		if (m_unk0x14->m_unk0x194) {
			RBX::DataModel::Lock lock(m_unk0x14->m_unk0x194);
			m_unk0x14->m_unk0x194->pushState(m_unk0x124->name.toString(), undoState.release(), redoState.release());
			m_unk0x14->m_unk0x194->setDirty(true);
		}
	}
	catch (const std::exception& e) {
		AfxMessageBox(e.what());
	}
}

// FUNCTION: ROBLOX 0x004401e0
template <>
void XItem<std::string, CXTPPropertyGridItem, RBX::Reflection::PropertyDescriptor>::VTable0xf0(
	RBX::Instance* instance, const std::string& value
)
{
	RBX::DataModel::Lock lock(m_unk0x14->m_unk0x194);
	m_unk0x124->setStringValue(instance, value);
}

// FUNCTION: ROBLOX 0x00440a70
template <>
void XItem<int, CXTPPropertyGridItemEnum, RBX::Reflection::EnumPropertyDescriptor>::VTable0xf0(
	RBX::Instance* instance, const int& value
)
{
	RBX::DataModel::Lock lock(m_unk0x14->m_unk0x194);
	m_unk0x124->setEnumValue(instance, value);
}

// FUNCTION: ROBLOX 0x00441940
XBoolItem::XBoolItem(const RBX::Reflection::TypedPropertyDescriptor<bool>& descriptor, CPropGrid* grid)
	: XItem<bool, CXTPPropertyGridItemBool, RBX::Reflection::TypedPropertyDescriptor<bool> >(descriptor, grid)
{
	SetCheckBoxStyle();
}

// FUNCTION: ROBLOX 0x004419c0
BOOL XBoolItem::OnDrawItemValue(CDC& dc, CRect rcValue)
{
	if (!m_bCheckBoxStyle) {
		return FALSE;
	}

	CRect rcCheck(rcValue.left - 1, rcValue.CenterPoint().y - 6, rcValue.left + 12, rcValue.CenterPoint().y + 7);
	CXTPPropertyGridPaintManager* paintManager = m_pGrid->GetPaintManager();

	if (paintManager->m_buttonsStyle == xtpGridButtonsThemed && paintManager->m_themeButton.IsAppThemed()) {
		int state;
		if (GetReadOnly()) {
			state = VTable0xf4() ? CBS_MIXEDDISABLED : (GetBool() ? CBS_CHECKEDDISABLED : CBS_UNCHECKEDDISABLED);
		}
		else {
			state = VTable0xf4() ? CBS_MIXEDNORMAL : (GetBool() ? CBS_CHECKEDNORMAL : CBS_UNCHECKEDNORMAL);
		}
		paintManager->m_themeButton.DrawThemeBackground(dc, BP_CHECKBOX, state, rcCheck, 0);
	}
	else {
		dc.DrawFrameControl(
			rcCheck, DFC_BUTTON, DFCS_BUTTONCHECK | (GetReadOnly() ? DFCS_INACTIVE : 0) | (GetBool() ? DFCS_CHECKED : 0)
		);
	}

	return TRUE;
}

// FUNCTION: ROBLOX 0x00441b10
XBrickColorItem::XBrickColorItem(
	const RBX::Reflection::TypedPropertyDescriptor<RBX::BrickColor>& descriptor, CPropGrid* grid
)
	: XItem<RBX::BrickColor, CPropertyGridItemBrickColor, RBX::Reflection::TypedPropertyDescriptor<RBX::BrickColor> >(
		  descriptor, grid
	  )
{
}

// FUNCTION: ROBLOX 0x00441b50
BOOL XBrickColorItem::OnDrawItemValue(CDC& dc, CRect rcValue)
{
	if (GetValue() == "") {
		return FALSE;
	}

	return CPropertyGridItemBrickColor::OnDrawItemValue(dc, rcValue);
}

// FUNCTION: ROBLOX 0x00441c10
Vector3Item::Vector3Item(const RBX::Reflection::TypedPropertyDescriptor<G3D::Vector3>& descriptor, CPropGrid* grid)
	: XItem<G3D::Vector3, CXTPPropertyGridItem, RBX::Reflection::TypedPropertyDescriptor<G3D::Vector3> >(
		  descriptor, grid
	  )
{
	m_unk0x128[0] = m_unk0x128[1] = m_unk0x128[2] = NULL;
}

// FUNCTION: ROBLOX 0x00441c60
BOOL Vector3Item::OnAfterEdit(CString& strValue)
{
	G3D::Vector3 value;
	return RBX::StringConverter<G3D::Vector3>::convertToValue((LPCTSTR) strValue, value);
}

// FUNCTION: ROBLOX 0x00441cf0
void Vector3Item::VTable0x00(
	const std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >& instances
)
{
	XItem<G3D::Vector3, CXTPPropertyGridItem, RBX::Reflection::TypedPropertyDescriptor<G3D::Vector3> >::VTable0x00(
		instances
	);
	for (int i = 0; i < 3; i++) {
		if (m_unk0x128[i]) {
			m_unk0x128[i]->VTable0x00(instances);
		}
	}
}

// FUNCTION: ROBLOX 0x00441d30
Vector3ComponentItem::Vector3ComponentItem(
	const RBX::Reflection::TypedPropertyDescriptor<G3D::Vector3>& descriptor, CPropGrid* grid, int component
)
	: XItem<G3D::Vector3, CXTPPropertyGridItem, RBX::Reflection::TypedPropertyDescriptor<G3D::Vector3> >(
		  descriptor, grid
	  ),
	  m_unk0x128(component)
{
	switch (component) {
	case 0:
		SetCaption("x");
		break;
	case 1:
		SetCaption("y");
		break;
	case 2:
		SetCaption("z");
		break;
	}
}

// FUNCTION: ROBLOX 0x00441df0
bool Vector3ComponentItem::VTable0xec(const G3D::Vector3& first, const G3D::Vector3& second)
{
	return first[m_unk0x128] == second[m_unk0x128];
}

// FUNCTION: ROBLOX 0x00441e20
void Vector3ComponentItem::VTable0xe8(const G3D::Vector3& value)
{
	m_unk0x12c = value;
	char buffer[32];
	sprintf(buffer, "%.3g", value[m_unk0x128]);
	SetValue(buffer);
}

// FUNCTION: ROBLOX 0x00441ea0
G3D::Vector3 Vector3ComponentItem::VTable0xe4()
{
	G3D::Vector3 result = m_unk0x12c;
	if (RBX::StringConverter<float>::convertToValue((LPCTSTR) GetValue(), result[m_unk0x128])) {
		return result;
	}
	return m_unk0x12c;
}

// FUNCTION: ROBLOX 0x00441f90
void Vector3ComponentItem::VTable0xf0(RBX::Instance* instance, const G3D::Vector3& value)
{
	RBX::DataModel::Lock lock(m_unk0x14->m_unk0x194);
	G3D::Vector3 current = m_unk0x124->getValue(instance);
	current[m_unk0x128] = value[m_unk0x128];
	m_unk0x124->setValue(instance, current);
}

// FUNCTION: ROBLOX 0x00442060
void Vector3Item::OnAddChildItem()
{
	CXTPPropertyGridItem::OnAddChildItem();
	for (int i = 0; i < 3; i++) {
		m_unk0x128[i] = new Vector3ComponentItem(*m_unk0x124, m_unk0x14, i);
		AddChildItem(m_unk0x128[i]);
	}
}

// FUNCTION: ROBLOX 0x00442100
RefItem::RefItem(const RBX::Reflection::RefPropertyDescriptor& descriptor, CPropGrid* grid)
	: XItem<std::string, CXTPPropertyGridItem, RBX::Reflection::PropertyDescriptor>(descriptor, grid),
	  m_unk0x128(&descriptor)
{
	SetReadOnly(TRUE);
}

// FUNCTION: ROBLOX 0x00442180
void RefItem::VTable0x00(
	const std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >& instances
)
{
	RBX::Instance* first;
	bool hasValue = false;
	std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >::const_iterator
		objectsEnd = instances.end();
	for (std::vector<const RBX::Reflection::ClassDescriptor*>::const_iterator iter = m_unk0x04.begin(),
																			  end = m_unk0x04.end();
		 iter != end;
		 ++iter) {
		const RBX::Reflection::ClassDescriptor* descriptor = *iter;
		std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >::
			const_iterator found = instances.find(descriptor);
		if (found != objectsEnd) {
			for (std::vector<boost::shared_ptr<RBX::Instance> >::const_iterator instance = found->second.begin(),
																				instancesEnd = found->second.end();
				 instance != instancesEnd;
				 ++instance) {
				if (!hasValue) {
					first = static_cast<RBX::Instance*>(m_unk0x128->getRefValue(instance->get()));
					VTable0xe8(first ? first->getName() : std::string(""));
				}
				else if (first != m_unk0x128->getRefValue(instance->get())) {
					CXTPPropertyGridItem::SetValue("");
					SetHidden(FALSE);
					return;
				}
				hasValue = true;
			}
		}
	}
	SetHidden(!hasValue);
}

// FUNCTION: ROBLOX 0x00442430
ScriptItem::ScriptItem(const RBX::Reflection::PropertyDescriptor& descriptor, CPropGrid* grid)
	: XItem<std::string, CXTPPropertyGridItem, RBX::Reflection::PropertyDescriptor>(descriptor, grid)
{
	SetFlags(GetFlags() | xtpGridItemHasExpandButton);
	SetFlags(GetFlags() & ~xtpGridItemHasEdit);
	CXTPPropertyGridItem::SetValue("(Script)");
}

// FUNCTION: ROBLOX 0x004424e0 FOLDED
void RefItem::OnValueChanged(CString strValue)
{
}

// FUNCTION: ROBLOX 0x004424e0 FOLDED
void ScriptItem::OnValueChanged(CString strValue)
{
}

// STUB: ROBLOX 0x004424f0
void ScriptItem::VTable0x00(
	const std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >& instances
)
{
	for (std::vector<const RBX::Reflection::ClassDescriptor*>::const_iterator iter = m_unk0x04.begin(),
																			  end = m_unk0x04.end();
		 iter != end;
		 ++iter) {
		const RBX::Reflection::ClassDescriptor* descriptor = *iter;
		std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >::
			const_iterator found = instances.find(descriptor);
		if (found != instances.end() && found->second.size() != 0) {
			SetHidden(FALSE);
			return;
		}
	}
	SetHidden(TRUE);
}

// FUNCTION: ROBLOX 0x00442620
void ScriptItem::OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton)
{
	std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >::iterator
		objectsEnd = m_unk0x14->m_unk0x1d0.end();
	for (std::vector<const RBX::Reflection::ClassDescriptor*>::const_iterator iter = m_unk0x04.begin(),
																			  end = m_unk0x04.end();
		 iter != end;
		 ++iter) {
		const RBX::Reflection::ClassDescriptor* descriptor = *iter;
		CPropGrid::Unk0x1d0& objects = m_unk0x14->m_unk0x1d0;
		std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >::iterator
			found = objects.find(descriptor);
		if (found != objectsEnd) {
			for (std::vector<boost::shared_ptr<RBX::Instance> >::iterator instance = found->second.begin(),
																		  instancesEnd = found->second.end();
				 instance != instancesEnd;
				 ++instance) {
				if (RBX::Script* script = dynamic_cast<RBX::Script*>(instance->get())) {
					static_cast<CRobloxApp*>(AfxGetApp())->FUN_0044ef90(RBX::shared_from(script));
				}
			}
		}
	}
}

// STUB: ROBLOX 0x00442800
EnumItem::EnumItem(const RBX::Reflection::EnumPropertyDescriptor& descriptor, CPropGrid* grid)
	: XItem<int, CXTPPropertyGridItemEnum, RBX::Reflection::EnumPropertyDescriptor>(descriptor, grid)
{
	for (std::vector<const RBX::Reflection::EnumDescriptor::Item*>::const_iterator
			 iter = descriptor.enumDescriptor.begin(),
			 end = descriptor.enumDescriptor.end();
		 iter != end;
		 ++iter) {
		GetConstraints()->AddConstraint((*iter)->name.c_str(), (*iter)->value);
	}
}

// FUNCTION: ROBLOX 0x00442d00
SoundItem::SoundItem(
	const RBX::Reflection::TypedPropertyDescriptor<RBX::Soundscape::SoundId>& descriptor, CPropGrid* grid
)
	: ContentItem<RBX::Soundscape::SoundId>(descriptor, grid)
{
	CXTPPropertyGridItem::SetValue("(Sound)");
}

// FUNCTION: ROBLOX 0x00442d90
TextureItem::TextureItem(const RBX::Reflection::TypedPropertyDescriptor<RBX::TextureId>& descriptor, CPropGrid* grid)
	: ContentItem<RBX::TextureId>(descriptor, grid)
{
	CXTPPropertyGridItem::SetValue("(Texture)");
}

// FUNCTION: ROBLOX 0x00442e20
void CPropGrid::FUN_00442e20(
	const RBX::Reflection::ClassDescriptor* classDescriptor, const RBX::Reflection::PropertyDescriptor* descriptor
)
{
	if (descriptor->owner != *classDescriptor || !descriptor->isPublic()) {
		return;
	}

	if (const RBX::Reflection::TypedPropertyDescriptor<bool>* d =
			dynamic_cast<const RBX::Reflection::TypedPropertyDescriptor<bool>*>(descriptor)) {
		FUN_0043e6b0(new XBoolItem(*d, this));
	}
	else if (
		const RBX::Reflection::TypedPropertyDescriptor<float>* d =
			dynamic_cast<const RBX::Reflection::TypedPropertyDescriptor<float>*>(descriptor)
	) {
		FUN_0043e6b0(
			new XItem<float, CXTPPropertyGridItem, RBX::Reflection::TypedPropertyDescriptor<float> >(*d, this)
		);
	}
	else if (
		const RBX::Reflection::TypedPropertyDescriptor<int>* d =
			dynamic_cast<const RBX::Reflection::TypedPropertyDescriptor<int>*>(descriptor)
	) {
		FUN_0043e6b0(new XItem<int, CXTPPropertyGridItem, RBX::Reflection::TypedPropertyDescriptor<int> >(*d, this));
	}
	else if (
		const RBX::Reflection::TypedPropertyDescriptor<G3D::Color3>* d =
			dynamic_cast<const RBX::Reflection::TypedPropertyDescriptor<G3D::Color3>*>(descriptor)
	) {
		FUN_0043e6b0(
			new XItem<G3D::Color3, CXTPPropertyGridItemColor, RBX::Reflection::TypedPropertyDescriptor<G3D::Color3> >(
				*d, this
			)
		);
	}
	else if (
		const RBX::Reflection::TypedPropertyDescriptor<RBX::BrickColor>* d =
			dynamic_cast<const RBX::Reflection::TypedPropertyDescriptor<RBX::BrickColor>*>(descriptor)
	) {
		FUN_0043e6b0(new XBrickColorItem(*d, this));
	}
	else if (
		const RBX::Reflection::TypedPropertyDescriptor<G3D::Vector3>* d =
			dynamic_cast<const RBX::Reflection::TypedPropertyDescriptor<G3D::Vector3>*>(descriptor)
	) {
		FUN_0043e6b0(new Vector3Item(*d, this));
	}
	else if (
		const RBX::Reflection::EnumPropertyDescriptor* d =
			dynamic_cast<const RBX::Reflection::EnumPropertyDescriptor*>(descriptor)
	) {
		FUN_0043e6b0(new EnumItem(*d, this));
	}
	else if (
		const RBX::Reflection::RefPropertyDescriptor* d =
			dynamic_cast<const RBX::Reflection::RefPropertyDescriptor*>(descriptor)
	) {
		FUN_0043e6b0(new RefItem(*d, this));
	}
	else if (descriptor == &RBX::Script::prop_EmbeddedSourceCode) {
		FUN_0043e6b0(new ScriptItem(*descriptor, this));
	}
	else if (
		const RBX::Reflection::TypedPropertyDescriptor<RBX::TextureId>* d =
			dynamic_cast<const RBX::Reflection::TypedPropertyDescriptor<RBX::TextureId>*>(descriptor)
	) {
		FUN_0043e6b0(new TextureItem(*d, this));
	}
	else if (
		const RBX::Reflection::TypedPropertyDescriptor<RBX::Soundscape::SoundId>* d =
			dynamic_cast<const RBX::Reflection::TypedPropertyDescriptor<RBX::Soundscape::SoundId>*>(descriptor)
	) {
		FUN_0043e6b0(new SoundItem(*d, this));
	}
	else if (descriptor->hasStringValue()) {
		FUN_0043e6b0(
			new XItem<std::string, CXTPPropertyGridItem, RBX::Reflection::PropertyDescriptor>(*descriptor, this)
		);
	}
}

// FUNCTION: ROBLOX 0x004452d0
void CPropGrid::Unk0x1d0::FUN_004452d0(RBX::Instance* instance)
{
	(*this)[&instance->getDescriptor()].push_back(shared_from(instance));
}

// FUNCTION: ROBLOX 0x00445380
void CPropGrid::FUN_00445380(boost::shared_ptr<RBX::Instance> instance)
{
	m_unk0x1d0.FUN_004452d0(instance.get());
	RBX::startListening<RBX::Instance, RBX::PropertyChanged>(instance, this);
}

// STUB: ROBLOX 0x00445430
void CSelectionPropGrid::onEvent(const RBX::Selection* source, RBX::SelectionChanged event)
{
	if (event.addedItem) {
		FUN_00445380(event.addedItem);
	}
	if (event.removedItem) {
		FUN_0043e7f0(event.removedItem);
	}

	boost::recursive_mutex::scoped_lock lock(m_unk0x188);
	m_unk0x1c1 = true;
	m_unk0x1c4.clear();

	boost::recursive_mutex::scoped_lock lock2(m_unk0x188);
	if (!m_unk0x1c0) {
		if (m_hWnd) {
			::PostMessage(m_hWnd, WM_USER + 101, 0, 0);
		}
		m_unk0x1c0 = true;
	}
}

// FUNCTION: ROBLOX 0x00445580
void CSelectionPropGrid::FUN_00445580(RBX::Selection* selection)
{
	boost::recursive_mutex::scoped_lock lock(m_unk0x188);

	if (m_unk0x1e0) {
		RBX::stopListening<RBX::Selection, RBX::SelectionChanged>(m_unk0x1e0, this);
		std::for_each(m_unk0x1e0->begin(), m_unk0x1e0->end(), boost::bind(&CSelectionPropGrid::FUN_0043e7f0, this, _1));
	}

	m_unk0x1c4.clear();
	m_unk0x1d0.clear();
	m_unk0x1e0 = shared_from(selection);

	if (m_unk0x1e0) {
		std::for_each(m_unk0x1e0->begin(), m_unk0x1e0->end(), boost::bind(&CSelectionPropGrid::FUN_00445380, this, _1));
		RBX::startListening<RBX::Selection, RBX::SelectionChanged>(m_unk0x1e0, this);
	}

	m_unk0x1c1 = true;

	boost::recursive_mutex::scoped_lock lock2(m_unk0x188);
	if (!m_unk0x1c0) {
		if (m_hWnd) {
			::PostMessage(m_hWnd, WM_USER + 101, 0, 0);
		}
		m_unk0x1c0 = true;
	}
}

// FUNCTION: ROBLOX 0x00445830
void CDataModelPropGrid::VTable0x00(
	boost::shared_ptr<RBX::DataModel> newDataModel, boost::shared_ptr<RBX::DataModel> oldDataModel
)
{
	if (m_unk0x194) {
		RBX::DataModel::Lock lock(m_unk0x194);
		FUN_00445580(NULL);
	}

	m_unk0x194 = newDataModel.get();
	if (m_unk0x194) {
		RBX::DataModel::Lock lock(m_unk0x194);
		FUN_00445580(m_unk0x194 ? m_unk0x194->create<RBX::Selection>() : NULL);
	}
}

// FUNCTION: ROBLOX 0x00444c80
void SoundItem::VTable0xf8()
{
	CString filter;
	filter.LoadString(134);
	CFileDialog dialog(TRUE, NULL, NULL, 0, filter, NULL, 0);
	if (dialog.DoModal() == IDOK) {
		RBX::Soundscape::SoundId id = RBX::ContentProvider::singleton().registerContent(
			std::ifstream((LPCTSTR) dialog.GetPathName(), std::ios::in | std::ios::binary),
			RBX::Name::declare("audio/x-wav", -1)
		);
		FUN_00443670(id);
	}
}

// FUNCTION: ROBLOX 0x00444e40
void TextureItem::VTable0xf8()
{
	CString filter;
	filter.LoadString(132);
	CFileDialog dialog(TRUE, NULL, NULL, 0, filter, NULL, 0);
	if (dialog.DoModal() == IDOK) {
		std::ifstream stream((LPCTSTR) dialog.GetPathName(), std::ios::in | std::ios::binary);
		RBX::TextureId id =
			RBX::ContentProvider::singleton().registerContent(stream, FUN_0043c120(dialog.GetPathName()));
		FUN_00443670(id);
	}
}

// clang-format off
BEGIN_MESSAGE_MAP(CPropGrid, CXTPPropertyGrid)
	ON_WM_CREATE()
	ON_MESSAGE(WM_USER + 101, FUN_0043e8a0)
END_MESSAGE_MAP()
// clang-format on

// FUNCTION: ROBLOX 0x00445960
CPropGrid::~CPropGrid()
{
}

// FUNCTION: ROBLOX 0x00445ae0
CPropGrid::CPropGrid() : m_unk0x194(NULL), m_unk0x1c0(false), m_unk0x1c1(false)
{
	m_bHelpVisible = FALSE;
}

template class XItem<float, CXTPPropertyGridItem, RBX::Reflection::TypedPropertyDescriptor<float> >;
template class XItem<int, CXTPPropertyGridItem, RBX::Reflection::TypedPropertyDescriptor<int> >;
template class XItem<bool, CXTPPropertyGridItemBool, RBX::Reflection::TypedPropertyDescriptor<bool> >;
template class XItem<G3D::Color3, CXTPPropertyGridItemColor, RBX::Reflection::TypedPropertyDescriptor<G3D::Color3> >;
template class XItem<G3D::Vector3, CXTPPropertyGridItem, RBX::Reflection::TypedPropertyDescriptor<G3D::Vector3> >;
template class XItem<
	RBX::BrickColor,
	CPropertyGridItemBrickColor,
	RBX::Reflection::TypedPropertyDescriptor<RBX::BrickColor> >;
template class XItem<int, CXTPPropertyGridItemEnum, RBX::Reflection::EnumPropertyDescriptor>;
template class XItem<std::string, CXTPPropertyGridItem, RBX::Reflection::PropertyDescriptor>;
template class XItem<
	RBX::Soundscape::SoundId,
	CXTPPropertyGridItem,
	RBX::Reflection::TypedPropertyDescriptor<RBX::Soundscape::SoundId> >;
template class XItem<RBX::TextureId, CXTPPropertyGridItem, RBX::Reflection::TypedPropertyDescriptor<RBX::TextureId> >;

template const RBX::FactoryProduct<RBX::Soundscape::SoundChannel, RBX::Instance, RBX::Soundscape::sSoundChannel>::
	Creator RBX::FactoryProduct<RBX::Soundscape::SoundChannel, RBX::Instance, RBX::Soundscape::sSoundChannel>::creator;
template const RBX::FactoryProduct<RBX::Soundscape::SoundService, RBX::Instance, RBX::Soundscape::sSoundService>::
	Creator RBX::FactoryProduct<RBX::Soundscape::SoundService, RBX::Instance, RBX::Soundscape::sSoundService>::creator;
