#ifndef REFLECTION_TYPE_H
#define REFLECTION_TYPE_H

#include "reflection/descriptor.h"
#include "placeholder.h"
#include "util/Utilities.h"

#include <G3D/format.h>
#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>
#include <list>
#include <stdexcept>
#include <typeinfo>
#include <vector>

namespace G3D {
class Vector3;
}

namespace RBX {

class ContentId;
class Instance;
namespace Reflection {

class Type : public Descriptor
{
public:
	template <class T>
	static const Type& singleton();

	bool operator==(const Type& other) const { return this == &other; }
	bool operator!=(const Type& other) const { return this != &other; }

	const std::type_info& type;
	const Name& tag;

protected:
	Type(const char* name, const std::type_info& type) : Descriptor(name), type(type), tag(Name::lookup(name)) {}

	Type(const char* name, const std::type_info& type, const char* tag)
		: Descriptor(name), type(type), tag(Name::declare(tag, -1))
	{
	}
};

class RefType : public Type
{
public:
	template <class T>
	static const Type& singleton();

private:
	RefType(const char* name, const std::type_info& type) : Type(name, type, "Ref") {}
};

template <class T>
const Type& RefType::singleton()
{
	static RefType type("Object", typeid(RefType));

	return type;
}

class Value
{
public:
	Value();

	template <class T>
	Value(const T& value) : _type(&Type::singleton<T>()), value(value)
	{
	}

	template <class T>
	T get() const
	{
		if (isType<T>()) {
			return boost::any_cast<const T&>(value);
		}

		Value v(*this);
		return v.convert<T>();
	}

	template <class T>
	Value& operator=(const T& rhs)
	{
		_type = &Type::singleton<T>();
		value = rhs;
		return *this;
	}

	bool isVoid() const;
	const Type& type() const { return *_type; }

	template <class T>
	bool isType() const
	{
		return typeid(T) == type().type;
	}

	template <class T>
	T cast() const
	{
		return boost::any_cast<T>(value);
	}

	template <class T>
	T cast()
	{
		return boost::any_cast<T>(value);
	}

	template <class T>
	T& convert();

private:
	template <class T>
	T& genericConvert();

	const Type* _type;
	boost::any value;
};

template <class T>
T& Value::genericConvert()
{
	if (T* t = boost::any_cast<T>(&value)) {
		return *t;
	}

	if (isType<std::string>()) {
		T v;
		if (StringConverter<T>::convertToValue(*boost::any_cast<std::string>(&value), v)) {
			value = v;
			_type = &Type::singleton<T>();
			return *boost::any_cast<T>(&value);
		}
	}

	throw std::runtime_error(
		G3D::format("Unable to cast %s to %s", _type->tag.c_str(), Type::singleton<T>().tag.c_str())
	);
}

template <class T>
T& Value::convert()
{
	return genericConvert<T>();
}

template <>
int& Value::convert<int>();
template <>
bool& Value::convert<bool>();
template <>
float& Value::convert<float>();
template <>
double& Value::convert<double>();
template <>
G3D::Vector3& Value::convert<G3D::Vector3>();
template <>
ContentId& Value::convert<ContentId>();
template <>
std::string& Value::convert<std::string>();
template <>
boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > >& Value::convert<
	boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > >();
template <>
std::vector<Value>& Value::convert<std::vector<Value> >();
template <>
boost::shared_ptr<Instance>& Value::convert<boost::shared_ptr<Instance> >();

class SignatureDescriptor
{
public:
	struct Item
	{
		const Name* name;
		const Type* type;
		const Value defaultValue;
	};

	typedef std::list<Item> Arguments;

	SignatureDescriptor();

	void addArgument(const Name& name, const Type& type, const Value& defaultValue);

	const Type* resultType;
	Arguments arguments;
};

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_TYPE_H
