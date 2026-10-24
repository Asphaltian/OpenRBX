#ifndef UTIL_MESHID_H
#define UTIL_MESHID_H

#include "decomp.h"
#include "reflection/type.h"
#include "util/ContentProvider.h"

namespace RBX {

// SIZE 0x20
class MeshId : public ContentId
{
public:
	MeshId() {}

	MeshId(const std::string& id) : ContentId(id) {}

	MeshId(const char* id) : ContentId(id) {}

	MeshId(const ContentId& id) : ContentId(id) {}
};

DECOMP_SIZE_ASSERT(MeshId, 0x20)

namespace Reflection {

template <>
const Type& Type::singleton<MeshId>();

} // namespace Reflection

} // namespace RBX

#endif // UTIL_MESHID_H
