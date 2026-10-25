#ifndef REFLECTION_PROPERTY_H
#define REFLECTION_PROPERTY_H

#include "decomp.h"
#include "reflection/enumconverter.h"
#include "reflection/member.h"
#include "reflection/type.h"
#include "util/MeshId.h"
#include "util/TextureId.h"
#include "v8datamodel/BrickColor.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/Vector3.h>
#include <boost/cast.hpp>
#include <memory>
#include <string>
#include <typeinfo>

class XmlElement;

namespace RBX {

class IReferenceBinder;

extern const char* const category_Part;

namespace Reflection {

class ConstProperty;
class DescribedBase;
class Property;
class EnumDescriptor;

// SIZE 0x18
class __declspec(novtable) PropertyDescriptor : public MemberDescriptor
{
public:
	enum Functionality
	{
		LEGACY = 0,
		UI = 1,
		STREAMING = 4,
		STANDARD = 5,
	};

	typedef ConstProperty ConstMember;
	typedef Property Member;

	virtual bool isReadOnly() const = 0;                                                      // vtable+0x04
	virtual bool equalValues(const DescribedBase* a, const DescribedBase* b) const = 0;       // vtable+0x08
	virtual bool hasStringValue() const = 0;                                                  // vtable+0x0c
	virtual std::string getStringValue(const DescribedBase* instance) const = 0;              // vtable+0x10
	virtual bool setStringValue(DescribedBase* instance, const std::string& value) const = 0; // vtable+0x14
	virtual void read(
		DescribedBase* instance,
		const XmlElement* element,
		IReferenceBinder& binder
	) const; // vtable+0x18

	XmlElement* write(const DescribedBase* instance, bool ignoreWriteProtection) const;

	bool isPublic() const { return bIsPublic; }
	bool canStreamWrite() const { return bCanStreamWrite; }

	bool operator==(const PropertyDescriptor& other) const { return this == &other; }

	unsigned int bIsPublic : 1;       // 0x10
	unsigned int bCanStreamWrite : 1; // 0x10

	const Type& type; // 0x14

protected:
	PropertyDescriptor(
		ClassDescriptor& classDescriptor,
		const Type& type,
		const char* name,
		const char* category,
		Functionality flags
	);

private:
	virtual void writeValue(const DescribedBase* instance, XmlElement* element) const = 0; // vtable+0x1c
	virtual void readValue(
		DescribedBase* instance,
		const XmlElement* element,
		IReferenceBinder& binder
	) const = 0; // vtable+0x20
};

DECOMP_SIZE_ASSERT(PropertyDescriptor, 0x18)

// FUNCTION: WEBSERVICE 0x1003ea40
inline std::string PropertyDescriptor::getStringValue(const DescribedBase* instance) const
{
	hasStringValue();

	return "";
}

inline bool PropertyDescriptor::setStringValue(DescribedBase* instance, const std::string& value) const
{
	hasStringValue();

	return false;
}

// SIZE 0x1c
template <class T>
class TypedPropertyDescriptor : public PropertyDescriptor
{
public:
	// SIZE 0x04
	class __declspec(novtable) GetSet
	{
	public:
		virtual bool isReadOnly() const = 0;                                      // vtable+0x00
		virtual T getValue(const DescribedBase* instance) const = 0;              // vtable+0x04
		virtual void setValue(DescribedBase* instance, const T& value) const = 0; // vtable+0x08
	};

	T getValue(const DescribedBase* instance) const { return getset->getValue(instance); }

	void setValue(DescribedBase* instance, const T& value) const { getset->setValue(instance, value); }

	virtual bool isReadOnly() const { return getset->isReadOnly(); }

	virtual bool equalValues(const DescribedBase* a, const DescribedBase* b) const
	{
		return getValue(a) == getValue(b);
	}

	virtual bool hasStringValue() const { return false; }

	virtual std::string getStringValue(const DescribedBase* instance) const;

	virtual bool setStringValue(DescribedBase* instance, const std::string& value) const;

protected:
	TypedPropertyDescriptor(
		ClassDescriptor& classDescriptor,
		const char* name,
		const char* category,
		std::auto_ptr<GetSet> getset,
		Functionality flags
	)
		: PropertyDescriptor(classDescriptor, Type::singleton<T>(), name, category, flags), getset(getset)
	{
	}

	TypedPropertyDescriptor(
		ClassDescriptor& classDescriptor,
		const Type& type,
		const char* name,
		const char* category,
		std::auto_ptr<GetSet> getset,
		Functionality flags
	)
		: PropertyDescriptor(classDescriptor, type, name, category, flags), getset(getset)
	{
	}

private:
	virtual void writeValue(const DescribedBase* instance, XmlElement* element) const;

	virtual void readValue(DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder) const;

protected:
	std::auto_ptr<GetSet> getset; // 0x18
};

template <class T>
std::string TypedPropertyDescriptor<T>::getStringValue(const DescribedBase* instance) const
{
	return PropertyDescriptor::getStringValue(instance);
}

template <class T>
bool TypedPropertyDescriptor<T>::setStringValue(DescribedBase* instance, const std::string& value) const
{
	return PropertyDescriptor::setStringValue(instance, value);
}

template <>
std::string TypedPropertyDescriptor<std::string>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<std::string>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<std::string>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<std::string>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
std::string TypedPropertyDescriptor<bool>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<bool>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<bool>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<bool>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
std::string TypedPropertyDescriptor<float>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<float>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<float>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<float>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
std::string TypedPropertyDescriptor<double>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<double>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<double>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<double>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
std::string TypedPropertyDescriptor<int>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<int>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<int>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<int>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
std::string TypedPropertyDescriptor<G3D::Vector3>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<G3D::Vector3>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<G3D::Vector3>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<G3D::Vector3>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
std::string TypedPropertyDescriptor<G3D::Color3>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<G3D::Color3>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<G3D::Color3>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<G3D::Color3>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
void TypedPropertyDescriptor<BrickColor>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<BrickColor>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
std::string TypedPropertyDescriptor<ContentId>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<ContentId>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<ContentId>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<ContentId>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
std::string TypedPropertyDescriptor<TextureId>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<TextureId>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<TextureId>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<TextureId>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
std::string TypedPropertyDescriptor<MeshId>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<MeshId>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<MeshId>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<MeshId>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
void TypedPropertyDescriptor<G3D::CoordinateFrame>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<G3D::CoordinateFrame>::writeValue(
	const DescribedBase* instance,
	XmlElement* element
) const;

// SIZE 0x1c
class __declspec(novtable) EnumPropertyDescriptor : public PropertyDescriptor
{
public:
	const EnumDescriptor& enumDescriptor; // 0x18

	virtual unsigned int getIndexValue(const DescribedBase* instance) const = 0;       // vtable+0x24
	virtual bool setIndexValue(DescribedBase* instance, unsigned int index) const = 0; // vtable+0x28
	virtual int getEnumValue(const DescribedBase* instance) const = 0;                 // vtable+0x2c
	virtual bool setEnumValue(DescribedBase* instance, int intValue) const = 0;        // vtable+0x30

protected:
	EnumPropertyDescriptor(
		ClassDescriptor& classDescriptor,
		const EnumDescriptor& enumDescriptor,
		const char* name,
		const char* category,
		Functionality functionality
	)
		: PropertyDescriptor(classDescriptor, enumDescriptor, name, category, functionality),
		  enumDescriptor(enumDescriptor)
	{
	}
};

DECOMP_SIZE_ASSERT(EnumPropertyDescriptor, 0x1c)

// SIZE 0x18
class __declspec(novtable) RefPropertyDescriptor : public PropertyDescriptor
{
public:
	virtual DescribedBase* getRefValue(const DescribedBase* instance) const = 0;       // vtable+0x24
	virtual void setRefValue(DescribedBase* instance, DescribedBase* value) const = 0; // vtable+0x28

protected:
	RefPropertyDescriptor(
		ClassDescriptor& classDescriptor,
		const Type& type,
		const char* name,
		const char* category,
		Functionality functionality
	)
		: PropertyDescriptor(classDescriptor, type, name, category, functionality)
	{
	}

	virtual bool hasStringValue() const { return false; }

	// FUNCTION: WEBSERVICE 0x10101910 FOLDED
	// RBX::Reflection::RefPropertyDescriptor::getStringValue

	virtual std::string getStringValue(const DescribedBase* instance) const
	{
		return PropertyDescriptor::getStringValue(instance);
	}

	// FUNCTION: WEBSERVICE 0x1003ea80 FOLDED
	// RBX::Reflection::RefPropertyDescriptor::setStringValue

	virtual bool setStringValue(DescribedBase* instance, const std::string& value) const
	{
		hasStringValue();

		return false;
	}
};

DECOMP_SIZE_ASSERT(RefPropertyDescriptor, 0x18)

// SIZE 0x1c
template <class Class, class T>
class PropDescriptor : public TypedPropertyDescriptor<T>
{
public:
	template <class Getter, class Setter>
	class GetSetImpl : public TypedPropertyDescriptor<T>::GetSet
	{
	public:
		GetSetImpl(Getter get, Setter set) : get(get), set(set) {}

		virtual bool isReadOnly() const { return false; }

		virtual T getValue(const DescribedBase* object) const
		{
			const Class* instance = static_cast<const Class*>(object);
			return (instance->*get)();
		}

		virtual void setValue(DescribedBase* object, const T& value) const
		{
			(static_cast<Class*>(object)->*set)(value);
		}

	private:
		Getter get;
		Setter set;
	};

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10042e80
	// RBX::Reflection::PropDescriptor<RBX::Instance,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >::GetImpl<std::basic_string<char,std::char_traits<char>,std::allocator<char> > (__thiscall RBX::Instance::*)(void)const >::getValue
	// TEMPLATE: WEBSERVICE 0x100b3730
	// RBX::Reflection::PropDescriptor<RBX::Instance,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >::GetImpl<std::basic_string<char,std::char_traits<char>,std::allocator<char> > (__thiscall RBX::Instance::*)(void)const >::setValue
	// clang-format on
	template <class Getter>
	class GetImpl : public TypedPropertyDescriptor<T>::GetSet
	{
	public:
		GetImpl(Getter get) : get(get) {}

		virtual bool isReadOnly() const { return true; }

		virtual T getValue(const DescribedBase* object) const
		{
			const Class* instance = static_cast<const Class*>(object);
			return (instance->*get)();
		}

		virtual void setValue(DescribedBase* object, const T& value) const
		{
			throw std::runtime_error("can't set value");
		}

	private:
		Getter get;
	};

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10099380
	// RBX::Reflection::PropDescriptor<RBX::PVInstance,G3D::CoordinateFrame>::SetImpl<void (__thiscall RBX::PVInstance::*)(G3D::CoordinateFrame const &)>::setValue
	// TEMPLATE: WEBSERVICE 0x100c75e0
	// RBX::Reflection::PropDescriptor<RBX::PVInstance,G3D::CoordinateFrame>::SetImpl<void (__thiscall RBX::PVInstance::*)(G3D::CoordinateFrame const &)>::getValue
	// clang-format on
	template <class Setter>
	class SetImpl : public TypedPropertyDescriptor<T>::GetSet
	{
	public:
		SetImpl(Setter set) : set(set) {}

		virtual bool isReadOnly() const { return false; }

		virtual T getValue(const DescribedBase* object) const { throw std::runtime_error("can't get value"); }

		virtual void setValue(DescribedBase* object, const T& value) const
		{
			Class* instance = static_cast<Class*>(object);
			(instance->*set)(value);
		}

	private:
		Setter set;
	};

	// clang-format off
	// FUNCTION: WEBSERVICE 0x100433c0
	// RBX::Reflection::TypedPropertyDescriptor<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >::TypedPropertyDescriptor<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >
	// FUNCTION: WEBSERVICE 0x10043430
	// RBX::Reflection::TypedPropertyDescriptor<G3D::Color3>::TypedPropertyDescriptor<G3D::Color3>
	// TEMPLATE: WEBSERVICE 0x1004b660
	// RBX::Reflection::BoundProp<bool,1>::BoundProp<bool,1><RBX::Instance>
	// TEMPLATE: WEBSERVICE 0x10068f70
	// RBX::Reflection::PropDescriptor<RBX::Script,RBX::ContentId>::getset<RBX::ContentId const & (__thiscall RBX::Script::*)(void)const ,void (__thiscall RBX::Script::*)(RBX::ContentId const &)>
	// TEMPLATE: WEBSERVICE 0x10069740
	// RBX::Reflection::BoundProp<bool,1>::BoundProp<bool,1><RBX::Script>
	// TEMPLATE: WEBSERVICE 0x100ebfe0
	// RBX::Reflection::PropDescriptor<RBX::Rocket,RBX::PartInstance *>::getset<RBX::PartInstance * (__thiscall RBX::Rocket::*)(void)const ,void (__thiscall RBX::Rocket::*)(RBX::PartInstance *)>
	// TEMPLATE: WEBSERVICE 0x100ec280
	// RBX::Reflection::BoundProp<float,1>::BoundProp<float,1><RBX::BodyPosition>
	// TEMPLATE: WEBSERVICE 0x100ec350
	// RBX::Reflection::BoundProp<G3D::Vector3,1>::BoundProp<G3D::Vector3,1><RBX::BodyPosition>
	// TEMPLATE: WEBSERVICE 0x100ec420
	// RBX::Reflection::BoundProp<float,1>::BoundProp<float,1><RBX::BodyVelocity>
	// TEMPLATE: WEBSERVICE 0x100ec4f0
	// RBX::Reflection::BoundProp<G3D::Vector3,1>::BoundProp<G3D::Vector3,1><RBX::BodyVelocity>
	// TEMPLATE: WEBSERVICE 0x100ecbf0
	// RBX::Reflection::BoundProp<float,1>::BoundProp<float,1><RBX::BodyGyro>
	// TEMPLATE: WEBSERVICE 0x100eccc0
	// RBX::Reflection::BoundProp<G3D::Vector3,1>::BoundProp<G3D::Vector3,1><RBX::BodyGyro>
	// TEMPLATE: WEBSERVICE 0x100ecd90
	// RBX::Reflection::BoundProp<G3D::CoordinateFrame,1>::BoundProp<G3D::CoordinateFrame,1><RBX::BodyGyro>
	// TEMPLATE: WEBSERVICE 0x100ed0e0
	// RBX::Reflection::BoundProp<G3D::Vector3,1>::BoundProp<G3D::Vector3,1><RBX::BodyForce>
	// TEMPLATE: WEBSERVICE 0x100ed2b0
	// RBX::Reflection::BoundProp<G3D::Vector3,1>::BoundProp<G3D::Vector3,1><RBX::BodyThrust>
	// TEMPLATE: WEBSERVICE 0x100ef0c0
	// RBX::Reflection::TypedPropertyDescriptor<double>::TypedPropertyDescriptor<double>
	// TEMPLATE: WEBSERVICE 0x100ef130
	// RBX::Reflection::PropDescriptor<RBX::ObjectValue,RBX::Instance *>::getset<RBX::Instance * (__thiscall RBX::ObjectValue::*)(void)const ,void (__thiscall RBX::ObjectValue::*)(RBX::Instance *)>
	// TEMPLATE: WEBSERVICE 0x100ef330
	// RBX::Reflection::TypedPropertyDescriptor<double>::equalValues
	// TEMPLATE: WEBSERVICE 0x100f3270
	// RBX::Reflection::BoundProp<int,1>::BoundProp<int,1><RBX::Value<int,&RBX::sIntValue> >
	// TEMPLATE: WEBSERVICE 0x100f3350
	// RBX::Reflection::BoundProp<bool,1>::BoundProp<bool,1><RBX::Value<bool,&RBX::sBoolValue> >
	// TEMPLATE: WEBSERVICE 0x100f3430
	// RBX::Reflection::BoundProp<double,1>::BoundProp<double,1><RBX::Value<double,&RBX::sDoubleValue> >
	// TEMPLATE: WEBSERVICE 0x100f35f0
	// RBX::Reflection::BoundProp<G3D::Vector3,1>::BoundProp<G3D::Vector3,1><RBX::Value<G3D::Vector3,&RBX::sVector3Value> >
	// TEMPLATE: WEBSERVICE 0x100f36d0
	// RBX::Reflection::BoundProp<G3D::CoordinateFrame,1>::BoundProp<G3D::CoordinateFrame,1><RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue> >
	// TEMPLATE: WEBSERVICE 0x100f37b0
	// RBX::Reflection::BoundProp<G3D::Color3,1>::BoundProp<G3D::Color3,1><RBX::Value<G3D::Color3,&RBX::sColor3Value> >
	// TEMPLATE: WEBSERVICE 0x100f3890
	// RBX::Reflection::BoundProp<RBX::BrickColor,1>::BoundProp<RBX::BrickColor,1><RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue> >
	// TEMPLATE: WEBSERVICE 0x1012cc30
	// RBX::Reflection::TypedPropertyDescriptor<RBX::MeshId>::TypedPropertyDescriptor<RBX::MeshId>
	// TEMPLATE: WEBSERVICE 0x1005b070
	// RBX::Reflection::TypedPropertyDescriptor<G3D::CoordinateFrame>::TypedPropertyDescriptor<G3D::CoordinateFrame>
	// TEMPLATE: WEBSERVICE 0x1005b480
	// RBX::Reflection::TypedPropertyDescriptor<G3D::CoordinateFrame>::equalValues
	// TEMPLATE: WEBSERVICE 0x1009baa0
	// RBX::Reflection::TypedPropertyDescriptor<G3D::Vector3>::TypedPropertyDescriptor<G3D::Vector3>
	// FUNCTION: WEBSERVICE 0x100434e0
	// RBX::Reflection::TypedPropertyDescriptor<float>::TypedPropertyDescriptor<float>
	// FUNCTION: WEBSERVICE 0x10048bd0
	// RBX::Reflection::TypedPropertyDescriptor<bool>::TypedPropertyDescriptor<bool>
	// FUNCTION: WEBSERVICE 0x1007deb0
	// RBX::Reflection::TypedPropertyDescriptor<int>::TypedPropertyDescriptor<int>
	// TEMPLATE: WEBSERVICE 0x10094df0
	// RBX::Reflection::BoundProp<RBX::TextureId,1>::BoundProp<RBX::TextureId,1><RBX::Sky>
	// TEMPLATE: WEBSERVICE 0x10094ec0
	// RBX::Reflection::BoundProp<int,1>::BoundProp<int,1><RBX::Sky>
	// TEMPLATE: WEBSERVICE 0x10094f90
	// RBX::Reflection::BoundProp<bool,1>::BoundProp<bool,1><RBX::Sky>
	// FUNCTION: WEBSERVICE 0x100996a0
	// RBX::Reflection::PropDescriptor<RBX::PVInstance,bool>::getset<bool (__thiscall RBX::PVInstance::*)(void)const ,void (__thiscall RBX::PVInstance::*)(bool)>
	// FUNCTION: WEBSERVICE 0x10094c50
	// RBX::Reflection::TypedPropertyDescriptor<RBX::TextureId>::TypedPropertyDescriptor<RBX::TextureId>
	// FUNCTION: WEBSERVICE 0x1009ba30
	// RBX::Reflection::TypedPropertyDescriptor<RBX::BrickColor>::TypedPropertyDescriptor<RBX::BrickColor>
	// TEMPLATE: WEBSERVICE 0x10043a70
	// RBX::Reflection::TypedPropertyDescriptor<G3D::Vector3>::equalValues
	// TEMPLATE: WEBSERVICE 0x100439d0
	// RBX::Reflection::TypedPropertyDescriptor<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >::equalValues
	// SYNTHETIC: WEBSERVICE 0x1004b730
	// RBX::Reflection::PropDescriptor<RBX::Humanoid,bool>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x1007dcd0
	// RBX::Reflection::TypedPropertyDescriptor<RBX::ContentId>::getValue
	// TEMPLATE: WEBSERVICE 0x1009bb10
	// RBX::Reflection::TypedPropertyDescriptor<bool>::isReadOnly
	// TEMPLATE: WEBSERVICE 0x1009bb20
	// RBX::Reflection::PropDescriptor<RBX::PartInstance,G3D::CoordinateFrame>::getset<G3D::CoordinateFrame const & (__thiscall RBX::PartInstance::*)(void)const ,void (__thiscall RBX::PartInstance::*)(G3D::CoordinateFrame const &)>
	// FUNCTION: WEBSERVICE 0x1009bce0
	// RBX::Reflection::PropDescriptor<RBX::PartInstance,RBX::BrickColor>::getset<RBX::BrickColor (__thiscall RBX::PartInstance::*)(void)const ,void (__thiscall RBX::PartInstance::*)(RBX::BrickColor)>
	// FUNCTION: WEBSERVICE 0x1009bd50
	// RBX::Reflection::PropDescriptor<RBX::PartInstance,float>::getset<float (__thiscall RBX::PartInstance::*)(void)const ,void (__thiscall RBX::PartInstance::*)(float)>
	// FUNCTION: WEBSERVICE 0x1009bdc0
	// RBX::Reflection::PropDescriptor<RBX::PartInstance,bool>::getset<bool (__thiscall RBX::PartInstance::*)(void)const ,void (__thiscall RBX::PartInstance::*)(bool)>
	// FUNCTION: WEBSERVICE 0x100a15f0
	// RBX::Reflection::PropDescriptor<RBX::Humanoid,float>::getset<float (__thiscall RBX::Humanoid::*)(void)const ,void (__thiscall RBX::Humanoid::*)(float const &)>
	// FUNCTION: WEBSERVICE 0x100a1660
	// RBX::Reflection::PropDescriptor<RBX::Humanoid,bool>::getset<bool (__thiscall RBX::Humanoid::*)(void)const ,void (__thiscall RBX::Humanoid::*)(bool)>
	// FUNCTION: WEBSERVICE 0x100a16d0
	// RBX::Reflection::PropDescriptor<RBX::Humanoid,float>::getset<float (__thiscall RBX::Humanoid::*)(void)const ,void (__thiscall RBX::Humanoid::*)(float)>
	// TEMPLATE: WEBSERVICE 0x100b36d0
	// RBX::Reflection::PropDescriptor<RBX::DebugSettings,bool>::getset<bool (__thiscall RBX::DebugSettings::*)(void)const ,void (__thiscall RBX::DebugSettings::*)(bool)>
	// TEMPLATE: WEBSERVICE 0x100be0e0
	// RBX::Reflection::PropDescriptor<RBX::Team,int>::getset<int (__thiscall RBX::Team::*)(void)const ,void (__thiscall RBX::Team::*)(int)>
	// TEMPLATE: WEBSERVICE 0x100be140
	// RBX::Reflection::PropDescriptor<RBX::Team,RBX::BrickColor>::getset<RBX::BrickColor (__thiscall RBX::Team::*)(void)const ,void (__thiscall RBX::Team::*)(RBX::BrickColor)>
	// TEMPLATE: WEBSERVICE 0x100be1a0
	// RBX::Reflection::PropDescriptor<RBX::Team,bool>::getset<bool (__thiscall RBX::Team::*)(void)const ,void (__thiscall RBX::Team::*)(bool)>
	// FUNCTION: WEBSERVICE 0x100ca3f0
	// RBX::Reflection::PropDescriptor<RBX::Tool,int>::GetSetImpl<int (__thiscall RBX::Tool::*)(void)const ,void (__thiscall RBX::Tool::*)(int)>::getValue
	// TEMPLATE: WEBSERVICE 0x100ca830
	// RBX::Reflection::PropDescriptor<RBX::Tool,int>::getset<int (__thiscall RBX::Tool::*)(void)const ,void (__thiscall RBX::Tool::*)(int)>
	// FUNCTION: WEBSERVICE 0x100cd3c0
	// RBX::Reflection::PropDescriptor<RBX::Flag,RBX::BrickColor>::GetSetImpl<RBX::BrickColor (__thiscall RBX::Flag::*)(void)const ,void (__thiscall RBX::Flag::*)(RBX::BrickColor)>::getValue
	// TEMPLATE: WEBSERVICE 0x1018c890
	// RBX::Reflection::PropDescriptor<RBX::Team,RBX::BrickColor>::GetSetImpl<RBX::BrickColor (__thiscall RBX::Team::*)(void)const ,void (__thiscall RBX::Team::*)(RBX::BrickColor)>::getValue
	// TEMPLATE: WEBSERVICE 0x100fda10
	// RBX::Reflection::PropDescriptor<RBX::PartInstance,RBX::BrickColor>::GetSetImpl<RBX::BrickColor (__thiscall RBX::PartInstance::*)(void)const ,void (__thiscall RBX::PartInstance::*)(RBX::BrickColor)>::getValue
	// TEMPLATE: WEBSERVICE 0x100b37a0
	// RBX::Reflection::PropDescriptor<RBX::Instance,RBX::Instance *>::GetSetImpl<RBX::Instance * (__thiscall RBX::Instance::*)(void)const ,void (__thiscall RBX::Instance::*)(RBX::Instance *)>::getValue
	// TEMPLATE: WEBSERVICE 0x1005a6b0
	// RBX::Reflection::PropDescriptor<RBX::PartInstance,G3D::CoordinateFrame>::GetSetImpl<G3D::CoordinateFrame const & (__thiscall RBX::PartInstance::*)(void)const ,void (__thiscall RBX::PartInstance::*)(G3D::CoordinateFrame const &)>::getValue
	// STUB: WEBSERVICE 0x100cd400
	// RBX::Reflection::PropDescriptor<RBX::Flag,RBX::BrickColor>::GetSetImpl<RBX::BrickColor (__thiscall RBX::Flag::*)(void)const ,void (__thiscall RBX::Flag::*)(RBX::BrickColor)>::setValue
	// TEMPLATE: WEBSERVICE 0x1018c8c0
	// RBX::Reflection::PropDescriptor<RBX::Instance,RBX::Instance *>::GetSetImpl<RBX::Instance * (__thiscall RBX::Instance::*)(void)const ,void (__thiscall RBX::Instance::*)(RBX::Instance *)>::setValue
	// TEMPLATE: WEBSERVICE 0x100e20d0
	// RBX::Reflection::PropDescriptor<RBX::Texture,float>::GetSetImpl<float (__thiscall RBX::Texture::*)(void)const ,void (__thiscall RBX::Texture::*)(float)>::setValue
	// TEMPLATE: WEBSERVICE 0x100b37d0
	// RBX::Reflection::PropDescriptor<RBX::Team,bool>::GetSetImpl<bool (__thiscall RBX::Team::*)(void)const ,void (__thiscall RBX::Team::*)(bool)>::setValue
	// TEMPLATE: WEBSERVICE 0x100cd6e0
	// RBX::Reflection::PropDescriptor<RBX::Flag,RBX::BrickColor>::getset<RBX::BrickColor (__thiscall RBX::Flag::*)(void)const ,void (__thiscall RBX::Flag::*)(RBX::BrickColor)>
	// TEMPLATE: WEBSERVICE 0x100d7fa0
	// RBX::Reflection::PropDescriptor<RBX::Motor,float>::getset<float (__thiscall RBX::Motor::*)(void)const ,void (__thiscall RBX::Motor::*)(float)>
	// TEMPLATE: WEBSERVICE 0x100de320
	// RBX::Reflection::BoundProp<bool,1>::BoundProp<bool,1><RBX::GameSettings>
	// TEMPLATE: WEBSERVICE 0x100de3f0
	// RBX::Reflection::BoundProp<int,1>::BoundProp<int,1><RBX::GameSettings>
	// STUB: WEBSERVICE 0x100dc050
	// RBX::Reflection::PropDescriptor<RBX::Accoutrement,int>::getset<int (__thiscall RBX::Accoutrement::*)(void)const ,void (__thiscall RBX::Accoutrement::*)(int)>
	// TEMPLATE: WEBSERVICE 0x100e2110
	// RBX::Reflection::PropDescriptor<RBX::Explosion,float>::getset<float (__thiscall RBX::Explosion::*)(void)const ,void (__thiscall RBX::Explosion::*)(float)>
	// TEMPLATE: WEBSERVICE 0x100e29c0
	// RBX::Reflection::BoundProp<float,1>::BoundProp<float,1><RBX::Explosion>
	// TEMPLATE: WEBSERVICE 0x100e2a90
	// RBX::Reflection::BoundProp<G3D::Vector3,1>::BoundProp<G3D::Vector3,1><RBX::Explosion>
	// TEMPLATE: WEBSERVICE 0x100e4a40
	// RBX::Reflection::PropDescriptor<RBX::VelocityMotor,float>::getset<float (__thiscall RBX::VelocityMotor::*)(void)const ,void (__thiscall RBX::VelocityMotor::*)(float)>
	// TEMPLATE: WEBSERVICE 0x100ee870
	// RBX::Reflection::PropDescriptor<RBX::Message,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >::getset<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const & (__thiscall RBX::Message::*)(void)const ,void (__thiscall RBX::Message::*)(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &)>
	// FUNCTION: WEBSERVICE 0x100fdac0
	// RBX::Reflection::PropDescriptor<RBX::SpawnLocation,RBX::BrickColor>::getset<RBX::BrickColor (__thiscall RBX::SpawnLocation::*)(void)const ,void (__thiscall RBX::SpawnLocation::*)(RBX::BrickColor)>
	// TEMPLATE: WEBSERVICE 0x100ff7d0
	// RBX::Reflection::PropDescriptor<RBX::Decal,float>::getset<float (__thiscall RBX::Decal::*)(void)const ,void (__thiscall RBX::Decal::*)(float)>
	// TEMPLATE: WEBSERVICE 0x100ff830
	// RBX::Reflection::PropDescriptor<RBX::Texture,float>::getset<float (__thiscall RBX::Texture::*)(void)const ,void (__thiscall RBX::Texture::*)(float)>
	// TEMPLATE: WEBSERVICE 0x100ffc90
	// RBX::Reflection::PropDescriptor<RBX::Decal,RBX::TextureId>::PropDescriptor<RBX::Decal,RBX::TextureId><RBX::TextureId (__thiscall RBX::Decal::*)(void)const ,void (__thiscall RBX::Decal::*)(RBX::TextureId)>
	// TEMPLATE: WEBSERVICE 0x10193310
	// RBX::Reflection::PropDescriptor<RBX::Network::Players,int>::PropDescriptor<RBX::Network::Players,int><int (__thiscall RBX::Network::Players::*)(void)const ,void (__thiscall RBX::Network::Players::*)(int)>
	// TEMPLATE: WEBSERVICE 0x1018e7f0
	// RBX::Reflection::PropDescriptor<RBX::Network::Player,bool>::PropDescriptor<RBX::Network::Player,bool><bool (__thiscall RBX::Network::Player::*)(void)const ,void (__thiscall RBX::Network::Player::*)(bool)>
	// TEMPLATE: WEBSERVICE 0x1018e740
	// RBX::Reflection::PropDescriptor<RBX::Network::Player,RBX::BrickColor>::PropDescriptor<RBX::Network::Player,RBX::BrickColor><RBX::BrickColor (__thiscall RBX::Network::Player::*)(void)const ,void (__thiscall RBX::Network::Player::*)(RBX::BrickColor)>
	// TEMPLATE: WEBSERVICE 0x1018c700
	// RBX::Reflection::PropDescriptor<RBX::Network::Player,bool>::getset<bool (__thiscall RBX::Network::Player::*)(void)const ,void (__thiscall RBX::Network::Player::*)(bool)>
	// TEMPLATE: WEBSERVICE 0x1018ce60
	// RBX::Reflection::PropDescriptor<RBX::Network::Player,RBX::BrickColor>::getset<RBX::BrickColor (__thiscall RBX::Network::Player::*)(void)const ,void (__thiscall RBX::Network::Player::*)(RBX::BrickColor)>
	// TEMPLATE: WEBSERVICE 0x10191910
	// RBX::Reflection::PropDescriptor<RBX::Network::Players,int>::getset<int (__thiscall RBX::Network::Players::*)(void)const ,void (__thiscall RBX::Network::Players::*)(int)>
	// TEMPLATE: WEBSERVICE 0x1019b5c0
	// RBX::Reflection::BoundProp<bool,1>::BoundProp<bool,1><RBX::NetworkSettings>
	// TEMPLATE: WEBSERVICE 0x1019b6a0
	// RBX::Reflection::BoundProp<int,1>::BoundProp<int,1><RBX::NetworkSettings>
	// TEMPLATE: WEBSERVICE 0x1019b780
	// RBX::Reflection::BoundProp<float,1>::BoundProp<float,1><RBX::NetworkSettings>
	// TEMPLATE: WEBSERVICE 0x100be490
	// RBX::Reflection::BoundProp<bool,1>::BoundProp<bool,1><RBX::Team>
	// clang-format on
	template <class Getter, class Setter>
	static std::auto_ptr<typename TypedPropertyDescriptor<T>::GetSet> getset(Getter get, Setter set)
	{
		return std::auto_ptr<typename TypedPropertyDescriptor<T>::GetSet>(new GetSetImpl<Getter, Setter>(get, set));
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10048c40
	// RBX::Reflection::PropDescriptor<RBX::Instance,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >::getset<std::basic_string<char,std::char_traits<char>,std::allocator<char> > (__thiscall RBX::Instance::*)(void)const ,int>
	// clang-format on
	template <class Getter, class Setter>
	static std::auto_ptr<typename TypedPropertyDescriptor<T>::GetSet> getset(Getter get, int)
	{
		return std::auto_ptr<typename TypedPropertyDescriptor<T>::GetSet>(new GetImpl<Getter>(get));
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10099640
	// RBX::Reflection::PropDescriptor<RBX::PVInstance,G3D::CoordinateFrame>::getset<int,void (__thiscall RBX::PVInstance::*)(G3D::CoordinateFrame const &)>
	// clang-format on
	template <class Getter, class Setter>
	static std::auto_ptr<typename TypedPropertyDescriptor<T>::GetSet> getset(int, Setter set)
	{
		return std::auto_ptr<typename TypedPropertyDescriptor<T>::GetSet>(new SetImpl<Setter>(set));
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10099c10
	// RBX::Reflection::PropDescriptor<RBX::PVInstance,G3D::CoordinateFrame>::PropDescriptor<RBX::PVInstance,G3D::CoordinateFrame><int,void (__thiscall RBX::PVInstance::*)(G3D::CoordinateFrame const &)>
	// TEMPLATE: WEBSERVICE 0x1009e0b0
	// RBX::Reflection::PropDescriptor<RBX::PartInstance,G3D::CoordinateFrame>::PropDescriptor<RBX::PartInstance,G3D::CoordinateFrame><G3D::CoordinateFrame const & (__thiscall RBX::PartInstance::*)(void)const ,void (__thiscall RBX::PartInstance::*)(G3D::CoordinateFrame const &)>
	// TEMPLATE: WEBSERVICE 0x100e2920
	// RBX::Reflection::PropDescriptor<RBX::Explosion,float>::PropDescriptor<RBX::Explosion,float><float (__thiscall RBX::Explosion::*)(void)const ,void (__thiscall RBX::Explosion::*)(float)>
	// TEMPLATE: WEBSERVICE 0x100b4610
	// RBX::Reflection::PropDescriptor<RBX::DebugSettings,bool>::PropDescriptor<RBX::DebugSettings,bool><bool (__thiscall RBX::DebugSettings::*)(void)const ,void (__thiscall RBX::DebugSettings::*)(bool)>
	// FUNCTION: WEBSERVICE 0x100ffd30
	// RBX::Reflection::PropDescriptor<RBX::Decal,float>::PropDescriptor<RBX::Decal,float><float (__thiscall RBX::Decal::*)(void)const ,void (__thiscall RBX::Decal::*)(float)>
	// TEMPLATE: WEBSERVICE 0x100ffdd0
	// RBX::Reflection::PropDescriptor<RBX::Texture,float>::PropDescriptor<RBX::Texture,float><float (__thiscall RBX::Texture::*)(void)const ,void (__thiscall RBX::Texture::*)(float)>
	// TEMPLATE: WEBSERVICE 0x100d7da0
	// RBX::Reflection::PropDescriptor<RBX::AutoJoint,G3D::CoordinateFrame>::GetSetImpl<G3D::CoordinateFrame const & (__thiscall RBX::AutoJoint::*)(void)const ,void (__thiscall RBX::AutoJoint::*)(G3D::CoordinateFrame const &)>::getValue
	// TEMPLATE: WEBSERVICE 0x100d7ee0
	// RBX::Reflection::PropDescriptor<RBX::AutoJoint,RBX::PartInstance *>::getset<RBX::PartInstance * (__thiscall RBX::AutoJoint::*)(void)const ,void (__thiscall RBX::AutoJoint::*)(RBX::PartInstance *)>
	// TEMPLATE: WEBSERVICE 0x100d7f40
	// RBX::Reflection::PropDescriptor<RBX::AutoJoint,G3D::CoordinateFrame>::getset<G3D::CoordinateFrame const & (__thiscall RBX::AutoJoint::*)(void)const ,void (__thiscall RBX::AutoJoint::*)(G3D::CoordinateFrame const &)>
	// TEMPLATE: WEBSERVICE 0x100fa0d0
	// RBX::Reflection::PropDescriptor<RBX::DebrisService,int>::getset<int (__thiscall RBX::DebrisService::*)(void)const ,void (__thiscall RBX::DebrisService::*)(int)>
	// TEMPLATE: WEBSERVICE 0x100faae0
	// RBX::Reflection::PropDescriptor<RBX::DebrisService,int>::PropDescriptor<RBX::DebrisService,int><int (__thiscall RBX::DebrisService::*)(void)const ,void (__thiscall RBX::DebrisService::*)(int)>
	// TEMPLATE: WEBSERVICE 0x1012cb10
	// RBX::Reflection::PropDescriptor<RBX::SpecialShape,G3D::Vector3>::GetSetImpl<G3D::Vector3 const & (__thiscall RBX::SpecialShape::*)(void)const ,void (__thiscall RBX::SpecialShape::*)(G3D::Vector3 const &)>::getValue
	// TEMPLATE: WEBSERVICE 0x1012cd00
	// RBX::Reflection::PropDescriptor<RBX::SpecialShape,G3D::Vector3>::getset<G3D::Vector3 const & (__thiscall RBX::SpecialShape::*)(void)const ,void (__thiscall RBX::SpecialShape::*)(G3D::Vector3 const &)>
	// TEMPLATE: WEBSERVICE 0x1012cd60
	// RBX::Reflection::PropDescriptor<RBX::SpecialShape,RBX::MeshId>::getset<RBX::MeshId const (__thiscall RBX::SpecialShape::*)(void)const ,void (__thiscall RBX::SpecialShape::*)(RBX::MeshId const &)>
	// TEMPLATE: WEBSERVICE 0x1012cdc0
	// RBX::Reflection::PropDescriptor<RBX::SpecialShape,RBX::TextureId>::getset<RBX::TextureId const (__thiscall RBX::SpecialShape::*)(void)const ,void (__thiscall RBX::SpecialShape::*)(RBX::TextureId const &)>
	// TEMPLATE: WEBSERVICE 0x1012d3e0
	// RBX::Reflection::PropDescriptor<RBX::SpecialShape,G3D::Vector3>::PropDescriptor<RBX::SpecialShape,G3D::Vector3><G3D::Vector3 const & (__thiscall RBX::SpecialShape::*)(void)const ,void (__thiscall RBX::SpecialShape::*)(G3D::Vector3 const &)>
	// TEMPLATE: WEBSERVICE 0x1012d480
	// RBX::Reflection::PropDescriptor<RBX::SpecialShape,RBX::MeshId>::PropDescriptor<RBX::SpecialShape,RBX::MeshId><RBX::MeshId const (__thiscall RBX::SpecialShape::*)(void)const ,void (__thiscall RBX::SpecialShape::*)(RBX::MeshId const &)>
	// TEMPLATE: WEBSERVICE 0x1012d520
	// RBX::Reflection::PropDescriptor<RBX::SpecialShape,RBX::TextureId>::PropDescriptor<RBX::SpecialShape,RBX::TextureId><RBX::TextureId const (__thiscall RBX::SpecialShape::*)(void)const ,void (__thiscall RBX::SpecialShape::*)(RBX::TextureId const &)>
	// TEMPLATE: WEBSERVICE 0x10048cf0
	// RBX::Reflection::PropDescriptor<RBX::Instance,RBX::Instance *>::getset<RBX::Instance * (__thiscall RBX::Instance::*)(void)const ,void (__thiscall RBX::Instance::*)(RBX::Instance *)>
	// TEMPLATE: WEBSERVICE 0x10099710
	// RBX::Reflection::PropDescriptor<RBX::PVInstance,enum RBX::Controller::ControllerType>::getset<enum RBX::Controller::ControllerType (__thiscall RBX::PVInstance::*)(void)const ,void (__thiscall RBX::PVInstance::*)(enum RBX::Controller::ControllerType)>
	// TEMPLATE: WEBSERVICE 0x1009b340
	// RBX::Reflection::PropDescriptor<RBX::PartInstance,G3D::Vector3>::GetSetImpl<G3D::Vector3 const & (__thiscall RBX::PartInstance::*)(void)const ,void (__thiscall RBX::PartInstance::*)(G3D::Vector3 const &)>::getValue
	// TEMPLATE: WEBSERVICE 0x1009bb90
	// RBX::Reflection::PropDescriptor<RBX::PartInstance,G3D::Vector3>::getset<G3D::Vector3 const & (__thiscall RBX::PartInstance::*)(void)const ,void (__thiscall RBX::PartInstance::*)(G3D::Vector3 const &)>
	// TEMPLATE: WEBSERVICE 0x1009bc00
	// RBX::Reflection::PropDescriptor<RBX::PartInstance,enum RBX::Part::PartType>::getset<enum RBX::Part::PartType (__thiscall RBX::PartInstance::*)(void)const ,void (__thiscall RBX::PartInstance::*)(enum RBX::Part::PartType)>
	// TEMPLATE: WEBSERVICE 0x1009bc70
	// RBX::Reflection::PropDescriptor<RBX::PartInstance,G3D::Color3>::getset<G3D::Color3 (__thiscall RBX::PartInstance::*)(void)const ,void (__thiscall RBX::PartInstance::*)(G3D::Color3 const &)>
	// TEMPLATE: WEBSERVICE 0x1009be30
	// RBX::Reflection::PropDescriptor<RBX::PartInstance,G3D::Vector3>::getset<G3D::Vector3 (__thiscall RBX::PartInstance::*)(void)const ,void (__thiscall RBX::PartInstance::*)(G3D::Vector3 const &)>
	// TEMPLATE: WEBSERVICE 0x1009bea0
	// RBX::Reflection::PropDescriptor<RBX::PartInstance,enum RBX::PartInstance::FormFactor>::getset<enum RBX::PartInstance::FormFactor (__thiscall RBX::PartInstance::*)(void)const ,void (__thiscall RBX::PartInstance::*)(enum RBX::PartInstance::FormFactor)>
	// TEMPLATE: WEBSERVICE 0x1009e170
	// RBX::Reflection::PropDescriptor<RBX::PartInstance,G3D::Vector3>::PropDescriptor<RBX::PartInstance,G3D::Vector3><G3D::Vector3 const & (__thiscall RBX::PartInstance::*)(void)const ,void (__thiscall RBX::PartInstance::*)(G3D::Vector3 const &)>
	// TEMPLATE: WEBSERVICE 0x1009e230
	// RBX::Reflection::PropDescriptor<RBX::PartInstance,G3D::Color3>::PropDescriptor<RBX::PartInstance,G3D::Color3><G3D::Color3 (__thiscall RBX::PartInstance::*)(void)const ,void (__thiscall RBX::PartInstance::*)(G3D::Color3 const &)>
	// TEMPLATE: WEBSERVICE 0x1009e530
	// RBX::Reflection::PropDescriptor<RBX::PartInstance,G3D::Vector3>::PropDescriptor<RBX::PartInstance,G3D::Vector3><G3D::Vector3 (__thiscall RBX::PartInstance::*)(void)const ,void (__thiscall RBX::PartInstance::*)(G3D::Vector3 const &)>
	// TEMPLATE: WEBSERVICE 0x100b3a70
	// RBX::Reflection::PropDescriptor<RBX::DebugSettings,enum RBX::Debugable::AssertAction>::getset<enum RBX::Debugable::AssertAction (__thiscall RBX::DebugSettings::*)(void)const ,void (__thiscall RBX::DebugSettings::*)(enum RBX::Debugable::AssertAction)>
	// TEMPLATE: WEBSERVICE 0x100e4aa0
	// RBX::Reflection::PropDescriptor<RBX::Feature,enum RBX::NormalId>::getset<enum RBX::NormalId (__thiscall RBX::Feature::*)(void)const ,void (__thiscall RBX::Feature::*)(enum RBX::NormalId)>
	// TEMPLATE: WEBSERVICE 0x100e4b00
	// RBX::Reflection::PropDescriptor<RBX::Feature,enum RBX::Feature::TopBottom>::getset<enum RBX::Feature::TopBottom (__thiscall RBX::Feature::*)(void)const ,void (__thiscall RBX::Feature::*)(enum RBX::Feature::TopBottom)>
	// TEMPLATE: WEBSERVICE 0x100e4b60
	// RBX::Reflection::PropDescriptor<RBX::Feature,enum RBX::Feature::LeftRight>::getset<enum RBX::Feature::LeftRight (__thiscall RBX::Feature::*)(void)const ,void (__thiscall RBX::Feature::*)(enum RBX::Feature::LeftRight)>
	// TEMPLATE: WEBSERVICE 0x100e4bc0
	// RBX::Reflection::PropDescriptor<RBX::Feature,enum RBX::Feature::InOut>::getset<enum RBX::Feature::InOut (__thiscall RBX::Feature::*)(void)const ,void (__thiscall RBX::Feature::*)(enum RBX::Feature::InOut)>
	// TEMPLATE: WEBSERVICE 0x100ff770
	// RBX::Reflection::PropDescriptor<RBX::Decal,RBX::TextureId>::getset<RBX::TextureId (__thiscall RBX::Decal::*)(void)const ,void (__thiscall RBX::Decal::*)(RBX::TextureId)>
	// TEMPLATE: WEBSERVICE 0x1012cca0
	// RBX::Reflection::PropDescriptor<RBX::SpecialShape,enum RBX::SpecialShape::MeshType>::getset<enum RBX::SpecialShape::MeshType const (__thiscall RBX::SpecialShape::*)(void)const ,void (__thiscall RBX::SpecialShape::*)(enum RBX::SpecialShape::MeshType)>
	// clang-format on
	template <class Getter, class Setter>
	PropDescriptor(
		const char* name,
		const char* category,
		Getter get,
		Setter set,
		typename PropertyDescriptor::Functionality flags = PropertyDescriptor::STANDARD
	)
		: TypedPropertyDescriptor<T>(Class::classDescriptor(), name, category, getset<Getter, Setter>(get, set), flags)
	{
	}
};

// SIZE 0x1c
template <class T, int bound>
class BoundProp : public TypedPropertyDescriptor<T>
{
public:
	// SIZE 0x18
	template <class Class>
	class BoundPropGetSet : public TypedPropertyDescriptor<T>::GetSet
	{
		BoundProp<T, bound>* desc; // 0x04
		T Class::* member;         // 0x08

		typedef void (Class::*ChangedMember)(const PropertyDescriptor&);

		ChangedMember changed; // 0x10

	public:
		BoundPropGetSet(BoundProp<T, bound>* desc, T Class::* member, ChangedMember changed)
			: desc(desc), member(member), changed(changed)
		{
		}

		virtual bool isReadOnly() const { return false; }

		virtual T getValue(const DescribedBase* instance) const { return static_cast<const Class*>(instance)->*member; }

		// clang-format off
		// TEMPLATE: WEBSERVICE 0x100de0f0
		// RBX::Reflection::BoundProp<bool,1>::BoundPropGetSet<RBX::Sky>::getValue
		// TEMPLATE: WEBSERVICE 0x100e0760
		// RBX::Reflection::BoundProp<RBX::TextureId,1>::BoundPropGetSet<RBX::Sky>::getValue
		// TEMPLATE: WEBSERVICE 0x100eb7a0
		// RBX::Reflection::BoundProp<float,1>::BoundPropGetSet<RBX::Explosion>::getValue
		// TEMPLATE: WEBSERVICE 0x100eb7c0
		// RBX::Reflection::BoundProp<G3D::Vector3,1>::BoundPropGetSet<RBX::Explosion>::getValue
		// TEMPLATE: WEBSERVICE 0x100b53a0
		// RBX::Reflection::BoundProp<bool,1>::BoundPropGetSet<RBX::Sky>::setValue
		// TEMPLATE: WEBSERVICE 0x100e1920
		// RBX::Reflection::BoundProp<RBX::TextureId,1>::BoundPropGetSet<RBX::Sky>::setValue
		// TEMPLATE: WEBSERVICE 0x100ee410
		// RBX::Reflection::BoundProp<float,1>::BoundPropGetSet<RBX::Explosion>::setValue
		// TEMPLATE: WEBSERVICE 0x100eef70
		// RBX::Reflection::BoundProp<double,1>::BoundPropGetSet<RBX::Value<double,&RBX::sDoubleValue> >::getValue
		// TEMPLATE: WEBSERVICE 0x100eef90
		// RBX::Reflection::BoundProp<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,1>::BoundPropGetSet<RBX::Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue> >::getValue
		// TEMPLATE: WEBSERVICE 0x100eefd0
		// RBX::Reflection::BoundProp<G3D::CoordinateFrame,1>::BoundPropGetSet<RBX::BodyGyro>::getValue
		// TEMPLATE: WEBSERVICE 0x100ef010
		// RBX::Reflection::BoundProp<RBX::BrickColor,1>::BoundPropGetSet<RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue> >::getValue
		// TEMPLATE: WEBSERVICE 0x100f75b0
		// RBX::Reflection::BoundProp<double,1>::BoundPropGetSet<RBX::Value<double,&RBX::sDoubleValue> >::setValue
		// TEMPLATE: WEBSERVICE 0x100f7600
		// RBX::Reflection::BoundProp<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,1>::BoundPropGetSet<RBX::Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue> >::setValue
		// TEMPLATE: WEBSERVICE 0x100f7660
		// RBX::Reflection::BoundProp<G3D::CoordinateFrame,1>::BoundPropGetSet<RBX::BodyGyro>::setValue
		// TEMPLATE: WEBSERVICE 0x100f76e0
		// RBX::Reflection::BoundProp<G3D::Vector3,1>::BoundPropGetSet<RBX::Explosion>::setValue
		// TEMPLATE: WEBSERVICE 0x1019bb70
		// RBX::Reflection::BoundProp<float,1>::BoundPropGetSet<RBX::NetworkSettings>::setValue
		// clang-format on

		virtual void setValue(DescribedBase* object, const T& value) const
		{
			Class* instance = static_cast<Class*>(object);

			if (instance->*member != value) {
				instance->*member = value;

				if (changed != NULL) {
					(instance->*changed)(*desc);
				}

				instance->raisePropertyChanged(*desc);
			}
		}
	};

	template <class Class>
	BoundProp(
		const char* name,
		const char* category,
		T Class::* member,
		typename PropertyDescriptor::Functionality flags = PropertyDescriptor::STANDARD
	)
		: TypedPropertyDescriptor<T>(
			  Class::classDescriptor(),
			  name,
			  category,
			  std::auto_ptr<typename TypedPropertyDescriptor<T>::GetSet>(),
			  flags
		  )
	{
		this->getset.reset(new BoundPropGetSet<Class>(this, member, NULL));
	}

	template <class Class>
	BoundProp(
		const char* name,
		const char* category,
		T Class::* member,
		void (Class::*changed)(const PropertyDescriptor&),
		typename PropertyDescriptor::Functionality flags = PropertyDescriptor::STANDARD
	)
		: TypedPropertyDescriptor<T>(
			  Class::classDescriptor(),
			  name,
			  category,
			  std::auto_ptr<typename TypedPropertyDescriptor<T>::GetSet>(),
			  flags
		  )
	{
		this->getset.reset(new BoundPropGetSet<Class>(this, member, changed));
	}
};

// SIZE 0x08
class ConstProperty
{
public:
	ConstProperty(const ConstProperty& other) : descriptor(other.descriptor), instance(other.instance) {}
	ConstProperty(const PropertyDescriptor& descriptor, const DescribedBase* instance)
		: descriptor(&descriptor), instance(instance)
	{
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x100ab2e0
	// RBX::Reflection::ConstProperty::getValue<bool>
	// TEMPLATE: WEBSERVICE 0x100ab340
	// RBX::Reflection::ConstProperty::getValue<float>
	// TEMPLATE: WEBSERVICE 0x100ab610
	// RBX::Reflection::ConstProperty::getValue<int>
	// TEMPLATE: WEBSERVICE 0x100ab670
	// RBX::Reflection::ConstProperty::getValue<double>
	// TEMPLATE: WEBSERVICE 0x100ab6d0
	// RBX::Reflection::ConstProperty::getValue<boost::shared_ptr<RBX::Instance> >
	// TEMPLATE: WEBSERVICE 0x100ab740
	// RBX::Reflection::ConstProperty::getValue<boost::shared_ptr<RBX::Reflection::DescribedBase> >
	// TEMPLATE: WEBSERVICE 0x100ab7b0
	// RBX::Reflection::ConstProperty::getValue<G3D::Vector3>
	// TEMPLATE: WEBSERVICE 0x100ab810
	// RBX::Reflection::ConstProperty::getValue<G3D::CoordinateFrame>
	// TEMPLATE: WEBSERVICE 0x100ab870
	// RBX::Reflection::ConstProperty::getValue<G3D::Color3>
	// TEMPLATE: WEBSERVICE 0x100ab8d0
	// RBX::Reflection::ConstProperty::getValue<RBX::BrickColor>
	// TEMPLATE: WEBSERVICE 0x100ab930
	// RBX::Reflection::ConstProperty::getValue<RBX::Lua::FunctionRef>
	// TEMPLATE: WEBSERVICE 0x100ab9a0
	// RBX::Reflection::ConstProperty::getValue<boost::shared_ptr<std::vector<boost::shared_ptr<RBX::Instance>,std::allocator<boost::shared_ptr<RBX::Instance> > > const > >
	// clang-format on
	template <class T>
	T getValue() const
	{
		return boost::polymorphic_cast<const TypedPropertyDescriptor<T>*>(descriptor)->getValue(instance);
	}

	const DescribedBase* getInstance() const { return instance; }

	const PropertyDescriptor& getDescriptor() const { return *descriptor; }

	const Name& getName() const { return descriptor->name; }

	// FUNCTION: WEBSERVICE 0x100aaf40
	std::string getStringValue() const { return descriptor->getStringValue(instance); }

protected:
	const PropertyDescriptor* descriptor; // 0x00
	const DescribedBase* instance;        // 0x04
};

DECOMP_SIZE_ASSERT(ConstProperty, 0x08)

// SIZE 0x08
class Property : public ConstProperty
{
public:
	Property(const Property& other) : ConstProperty(other) {}

	Property(const PropertyDescriptor& descriptor, DescribedBase* instance) : ConstProperty(descriptor, instance) {}

	DescribedBase* getInstance() const { return const_cast<DescribedBase*>(instance); }

	void read(const XmlElement* element, IReferenceBinder& binder) { descriptor->read(getInstance(), element, binder); }

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x100ab3a0
	// RBX::Reflection::Property::setValue<bool>
	// TEMPLATE: WEBSERVICE 0x100ab400
	// RBX::Reflection::Property::setValue<float>
	// TEMPLATE: WEBSERVICE 0x100ab460
	// RBX::Reflection::Property::setValue<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >
	// TEMPLATE: WEBSERVICE 0x100ab4c0
	// RBX::Reflection::Property::setValue<RBX::Soundscape::SoundId>
	// TEMPLATE: WEBSERVICE 0x100ab520
	// RBX::Reflection::Property::setValue<RBX::TextureId>
	// TEMPLATE: WEBSERVICE 0x100aba10
	// RBX::Reflection::Property::setValue<int>
	// TEMPLATE: WEBSERVICE 0x100aba70
	// RBX::Reflection::Property::setValue<RBX::Lua::FunctionRef>
	// TEMPLATE: WEBSERVICE 0x100abad0
	// RBX::Reflection::Property::setValue<boost::shared_ptr<RBX::Instance> >
	// TEMPLATE: WEBSERVICE 0x100abb30
	// RBX::Reflection::Property::setValue<boost::shared_ptr<RBX::Reflection::DescribedBase> >
	// TEMPLATE: WEBSERVICE 0x100abb90
	// RBX::Reflection::Property::setValue<G3D::Vector3>
	// TEMPLATE: WEBSERVICE 0x100abbf0
	// RBX::Reflection::Property::setValue<G3D::CoordinateFrame>
	// TEMPLATE: WEBSERVICE 0x100abc50
	// RBX::Reflection::Property::setValue<G3D::Color3>
	// TEMPLATE: WEBSERVICE 0x100abcb0
	// RBX::Reflection::Property::setValue<RBX::BrickColor>
	// clang-format on
	template <class T>
	void setValue(const T& value)
	{
		boost::polymorphic_cast<const TypedPropertyDescriptor<T>*>(descriptor)
			->setValue(const_cast<DescribedBase*>(instance), value);
	}
};

DECOMP_SIZE_ASSERT(Property, 0x08)

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_PROPERTY_H
