#include "PropGrid.h"

#include "v8datamodel/Commands.h"
#include "v8datamodel/DataModel.h"

#include <algorithm>
#include <boost/bind.hpp>
#include <cstdio>
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
