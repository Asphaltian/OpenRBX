#include "PropGrid.h"

#include "Roblox.h"
#include "script/Script.h"
#include "util/ContentProvider.h"
#include "v8datamodel/BrickColor.h"
#include "v8datamodel/Commands.h"
#include "v8datamodel/DataModel.h"

#include <G3D/GImage.h>
#include <algorithm>
#include <boost/bind.hpp>
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

// STUB: ROBLOX 0x0043bdc0
static CString FUN_0043bdc0(COLORREF color)
{
	RBX::BrickColor brickColor = RBX::BrickColor::closest(
		G3D::Color3(GetRValue(color) / 255.0, GetGValue(color) / 255.0, GetBValue(color) / 255.0)
	);
	return brickColor.name().c_str();
}

// STUB: ROBLOX 0x0043be40
void CPropertyGridItemBrickColor::SetColor(COLORREF clr)
{
	m_clrValue = clr;
	CXTPPropertyGridItem::SetValue(FUN_0043bdc0(clr));
}

// STUB: ROBLOX 0x0043be70
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
static const RBX::Name& FUN_0043c120(const char* path)
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

// STUB: ROBLOX 0x0043c7f0
void CPropertyGridItemBrickColor::OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton)
{
	STUB(0x43c7f0);
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

// STUB: ROBLOX 0x0043df20
void CPropGrid::onEvent(const RBX::Instance* source, RBX::PropertyChanged event)
{
	STUB(0x43df20);
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

// FUNCTION: ROBLOX 0x0043eba0
CPropGrid::Item::Item(const RBX::Reflection::PropertyDescriptor& descriptor, CPropGrid* grid) : m_unk0x14(grid)
{
	FUN_0043e120(&descriptor.owner);
}

// STUB: ROBLOX 0x0043f280
template <class T, class Base, class Descriptor>
void XItem<T, Base, Descriptor>::VTable0x00(
	const std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >& instances
)
{
	STUB(0x43f280);
}

template <class T, class Base, class Descriptor>
void XItem<T, Base, Descriptor>::FUN_00443670(const T& value)
{
	try {
		std::auto_ptr<RBX::XmlState> undoState(new RBX::UndoState());
		std::auto_ptr<RBX::XmlState> redoState(new RBX::RedoState());

		std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >::iterator
			objectsEnd = m_unk0x14->m_unk0x1d0.end();
		std::vector<const RBX::Reflection::ClassDescriptor*>::iterator end = m_unk0x04.end();
		for (std::vector<const RBX::Reflection::ClassDescriptor*>::iterator iter = m_unk0x04.begin(); iter != end;
			 ++iter) {
			const RBX::Reflection::ClassDescriptor* descriptor = *iter;
			std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >::iterator
				found = m_unk0x14->m_unk0x1d0.find(descriptor);
			if (found != objectsEnd) {
				std::vector<boost::shared_ptr<RBX::Instance> >& instances = found->second;
				for (std::vector<boost::shared_ptr<RBX::Instance> >::iterator instance = instances.begin(),
																			  instancesEnd = instances.end();
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

// STUB: ROBLOX 0x004419c0
BOOL XBoolItem::OnDrawItemValue(CDC& dc, CRect rcValue)
{
	STUB(0x4419c0);
	return FALSE;
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

// STUB: ROBLOX 0x00441b50
BOOL XBrickColorItem::OnDrawItemValue(CDC& dc, CRect rcValue)
{
	STUB(0x441b50);
	return FALSE;
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

// STUB: ROBLOX 0x00441cf0
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
RefItem::RefItem(const RBX::Reflection::PropertyDescriptor& descriptor, CPropGrid* grid)
	: XItem<std::string, CXTPPropertyGridItem, RBX::Reflection::PropertyDescriptor>(descriptor, grid),
	  m_unk0x128(&descriptor)
{
	SetReadOnly(TRUE);
}

// STUB: ROBLOX 0x00442180
void RefItem::VTable0x00(
	const std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >& instances
)
{
	STUB(0x442180);
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
	std::vector<const RBX::Reflection::ClassDescriptor*>::iterator end = m_unk0x04.end();
	for (std::vector<const RBX::Reflection::ClassDescriptor*>::iterator iter = m_unk0x04.begin(); iter != end; ++iter) {
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

// STUB: ROBLOX 0x00442620
void ScriptItem::OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton)
{
	std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >::iterator
		objectsEnd = m_unk0x14->m_unk0x1d0.end();
	for (std::vector<const RBX::Reflection::ClassDescriptor*>::iterator iter = m_unk0x04.begin(), end = m_unk0x04.end();
		 iter != end;
		 ++iter) {
		const RBX::Reflection::ClassDescriptor* descriptor = *iter;
		std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >::iterator
			found = m_unk0x14->m_unk0x1d0.find(descriptor);
		if (found != objectsEnd) {
			std::vector<boost::shared_ptr<RBX::Instance> >& instances = found->second;
			for (std::vector<boost::shared_ptr<RBX::Instance> >::iterator instance = instances.begin(),
																		  instancesEnd = instances.end();
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
		m_pConstraints->AddConstraint((*iter)->name.c_str(), (*iter)->value);
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

// STUB: ROBLOX 0x00445430
void CSelectionPropGrid::onEvent(const RBX::Selection* source, RBX::SelectionChanged event)
{
	STUB(0x445430);
}

// STUB: ROBLOX 0x00445580
void CSelectionPropGrid::FUN_00445580(RBX::Selection* selection)
{
	STUB(0x445580);
}

// STUB: ROBLOX 0x00445830
void CDataModelPropGrid::VTable0x00(
	boost::shared_ptr<RBX::DataModel> newDataModel, boost::shared_ptr<RBX::DataModel> oldDataModel
)
{
	STUB(0x445830);
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
