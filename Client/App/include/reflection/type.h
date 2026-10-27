#ifndef REFLECTION_TYPE_H
#define REFLECTION_TYPE_H

#include "decomp.h"
#include "reflection/descriptor.h"
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

// VTABLE: WEBSERVICE 0x1022da5c
// SIZE 0x10
class Type : public Descriptor
{
public:
	template <class T>
	static const Type& singleton();

	bool operator==(const Type& other) const { return this == &other; }
	bool operator!=(const Type& other) const { return this != &other; }

	const std::type_info& type; // 0x08
	const Name& tag;            // 0x0c

protected:
	// FUNCTION: WEBSERVICE 0x1003e950
	Type(const char* name, const std::type_info& type) : Descriptor(name), type(type), tag(Name::lookup(name)) {}

	// FUNCTION: WEBSERVICE 0x1003e9c0
	Type(const char* name, const std::type_info& type, const char* tag)
		: Descriptor(name), type(type), tag(Name::declare(tag, -1))
	{
	}
};

DECOMP_SIZE_ASSERT(Type, 0x10)

// SIZE 0x10
class RefType : public Type
{
public:
	template <class T>
	static const Type& singleton();

private:
	RefType(const char* name, const std::type_info& type) : Type(name, type, "Ref") {}
};

DECOMP_SIZE_ASSERT(RefType, 0x10)

template <class T>
const Type& RefType::singleton()
{
	static RefType type("Object", typeid(RefType));

	return type;
}

// clang-format off
// TEMPLATE: WEBSERVICE 0x10047ec0
// RBX::Reflection::RefType::singleton<RBX::Instance *>
// TEMPLATE: WEBSERVICE 0x1005a610
// RBX::Reflection::RefType::singleton<RBX::PartInstance *>
// TEMPLATE: WEBSERVICE 0x1006ae30
// RBX::Reflection::RefType::singleton<RBX::Camera *>
// TEMPLATE: WEBSERVICE 0x100e4330
// RBX::Reflection::RefType::singleton<RBX::Hole *>
// TEMPLATE: WEBSERVICE 0x1018c680
// RBX::Reflection::RefType::singleton<RBX::ModelInstance *>
// TEMPLATE: WEBSERVICE 0x10191300
// RBX::Reflection::RefType::singleton<RBX::Network::Player *>
// SYNTHETIC: WEBSERVICE 0x10221690
// `RBX::Reflection::RefType::singleton<RBX::Instance *>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10221900
// `RBX::Reflection::RefType::singleton<RBX::PartInstance *>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10221c40
// `RBX::Reflection::RefType::singleton<RBX::Camera *>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x102241f0
// `RBX::Reflection::RefType::singleton<RBX::Hole *>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10225e40
// `RBX::Reflection::RefType::singleton<RBX::ModelInstance *>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10225f90
// `RBX::Reflection::RefType::singleton<RBX::Network::Player *>'::`2'::`dynamic atexit destructor for 'type''
// clang-format on

// SIZE 0x08
class Value
{
public:
	Value();

	template <class T>
	Value(const T& value) : _type(&Type::singleton<T>()), value(value)
	{
	}

	template <class T>
	Value& operator=(const T& rhs)
	{
		_type = &Type::singleton<T>();
		value = rhs;
		return *this;
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1000b060
	// RBX::Reflection::Value::operator=<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >
	// TEMPLATE: WEBSERVICE 0x10045d80
	// RBX::Reflection::Value::operator=<G3D::Vector3>
	// TEMPLATE: WEBSERVICE 0x10045eb0
	// RBX::Reflection::Value::operator=<double>
	// TEMPLATE: WEBSERVICE 0x1004ca00
	// RBX::Reflection::Value::operator=<bool>
	// TEMPLATE: WEBSERVICE 0x10062800
	// RBX::Reflection::Value::operator=<RBX::Lua::FunctionRef>
	// TEMPLATE: WEBSERVICE 0x100628f0
	// RBX::Reflection::Value::operator=<G3D::CoordinateFrame>
	// TEMPLATE: WEBSERVICE 0x10062930
	// RBX::Reflection::Value::operator=<G3D::Color3>
	// TEMPLATE: WEBSERVICE 0x10062980
	// RBX::Reflection::Value::operator=<RBX::BrickColor>
	// clang-format on

	bool isVoid() const;
	const Type& type() const { return *_type; }

	template <class T>
	bool isType() const
	{
		return typeid(T) == type().type;
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1001c410
	// RBX::Reflection::Value::isType<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >
	// TEMPLATE: WEBSERVICE 0x1001c430
	// RBX::Reflection::Value::isType<RBX::ContentId>
	// TEMPLATE: WEBSERVICE 0x1001c450
	// RBX::Reflection::Value::isType<std::vector<RBX::Reflection::Value,std::allocator<RBX::Reflection::Value> > >
	// TEMPLATE: WEBSERVICE 0x1003f630
	// RBX::Reflection::Value::isType<G3D::Vector3>
	// TEMPLATE: WEBSERVICE 0x1003f650
	// RBX::Reflection::Value::isType<boost::shared_ptr<RBX::Reflection::DescribedBase> >
	// TEMPLATE: WEBSERVICE 0x100603c0
	// RBX::Reflection::Value::isType<RBX::Lua::FunctionRef>
	// TEMPLATE: WEBSERVICE 0x100603e0
	// RBX::Reflection::Value::isType<G3D::CoordinateFrame>
	// TEMPLATE: WEBSERVICE 0x10060400
	// RBX::Reflection::Value::isType<G3D::Color3>
	// TEMPLATE: WEBSERVICE 0x10060420
	// RBX::Reflection::Value::isType<RBX::BrickColor>
	// TEMPLATE: WEBSERVICE 0x10060440
	// RBX::Reflection::Value::isType<boost::shared_ptr<RBX::Instance> >
	// TEMPLATE: WEBSERVICE 0x10060530
	// RBX::Reflection::Value::isType<boost::shared_ptr<std::vector<boost::shared_ptr<RBX::Instance>,std::allocator<boost::shared_ptr<RBX::Instance> > > const > >
	// clang-format on

	template <class T>
	T cast() const
	{
		return boost::any_cast<T>(value);
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10062840
	// ??$cast@V?$shared_ptr@$$CBV?$vector@V?$shared_ptr@VInstance@RBX@@@boost@@V?$allocator@V?$shared_ptr@VInstance@RBX@@@boost@@@std@@@std@@@boost@@@Value@Reflection@RBX@@QBE?AV?$shared_ptr@$$CBV?$vector@V?$shared_ptr@VInstance@RBX@@@boost@@V?$allocator@V?$shared_ptr@VInstance@RBX@@@boost@@@std@@@std@@@boost@@XZ
	// clang-format on

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

	const Type* _type; // 0x00
	boost::any value;  // 0x04
};

DECOMP_SIZE_ASSERT(Value, 0x08)

// SYNTHETIC: WEBSERVICE 0x1005f960
// RBX::Reflection::Value::operator=

// clang-format off
// TEMPLATE: WEBSERVICE 0x1003fa40
// RBX::Reflection::Value::genericConvert<int>
// TEMPLATE: WEBSERVICE 0x1003fc30
// RBX::Reflection::Value::genericConvert<bool>
// TEMPLATE: WEBSERVICE 0x1003fd90
// RBX::Reflection::Value::genericConvert<float>
// TEMPLATE: WEBSERVICE 0x1003fef0
// RBX::Reflection::Value::genericConvert<double>
// TEMPLATE: WEBSERVICE 0x10040010
// RBX::Reflection::Value::genericConvert<G3D::Vector3>
// TEMPLATE: WEBSERVICE 0x10040160
// RBX::Reflection::Value::genericConvert<RBX::ContentId>
// clang-format on
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

// SIZE 0x10
class SignatureDescriptor
{
public:
	// SIZE 0x10
	struct Item
	{
		const Name* name;         // 0x00
		const Type* type;         // 0x04
		const Value defaultValue; // 0x08
	};

	typedef std::list<Item> Arguments;

	SignatureDescriptor();

	void addArgument(const Name& name, const Type& type, const Value& defaultValue);

	const Type* resultType; // 0x00
	Arguments arguments;    // 0x04
};

DECOMP_SIZE_ASSERT(SignatureDescriptor::Item, 0x10)
DECOMP_SIZE_ASSERT(SignatureDescriptor, 0x10)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10221440
// `RBX::Reflection::Type::singleton<boost::shared_ptr<RBX::Reflection::DescribedBase> >'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10221450
// `RBX::Reflection::Type::singleton<boost::shared_ptr<RBX::Instance> >'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10221460
// `RBX::Reflection::Type::singleton<boost::shared_ptr<std::vector<boost::shared_ptr<RBX::Instance>,std::allocator<boost::shared_ptr<RBX::Instance> > > const > >'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10221470
// `RBX::Reflection::Type::singleton<int>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10221480
// `RBX::Reflection::Type::singleton<bool>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10221490
// `RBX::Reflection::Type::singleton<float>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x102214a0
// `RBX::Reflection::Type::singleton<double>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x102214b0
// `RBX::Reflection::Type::singleton<RBX::ContentId>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x102214c0
// `RBX::Reflection::Type::singleton<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x102214d0
// `RBX::Reflection::Type::singleton<G3D::Vector3>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x102214e0
// `RBX::Reflection::Type::singleton<G3D::Color3>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x102214f0
// `RBX::Reflection::Type::singleton<std::vector<RBX::Reflection::Value,std::allocator<RBX::Reflection::Value> > >'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10221500
// `RBX::Reflection::Type::singleton<RBX::BrickColor>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10221ea0
// `RBX::Reflection::Type::singleton<void>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10222e90
// `RBX::Reflection::Type::singleton<RBX::Reflection::PropertyDescriptor const *>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10223070
// `RBX::Reflection::Type::singleton<G3D::CoordinateFrame>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10223630
// `RBX::Reflection::Type::singleton<RBX::Lua::FunctionRef>'::`2'::`dynamic atexit destructor for 'type''
// clang-format on

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_TYPE_H
