#ifndef REFLECTION_DESCRIPTOR_H
#define REFLECTION_DESCRIPTOR_H

#include "decomp.h"
#include "util/Name.h"

#include <boost/noncopyable.hpp>

namespace RBX {
namespace Reflection {

// SYNTHETIC: WEBSERVICE 0x10005990
// RBX::Reflection::Descriptor::`scalar deleting destructor'

// VTABLE: WEBSERVICE 0x1022da44
// SIZE 0x08
class Descriptor : public boost::noncopyable
{
public:
	virtual ~Descriptor() {} // vtable+0x00

	const Name& name; // 0x04

protected:
	Descriptor(const char* name) : name(Name::declare(name, -1)) {}
};

DECOMP_SIZE_ASSERT(Descriptor, 0x08)

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_DESCRIPTOR_H
