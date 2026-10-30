#ifndef V8DATAMODEL_FLAGSTAND_H
#define V8DATAMODEL_FLAGSTAND_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8datamodel/BrickColor.h"
#include "v8datamodel/PartInstance.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>
#include <list>

namespace RBX {

class Flag;

extern const char sFlagStandService[];

extern char sFlagStand[];

// SIZE 0x2c4
class FlagStand : public DescribedCreatable<FlagStand, PartInstance, sFlagStand>
{
private:
	boost::signals::scoped_connection standTouched; // 0x290

	void onEvent_standTouched(boost::shared_ptr<Instance> other);

public:
	FlagStand();

	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

	void AffixFlag(Flag* flag);
	bool HasFlag() { return GetFlag() != NULL; }
	Flag* GetFlag();

	BrickColor teamColor; // 0x2a4

	BrickColor getTeamColor() const;
	void setTeamColor(BrickColor value);

	// SYNTHETIC: WEBSERVICE 0x100e8e20
	// SYNTHETIC: ROBLOX 0x00627fd0
	// RBX::FlagStand::~FlagStand

	// SYNTHETIC: WEBSERVICE 0x100e8e70
	// SYNTHETIC: ROBLOX 0x00628020
	// RBX::FlagStand::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(FlagStand, 0x2c4)

// SIZE 0x104
class FlagStandService : public DescribedCreatable<FlagStandService, Instance, sFlagStandService>, public Service
{
private:
	std::list<FlagStand*> flagStands; // 0xf8

public:
	FlagStandService();
	virtual ~FlagStandService();

	// SYNTHETIC: WEBSERVICE 0x100e84c0
	// RBX::FlagStandService::`scalar deleting destructor'

	FlagStand* FindClosestEmptyStandForFlag(Flag* flag);
	FlagStand* FindStandWithFlag(Flag* flag);
	void RegisterFlagStand(FlagStand* flagStand) { flagStands.push_back(flagStand); }
	void UnregisterFlagStand(FlagStand* flagStand) { flagStands.remove(flagStand); }
};

DECOMP_SIZE_ASSERT(FlagStandService, 0x104)

// clang-format off
// FUNCTION: WEBSERVICE 0x1021cea0
// RBX::`dynamic initializer for 'prop_Color''
// FUNCTION: WEBSERVICE 0x1021cf00
// RBX::`dynamic initializer for 'event_FlagCaptured''
// SYNTHETIC: WEBSERVICE 0x10224350
// RBX::`dynamic atexit destructor for 'prop_Color''
// SYNTHETIC: WEBSERVICE 0x10224370
// RBX::`dynamic atexit destructor for 'event_FlagCaptured''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_FLAGSTAND_H
