#ifndef UTIL_MESHID_H
#define UTIL_MESHID_H

#include "reflection/type.h"
#include "placeholder.h"
#include "util/ContentProvider.h"

namespace RBX {

class MeshId : public ContentId
{
public:
	MeshId() {}

	MeshId(const std::string& id) : ContentId(id) {}

	MeshId(const char* id) : ContentId(id) {}

	MeshId(const ContentId& id) : ContentId(id) {}
};

namespace Reflection {

template <>
const Type& Type::singleton<MeshId>();

} // namespace Reflection

} // namespace RBX

#endif // UTIL_MESHID_H
