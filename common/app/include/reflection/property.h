#ifndef REFLECTION_PROPERTY_H
#define REFLECTION_PROPERTY_H

#include "decomp.h"
#include "reflection/enumconverter.h"
#include "reflection/member.h"
#include "reflection/type.h"

#include <memory>
#include <string>

class XmlElement;

namespace RBX {

class IReferenceBinder;

extern const char* const category_Part;

namespace Reflection {

class DescribedBase;
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

	bool isPublic() const { return bIsPublic; }
	bool canStreamWrite() const { return bCanStreamWrite; }

	unsigned int bIsPublic : 1;       // 0x10
	unsigned int bCanStreamWrite : 1; // 0x10

	const Type& type; // 0x14

protected:
	PropertyDescriptor(
		ClassDescriptor& classDescriptor,
		const Type& type,
		const char* name,
		const char* category,
		Functionality functionality
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

// SIZE 0x1c
template <class T>
class __declspec(novtable) TypedPropertyDescriptor : public PropertyDescriptor
{
public:
	// SIZE 0x04
	class __declspec(novtable) GetSet
	{
	public:
		virtual ~GetSet() {} // vtable+0x00

		virtual T getValue(const DescribedBase* instance) const = 0;              // vtable+0x04
		virtual void setValue(DescribedBase* instance, const T& value) const = 0; // vtable+0x08
	};

	T getValue(const DescribedBase* instance) const { return getset->getValue(instance); }

	void setValue(DescribedBase* instance, const T& value) const { getset->setValue(instance, value); }

	virtual bool isReadOnly() const { return false; }

	virtual bool equalValues(const DescribedBase* a, const DescribedBase* b) const
	{
		return getValue(a) == getValue(b);
	}

	virtual bool hasStringValue() const { return false; }

	virtual std::string getStringValue(const DescribedBase* instance) const
	{
		STUB(0x10094120);
		return std::string();
	}

	virtual bool setStringValue(DescribedBase* instance, const std::string& value) const
	{
		STUB(0x100941a0);
		return false;
	}

protected:
	TypedPropertyDescriptor(
		ClassDescriptor& classDescriptor,
		const char* name,
		const char* category,
		std::auto_ptr<GetSet> getset,
		Functionality functionality
	)
		: PropertyDescriptor(classDescriptor, Type::singleton<T>(), name, category, functionality), getset(getset)
	{
	}

	TypedPropertyDescriptor(
		ClassDescriptor& classDescriptor,
		const Type& type,
		const char* name,
		const char* category,
		std::auto_ptr<GetSet> getset,
		Functionality functionality
	)
		: PropertyDescriptor(classDescriptor, type, name, category, functionality), getset(getset)
	{
	}

private:
	virtual void writeValue(const DescribedBase* instance, XmlElement* element) const { STUB(0x10094410); }

	virtual void readValue(DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder) const
	{
		STUB(0x10094420);
	}

	std::auto_ptr<GetSet> getset; // 0x18
};

// SIZE 0x1c
class __declspec(novtable) EnumPropertyDescriptor : public PropertyDescriptor
{
public:
	const EnumDescriptor& enumDescriptor; // 0x18

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

// SIZE 0x1c
template <class Class, class T>
class PropDescriptor : public TypedPropertyDescriptor<T>
{
public:
	template <class Getter, class Setter>
	class GetSetImpl : public TypedPropertyDescriptor<T>::GetSet
	{
	public:
		GetSetImpl(Getter getter, Setter setter) : getter(getter), setter(setter) {}

		virtual T getValue(const DescribedBase* instance) const
		{
			return (static_cast<const Class*>(instance)->*getter)();
		}

		virtual void setValue(DescribedBase* instance, const T& value) const
		{
			(static_cast<Class*>(instance)->*setter)(value);
		}

	private:
		Getter getter; // 0x04
		Setter setter; // 0x10
	};

	template <class Getter, class Setter>
	PropDescriptor(
		const char* name,
		const char* category,
		Getter getter,
		Setter setter,
		typename PropertyDescriptor::Functionality functionality = PropertyDescriptor::STANDARD
	)
		: TypedPropertyDescriptor<T>(
			  Class::classDescriptor(),
			  name,
			  category,
			  std::auto_ptr<typename TypedPropertyDescriptor<T>::GetSet>(
				  new GetSetImpl<Getter, Setter>(getter, setter)
			  ),
			  functionality
		  )
	{
	}
};

// SIZE 0x20
template <class Class, class T>
class EnumPropDescriptor : public EnumPropertyDescriptor
{
public:
	// SIZE 0x04
	class __declspec(novtable) GetSet
	{
	public:
		virtual ~GetSet() {} // vtable+0x00

		virtual T getValue(const DescribedBase* instance) const = 0;              // vtable+0x04
		virtual void setValue(DescribedBase* instance, const T& value) const = 0; // vtable+0x08
	};

	template <class Getter, class Setter>
	class GetSetImpl : public GetSet
	{
	public:
		GetSetImpl(Getter getter, Setter setter) : getter(getter), setter(setter) {}

		virtual T getValue(const DescribedBase* instance) const
		{
			return (static_cast<const Class*>(instance)->*getter)();
		}

		virtual void setValue(DescribedBase* instance, const T& value) const
		{
			(static_cast<Class*>(instance)->*setter)(value);
		}

	private:
		Getter getter; // 0x04
		Setter setter; // 0x10
	};

	template <class Getter, class Setter>
	EnumPropDescriptor(
		const char* name,
		const char* category,
		Getter getter,
		Setter setter,
		typename PropertyDescriptor::Functionality functionality = PropertyDescriptor::STANDARD
	)
		: EnumPropertyDescriptor(Class::classDescriptor(), EnumDesc<T>::singleton(), name, category, functionality),
		  getset(new GetSetImpl<Getter, Setter>(getter, setter))
	{
	}

	virtual bool isReadOnly() const { return false; }

	virtual bool equalValues(const DescribedBase* a, const DescribedBase* b) const
	{
		return getset->getValue(a) == getset->getValue(b);
	}

	virtual bool hasStringValue() const { return true; }

	virtual std::string getStringValue(const DescribedBase* instance) const
	{
		STUB(0x10094440);
		return std::string();
	}

	virtual bool setStringValue(DescribedBase* instance, const std::string& value) const
	{
		STUB(0x10094450);
		return false;
	}

private:
	virtual void writeValue(const DescribedBase* instance, XmlElement* element) const { STUB(0x10094460); }

	virtual void readValue(DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder) const
	{
		STUB(0x10094470);
	}

	std::auto_ptr<GetSet> getset; // 0x1c
};

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_PROPERTY_H
