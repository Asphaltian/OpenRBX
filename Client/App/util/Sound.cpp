#include "util/Sound.h"

#include "reflection/enumconverter.h"
#include "util/Events.h"
#include "util/Math.h"
#include "util/Name.h"
#include "util/StandardOut.h"
#include "util/Velocity.h"
#include "v8datamodel/DebugSettings.h"
#include "v8datamodel/GameSettings.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Workspace.h"
#include "v8xml/XmlElement.h"

#include <G3D/System.h>
#include <G3D/format.h>
#include <algorithm>
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

// FUNCTION: WEBSERVICE 0x1007cde0 FOLDED
float SoundChannel::getVolume() const
{
	return volume;
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

RBX::Reflection::BoundProp<float, 1> RBX::Soundscape::SoundService::prop_dopplerscale(
	"DopplerScale",
	"Data",
	&RBX::Soundscape::SoundService::dopplerscale,
	&RBX::Soundscape::SoundService::on3DSettingChanged
);
RBX::Reflection::BoundProp<float, 1> RBX::Soundscape::SoundService::prop_distancefactor(
	"DistanceFactor",
	"Data",
	&RBX::Soundscape::SoundService::distancefactor,
	&RBX::Soundscape::SoundService::on3DSettingChanged
);
RBX::Reflection::BoundProp<float, 1> RBX::Soundscape::SoundService::prop_rolloffscale(
	"RolloffScale",
	"Data",
	&RBX::Soundscape::SoundService::rolloffscale,
	&RBX::Soundscape::SoundService::on3DSettingChanged
);
RBX::Reflection::EnumPropDescriptor<RBX::Soundscape::SoundService, RBX::Soundscape::ReverbType> RBX::Soundscape::
	SoundService::prop_AmbientReverb(
		"AmbientReverb",
		"Data",
		&RBX::Soundscape::SoundService::getAmbientReverb,
		&RBX::Soundscape::SoundService::setAmbientReverb
	);
static RBX::Reflection::PropDescriptor<RBX::Soundscape::SoundChannel, RBX::Soundscape::SoundId> sound_desc_SoundId(
	"SoundId",
	"Data",
	&RBX::Soundscape::SoundChannel::getSoundId,
	&RBX::Soundscape::SoundChannel::setSoundId
);
static RBX::Reflection::PropDescriptor<RBX::Soundscape::SoundChannel, float> sound_desc_Volume(
	"Volume",
	"Data",
	&RBX::Soundscape::SoundChannel::getVolume,
	&RBX::Soundscape::SoundChannel::setVolume
);
static RBX::Reflection::PropDescriptor<RBX::Soundscape::SoundChannel, int> sound_desc_PlayCount(
	"PlayCount",
	"Data",
	&RBX::Soundscape::SoundChannel::getPlayCount,
	&RBX::Soundscape::SoundChannel::setPlayCount,
	RBX::Reflection::PropertyDescriptor::STREAMING
);
static RBX::Reflection::PropDescriptor<RBX::Soundscape::SoundChannel, bool> sound_desc_Looped(
	"Looped",
	"Behavior",
	&RBX::Soundscape::SoundChannel::getLooped,
	&RBX::Soundscape::SoundChannel::setLooped
);
static RBX::Reflection::PropDescriptor<RBX::Soundscape::SoundChannel, bool> sound_desc_IsPlaying(
	"IsPlaying",
	"Data",
	&RBX::Soundscape::SoundChannel::isPlaying,
	0,
	RBX::Reflection::PropertyDescriptor::UI
);
static RBX::Reflection::PropDescriptor<RBX::Soundscape::SoundChannel, bool> sound_desc_isPaused(
	"IsPaused",
	"Data",
	&RBX::Soundscape::SoundChannel::isPaused,
	0,
	RBX::Reflection::PropertyDescriptor::UI
);
RBX::Reflection::BoundProp<bool, 1> RBX::Soundscape::SoundChannel::sound_desc_playOnRemove(
	"PlayOnRemove",
	"Behavior",
	&RBX::Soundscape::SoundChannel::playOnRemove
);
static RBX::Reflection::BoundFuncDesc<RBX::Soundscape::SoundChannel, void(), 0> sound_playFunction(
	&RBX::Soundscape::SoundChannel::play,
	"Play",
	RBX::Reflection::FunctionDescriptor::AnyCaller
);
static RBX::Reflection::BoundFuncDesc<RBX::Soundscape::SoundChannel, void(), 0> sound_pauseFunction(
	&RBX::Soundscape::SoundChannel::pause,
	"Pause",
	RBX::Reflection::FunctionDescriptor::AnyCaller
);
static RBX::Reflection::BoundFuncDesc<RBX::Soundscape::SoundChannel, void(), 0> sound_stopFunction(
	&RBX::Soundscape::SoundChannel::stop,
	"Stop",
	RBX::Reflection::FunctionDescriptor::AnyCaller
);

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

// FUNCTION: WEBSERVICE 0x100ff530 FOLDED
template <>
std::string StringConverter<Soundscape::SoundId>::convertToString(const Soundscape::SoundId& value)
{
	return value.toString();
}

template <>
bool StringConverter<Soundscape::SoundId>::convertToValue(const std::string& text, Soundscape::SoundId& value)
{
	value = Soundscape::SoundId(text);
	return true;
}

namespace Reflection {

template <>
const Type& Type::singleton<Soundscape::SoundId>()
{
	return singleton<ContentId>();
}

// FUNCTION: WEBSERVICE 0x1012dd70 FOLDED
template <>
void TypedPropertyDescriptor<Soundscape::SoundId>::readValue(
	DescribedBase* instance,
	const XmlElement* element,
	IReferenceBinder& binder
) const
{
	if (!element->isXsiNil()) {
		ContentId value;
		if (element->getValue(value)) {
			setValue(instance, Soundscape::SoundId(value));
		}
	}
}

template <>
void TypedPropertyDescriptor<Soundscape::SoundId>::writeValue(const DescribedBase* instance, XmlElement* element) const
{
	element->setValue(getValue(instance));
}

template <>
std::string TypedPropertyDescriptor<Soundscape::SoundId>::getStringValue(const DescribedBase* instance) const
{
	return StringConverter<Soundscape::SoundId>::convertToString(getValue(instance));
}

template <>
bool TypedPropertyDescriptor<Soundscape::SoundId>::setStringValue(
	DescribedBase* instance,
	const std::string& text
) const
{
	Soundscape::SoundId value;
	if (StringConverter<Soundscape::SoundId>::convertToValue(text, value)) {
		setValue(instance, value);
		return true;
	}

	return false;
}

} // namespace Reflection
} // namespace RBX

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

// FUNCTION: WEBSERVICE 0x1007fbf0
void SoundService::update3DSettings()
{
	if (system) {
		checkResultNoThrow(system->set3DSettings(dopplerscale, distancefactor, rolloffscale));
	}
}

// FUNCTION: WEBSERVICE 0x1007fc30
void SoundChannel::releaseChannel()
{
	if (fmod_channel) {
		checkResultNoThrow(fmod_channel->setUserData(NULL));

		fmod_channel = NULL;
	}

	if (sound) {
		sound->refCount--;
		sound.reset();
	}
}

// FUNCTION: WEBSERVICE 0x1007fcc0
void SoundChannel::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	releaseChannel();
	Instance::onServiceProvider(oldProvider, newProvider);
}

// FUNCTION: WEBSERVICE 0x1007fce0
void SoundChannel::updateLooped()
{
	if (fmod_channel) {
		FMOD_MODE mode;
		checkResultNoThrow(fmod_channel->getMode(&mode));
		mode |= looped ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		mode &= looped ? ~FMOD_LOOP_OFF : ~FMOD_LOOP_NORMAL;
		checkResultNoThrow(fmod_channel->setMode(mode));
		checkResultNoThrow(fmod_channel->setLoopCount(looped ? -1 : 0));
	}
}

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

// FUNCTION: WEBSERVICE 0x1007fdf0
FMOD::Sound* Sound::get()
{
	if (!fmod_sound) {
		boost::shared_ptr<const std::string> data = ContentProvider::singleton().requestContentString(id);
		if (!data) {
			return NULL;
		}

		FMOD_MODE mode = is3D ? FMOD_3D : 0;
		mode |= FMOD_OPENMEMORY | (GameSettings::singleton().softwareSound ? FMOD_SOFTWARE : FMOD_HARDWARE);

		FMOD_CREATESOUNDEXINFO info;
		memset(&info, 0, sizeof(FMOD_CREATESOUNDEXINFO));
		info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		info.length = data->size();

		checkResult(system->createSound(data->c_str(), mode, &info, &fmod_sound));
	}

	return fmod_sound;
}

// FUNCTION: WEBSERVICE 0x10080830
void SoundChannel::onEvent(const RunService* source, Heartbeat event)
{
	if (!isPaused()) {
		update3D(fmod_channel);
	}
}

void Sound::release()
{
	if (fmod_sound) {
		checkResultNoThrow(fmod_sound->release());
		fmod_sound = NULL;
	}
}

// FUNCTION: WEBSERVICE 0x10080870
Sound::~Sound()
{
	release();
}

// FUNCTION: WEBSERVICE 0x10081200
SoundService::~SoundService()
{
}

} // namespace Soundscape

namespace Reflection {

// FUNCTION: WEBSERVICE 0x10081670
template <>
EnumDesc<Soundscape::ReverbType>::EnumDesc() : EnumDescriptor("ReverbType", typeid(Soundscape::ReverbType))
{
	addPair(Soundscape::NoReverb, "NoReverb");
	addPair(Soundscape::GenericReverb, "GenericReverb");
	addPair(Soundscape::PaddedCell, "PaddedCell");
	addPair(Soundscape::Room, "Room");
	addPair(Soundscape::Bathroom, "Bathroom");
	addPair(Soundscape::LivingRoom, "LivingRoom");
	addPair(Soundscape::StoneRoom, "StoneRoom");
	addPair(Soundscape::Auditorium, "Auditorium");
	addPair(Soundscape::ConcertHall, "ConcertHall");
	addPair(Soundscape::Cave, "Cave");
	addPair(Soundscape::Arena, "Arena");
	addPair(Soundscape::Hangar, "Hangar");
	addPair(Soundscape::CarpettedHallway, "CarpettedHallway");
	addPair(Soundscape::Hallway, "Hallway");
	addPair(Soundscape::StoneCorridor, "StoneCorridor");
	addPair(Soundscape::Alley, "Alley");
	addPair(Soundscape::Forest, "Forest");
	addPair(Soundscape::City, "City");
	addPair(Soundscape::Mountains, "Mountains");
	addPair(Soundscape::Quarry, "Quarry");
	addPair(Soundscape::Plain, "Plain");
	addPair(Soundscape::ParkingLot, "ParkingLot");
	addPair(Soundscape::SewerPipe, "SewerPipe");
	addPair(Soundscape::UnderWater, "UnderWater");
}

} // namespace Reflection

namespace Soundscape {

// STUB: WEBSERVICE 0x10081930
boost::shared_ptr<Sound> SoundService::loadSound(SoundId id, bool is3D)
{
	STUB(0x10081930);
	return boost::shared_ptr<Sound>();
}

// FUNCTION: WEBSERVICE 0x10082a30
SoundService::SoundService()
	: system(NULL), fmod_version(0), dopplerscale(1.0f), distancefactor(10.0f), rolloffscale(1.0f),
	  ambientReverb(NoReverb), nextGarbageCollectTime(G3D::System::getTick()), mySoundDisabled(false)
{
	setName("Soundscape");
}

// STUB: WEBSERVICE 0x10082bd0
void SoundService::onEvent(const RunService* source, Heartbeat event)
{
	STUB(0x10082bd0);
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

// FUNCTION: WEBSERVICE 0x10082ec0
void SoundChannel::updateListenState()
{
	if (part && is3D && fmod_channel) {
		startListening(ServiceProvider::find<RunService>(this), this);
	}
	else {
		stopListening(ServiceProvider::find<RunService>(this), this);
	}
}

} // namespace Soundscape
} // namespace RBX

// FUNCTION: WEBSERVICE 0x10082f20
FMOD_RESULT F_CALLBACK callbackChannelEnd(
	FMOD_CHANNEL* channel,
	FMOD_CHANNEL_CALLBACKTYPE type,
	int command,
	unsigned int commanddata1,
	unsigned int commanddata2
)
{
	RBX::Soundscape::SoundChannel* sound;
	FMOD_Channel_GetUserData(channel, reinterpret_cast<void**>(&sound));
	if (sound) {
		sound->onChannelEnd();
	}

	return FMOD_OK;
}

namespace RBX {
namespace Soundscape {

void SoundChannel::onChannelEnd()
{
	releaseChannel();
	updateListenState();
}

// FUNCTION: WEBSERVICE 0x10083540
void SoundChannel::playSound(Instance* context)
{
	if (soundDisabled) {
		return;
	}

	try {
		if (looped && fmod_channel) {
			bool playing;
			if (fmod_channel->isPlaying(&playing) != FMOD_ERR_INVALID_HANDLE && playing) {
				checkResult(fmod_channel->setPaused(false));
				return;
			}
		}

		SoundService* soundService = ServiceProvider::create<SoundService>(context);

		if (soundService) {
			if (!soundService->enabled()) {
				soundDisabled = true;
				return;
			}

			releaseChannel();

			is3D = part != NULL;
			sound = soundService->loadSound(soundId, is3D);
			sound->refCount++;
			FMOD::Sound* fmodSound = sound->get();
			if (fmodSound) {
				checkResult(soundService->system->playSound(FMOD_CHANNEL_FREE, fmodSound, true, &fmod_channel));

				updateLooped();

				update3D(fmod_channel);

				checkResult(fmod_channel->setVolume(volume));

				checkResult(fmod_channel->setUserData(this));
				checkResult(fmod_channel->setCallback(FMOD_CHANNEL_CALLBACKTYPE_END, callbackChannelEnd, 0));

				checkResult(fmod_channel->setPaused(false));

				updateListenState();
			}
		}
	}
	catch (std::exception& e) {
		StandardOut::singleton()->print(MESSAGE_ERROR, e.what());
	}
}

// STUB: WEBSERVICE 0x10083a00
void SoundService::setAmbientReverb(const ReverbType& value)
{
	STUB(0x10083a00);
}

// FUNCTION: WEBSERVICE 0x10083a30
void SoundChannel::setSoundId(SoundId value)
{
	if (soundId != value) {
		soundId = value;

		if (!soundDisabled && GameSettings::singleton().soundEnabled) {
			ContentProvider::singleton().requestContentString(soundId);
		}

		releaseChannel();

		raisePropertyChanged(sound_desc_SoundId);
	}
}

// FUNCTION: WEBSERVICE 0x10083b10
void SoundChannel::setVolume(float value)
{
	value = G3D::clamp(value, 0.0f, 1.0f);
	if (volume != value) {
		volume = value;
		if (fmod_channel) {
			checkResultNoThrow(fmod_channel->setVolume(volume));
		}

		raisePropertyChanged(sound_desc_Volume);
	}
}

// FUNCTION: WEBSERVICE 0x10083b90
void SoundChannel::setLooped(bool value)
{
	if (looped != value) {
		looped = value;
		updateLooped();

		raisePropertyChanged(sound_desc_Looped);
	}
}

// FUNCTION: WEBSERVICE 0x10083bd0
void SoundChannel::play()
{
	playSound(this);
	playCount = std::max(playCount + 1, 1);
	raisePropertyChanged(sound_desc_PlayCount);
}

// FUNCTION: WEBSERVICE 0x10083c20
void SoundChannel::pause()
{
	if (fmod_channel) {
		checkResultNoThrow(fmod_channel->setPaused(true));
	}

	updateListenState();

	if (playCount != 0) {
		playCount = 0;
		raisePropertyChanged(sound_desc_PlayCount);
	}
}

// FUNCTION: WEBSERVICE 0x10083c70
void SoundChannel::stop()
{
	if (fmod_channel) {
		checkResultNoThrow(fmod_channel->setUserData(NULL));
		checkResultNoThrow(fmod_channel->stop());
		fmod_channel = NULL;
	}

	updateListenState();

	if (playCount != -1) {
		playCount = -1;
		raisePropertyChanged(sound_desc_PlayCount);
	}
}

// FUNCTION: WEBSERVICE 0x10083eb0
void SoundChannel::onAncestorChanged(const AncestorChanged& event)
{
	const ServiceProvider* oldProvider = ServiceProvider::findServiceProvider(event.oldParent);
	const ServiceProvider* newProvider = ServiceProvider::findServiceProvider(event.newParent);

	stopListening(oldProvider ? oldProvider->find<RunService>() : NULL, this);

	if (oldProvider) {
		if (looped) {
			stop();
		}
		releaseChannel();
	}

	Instance::onAncestorChanged(event);

	if (event.child == this) {
		part = dynamic_cast<PartInstance*>(event.newParent);
		updateListenState();
	}

	if (newProvider) {
		SoundService* soundService = newProvider->create<SoundService>();
		if (soundService && soundService->enabled()) {
			soundDisabled = false;

			ContentProvider::singleton().requestContentString(soundId);

			updateListenState();
			if (playCount > 0) {
				playSound(this);
			}
		}
	}

	if (oldProvider) {
		Workspace* oldWorkspace = oldProvider->find<Workspace>();
		if (oldWorkspace && (event.oldParent == oldWorkspace || event.oldParent->isDescendentOf(oldWorkspace))) {
			if (newProvider) {
				Workspace* newWorkspace = newProvider->find<Workspace>();
				if (newWorkspace &&
					(event.newParent == newWorkspace || event.newParent->isDescendentOf(newWorkspace))) {
					return;
				}
			}

			if (playOnRemove) {
				playSound(event.oldParent);
			}
			releaseChannel();
		}
	}
}

// FUNCTION: WEBSERVICE 0x10084070
void SoundChannel::setPlayCount(int value)
{
	if (value == -1) {
		stop();
	}
	else if (value == 0) {
		pause();
	}
	else if (value > playCount) {
		playSound(this);
		playCount = value;
		raisePropertyChanged(sound_desc_PlayCount);
	}
}

// STUB: WEBSERVICE 0x10084350
void SoundService::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	STUB(0x10084350);
}

} // namespace Soundscape
} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sGameSettings>();
template const RBX::Name& RBX::Name::doDeclare<sStockSound>();
template void RBX::Name::callDoDeclare<RBX::sGameSettings>();
template void RBX::Name::callDoDeclare<sStockSound>();
template RBX::GameSettings& RBX::GlobalSettingsItem<RBX::GameSettings, RBX::sGameSettings>::singleton();
