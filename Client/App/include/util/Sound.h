#ifndef UTIL_SOUND_H
#define UTIL_SOUND_H

#include "placeholder.h"
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

class SoundId : public ContentId
{
public:
	SoundId() {}

	SoundId(const std::string& id) : ContentId(id) {}

	SoundId(const char* id) : ContentId(id) {}

	SoundId(const ContentId& id) : ContentId(id) {}
};

class Sound : private boost::noncopyable
{
public:
	Sound(FMOD::System* system, SoundId id, bool is3D);
	~Sound();

	FMOD::Sound* get();
	void detatch();
	void release();

private:
	FMOD::Sound* fmod_sound;
	FMOD::System* system;

public:
	int refCount;
	const SoundId id;
	const bool is3D;
};

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

class SoundService : public DescribedCreatable<SoundService, Instance, sSoundService>,
					 public Service,
					 public Listener<RunService, Heartbeat>
{
private:
	typedef std::map<SoundType, boost::shared_ptr<SoundChannel> > StockSounds;

	FMOD::System* system;
	unsigned int fmod_version;
	StockSounds stockSounds;
	float dopplerscale;
	float distancefactor;
	float rolloffscale;

	boost::shared_ptr<Instance> statsItem;
	ReverbType ambientReverb;
	double nextGarbageCollectTime;

	typedef std::map<SoundId, boost::shared_ptr<Sound> > LoadedSounds;

	LoadedSounds loadedSounds;
	LoadedSounds loaded3DSounds;

	bool mySoundDisabled;

public:
	static bool soundDisabled;

	boost::shared_ptr<Sound> loadSound(SoundId id, bool is3D);

	bool enabled() const { return system != NULL; }

	void disable();

	SoundService();

	virtual ~SoundService();

	void playSound(SoundType soundType);

	unsigned int getfmod_version() const { return fmod_version; }

private:
	static void getSoundStats(const LoadedSounds& sounds, unsigned int& numSounds, unsigned int& numUnusedSounds);

public:
	void getSoundStats(unsigned int& numSounds, unsigned int& numUnusedSounds) const;

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
	void on3DSettingChanged(const Reflection::PropertyDescriptor&) { update3DSettings(); }
	void updateAmbientReverb();

	friend class SoundChannel;
};

class SoundChannel : public DescribedCreatable<SoundChannel, Instance, sSoundChannel>,
					 public Listener<RunService, Heartbeat>
{
public:
	SoundChannel();

	virtual ~SoundChannel();

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

	virtual void onEvent(const RunService* source, Heartbeat event);

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
	boost::shared_ptr<Sound> sound;
	FMOD::Channel* fmod_channel;
	SoundId soundId;
	float volume;
	bool playOnRemove;
	bool is3D : 1;
	bool looped : 1;
	bool soundDisabled : 1;
	int playCount;
	PartInstance* part;
};

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
