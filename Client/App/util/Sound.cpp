#include "util/Sound.h"

#include "util/Math.h"
#include "util/Name.h"
#include "util/StandardOut.h"
#include "util/Velocity.h"
#include "v8datamodel/DebugSettings.h"
#include "v8datamodel/GameSettings.h"
#include "v8datamodel/PartInstance.h"

#include <G3D/format.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <stdexcept>

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

// FUNCTION: WEBSERVICE 0x1007cdb0 FOLDED
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

} // namespace Soundscape
} // namespace RBX

// FUNCTION: WEBSERVICE 0x1007d490
void checkResult(FMOD_RESULT result)
{
	if (result != FMOD_OK) {
		std::string message = G3D::format("FMOD %d: %s", result, FMOD_ErrorString(result));
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_WARNING, message.c_str());
		throw std::runtime_error(message);
	}
}

// FUNCTION: WEBSERVICE 0x1007d570
bool convert(const G3D::Vector3& src, FMOD_VECTOR& dst)
{
	G3D::Vector3 filter = src;

	if (RBX::Math::isDenormal(filter.x)) {
		filter.x = 0.0f;
	}
	if (RBX::Math::isDenormal(filter.y)) {
		filter.y = 0.0f;
	}
	if (RBX::Math::isDenormal(filter.z)) {
		filter.z = 0.0f;
	}

	if (RBX::Math::isNanInfDenormVector3(filter)) {
		return false;
	}

	filter = filter.clamp(-5000.0f, 5000.0f);
	dst.x = filter.x;
	dst.y = filter.y;
	dst.z = filter.z;

	return true;
}

namespace RBX {
namespace Soundscape {

// FUNCTION: WEBSERVICE 0x1007db00
SoundChannel::~SoundChannel()
{
}

} // namespace Soundscape
} // namespace RBX

// FUNCTION: WEBSERVICE 0x1007ebe0
void checkResultNoThrow(FMOD_RESULT result)
{
	if (result != FMOD_OK) {
		if (RBX::DebugSettings::singleton().soundWarnings) {
			char err[512];
			strncpy(err, FMOD_ErrorString(result), 512);
			RBX::StandardOut::singleton()->print(RBX::MESSAGE_WARNING, "FMOD %d: %s", result, err);
		}
	}
}

namespace RBX {
namespace Soundscape {

// FUNCTION: WEBSERVICE 0x1007fd70
void SoundChannel::update3D(FMOD::Channel* channel)
{
	if (is3D && part) {
		FMOD_VECTOR pos;
		if (convert(part->getCoordinateFrame().translation, pos)) {
			FMOD_VECTOR vel;
			if (convert(part->getLinearVelocity(), vel)) {
				checkResultNoThrow(channel->set3DAttributes(&pos, &vel));
			}
		}
	}
}

// FUNCTION: WEBSERVICE 0x10080830
void SoundChannel::onEvent(const RunService* source, Heartbeat event)
{
	if (!isPaused()) {
		update3D(fmod_channel);
	}
}

// FUNCTION: WEBSERVICE 0x100e44b0 FOLDED
bool SoundChannel::askSetParent(const Instance* parent) const
{
	return true;
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
template RBX::GameSettings& RBX::GlobalSettingsItem<RBX::GameSettings, RBX::sGameSettings>::singleton();
