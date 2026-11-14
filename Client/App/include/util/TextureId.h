#ifndef UTIL_TEXTUREID_H
#define UTIL_TEXTUREID_H

#include "reflection/type.h"
#include "placeholder.h"
#include "util/ContentProvider.h"

namespace RBX {

class TextureId : public ContentId
{
public:
	TextureId() {}

	TextureId(const std::string& id) : ContentId(id) {}

	TextureId(const char* id) : ContentId(id) {}

	TextureId(const ContentId& id) : ContentId(id) {}
};

namespace Reflection {

template <>
const Type& Type::singleton<TextureId>();

} // namespace Reflection

} // namespace RBX

#endif // UTIL_TEXTUREID_H
