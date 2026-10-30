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
// VTABLE: ROBLOX 0x00810f14
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
	// FUNCTION: ROBLOX 0x005a4a00
	Type(const char* name, const std::type_info& type) : Descriptor(name), type(type), tag(Name::lookup(name)) {}

	// FUNCTION: WEBSERVICE 0x1003e9c0
	// STUB: ROBLOX 0x004a4cc0
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
// TEMPLATE: ROBLOX 0x00576470
// RBX::Reflection::RefType::singleton<RBX::Instance *>
// TEMPLATE: WEBSERVICE 0x1005a610
// TEMPLATE: ROBLOX 0x005653e0
// RBX::Reflection::RefType::singleton<RBX::PartInstance *>
// TEMPLATE: WEBSERVICE 0x1006ae30
// TEMPLATE: ROBLOX 0x005b4d80
// RBX::Reflection::RefType::singleton<RBX::Camera *>
// TEMPLATE: WEBSERVICE 0x100e4330
// RBX::Reflection::RefType::singleton<RBX::Hole *>
// TEMPLATE: WEBSERVICE 0x1018c680
// TEMPLATE: ROBLOX 0x004a5c60
// RBX::Reflection::RefType::singleton<RBX::ModelInstance *>
// TEMPLATE: WEBSERVICE 0x10191300
// TEMPLATE: ROBLOX 0x004b0470
// RBX::Reflection::RefType::singleton<RBX::Network::Player *>
// SYNTHETIC: WEBSERVICE 0x10221690
// SYNTHETIC: ROBLOX 0x007f1e30
// `RBX::Reflection::RefType::singleton<RBX::Instance *>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10221900
// SYNTHETIC: ROBLOX 0x007f1c00
// `RBX::Reflection::RefType::singleton<RBX::PartInstance *>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10221c40
// SYNTHETIC: ROBLOX 0x007f2ca0
// `RBX::Reflection::RefType::singleton<RBX::Camera *>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x102241f0
// `RBX::Reflection::RefType::singleton<RBX::Hole *>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10225e40
// SYNTHETIC: ROBLOX 0x007f07c0
// `RBX::Reflection::RefType::singleton<RBX::ModelInstance *>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10225f90
// SYNTHETIC: ROBLOX 0x007f0c60
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
	T get() const
	{
		if (isType<T>()) {
			return boost::any_cast<const T&>(value);
		}

		Value v(*this);
		return v.convert<T>();
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1001c340
	// RBX::Reflection::Value::get<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >
	// clang-format on

	template <class T>
	Value& operator=(const T& rhs)
	{
		_type = &Type::singleton<T>();
		value = rhs;
		return *this;
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1000b060
	// TEMPLATE: ROBLOX 0x0056fc00
	// RBX::Reflection::Value::operator=<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >
	// TEMPLATE: WEBSERVICE 0x1001bf90
	// STUB: ROBLOX 0x00571f10
	// RBX::Reflection::Value::operator=<std::vector<RBX::Reflection::Value,std::allocator<RBX::Reflection::Value> > >
	// TEMPLATE: WEBSERVICE 0x10045d80
	// TEMPLATE: ROBLOX 0x0056fcf0
	// RBX::Reflection::Value::operator=<G3D::Vector3>
	// TEMPLATE: WEBSERVICE 0x10045eb0
	// TEMPLATE: ROBLOX 0x0056fb60
	// RBX::Reflection::Value::operator=<double>
	// TEMPLATE: WEBSERVICE 0x1004ca00
	// TEMPLATE: ROBLOX 0x0056fbb0
	// RBX::Reflection::Value::operator=<bool>
	// TEMPLATE: WEBSERVICE 0x10062800
	// TEMPLATE: ROBLOX 0x0056fc40
	// RBX::Reflection::Value::operator=<RBX::Lua::FunctionRef>
	// TEMPLATE: WEBSERVICE 0x100628f0
	// TEMPLATE: ROBLOX 0x0056fcb0
	// RBX::Reflection::Value::operator=<G3D::CoordinateFrame>
	// TEMPLATE: WEBSERVICE 0x10062930
	// TEMPLATE: ROBLOX 0x0056fd40
	// RBX::Reflection::Value::operator=<G3D::Color3>
	// TEMPLATE: WEBSERVICE 0x10062980
	// TEMPLATE: ROBLOX 0x0056fd90
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
	// TEMPLATE: ROBLOX 0x00412ba0
	// RBX::Reflection::Value::isType<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >
	// TEMPLATE: WEBSERVICE 0x1001c430
	// TEMPLATE: ROBLOX 0x00412bc0
	// RBX::Reflection::Value::isType<RBX::ContentId>
	// TEMPLATE: WEBSERVICE 0x1001c450
	// STUB: ROBLOX 0x00412be0
	// RBX::Reflection::Value::isType<std::vector<RBX::Reflection::Value,std::allocator<RBX::Reflection::Value> > >
	// TEMPLATE: WEBSERVICE 0x1003f630
	// TEMPLATE: ROBLOX 0x0056d680
	// RBX::Reflection::Value::isType<G3D::Vector3>
	// TEMPLATE: WEBSERVICE 0x1003f650
	// TEMPLATE: ROBLOX 0x0056d720
	// RBX::Reflection::Value::isType<boost::shared_ptr<RBX::Reflection::DescribedBase> >
	// TEMPLATE: WEBSERVICE 0x100603c0
	// TEMPLATE: ROBLOX 0x0056d660
	// RBX::Reflection::Value::isType<RBX::Lua::FunctionRef>
	// TEMPLATE: WEBSERVICE 0x100603e0
	// TEMPLATE: ROBLOX 0x0056d6a0
	// RBX::Reflection::Value::isType<G3D::CoordinateFrame>
	// TEMPLATE: WEBSERVICE 0x10060400
	// TEMPLATE: ROBLOX 0x0056d6c0
	// RBX::Reflection::Value::isType<G3D::Color3>
	// TEMPLATE: WEBSERVICE 0x10060420
	// TEMPLATE: ROBLOX 0x0056d6e0
	// RBX::Reflection::Value::isType<RBX::BrickColor>
	// TEMPLATE: WEBSERVICE 0x10060440
	// TEMPLATE: ROBLOX 0x0056d700
	// RBX::Reflection::Value::isType<boost::shared_ptr<RBX::Instance> >
	// TEMPLATE: WEBSERVICE 0x10060530
	// TEMPLATE: ROBLOX 0x0056d7e0
	// RBX::Reflection::Value::isType<boost::shared_ptr<std::vector<boost::shared_ptr<RBX::Instance>,std::allocator<boost::shared_ptr<RBX::Instance> > > const > >
	// clang-format on

	template <class T>
	T cast() const
	{
		return boost::any_cast<T>(value);
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10062840
	// TEMPLATE: ROBLOX 0x0056fc80
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
// SYNTHETIC: ROBLOX 0x004312b0
// RBX::Reflection::Value::operator=

// clang-format off
// TEMPLATE: WEBSERVICE 0x1003fa40
// STUB: ROBLOX 0x005a63f0
// RBX::Reflection::Value::genericConvert<int>
// TEMPLATE: WEBSERVICE 0x1003fc30
// TEMPLATE: ROBLOX 0x005a65a0
// RBX::Reflection::Value::genericConvert<bool>
// TEMPLATE: WEBSERVICE 0x1003fd90
// TEMPLATE: ROBLOX 0x005a66c0
// RBX::Reflection::Value::genericConvert<float>
// TEMPLATE: WEBSERVICE 0x1003fef0
// TEMPLATE: ROBLOX 0x005a67e0
// RBX::Reflection::Value::genericConvert<double>
// TEMPLATE: WEBSERVICE 0x10040010
// TEMPLATE: ROBLOX 0x005a6900
// RBX::Reflection::Value::genericConvert<G3D::Vector3>
// TEMPLATE: WEBSERVICE 0x10040160
// TEMPLATE: ROBLOX 0x005a6af0
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
// SYNTHETIC: ROBLOX 0x007f26d0
// `RBX::Reflection::Type::singleton<boost::shared_ptr<RBX::Reflection::DescribedBase> >'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10221450
// SYNTHETIC: ROBLOX 0x007f26e0
// `RBX::Reflection::Type::singleton<boost::shared_ptr<RBX::Instance> >'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10221460
// SYNTHETIC: ROBLOX 0x007f26f0
// `RBX::Reflection::Type::singleton<boost::shared_ptr<std::vector<boost::shared_ptr<RBX::Instance>,std::allocator<boost::shared_ptr<RBX::Instance> > > const > >'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10221470
// SYNTHETIC: ROBLOX 0x007f2700
// `RBX::Reflection::Type::singleton<int>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10221480
// SYNTHETIC: ROBLOX 0x007f2710
// `RBX::Reflection::Type::singleton<bool>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10221490
// SYNTHETIC: ROBLOX 0x007f2720
// `RBX::Reflection::Type::singleton<float>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x102214a0
// SYNTHETIC: ROBLOX 0x007f2730
// `RBX::Reflection::Type::singleton<double>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x102214b0
// SYNTHETIC: ROBLOX 0x007f2740
// `RBX::Reflection::Type::singleton<RBX::ContentId>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x102214c0
// SYNTHETIC: ROBLOX 0x007f2750
// `RBX::Reflection::Type::singleton<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x102214d0
// SYNTHETIC: ROBLOX 0x007f2760
// `RBX::Reflection::Type::singleton<G3D::Vector3>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x102214e0
// SYNTHETIC: ROBLOX 0x007f2780
// `RBX::Reflection::Type::singleton<G3D::Color3>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x102214f0
// `RBX::Reflection::Type::singleton<std::vector<RBX::Reflection::Value,std::allocator<RBX::Reflection::Value> > >'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10221500
// SYNTHETIC: ROBLOX 0x007f27a0
// `RBX::Reflection::Type::singleton<RBX::BrickColor>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10221ea0
// SYNTHETIC: ROBLOX 0x007f26c0
// `RBX::Reflection::Type::singleton<void>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10222e90
// `RBX::Reflection::Type::singleton<RBX::Reflection::PropertyDescriptor const *>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10223070
// SYNTHETIC: ROBLOX 0x007f28b0
// `RBX::Reflection::Type::singleton<G3D::CoordinateFrame>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10223630
// SYNTHETIC: ROBLOX 0x007f2670
// `RBX::Reflection::Type::singleton<RBX::Lua::FunctionRef>'::`2'::`dynamic atexit destructor for 'type''
// clang-format on

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_TYPE_H
