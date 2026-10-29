#include "util/Sound.h"

#include "reflection/enumconverter.h"
#include "util/Events.h"
#include "util/Math.h"
#include "util/Name.h"
#include "util/StandardOut.h"
#include "util/Velocity.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/DebugSettings.h"
#include "v8datamodel/GameSettings.h"
#include "v8datamodel/PVInstance.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Stats.h"
#include "v8datamodel/Workspace.h"
#include "v8xml/XmlElement.h"

#include <G3D/System.h>
#include <G3D/format.h>
#include <G3D/g3dmath.h>
#include <algorithm>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <stdexcept>

char sStockSound[] = "StockSound";

static const double gcTime = 5.0;

class StockSound : public RBX::DescribedCreatable<StockSound, RBX::Soundscape::SoundChannel, sStockSound>
{
public:
	StockSound();
};

// SIZE 0x138
class SoundServiceStatsItem : public RBX::Stats::Item
{
private:
	const RBX::Soundscape::SoundService* service; // 0x120
	unsigned int currentalloced;                  // 0x124
	unsigned int maxalloced;                      // 0x128
	unsigned int numSounds;                       // 0x12c
	unsigned int numUnusedSounds;                 // 0x130

public:
	SoundServiceStatsItem(const RBX::Soundscape::SoundService* service);

	virtual void update();
};

namespace RBX {
namespace Soundscape {

char sSoundService[] = "SoundService";

char sSoundChannel[] = "Sound";

// GLOBAL: WEBSERVICE 0x102f6f89
bool SoundService::soundDisabled = false;

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

// FUNCTION: WEBSERVICE 0x1007da70 FOLDED
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

// FUNCTION: WEBSERVICE 0x1007e3c0 FOLDED
template <>
void TypedPropertyDescriptor<Soundscape::SoundId>::writeValue(const DescribedBase* instance, XmlElement* element) const
{
	element->setValue(getValue(instance));
}

// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
template <>
bool TypedPropertyDescriptor<Soundscape::SoundId>::hasStringValue() const
{
	return true;
}

// FUNCTION: WEBSERVICE 0x1007e440 FOLDED
template <>
std::string TypedPropertyDescriptor<Soundscape::SoundId>::getStringValue(const DescribedBase* instance) const
{
	return StringConverter<Soundscape::SoundId>::convertToString(getValue(instance));
}

// FUNCTION: WEBSERVICE 0x1007e4c0 FOLDED
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

namespace RBX {
namespace Soundscape {

// FUNCTION: WEBSERVICE 0x1007e5a0
void SoundService::getSoundStats(const LoadedSounds& sounds, unsigned int& numSounds, unsigned int& numUnusedSounds)
{
	for (LoadedSounds::const_iterator iter = sounds.begin(); iter != sounds.end(); ++iter) {
		numSounds++;
		if (iter->second->refCount == 0) {
			numUnusedSounds++;
		}
	}
}

void SoundService::getSoundStats(unsigned int& numSounds, unsigned int& numUnusedSounds) const
{
	getSoundStats(loadedSounds, numSounds, numUnusedSounds);
	getSoundStats(loaded3DSounds, numSounds, numUnusedSounds);
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

// FUNCTION: WEBSERVICE 0x1007ecc0
void SoundService::updateAmbientReverb()
{
	if (!system) {
		return;
	}

	switch (ambientReverb) {
	case NoReverb: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_OFF;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case GenericReverb: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_GENERIC;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case PaddedCell: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_PADDEDCELL;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case Room: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_PADDEDCELL;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case Bathroom: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_BATHROOM;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case LivingRoom: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_LIVINGROOM;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case StoneRoom: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_STONEROOM;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case Auditorium: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_AUDITORIUM;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case ConcertHall: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_CONCERTHALL;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case Cave: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_CAVE;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case Arena: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_ARENA;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case Hangar: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_HANGAR;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case CarpettedHallway: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_CARPETTEDHALLWAY;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case Hallway: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_HALLWAY;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case StoneCorridor: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_STONECORRIDOR;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case Alley: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_ALLEY;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case Forest: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_FOREST;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case City: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_CITY;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case Mountains: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_MOUNTAINS;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case Quarry: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_QUARRY;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case Plain: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_PLAIN;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case ParkingLot: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_PARKINGLOT;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case SewerPipe: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_SEWERPIPE;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	case UnderWater: {
		FMOD_REVERB_PROPERTIES p = FMOD_PRESET_UNDERWATER;
		checkResultNoThrow(system->setReverbProperties(&p));
		break;
	}
	}
}

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

// FUNCTION: WEBSERVICE 0x10080490
void SoundService::openFmod()
{
	if (soundDisabled || mySoundDisabled || !GameSettings::singleton().soundEnabled) {
		return;
	}

	try {
		checkResult(FMOD::System_Create(&system));

		checkResult(system->getVersion(&fmod_version));
		if (fmod_version < FMOD_VERSION) {
			throw std::runtime_error("Incorrect FMODEX DLL version");
		}

		int driver = -1;
		checkResultNoThrow(system->getDriver(&driver));
		if (driver >= 0) {
			char name[256] = "?";
			checkResultNoThrow(system->getDriverName(driver, name, 256));
		}
		else {
			StandardOut::singleton()->print(MESSAGE_WARNING, "No active sound driver detected");
		}

		FMOD_CAPS caps;
		FMOD_SPEAKERMODE speakermode;
		checkResult(system->getDriverCaps(0, &caps, 0, 0, &speakermode));

		checkResult(system->setSpeakerMode(speakermode));

		if (caps & FMOD_CAPS_HARDWARE_EMULATED) {
			StandardOut::singleton()->print(
				MESSAGE_WARNING,
				"The acceleration slider in the Sound Control Panel set to 'off'"
			);
			checkResult(system->setDSPBufferSize(1024, 10));
		}

		FMOD_RESULT result = system->init(100, FMOD_INIT_NORMAL, 0);
		if (result == FMOD_ERR_OUTPUT_CREATEBUFFER) {
			StandardOut::singleton()->print(
				MESSAGE_WARNING,
				"The speaker mode selected isn't supported by this soundcard.  Using Stereo"
			);
			checkResult(system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO));

			checkResult(system->init(100, FMOD_INIT_NORMAL, 0));
		}

		update3DSettings();
		if (ambientReverb != NoReverb) {
			updateAmbientReverb();
		}
	}
	catch (const std::exception&) {
		if (system) {
			checkResultNoThrow(system->release());
			system = NULL;
		}
	}
}

} // namespace Soundscape
} // namespace RBX

// FUNCTION: WEBSERVICE 0x10080720
static void releaseSound(const std::pair<RBX::Soundscape::SoundId, boost::shared_ptr<RBX::Soundscape::Sound> >& p)
{
	p.second->release();
}

namespace RBX {
namespace Soundscape {

// FUNCTION: WEBSERVICE 0x10080750
void SoundService::gcSounds(LoadedSounds& sounds)
{
	LoadedSounds::iterator iter = sounds.begin();
	while (iter != sounds.end()) {
		if (iter->second->refCount == 0 && G3D::uniformRandom() >= 0.7f) {
			iter->second->release();
			iter = sounds.erase(iter);
		}
		else {
			++iter;
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

// FUNCTION: WEBSERVICE 0x10080d80
void SoundService::closeFmod()
{
	stockSounds.clear();
	removeAllChildren();

	std::for_each(loadedSounds.begin(), loadedSounds.end(), releaseSound);
	std::for_each(loaded3DSounds.begin(), loaded3DSounds.end(), releaseSound);

	loadedSounds.clear();
	loaded3DSounds.clear();

	if (system) {
		checkResultNoThrow(system->release());

		system = NULL;
	}
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

// FUNCTION: WEBSERVICE 0x10081930
boost::shared_ptr<Sound> SoundService::loadSound(SoundId id, bool is3D)
{
	LoadedSounds& sounds = is3D ? loaded3DSounds : loadedSounds;

	LoadedSounds::iterator iter = sounds.find(id);
	if (iter != sounds.end()) {
		return iter->second;
	}

	boost::shared_ptr<Sound> result(new Sound(system, id, is3D));
	sounds[id] = result;
	return result;
}

} // namespace Soundscape

// FUNCTION: WEBSERVICE 0x10082330
void registerSound()
{
	Soundscape::SoundChannel::classDescriptor();
	StockSound::classDescriptor();
	Soundscape::SoundService::classDescriptor();
}

namespace Soundscape {

// FUNCTION: WEBSERVICE 0x10082a30
SoundService::SoundService()
	: system(NULL), fmod_version(0), dopplerscale(1.0f), distancefactor(10.0f), rolloffscale(1.0f),
	  ambientReverb(NoReverb), nextGarbageCollectTime(G3D::System::getTick()), mySoundDisabled(false)
{
	setName("Soundscape");
}

void SoundService::garbageCollectSounds()
{
	if (!(G3D::System::getTick() < nextGarbageCollectTime)) {
		gcSounds(loadedSounds);
		gcSounds(loaded3DSounds);
		nextGarbageCollectTime = G3D::System::getTick() + gcTime;
	}
}

// FUNCTION: WEBSERVICE 0x10082bd0
void SoundService::onEvent(const RunService* source, Heartbeat event)
{
	if (system) {
		Workspace* workspace = ServiceProvider::find<Workspace>(this);
		if (workspace) {
			Camera* camera = workspace->getCamera();
			if (camera) {
				G3D::CoordinateFrame cf = camera->getCameraCoordinateFrame();

				FMOD_VECTOR listener_pos;
				if (convert(cf.translation, listener_pos)) {
					FMOD_VECTOR listener_forward;
					if (convert(-cf.lookVector(), listener_forward)) {
						FMOD_VECTOR listener_up;
						if (convert(cf.upVector(), listener_up)) {
							FMOD_VECTOR listener_vel;
							listener_vel.x = listener_vel.y = listener_vel.z = 0.0f;
							PVInstance* pvInstance = dynamic_cast<PVInstance*>(camera->getCameraSubjectInstance());
							if (pvInstance) {
								PartInstance* part = static_cast<IPrimaryPart*>(pvInstance)->getPrimaryPart();
								if (part) {
									convert(part->getVelocity().linear, listener_vel);
								}
							}

							checkResultNoThrow(system->set3DListenerAttributes(
								0,
								&listener_pos,
								&listener_vel,
								&listener_forward,
								&listener_up
							));
						}
					}
				}
			}
		}

		garbageCollectSounds();
		checkResultNoThrow(system->update());
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

} // namespace Soundscape
} // namespace RBX

// FUNCTION: WEBSERVICE 0x100832c0
SoundServiceStatsItem::SoundServiceStatsItem(const RBX::Soundscape::SoundService* service)
	: service(service), currentalloced(0), maxalloced(0)
{
	setName("Sound");
	createBoundMemChildItem("Current", currentalloced);
	createBoundMemChildItem("Max", maxalloced);
	createBoundChildItem("# Sounds", numSounds);
	createBoundChildItem("# Unused", numUnusedSounds);
}

// FUNCTION: WEBSERVICE 0x10083410
void SoundServiceStatsItem::update()
{
	if (service->enabled()) {
		formatValue(service->getfmod_version(), "fmod %08x", service->getfmod_version());

		int a, b;
		if (FMOD_Memory_GetStats(&a, &b) == FMOD_OK) {
			currentalloced = a;
			maxalloced = b;
		}

		numUnusedSounds = 0;
		numSounds = 0;
		service->getSoundStats(numSounds, numUnusedSounds);
	}
	else {
		setValue(0, "-disabled-");
	}
}

namespace RBX {
namespace Soundscape {

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

} // namespace Soundscape
} // namespace RBX

// FUNCTION: WEBSERVICE 0x10083940
StockSound::StockSound()
{
	propArchivable.setValue(this, false);
}

namespace RBX {
namespace Soundscape {

// FUNCTION: WEBSERVICE 0x10083a00
void SoundService::setAmbientReverb(const ReverbType& value)
{
	if (value != ambientReverb) {
		ambientReverb = value;

		raisePropertyChanged(prop_AmbientReverb);
		updateAmbientReverb();
	}
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

// FUNCTION: WEBSERVICE 0x10083d60
void SoundService::loadStockSound(SoundType soundType, std::string file)
{
	boost::shared_ptr<StockSound> sound = Creatable<Instance>::create<StockSound>();

	sound->setName(Reflection::EnumDesc<SoundType>::singleton().convertToString(soundType));
	sound->setSoundId(ContentId::fromAssets(file));

	sound->setParent(this);
	stockSounds[soundType] = sound;
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

// FUNCTION: WEBSERVICE 0x10084170
void SoundService::loadStockSounds()
{
	stockSounds.clear();
	removeAllChildren();

	loadStockSound(BOING_SOUND, "Sounds\\bass.wav");
	loadStockSound(BOMB_SOUND, "Sounds\\collide.wav");
	loadStockSound(BREAK_SOUND, "Sounds\\glassbreak.wav");
	loadStockSound(CLICK_SOUND, "Sounds\\switch.wav");
	loadStockSound(CLOCK_SOUND, "Sounds\\clickfast.wav");
	loadStockSound(RUBBERBAND_SOUND, "Sounds\\Rubber band sling shot.wav");
	loadStockSound(PAGE_SOUND, "Sounds\\pageturn.wav");
	loadStockSound(PING_SOUND, "Sounds\\electronicpingshort.wav");
	loadStockSound(SNAP_SOUND, "Sounds\\snap.wav");
	loadStockSound(SPLAT_SOUND, "Sounds\\splat.wav");
	loadStockSound(STEP_SOUND, "Sounds\\SWITCH3.wav");
	loadStockSound(STEP_ON_SOUND, "Sounds\\flashbulb.wav");
	loadStockSound(SWOOSH_SOUND, "Sounds\\swoosh.wav");
	loadStockSound(VICTORY_SOUND, "Sounds\\victory.wav");
}

// FUNCTION: WEBSERVICE 0x10084350
void SoundService::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	stopListening(oldProvider ? oldProvider->find<RunService>() : NULL, this);

	if (statsItem) {
		statsItem->setParent(NULL);
		statsItem.reset();
	}

	if (oldProvider) {
		closeFmod();
	}

	Instance::onServiceProvider(oldProvider, newProvider);

	startListening(newProvider ? newProvider->find<RunService>() : NULL, this);

	if (newProvider) {
		openFmod();
		loadStockSounds();

		Stats::StatsService* stats = newProvider->create<Stats::StatsService>();
		if (stats) {
			statsItem = Creatable<Instance>::create<SoundServiceStatsItem>(this);
			statsItem->setParent(stats);
		}
	}
}

} // namespace Soundscape
} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sGameSettings>();
template const RBX::Name& RBX::Name::doDeclare<sStockSound>();
template void RBX::Name::callDoDeclare<RBX::sGameSettings>();
template void RBX::Name::callDoDeclare<sStockSound>();
template RBX::GameSettings& RBX::GlobalSettingsItem<RBX::GameSettings, RBX::sGameSettings>::singleton();
template const RBX::FactoryProduct<StockSound, RBX::Soundscape::SoundChannel, sStockSound>::Creator
	RBX::FactoryProduct<StockSound, RBX::Soundscape::SoundChannel, sStockSound>::creator;
