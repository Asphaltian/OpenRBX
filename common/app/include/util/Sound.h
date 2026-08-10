#ifndef UTIL_SOUND_H
#define UTIL_SOUND_H

#include "decomp.h"
#include "reflection/type.h"
#include "util/ContentProvider.h"

extern char sStockSound[];

namespace RBX {
namespace Soundscape {

extern char sSoundService[];

extern char sSoundChannel[];

// SIZE 0x20
class SoundId : public ContentId
{
public:
	SoundId() {}

	SoundId(const std::string& id) : ContentId(id) {}

	SoundId(const char* id) : ContentId(id) {}

	SoundId(const ContentId& id) : ContentId(id) {}
};

DECOMP_SIZE_ASSERT(SoundId, 0x20)

} // namespace Soundscape

namespace Reflection {

template <>
const Type& Type::singleton<Soundscape::SoundId>();

} // namespace Reflection

} // namespace RBX

#endif // UTIL_SOUND_H
