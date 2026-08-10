#ifndef UTIL_SOUND_H
#define UTIL_SOUND_H

#include "decomp.h"
#include "reflection/type.h"
#include "util/ContentProvider.h"
#include "util/RunStateOwner.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>

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

class Sound;

// SIZE 0x138
class SoundChannel : public DescribedCreatable<SoundChannel, Instance, sSoundChannel>,
					 public Listener<RunService, Heartbeat>
{
private:
	boost::shared_ptr<Sound> sound; // 0xfc
	undefined4 fmod_channel;        // 0x104
	SoundId soundId;                // 0x108
	float volume;                   // 0x128
	bool playOnRemove;              // 0x12c
	unsigned int is3D : 1;          // 0x12d
	unsigned int looped : 1;        // 0x12d
	unsigned int soundDisabled : 1; // 0x12d
	int playCount;                  // 0x130
	undefined4 part;                // 0x134
};

DECOMP_SIZE_ASSERT(SoundChannel, 0x138)

} // namespace Soundscape

namespace Reflection {

template <>
const Type& Type::singleton<Soundscape::SoundId>();

} // namespace Reflection

} // namespace RBX

#endif // UTIL_SOUND_H
