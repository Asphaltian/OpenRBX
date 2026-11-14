#ifndef REFLECTION_PROPERTY_H
#define REFLECTION_PROPERTY_H

#include "reflection/enumconverter.h"
#include "reflection/member.h"
#include "reflection/type.h"
#include "placeholder.h"
#include "util/MeshId.h"
#include "util/TextureId.h"
#include "v8datamodel/BrickColor.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/Vector2int16.h>
#include <G3D/Vector3.h>
#include <boost/cast.hpp>
#include <memory>
#include <string>
#include <typeinfo>

class XmlElement;

namespace RBX {

class IReferenceBinder;

extern const char* category_Part;

namespace Reflection {

class ConstProperty;
class DescribedBase;
class Property;
class EnumDescriptor;

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

	virtual bool isReadOnly() const = 0;
	virtual bool equalValues(const DescribedBase* a, const DescribedBase* b) const = 0;
	virtual bool hasStringValue() const = 0;
	virtual std::string getStringValue(const DescribedBase* instance) const = 0;
	virtual bool setStringValue(DescribedBase* instance, const std::string& value) const = 0;
	virtual void read(DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder) const;

	XmlElement* write(const DescribedBase* instance, bool ignoreWriteProtection) const;

	bool isPublic() const { return bIsPublic; }
	bool canStreamWrite() const { return bCanStreamWrite; }

	bool operator==(const PropertyDescriptor& other) const { return this == &other; }

private:
	unsigned int bIsPublic : 1;
	unsigned int bCanStreamWrite : 1;

public:
	const Type& type;

protected:
	PropertyDescriptor(
		ClassDescriptor& classDescriptor, const Type& type, const char* name, const char* category, Functionality flags
	);

private:
	virtual void writeValue(const DescribedBase* instance, XmlElement* element) const = 0;
	virtual void readValue(DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder) const = 0;
};

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

template <class T>
class TypedPropertyDescriptor : public PropertyDescriptor
{
public:
	class __declspec(novtable) GetSet
	{
	public:
		virtual bool isReadOnly() const = 0;
		virtual T getValue(const DescribedBase* instance) const = 0;
		virtual void setValue(DescribedBase* instance, const T& value) const = 0;
	};

	T getValue(const DescribedBase* object) const { return getset->getValue(object); }

	void setValue(DescribedBase* instance, const T& value) const { getset->setValue(instance, value); }

	virtual bool isReadOnly() const { return getset->isReadOnly(); }

	virtual bool equalValues(const DescribedBase* a, const DescribedBase* b) const
	{
		return getValue(a) == getValue(b);
	}

	virtual bool hasStringValue() const;

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
	std::auto_ptr<GetSet> getset;
};

template <class T>
bool TypedPropertyDescriptor<T>::hasStringValue() const
{
	return false;
}

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
bool TypedPropertyDescriptor<std::string>::hasStringValue() const;
template <>
std::string TypedPropertyDescriptor<std::string>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<std::string>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<std::string>::readValue(
	DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<std::string>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
bool TypedPropertyDescriptor<bool>::hasStringValue() const;
template <>
std::string TypedPropertyDescriptor<bool>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<bool>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<bool>::readValue(
	DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<bool>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
bool TypedPropertyDescriptor<float>::hasStringValue() const;
template <>
std::string TypedPropertyDescriptor<float>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<float>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<float>::readValue(
	DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<float>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
bool TypedPropertyDescriptor<double>::hasStringValue() const;
template <>
std::string TypedPropertyDescriptor<double>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<double>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<double>::readValue(
	DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<double>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
bool TypedPropertyDescriptor<int>::hasStringValue() const;
template <>
std::string TypedPropertyDescriptor<int>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<int>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<int>::readValue(
	DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<int>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
bool TypedPropertyDescriptor<G3D::Vector3>::hasStringValue() const;
template <>
std::string TypedPropertyDescriptor<G3D::Vector3>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<G3D::Vector3>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<G3D::Vector3>::readValue(
	DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<G3D::Vector3>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
bool TypedPropertyDescriptor<G3D::Vector2int16>::hasStringValue() const;
template <>
std::string TypedPropertyDescriptor<G3D::Vector2int16>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<G3D::Vector2int16>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<G3D::Vector2int16>::readValue(
	DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<G3D::Vector2int16>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
bool TypedPropertyDescriptor<G3D::Color3>::hasStringValue() const;
template <>
std::string TypedPropertyDescriptor<G3D::Color3>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<G3D::Color3>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<G3D::Color3>::readValue(
	DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<G3D::Color3>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
bool TypedPropertyDescriptor<BrickColor>::hasStringValue() const;
template <>
std::string TypedPropertyDescriptor<BrickColor>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<BrickColor>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<BrickColor>::readValue(
	DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<BrickColor>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
bool TypedPropertyDescriptor<ContentId>::hasStringValue() const;
template <>
std::string TypedPropertyDescriptor<ContentId>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<ContentId>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<ContentId>::readValue(
	DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<ContentId>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
bool TypedPropertyDescriptor<TextureId>::hasStringValue() const;
template <>
std::string TypedPropertyDescriptor<TextureId>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<TextureId>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<TextureId>::readValue(
	DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<TextureId>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
bool TypedPropertyDescriptor<MeshId>::hasStringValue() const;
template <>
std::string TypedPropertyDescriptor<MeshId>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<MeshId>::setStringValue(DescribedBase* instance, const std::string& text) const;
template <>
void TypedPropertyDescriptor<MeshId>::readValue(
	DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<MeshId>::writeValue(const DescribedBase* instance, XmlElement* element) const;

template <>
void TypedPropertyDescriptor<G3D::CoordinateFrame>::readValue(
	DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<G3D::CoordinateFrame>::writeValue(
	const DescribedBase* instance, XmlElement* element
) const;

class __declspec(novtable) EnumPropertyDescriptor : public PropertyDescriptor
{
public:
	const EnumDescriptor& enumDescriptor;

	virtual unsigned int getIndexValue(const DescribedBase* instance) const = 0;
	virtual bool setIndexValue(DescribedBase* instance, unsigned int index) const = 0;
	virtual int getEnumValue(const DescribedBase* instance) const = 0;
	virtual bool setEnumValue(DescribedBase* instance, int intValue) const = 0;

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

class __declspec(novtable) RefPropertyDescriptor : public PropertyDescriptor
{
public:
	virtual DescribedBase* getRefValue(const DescribedBase* instance) const = 0;
	virtual void setRefValue(DescribedBase* instance, DescribedBase* value) const = 0;

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

	virtual std::string getStringValue(const DescribedBase* instance) const
	{
		return PropertyDescriptor::getStringValue(instance);
	}

	virtual bool setStringValue(DescribedBase* instance, const std::string& value) const
	{
		hasStringValue();

		return false;
	}
};

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
			Class* instance = static_cast<Class*>(object);
			(instance->*set)(value);
		}

	private:
		Getter get;
		Setter set;
	};

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

	template <class Getter, class Setter>
	static std::auto_ptr<typename TypedPropertyDescriptor<T>::GetSet> getset(Getter get, Setter set)
	{
		return std::auto_ptr<typename TypedPropertyDescriptor<T>::GetSet>(new GetSetImpl<Getter, Setter>(get, set));
	}

	template <class Getter, class Setter>
	static std::auto_ptr<typename TypedPropertyDescriptor<T>::GetSet> getset(Getter get, int set)
	{
		return std::auto_ptr<typename TypedPropertyDescriptor<T>::GetSet>(new GetImpl<Getter>(get));
	}

	template <class Getter, class Setter>
	static std::auto_ptr<typename TypedPropertyDescriptor<T>::GetSet> getset(int get, Setter set)
	{
		return std::auto_ptr<typename TypedPropertyDescriptor<T>::GetSet>(new SetImpl<Setter>(set));
	}

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

template <class T, int bound>
class BoundProp : public TypedPropertyDescriptor<T>
{
public:
	template <class Class>
	class BoundPropGetSet : public TypedPropertyDescriptor<T>::GetSet
	{
		BoundProp<T, bound>* desc;
		T Class::* member;

		typedef void (Class::*ChangedMember)(const PropertyDescriptor&);

		ChangedMember changed;

	public:
		BoundPropGetSet(BoundProp<T, bound>* desc, T Class::* member, ChangedMember changed)
			: desc(desc), member(member), changed(changed)
		{
		}

		virtual bool isReadOnly() const { return false; }

		virtual T getValue(const DescribedBase* object) const
		{
			const Class* instance = static_cast<const Class*>(object);
			return instance->*member;
		}

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

class ConstProperty
{
public:
	ConstProperty(const ConstProperty& other) : descriptor(other.descriptor), instance(other.instance) {}
	ConstProperty(const PropertyDescriptor& descriptor, const DescribedBase* instance)
		: descriptor(&descriptor), instance(instance)
	{
	}

	template <class T>
	T getValue() const
	{
		return boost::polymorphic_cast<const TypedPropertyDescriptor<T>*>(descriptor)->getValue(instance);
	}

	const DescribedBase* getInstance() const { return instance; }

	const PropertyDescriptor& getDescriptor() const { return *descriptor; }

	ConstProperty& operator=(const ConstProperty& other)
	{
		descriptor = other.descriptor;
		instance = other.instance;
		return *this;
	}

	const Name& getName() const { return descriptor->name; }
	bool hasStringValue() const;

	std::string getStringValue() const { return descriptor->getStringValue(instance); }
	XmlElement* write() const;

protected:
	const PropertyDescriptor* descriptor;
	const DescribedBase* instance;
};

class Property : public ConstProperty
{
public:
	Property(const Property& other) : ConstProperty(other) {}

	Property(const PropertyDescriptor& descriptor, DescribedBase* instance) : ConstProperty(descriptor, instance) {}

	Property& operator=(const Property& other)
	{
		ConstProperty::operator=(other);
		return *this;
	}

	bool operator==(const Property& other) const
	{
		return descriptor == other.descriptor && instance == other.instance;
	}
	bool operator!=(const Property& other) const { return !(*this == other); }

	DescribedBase* getInstance() const { return const_cast<DescribedBase*>(instance); }

	bool setStringValue(const std::string& value)
	{
		return descriptor->setStringValue(const_cast<DescribedBase*>(instance), value);
	}

	void read(const XmlElement* element, IReferenceBinder& binder) { descriptor->read(getInstance(), element, binder); }

	template <class T>
	void setValue(const T& value)
	{
		boost::polymorphic_cast<const TypedPropertyDescriptor<T>*>(descriptor)
			->setValue(const_cast<DescribedBase*>(instance), value);
	}
};

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_PROPERTY_H
