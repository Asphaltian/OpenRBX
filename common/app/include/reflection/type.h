#ifndef REFLECTION_TYPE_H
#define REFLECTION_TYPE_H

#include "decomp.h"
#include "reflection/descriptor.h"

#include <typeinfo>

namespace RBX {
namespace Reflection {

// SIZE 0x10
class Type : public Descriptor
{
public:
	bool operator==(const Type& other) const;
	bool operator!=(const Type& other) const;

	const std::type_info& type; // 0x08
	const Name& tag;            // 0x0c

protected:
	Type(const char* name, const std::type_info& type);
	Type(const char* name, const std::type_info& type, const char* tag);
};

DECOMP_SIZE_ASSERT(Type, 0x10)

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_TYPE_H
