#ifndef REFLECTION_TYPE_H
#define REFLECTION_TYPE_H

#include "decomp.h"
#include "reflection/descriptor.h"

#include <boost/any.hpp>
#include <list>
#include <typeinfo>

namespace RBX {
namespace Reflection {

// VTABLE: WEBSERVICE 0x1022da5c
// SIZE 0x10
class Type : public Descriptor
{
public:
	template <class T>
	static const Type& singleton();

	bool operator==(const Type& other) const;
	bool operator!=(const Type& other) const;

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

// SIZE 0x08
class Value
{
public:
	Value();

	bool isVoid() const;
	const Type& type() const;

private:
	const Type* _type; // 0x00
	boost::any value;  // 0x04
};

DECOMP_SIZE_ASSERT(Value, 0x08)

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
// SYNTHETIC: WEBSERVICE 0x10221500
// `RBX::Reflection::Type::singleton<RBX::BrickColor>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10221ea0
// `RBX::Reflection::Type::singleton<void>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10222e90
// `RBX::Reflection::Type::singleton<RBX::Reflection::PropertyDescriptor const *>'::`2'::`dynamic atexit destructor for 'type''
// clang-format on

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_TYPE_H
