#ifndef PROPGRID_H
#define PROPGRID_H

#include "ActiveDocView.h"
#include "decomp.h"
#include "util/Events.h"
#include "util/Sound.h"
#include "util/TextureId.h"
#include "v8datamodel/Selection.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <map>
#include <set>
#include <vector>

// SIZE 0x1dc
class CPropGrid : public CXTPPropertyGrid, public RBX::Listener<RBX::Instance, RBX::PropertyChanged>
{
	DECLARE_DYNAMIC(CPropGrid)

	// SYNTHETIC: ROBLOX 0x0043bb90
	// ?_GetBaseClass@CPropGrid@@KGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x0043bba0
	// ?GetThisClass@CPropGrid@@SGPAUCRuntimeClass@@XZ

public:
	CPropGrid();
	virtual ~CPropGrid();

	// SYNTHETIC: ROBLOX 0x00445c20
	// CPropGrid::`scalar deleting destructor'

	// SIZE 0x18
	class Item
	{
	public:
		Item(const RBX::Reflection::PropertyDescriptor& descriptor, CPropGrid* grid);

		virtual void VTable0x00(
			const std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >&
				instances
		) = 0; // vtable+0x00

	protected:
		void FUN_0043e120(const RBX::Reflection::ClassDescriptor* classDescriptor);

		std::vector<const RBX::Reflection::ClassDescriptor*> m_unk0x04; // 0x04
		CPropGrid* m_unk0x14;                                           // 0x14
	};

protected:
	virtual void onEvent(const RBX::Instance* source, RBX::PropertyChanged event);

private:
	template <class T, class Base, class Descriptor>
	friend class XItem;
	friend class Vector3ComponentItem;
	friend class ScriptItem;

	boost::recursive_mutex m_unk0x188;                                            // 0x188
	RBX::DataModel* m_unk0x194;                                                   // 0x194
	std::vector<undefined4*> m_unk0x198;                                          // 0x198
	std::map<const RBX::Reflection::PropertyDescriptor*, undefined4*> m_unk0x1a8; // 0x1a8
	std::map<undefined4, undefined4> m_unk0x1b4;                                  // 0x1b4
	bool m_unk0x1c0;                                                              // 0x1c0
	bool m_unk0x1c1;                                                              // 0x1c1
	std::set<undefined4*> m_unk0x1c4;                                             // 0x1c4
	std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >
		m_unk0x1d0; // 0x1d0
};

DECOMP_SIZE_ASSERT(CPropGrid, 0x1dc)

// SIZE 0x118
class CPropertyGridItemBrickColor : public CXTPPropertyGridItemColor
{
public:
	CPropertyGridItemBrickColor(CString strCaption, COLORREF clr = 0);

	virtual void SetValue(CString strValue);
	virtual void SetColor(COLORREF clr);

protected:
	virtual void OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton);
};

DECOMP_SIZE_ASSERT(CPropertyGridItemBrickColor, 0x118)

// SIZE 0x128
template <class T, class Base, class Descriptor>
class XItem : public Base, public CPropGrid::Item
{
public:
	XItem(const Descriptor& descriptor, CPropGrid* grid)
		: Base(descriptor.name.c_str()), Item(descriptor, grid), m_unk0x124(&descriptor)
	{
		m_bHidden = TRUE;
		SetReadOnly(descriptor.isReadOnly());
	}

	virtual void OnValueChanged(CString strValue)
	{
		Base::OnValueChanged(strValue);
		FUN_00443670(VTable0xe4());
	}

	virtual void VTable0x00(
		const std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >&
			instances
	);

	virtual T VTable0xe4() // vtable+0xe4
	{
		T result;
		if (RBX::StringConverter<T>::convertToValue((LPCTSTR) GetValue(), result)) {
			return result;
		}
		return T();
	}
	virtual void VTable0xe8(const T& value);                 // vtable+0xe8
	virtual bool VTable0xec(const T& first, const T& second) // vtable+0xec
	{
		return first == second;
	}
	virtual void VTable0xf0(RBX::Instance* instance, const T& value) // vtable+0xf0
	{
		RBX::DataModel::Lock lock(m_unk0x14->m_unk0x194);
		m_unk0x124->setValue(instance, value);
	}
	virtual bool VTable0xf4() // vtable+0xf4
	{
		return GetValue().IsEmpty();
	}

protected:
	void FUN_00443670(const T& value);

	const Descriptor* m_unk0x124; // 0x124
};

// clang-format off
// TEMPLATE: ROBLOX 0x0043c5f0
// ?VTable0xe4@?$XItem@MVCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@M@Reflection@RBX@@@@UAEMXZ
// TEMPLATE: ROBLOX 0x0043c6a0
// ?VTable0xe4@?$XItem@HVCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@H@Reflection@RBX@@@@UAEHXZ
// TEMPLATE: ROBLOX 0x0043ca00
// ?VTable0xe4@?$XItem@VVector3@G3D@@VCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@VVector3@G3D@@@Reflection@RBX@@@@UAE?AVVector3@G3D@@XZ
// TEMPLATE: ROBLOX 0x0043ec10
// ??0?$XItem@_NVCXTPPropertyGridItemBool@@V?$TypedPropertyDescriptor@_N@Reflection@RBX@@@@QAE@ABV?$TypedPropertyDescriptor@_N@Reflection@RBX@@PAVCPropGrid@@@Z
// TEMPLATE: ROBLOX 0x0043ecd0
// ?VTable0xec@?$XItem@_NVCXTPPropertyGridItemBool@@V?$TypedPropertyDescriptor@_N@Reflection@RBX@@@@UAE_NAB_N0@Z
// TEMPLATE: ROBLOX 0x0043ed90
// ?VTable0xf0@?$XItem@_NVCXTPPropertyGridItemBool@@V?$TypedPropertyDescriptor@_N@Reflection@RBX@@@@UAEXPAVInstance@RBX@@AB_N@Z
// SYNTHETIC: ROBLOX 0x0043f0a0
// ??_E?$XItem@_NVCXTPPropertyGridItemBool@@V?$TypedPropertyDescriptor@_N@Reflection@RBX@@@@UAEPAXI@Z
// TEMPLATE: ROBLOX 0x0043f100
// ??0?$XItem@MVCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@M@Reflection@RBX@@@@QAE@ABV?$TypedPropertyDescriptor@M@Reflection@RBX@@PAVCPropGrid@@@Z
// TEMPLATE: ROBLOX 0x0043f1c0
// ?VTable0xec@?$XItem@MVCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@M@Reflection@RBX@@@@UAE_NABM0@Z
// TEMPLATE: ROBLOX 0x0043f490
// ??0?$XItem@HVCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@H@Reflection@RBX@@@@QAE@ABV?$TypedPropertyDescriptor@H@Reflection@RBX@@PAVCPropGrid@@@Z
// TEMPLATE: ROBLOX 0x0043f560
// ?VTable0xf4@?$XItem@MVCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@M@Reflection@RBX@@@@UAE_NXZ
// SYNTHETIC: ROBLOX 0x0043f830
// ??_E?$XItem@MVCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@M@Reflection@RBX@@@@UAEPAXI@Z
// TEMPLATE: ROBLOX 0x0043f890
// ??0?$XItem@VColor3@G3D@@VCXTPPropertyGridItemColor@@V?$TypedPropertyDescriptor@VColor3@G3D@@@Reflection@RBX@@@@QAE@ABV?$TypedPropertyDescriptor@VColor3@G3D@@@Reflection@RBX@@PAVCPropGrid@@@Z
// TEMPLATE: ROBLOX 0x0043f9a0
// ?VTable0xf0@?$XItem@VColor3@G3D@@VCXTPPropertyGridItemColor@@V?$TypedPropertyDescriptor@VColor3@G3D@@@Reflection@RBX@@@@UAEXPAVInstance@RBX@@ABVColor3@G3D@@@Z
// SYNTHETIC: ROBLOX 0x0043fc60
// ??_E?$XItem@VColor3@G3D@@VCXTPPropertyGridItemColor@@V?$TypedPropertyDescriptor@VColor3@G3D@@@Reflection@RBX@@@@UAEPAXI@Z
// TEMPLATE: ROBLOX 0x0043fcc0
// ??0?$XItem@VBrickColor@RBX@@VCPropertyGridItemBrickColor@@V?$TypedPropertyDescriptor@VBrickColor@RBX@@@Reflection@2@@@QAE@ABV?$TypedPropertyDescriptor@VBrickColor@RBX@@@Reflection@RBX@@PAVCPropGrid@@@Z
// TEMPLATE: ROBLOX 0x0043fd80
// ?VTable0xec@?$XItem@HVCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@H@Reflection@RBX@@@@UAE_NABH0@Z
// TEMPLATE: ROBLOX 0x00440920
// ??0?$XItem@HVCXTPPropertyGridItemEnum@@VEnumPropertyDescriptor@Reflection@RBX@@@@QAE@ABVEnumPropertyDescriptor@Reflection@RBX@@PAVCPropGrid@@@Z
// TEMPLATE: ROBLOX 0x00440d10
// ??0?$XItem@VSoundId@Soundscape@RBX@@VCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@VSoundId@Soundscape@RBX@@@Reflection@3@@@QAE@ABV?$TypedPropertyDescriptor@VSoundId@Soundscape@RBX@@@Reflection@RBX@@PAVCPropGrid@@@Z
// TEMPLATE: ROBLOX 0x00440e30 FOLDED
// ?VTable0xec@?$XItem@VSoundId@Soundscape@RBX@@VCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@VSoundId@Soundscape@RBX@@@Reflection@3@@@UAE_NABVSoundId@Soundscape@RBX@@0@Z
// TEMPLATE: ROBLOX 0x00440e30 FOLDED
// ?VTable0xec@?$XItem@VTextureId@RBX@@VCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@VTextureId@RBX@@@Reflection@2@@@UAE_NABVTextureId@RBX@@0@Z
// TEMPLATE: ROBLOX 0x00441150
// ??0?$XItem@VTextureId@RBX@@VCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@VTextureId@RBX@@@Reflection@2@@@QAE@ABV?$TypedPropertyDescriptor@VTextureId@RBX@@@Reflection@RBX@@PAVCPropGrid@@@Z
// TEMPLATE: ROBLOX 0x004412e0
// ?VTable0xf0@?$XItem@MVCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@M@Reflection@RBX@@@@UAEXPAVInstance@RBX@@ABM@Z
// TEMPLATE: ROBLOX 0x00440060
// ??0?$XItem@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@VCXTPPropertyGridItem@@VPropertyDescriptor@Reflection@RBX@@@@QAE@ABVPropertyDescriptor@Reflection@RBX@@PAVCPropGrid@@@Z
// TEMPLATE: ROBLOX 0x00440120
// ?VTable0xec@?$XItem@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@VCXTPPropertyGridItem@@VPropertyDescriptor@Reflection@RBX@@@@UAE_NABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@0@Z
// TEMPLATE: ROBLOX 0x00440550
// ??0?$XItem@VVector3@G3D@@VCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@VVector3@G3D@@@Reflection@RBX@@@@QAE@ABV?$TypedPropertyDescriptor@VVector3@G3D@@@Reflection@RBX@@PAVCPropGrid@@@Z
// TEMPLATE: ROBLOX 0x00440610
// ?VTable0xec@?$XItem@VColor3@G3D@@VCXTPPropertyGridItemColor@@V?$TypedPropertyDescriptor@VColor3@G3D@@@Reflection@RBX@@@@UAE_NABVColor3@G3D@@0@Z
// STUB: ROBLOX 0x004432b0
// ?FUN_00443670@?$XItem@_NVCXTPPropertyGridItemBool@@V?$TypedPropertyDescriptor@_N@Reflection@RBX@@@@IAEXAB_N@Z
// SYNTHETIC: ROBLOX 0x00442920
// ??_E?$XItem@HVCXTPPropertyGridItemEnum@@VEnumPropertyDescriptor@Reflection@RBX@@@@UAEPAXI@Z
// STUB: ROBLOX 0x00443670
// ?FUN_00443670@?$XItem@MVCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@M@Reflection@RBX@@@@IAEXABM@Z
// STUB: ROBLOX 0x00443a30
// ?FUN_00443670@?$XItem@VColor3@G3D@@VCXTPPropertyGridItemColor@@V?$TypedPropertyDescriptor@VColor3@G3D@@@Reflection@RBX@@@@IAEXABVColor3@G3D@@@Z
// STUB: ROBLOX 0x00443df0
// ?FUN_00443670@?$XItem@HVCXTPPropertyGridItemEnum@@VEnumPropertyDescriptor@Reflection@RBX@@@@IAEXABH@Z
// TEMPLATE: ROBLOX 0x00444560
// ?OnValueChanged@?$XItem@_NVCXTPPropertyGridItemBool@@V?$TypedPropertyDescriptor@_N@Reflection@RBX@@@@UAEXV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
// TEMPLATE: ROBLOX 0x004445e0
// ?OnValueChanged@?$XItem@MVCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@M@Reflection@RBX@@@@UAEXV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
// TEMPLATE: ROBLOX 0x00444660
// ?OnValueChanged@?$XItem@HVCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@H@Reflection@RBX@@@@UAEXV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
// TEMPLATE: ROBLOX 0x004446e0
// ?OnValueChanged@?$XItem@VColor3@G3D@@VCXTPPropertyGridItemColor@@V?$TypedPropertyDescriptor@VColor3@G3D@@@Reflection@RBX@@@@UAEXV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
// TEMPLATE: ROBLOX 0x00444760
// ?OnValueChanged@?$XItem@VBrickColor@RBX@@VCPropertyGridItemBrickColor@@V?$TypedPropertyDescriptor@VBrickColor@RBX@@@Reflection@2@@@UAEXV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
// TEMPLATE: ROBLOX 0x004447e0
// ?OnValueChanged@?$XItem@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@VCXTPPropertyGridItem@@VPropertyDescriptor@Reflection@RBX@@@@UAEXV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
// TEMPLATE: ROBLOX 0x00444880
// ?OnValueChanged@?$XItem@VVector3@G3D@@VCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@VVector3@G3D@@@Reflection@RBX@@@@UAEXV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
// TEMPLATE: ROBLOX 0x004449d0
// ?OnValueChanged@?$XItem@HVCXTPPropertyGridItemEnum@@VEnumPropertyDescriptor@Reflection@RBX@@@@UAEXV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
// TEMPLATE: ROBLOX 0x00445160 FOLDED
// ?OnValueChanged@?$XItem@VSoundId@Soundscape@RBX@@VCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@VSoundId@Soundscape@RBX@@@Reflection@3@@@UAEXV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
// TEMPLATE: ROBLOX 0x00445160 FOLDED
// ?OnValueChanged@?$XItem@VTextureId@RBX@@VCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@VTextureId@RBX@@@Reflection@2@@@UAEXV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
// clang-format on

template <>
std::string XItem<std::string, CXTPPropertyGridItem, RBX::Reflection::PropertyDescriptor>::VTable0xe4();
template <>
void XItem<std::string, CXTPPropertyGridItem, RBX::Reflection::PropertyDescriptor>::VTable0xe8(
	const std::string& value
);
template <>
void XItem<std::string, CXTPPropertyGridItem, RBX::Reflection::PropertyDescriptor>::VTable0xf0(
	RBX::Instance* instance, const std::string& value
);
template <>
int XItem<int, CXTPPropertyGridItemEnum, RBX::Reflection::EnumPropertyDescriptor>::VTable0xe4();
template <>
void XItem<int, CXTPPropertyGridItemEnum, RBX::Reflection::EnumPropertyDescriptor>::VTable0xe8(const int& value);
template <>
void XItem<int, CXTPPropertyGridItemEnum, RBX::Reflection::EnumPropertyDescriptor>::VTable0xf0(
	RBX::Instance* instance, const int& value
);

// SIZE 0x13c
class XBoolItem : public XItem<bool, CXTPPropertyGridItemBool, RBX::Reflection::TypedPropertyDescriptor<bool> >
{
public:
	XBoolItem(const RBX::Reflection::TypedPropertyDescriptor<bool>& descriptor, CPropGrid* grid);

	virtual BOOL OnDrawItemValue(CDC& dc, CRect rcValue);
};

DECOMP_SIZE_ASSERT(XBoolItem, 0x13c)

// SIZE 0x134
class XBrickColorItem : public XItem<
							RBX::BrickColor,
							CPropertyGridItemBrickColor,
							RBX::Reflection::TypedPropertyDescriptor<RBX::BrickColor> >
{
public:
	XBrickColorItem(const RBX::Reflection::TypedPropertyDescriptor<RBX::BrickColor>& descriptor, CPropGrid* grid);

	virtual BOOL OnDrawItemValue(CDC& dc, CRect rcValue);
};

DECOMP_SIZE_ASSERT(XBrickColorItem, 0x134)

// SIZE 0x138
class Vector3ComponentItem
	: public XItem<G3D::Vector3, CXTPPropertyGridItem, RBX::Reflection::TypedPropertyDescriptor<G3D::Vector3> >
{
public:
	Vector3ComponentItem(
		const RBX::Reflection::TypedPropertyDescriptor<G3D::Vector3>& descriptor, CPropGrid* grid, int component
	);

	virtual G3D::Vector3 VTable0xe4();                                              // vtable+0xe4
	virtual void VTable0xe8(const G3D::Vector3& value);                             // vtable+0xe8
	virtual bool VTable0xec(const G3D::Vector3& first, const G3D::Vector3& second); // vtable+0xec
	virtual void VTable0xf0(RBX::Instance* instance, const G3D::Vector3& value);    // vtable+0xf0

private:
	int m_unk0x128;          // 0x128
	G3D::Vector3 m_unk0x12c; // 0x12c
};

DECOMP_SIZE_ASSERT(Vector3ComponentItem, 0x138)

// SIZE 0x134
class Vector3Item
	: public XItem<G3D::Vector3, CXTPPropertyGridItem, RBX::Reflection::TypedPropertyDescriptor<G3D::Vector3> >
{
public:
	Vector3Item(const RBX::Reflection::TypedPropertyDescriptor<G3D::Vector3>& descriptor, CPropGrid* grid);

	virtual BOOL OnAfterEdit(CString& strValue);
	virtual void VTable0x00(
		const std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >&
			instances
	);

protected:
	virtual void OnAddChildItem();

private:
	Vector3ComponentItem* m_unk0x128[3]; // 0x128
};

DECOMP_SIZE_ASSERT(Vector3Item, 0x134)

// SIZE 0x12c
class RefItem : public XItem<std::string, CXTPPropertyGridItem, RBX::Reflection::PropertyDescriptor>
{
public:
	RefItem(const RBX::Reflection::PropertyDescriptor& descriptor, CPropGrid* grid);

	virtual void OnValueChanged(CString strValue);
	virtual void VTable0x00(
		const std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >&
			instances
	);

private:
	const RBX::Reflection::PropertyDescriptor* m_unk0x128; // 0x128
};

DECOMP_SIZE_ASSERT(RefItem, 0x12c)

// SIZE 0x128
class ScriptItem : public XItem<std::string, CXTPPropertyGridItem, RBX::Reflection::PropertyDescriptor>
{
public:
	ScriptItem(const RBX::Reflection::PropertyDescriptor& descriptor, CPropGrid* grid);

	virtual void OnValueChanged(CString strValue);
	virtual void VTable0x00(
		const std::map<const RBX::Reflection::ClassDescriptor*, std::vector<boost::shared_ptr<RBX::Instance> > >&
			instances
	);

protected:
	virtual void OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton);
};

DECOMP_SIZE_ASSERT(ScriptItem, 0x128)

// SIZE 0x130
class EnumItem : public XItem<int, CXTPPropertyGridItemEnum, RBX::Reflection::EnumPropertyDescriptor>
{
public:
	EnumItem(const RBX::Reflection::EnumPropertyDescriptor& descriptor, CPropGrid* grid);
};

DECOMP_SIZE_ASSERT(EnumItem, 0x130)

// SIZE 0x128
template <class T>
class ContentItem : public XItem<T, CXTPPropertyGridItem, RBX::Reflection::TypedPropertyDescriptor<T> >
{
public:
	ContentItem(const RBX::Reflection::TypedPropertyDescriptor<T>& descriptor, CPropGrid* grid)
		: XItem<T, CXTPPropertyGridItem, RBX::Reflection::TypedPropertyDescriptor<T> >(descriptor, grid)
	{
		this->SetFlags(this->GetFlags() | xtpGridItemHasComboButton);
		this->GetConstraints()->AddConstraint("Browse...", 0, ID_FILE_OPEN);
		this->GetConstraints()->AddConstraint("Clear", 1, ID_EDIT_CLEAR);
	}

	virtual void OnValueChanged(CString strValue)
	{
		switch (this->GetConstraints()->GetCurrent()) {
		case 0:
			VTable0xf8();
			break;
		default:
			this->FUN_00443670(T((LPCTSTR) strValue));
			break;
		case 1:
			this->FUN_00443670(T());
			break;
		}
	}

	virtual void VTable0xf8() = 0; // vtable+0xf8
};

// clang-format off
// TEMPLATE: ROBLOX 0x00442980
// ??0?$ContentItem@VSoundId@Soundscape@RBX@@@@QAE@ABV?$TypedPropertyDescriptor@VSoundId@Soundscape@RBX@@@Reflection@RBX@@PAVCPropGrid@@@Z
// TEMPLATE: ROBLOX 0x00442a40
// ??0?$ContentItem@VTextureId@RBX@@@@QAE@ABV?$TypedPropertyDescriptor@VTextureId@RBX@@@Reflection@RBX@@PAVCPropGrid@@@Z
// TEMPLATE: ROBLOX 0x00444900 FOLDED
// ?OnValueChanged@?$ContentItem@VSoundId@Soundscape@RBX@@@@UAEXV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
// TEMPLATE: ROBLOX 0x00444900 FOLDED
// ?OnValueChanged@?$ContentItem@VTextureId@RBX@@@@UAEXV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
// clang-format on

// SIZE 0x128
class SoundItem : public ContentItem<RBX::Soundscape::SoundId>
{
public:
	SoundItem(const RBX::Reflection::TypedPropertyDescriptor<RBX::Soundscape::SoundId>& descriptor, CPropGrid* grid);

	virtual void VTable0xf8(); // vtable+0xf8
};

DECOMP_SIZE_ASSERT(SoundItem, 0x128)

// SIZE 0x128
class TextureItem : public ContentItem<RBX::TextureId>
{
public:
	TextureItem(const RBX::Reflection::TypedPropertyDescriptor<RBX::TextureId>& descriptor, CPropGrid* grid);

	virtual void VTable0xf8(); // vtable+0xf8
};

DECOMP_SIZE_ASSERT(TextureItem, 0x128)

// SIZE 0x1e8
class CSelectionPropGrid : public CPropGrid, public RBX::Listener<RBX::Selection, RBX::SelectionChanged>
{
	DECLARE_DYNAMIC(CSelectionPropGrid)

	// SYNTHETIC: ROBLOX 0x0043bbb0
	// ?GetThisClass@CSelectionPropGrid@@SGPAUCRuntimeClass@@XZ

public:
	CSelectionPropGrid() {}

	// FUNCTION: ROBLOX 0x00434880
	virtual ~CSelectionPropGrid() { FUN_00445580(NULL); }

	void FUN_00445580(RBX::Selection* selection);

protected:
	virtual void onEvent(const RBX::Selection* source, RBX::SelectionChanged event);

private:
	boost::shared_ptr<RBX::Selection> m_unk0x1e0; // 0x1e0
};

DECOMP_SIZE_ASSERT(CSelectionPropGrid, 0x1e8)

// SIZE 0x1ec
class CDataModelPropGrid : public CSelectionPropGrid, public ActiveDocView
{
	DECLARE_DYNAMIC(CDataModelPropGrid)

	// SYNTHETIC: ROBLOX 0x0043bbc0
	// ?GetThisClass@CDataModelPropGrid@@SGPAUCRuntimeClass@@XZ

public:
	// FUNCTION: ROBLOX 0x00435320
	CDataModelPropGrid() {}

	// SYNTHETIC: ROBLOX 0x004353e0
	// CDataModelPropGrid::~CDataModelPropGrid

	// SYNTHETIC: ROBLOX 0x00435440
	// CDataModelPropGrid::`scalar deleting destructor'

protected:
	virtual void VTable0x00(
		boost::shared_ptr<RBX::DataModel> newDataModel, boost::shared_ptr<RBX::DataModel> oldDataModel
	);
};

DECOMP_SIZE_ASSERT(CDataModelPropGrid, 0x1ec)

#endif // PROPGRID_H
