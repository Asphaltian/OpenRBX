#ifndef UTIL_TEXTUREID_H
#define UTIL_TEXTUREID_H

#include "decomp.h"
#include "reflection/type.h"
#include "util/ContentProvider.h"

namespace RBX {

// SIZE 0x20
class TextureId : public ContentId
{
public:
	TextureId() {}

	TextureId(const std::string& id) : ContentId(id) {}
};

DECOMP_SIZE_ASSERT(TextureId, 0x20)

namespace Reflection {

template <>
const Type& Type::singleton<TextureId>();

} // namespace Reflection

} // namespace RBX

#endif // UTIL_TEXTUREID_H
