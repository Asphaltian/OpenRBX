#ifndef UTIL_SOUND_H
#define UTIL_SOUND_H

#include "decomp.h"
#include "reflection/property.h"
#include "reflection/reflection.h"
#include "reflection/type.h"
#include "util/ContentProvider.h"
#include "util/RunStateOwner.h"
#include "util/SoundWorld.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>

extern char sStockSound[];

namespace FMOD {
class Channel;
class Sound;
class System;
} // namespace FMOD

namespace RBX {

class PartInstance;

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

	// FUNCTION: WEBSERVICE 0x1003e930 FOLDED
	// FUNCTION: ROBLOX 0x00412910 FOLDED
	SoundId(const ContentId& id) : ContentId(id) {}
};

DECOMP_SIZE_ASSERT(SoundId, 0x20)

// SIZE 0x30
class Sound : private boost::noncopyable
{
public:
	Sound(FMOD::System* system, SoundId id, bool is3D);
	~Sound();

	FMOD::Sound* get();
	void detatch();
	void release();

private:
	FMOD::Sound* fmod_sound; // 0x00
	FMOD::System* system;    // 0x04

public:
	int refCount;     // 0x08
	const SoundId id; // 0x0c
	const bool is3D;  // 0x2c
};

DECOMP_SIZE_ASSERT(Sound, 0x30)

class SoundChannel;

enum ReverbType
{
	NoReverb = 0,
	GenericReverb = 1,
	PaddedCell = 2,
	Room = 3,
	Bathroom = 4,
	LivingRoom = 5,
	StoneRoom = 6,
	Auditorium = 7,
	ConcertHall = 8,
	Cave = 9,
	Arena = 10,
	Hangar = 11,
	CarpettedHallway = 12,
	Hallway = 13,
	StoneCorridor = 14,
	Alley = 15,
	Forest = 16,
	City = 17,
	Mountains = 18,
	Quarry = 19,
	Plain = 20,
	ParkingLot = 21,
	SewerPipe = 22,
	UnderWater = 23
};

// SIZE 0x150
class SoundService : public DescribedCreatable<SoundService, Instance, sSoundService>,
					 public Service,
					 public Listener<RunService, Heartbeat>
{
private:
	typedef std::map<SoundType, boost::shared_ptr<SoundChannel> > StockSounds;

	FMOD::System* system;      // 0xfc
	unsigned int fmod_version; // 0x100
	StockSounds stockSounds;   // 0x104
	float dopplerscale;        // 0x110
	float distancefactor;      // 0x114
	float rolloffscale;        // 0x118

	boost::shared_ptr<Instance> statsItem; // 0x11c
	ReverbType ambientReverb;              // 0x124
	double nextGarbageCollectTime;         // 0x128

	typedef std::map<SoundId, boost::shared_ptr<Sound> > LoadedSounds;

	LoadedSounds loadedSounds;   // 0x130
	LoadedSounds loaded3DSounds; // 0x13c

	bool mySoundDisabled; // 0x148

public:
	static bool soundDisabled;

	boost::shared_ptr<Sound> loadSound(SoundId id, bool is3D);

	bool enabled() const { return system != NULL; }

	void disable();

	SoundService();

	virtual ~SoundService();

	// SYNTHETIC: WEBSERVICE 0x10081910
	// SYNTHETIC: ROBLOX 0x005c5460
	// RBX::Soundscape::SoundService::`scalar deleting destructor'

	void playSound(SoundType soundType);

	unsigned int getfmod_version() const { return fmod_version; }

private:
	static void getSoundStats(const LoadedSounds& sounds, unsigned int& numSounds, unsigned int& numUnusedSounds);

public:
	void getSoundStats(unsigned int& numSounds, unsigned int& numUnusedSounds) const;

	// FUNCTION: WEBSERVICE 0x1007c9a0
	ReverbType getAmbientReverb() const { return ambientReverb; }

	void setAmbientReverb(const ReverbType& value);

	static Reflection::BoundProp<float, 1> prop_dopplerscale;
	static Reflection::BoundProp<float, 1> prop_distancefactor;
	static Reflection::BoundProp<float, 1> prop_rolloffscale;
	static Reflection::EnumPropDescriptor<SoundService, ReverbType> prop_AmbientReverb;

private:
	void openFmod();
	void closeFmod();
	void garbageCollectSounds();
	static void gcSounds(LoadedSounds& sounds);

protected:
	virtual void onEvent(const RunService* source, Heartbeat event);
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

private:
	void loadStockSounds();
	void loadStockSound(SoundType soundType, std::string file);
	void update3DSettings();
	// FUNCTION: WEBSERVICE 0x10080450
	// FUNCTION: ROBLOX 0x005c3fd0
	void on3DSettingChanged(const Reflection::PropertyDescriptor&) { update3DSettings(); }
	void updateAmbientReverb();

	friend class SoundChannel;
};

DECOMP_SIZE_ASSERT(SoundService, 0x150)

// SIZE 0x138
class SoundChannel : public DescribedCreatable<SoundChannel, Instance, sSoundChannel>,
					 public Listener<RunService, Heartbeat>
{
public:
	SoundChannel();

	virtual ~SoundChannel();

	// clang-format off
	// SYNTHETIC: WEBSERVICE 0x1007e580
	// SYNTHETIC: ROBLOX 0x005c20d0
	// RBX::Soundscape::SoundChannel::`scalar deleting destructor'
	// clang-format on

	void setSoundId(SoundId value);
	SoundId getSoundId() const;

	float getVolume() const;
	void setVolume(float value);

	bool getLooped() const;
	void setLooped(bool value);

	int getPlayCount() const;
	void setPlayCount(int value);

	void play();
	void pause();
	void stop();

	bool isPlaying() const;
	bool isPaused() const;

	static Reflection::BoundProp<bool, 1> sound_desc_playOnRemove;

protected:
	virtual bool askSetParent(const Instance* parent) const;

	virtual void onAncestorChanged(const AncestorChanged& event);

	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

	virtual void onEvent(const RunService* source, Heartbeat event); // vtable+0x00

private:
	void updateListenState();
	void updateLooped();
	void update3D(FMOD::Channel* channel);
	void playSound(Instance* context);
	void playSound();
	void releaseChannel();

public:
	void onChannelEnd();

private:
	boost::shared_ptr<Sound> sound; // 0xfc
	FMOD::Channel* fmod_channel;    // 0x104
	SoundId soundId;                // 0x108
	float volume;                   // 0x128
	bool playOnRemove;              // 0x12c
	bool is3D : 1;                  // 0x12d
	bool looped : 1;                // 0x12d
	bool soundDisabled : 1;         // 0x12d
	int playCount;                  // 0x130
	PartInstance* part;             // 0x134
};

DECOMP_SIZE_ASSERT(SoundChannel, 0x138)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10219640
// SYNTHETIC: ROBLOX 0x007ea990
// `dynamic initializer for 'RBX::Soundscape::SoundService::prop_dopplerscale''
// SYNTHETIC: WEBSERVICE 0x10219670
// SYNTHETIC: ROBLOX 0x007ea9c0
// `dynamic initializer for 'RBX::Soundscape::SoundService::prop_distancefactor''
// SYNTHETIC: WEBSERVICE 0x102196a0
// `dynamic initializer for 'RBX::Soundscape::SoundService::prop_rolloffscale''
// SYNTHETIC: WEBSERVICE 0x102196d0
// `dynamic initializer for 'RBX::Soundscape::SoundService::prop_AmbientReverb''
// SYNTHETIC: WEBSERVICE 0x10219710
// `dynamic initializer for 'sound_desc_SoundId''
// SYNTHETIC: WEBSERVICE 0x10219750
// `dynamic initializer for 'sound_desc_Volume''
// SYNTHETIC: WEBSERVICE 0x10219790
// `dynamic initializer for 'sound_desc_PlayCount''
// SYNTHETIC: WEBSERVICE 0x102197d0
// `dynamic initializer for 'sound_desc_Looped''
// SYNTHETIC: WEBSERVICE 0x10219810
// `dynamic initializer for 'sound_desc_IsPlaying''
// SYNTHETIC: WEBSERVICE 0x10219840
// `dynamic initializer for 'sound_desc_isPaused''
// SYNTHETIC: WEBSERVICE 0x10219870
// `dynamic initializer for 'RBX::Soundscape::SoundChannel::sound_desc_playOnRemove''
// SYNTHETIC: WEBSERVICE 0x102198a0
// `dynamic initializer for 'sound_playFunction''
// SYNTHETIC: WEBSERVICE 0x102198d0
// `dynamic initializer for 'sound_pauseFunction''
// SYNTHETIC: WEBSERVICE 0x10219900
// `dynamic initializer for 'sound_stopFunction''
// SYNTHETIC: WEBSERVICE 0x102222c0
// `dynamic atexit destructor for 'RBX::Soundscape::SoundService::prop_rolloffscale''
// SYNTHETIC: WEBSERVICE 0x102222e0
// `dynamic atexit destructor for 'sound_desc_Volume''
// SYNTHETIC: WEBSERVICE 0x10222300
// `dynamic atexit destructor for 'RBX::Soundscape::SoundService::prop_AmbientReverb''
// SYNTHETIC: WEBSERVICE 0x10222320
// `dynamic atexit destructor for 'RBX::Soundscape::SoundChannel::sound_desc_playOnRemove''
// SYNTHETIC: WEBSERVICE 0x10222340
// SYNTHETIC: ROBLOX 0x007f2f70
// `dynamic atexit destructor for 'RBX::Soundscape::SoundService::prop_distancefactor''
// SYNTHETIC: WEBSERVICE 0x10222360
// SYNTHETIC: ROBLOX 0x007f2f90
// `dynamic atexit destructor for 'RBX::Soundscape::SoundService::prop_dopplerscale''
// SYNTHETIC: WEBSERVICE 0x10222380
// `dynamic atexit destructor for 'sound_desc_isPaused''
// SYNTHETIC: WEBSERVICE 0x102223a0
// `dynamic atexit destructor for 'sound_desc_IsPlaying''
// SYNTHETIC: WEBSERVICE 0x102223c0
// `dynamic atexit destructor for 'sound_desc_Looped''
// SYNTHETIC: WEBSERVICE 0x102223e0
// `dynamic atexit destructor for 'sound_desc_PlayCount''
// SYNTHETIC: WEBSERVICE 0x10222400
// `dynamic atexit destructor for 'sound_desc_SoundId''
// SYNTHETIC: WEBSERVICE 0x102224e0
// `dynamic atexit destructor for 'sound_pauseFunction''
// SYNTHETIC: WEBSERVICE 0x102224f0
// `dynamic atexit destructor for 'sound_stopFunction''
// SYNTHETIC: WEBSERVICE 0x10222510
// `dynamic atexit destructor for 'sound_playFunction''
// clang-format on

} // namespace Soundscape

void registerSound();

namespace Reflection {

template <>
const Type& Type::singleton<Soundscape::SoundId>();
template <>
void TypedPropertyDescriptor<Soundscape::SoundId>::readValue(
	DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder
) const;
template <>
void TypedPropertyDescriptor<Soundscape::SoundId>::writeValue(const DescribedBase* instance, XmlElement* element) const;
template <>
bool TypedPropertyDescriptor<Soundscape::SoundId>::hasStringValue() const;
template <>
std::string TypedPropertyDescriptor<Soundscape::SoundId>::getStringValue(const DescribedBase* instance) const;
template <>
bool TypedPropertyDescriptor<Soundscape::SoundId>::setStringValue(
	DescribedBase* instance, const std::string& text
) const;

} // namespace Reflection

} // namespace RBX

#endif // UTIL_SOUND_H
