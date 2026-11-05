#ifndef PROPGRID_H
#define PROPGRID_H

#include "ActiveDocView.h"
#include "decomp.h"
#include "util/Events.h"
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
// TEMPLATE: ROBLOX 0x0043fd80
// ?VTable0xec@?$XItem@HVCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@H@Reflection@RBX@@@@UAE_NABH0@Z
// TEMPLATE: ROBLOX 0x004412e0
// ?VTable0xf0@?$XItem@MVCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@M@Reflection@RBX@@@@UAEXPAVInstance@RBX@@ABM@Z
// TEMPLATE: ROBLOX 0x00440550
// ??0?$XItem@VVector3@G3D@@VCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@VVector3@G3D@@@Reflection@RBX@@@@QAE@ABV?$TypedPropertyDescriptor@VVector3@G3D@@@Reflection@RBX@@PAVCPropGrid@@@Z
// TEMPLATE: ROBLOX 0x00440610
// ?VTable0xec@?$XItem@VColor3@G3D@@VCXTPPropertyGridItemColor@@V?$TypedPropertyDescriptor@VColor3@G3D@@@Reflection@RBX@@@@UAE_NABVColor3@G3D@@0@Z
// STUB: ROBLOX 0x00444560
// ?OnValueChanged@?$XItem@_NVCXTPPropertyGridItemBool@@V?$TypedPropertyDescriptor@_N@Reflection@RBX@@@@UAEXV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
// STUB: ROBLOX 0x004445e0
// ?OnValueChanged@?$XItem@MVCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@M@Reflection@RBX@@@@UAEXV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
// STUB: ROBLOX 0x00444660
// ?OnValueChanged@?$XItem@HVCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@H@Reflection@RBX@@@@UAEXV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
// STUB: ROBLOX 0x004446e0
// ?OnValueChanged@?$XItem@VColor3@G3D@@VCXTPPropertyGridItemColor@@V?$TypedPropertyDescriptor@VColor3@G3D@@@Reflection@RBX@@@@UAEXV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
// STUB: ROBLOX 0x00444880
// ?OnValueChanged@?$XItem@VVector3@G3D@@VCXTPPropertyGridItem@@V?$TypedPropertyDescriptor@VVector3@G3D@@@Reflection@RBX@@@@UAEXV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
// clang-format on

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
