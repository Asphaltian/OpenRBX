#ifndef REFLECTION_DESCRIPTOR_H
#define REFLECTION_DESCRIPTOR_H

#include "decomp.h"
#include "util/Name.h"

#include <boost/noncopyable.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace RBX {
namespace Reflection {

// FUNCTION: WEBSERVICE 0x10047470
inline boost::recursive_mutex& sync()
{
	static boost::recursive_mutex s;

	return s;
}

// SYNTHETIC: WEBSERVICE 0x10005990
// RBX::Reflection::Descriptor::`scalar deleting destructor'

// VTABLE: WEBSERVICE 0x1022da44
// SIZE 0x08
class Descriptor : public boost::noncopyable
{
public:
	virtual ~Descriptor() {} // vtable+0x00

	const Name& name; // 0x04

	Descriptor(const char* name) : name(Name::declare(name, -1)) {}
};

DECOMP_SIZE_ASSERT(Descriptor, 0x08)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10221680
// `RBX::Reflection::sync'::`2'::`dynamic atexit destructor for 's''
// clang-format on

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_DESCRIPTOR_H
