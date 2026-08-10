#include "util/Sound.h"

#include "util/Name.h"
#include "v8datamodel/GameSettings.h"

#include <fmod.hpp>

char sStockSound[] = "StockSound";

namespace RBX {
namespace Soundscape {

char sSoundService[] = "SoundService";

char sSoundChannel[] = "Sound";

// FUNCTION: WEBSERVICE 0x1007c9b0
int SoundChannel::getPlayCount() const
{
	return playCount;
}

// FUNCTION: WEBSERVICE 0x1007cd50
bool SoundChannel::isPaused() const
{
	if (fmod_channel == NULL) {
		return true;
	}

	bool paused;
	if (fmod_channel->getPaused(&paused) == FMOD_ERR_INVALID_HANDLE) {
		return true;
	}

	return paused;
}

// FUNCTION: WEBSERVICE 0x1007cd80
bool SoundChannel::isPlaying() const
{
	if (fmod_channel == NULL) {
		return false;
	}

	bool playing;
	if (fmod_channel->isPlaying(&playing) == FMOD_ERR_INVALID_HANDLE) {
		return false;
	}

	return playing;
}

// FUNCTION: WEBSERVICE 0x1007cdb0
SoundId SoundChannel::getSoundId() const
{
	return soundId;
}

// FUNCTION: WEBSERVICE 0x1007cdf0
bool SoundChannel::getLooped() const
{
	return looped;
}

// FUNCTION: WEBSERVICE 0x1007d420
Sound::Sound(FMOD::System* system, SoundId id, bool is3D)
	: fmod_sound(NULL), system(system), refCount(0), id(id), is3D(is3D)
{
}

// STUB: WEBSERVICE 0x10080830
void SoundChannel::onEvent(const RunService* source, Heartbeat event)
{
	STUB(0x10080830);
}

// FUNCTION: WEBSERVICE 0x10082d90
SoundChannel::SoundChannel()
	: sound(), fmod_channel(NULL), volume(0.5f), playOnRemove(false), is3D(0), looped(0), soundDisabled(0),
	  playCount(-1), part(NULL)
{
	setName("Sound");
}

} // namespace Soundscape
} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sGameSettings>();
template const RBX::Name& RBX::Name::doDeclare<sStockSound>();
template void RBX::Name::callDoDeclare<RBX::sGameSettings>();
template void RBX::Name::callDoDeclare<sStockSound>();
