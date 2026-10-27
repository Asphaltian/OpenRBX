#ifndef V8DATAMODEL_SPAWNLOCATION_H
#define V8DATAMODEL_SPAWNLOCATION_H

#include "decomp.h"
#include "reflection/property.h"
#include "util/Handle.h"
#include "v8datamodel/BrickColor.h"
#include "v8datamodel/PartInstance.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>
#include <list>

namespace RBX {

class ModelInstance;

namespace Network {
class Player;
}

extern const char sSpawnerService[];

extern char sSpawnLocation[];

// SIZE 0x2cc
class SpawnLocation : public DescribedCreatable<SpawnLocation, PartInstance, sSpawnLocation>
{
private:
	BrickColor teamColor;                             // 0x290
	boost::signals::scoped_connection spawnerTouched; // 0x294

	void onEvent_spawnerTouched(boost::shared_ptr<Instance> other);

public:
	SpawnLocation();

	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

	bool neutral;                // 0x2a8
	bool allowTeamChangeOnTouch; // 0x2a9
	int forcefieldDuration;      // 0x2ac

	static Reflection::BoundProp<bool, 1> prop_Neutral;
	static Reflection::BoundProp<bool, 1> prop_AllowTeamChangeOnTouch;
	static Reflection::BoundProp<int, 1> prop_SpawnForcefieldDuration;

	BrickColor getTeamColor() const;
	void setTeamColor(BrickColor value);

	// SYNTHETIC: WEBSERVICE 0x100ff230
	// RBX::SpawnLocation::~SpawnLocation

	// SYNTHETIC: WEBSERVICE 0x100ff280
	// RBX::SpawnLocation::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(SpawnLocation, 0x2cc)

// SIZE 0x104
class SpawnerService : public DescribedCreatable<SpawnerService, Instance, sSpawnerService>, public Service
{
public:
	SpawnerService();
	virtual ~SpawnerService();

	void RegisterSpawner(SpawnLocation* spawner) { spawners.push_back(spawner); }
	void UnregisterSpawner(SpawnLocation* spawner) { spawners.remove(spawner); }
	void SpawnPlayer(boost::shared_ptr<ModelInstance> model, Network::Player* p);

private:
	std::list<SpawnLocation*> spawners; // 0x0f8
};

DECOMP_SIZE_ASSERT(SpawnerService, 0x104)

// clang-format off
// FUNCTION: WEBSERVICE 0x1021dbb0
// `dynamic initializer for 'RBX::SpawnLocation::prop_SpawnForcefieldDuration''
// FUNCTION: WEBSERVICE 0x1021dbe0
// `dynamic initializer for 'RBX::SpawnLocation::prop_Neutral''
// FUNCTION: WEBSERVICE 0x1021dc10
// `dynamic initializer for 'RBX::SpawnLocation::prop_AllowTeamChangeOnTouch''
// FUNCTION: WEBSERVICE 0x1021dc40
// RBX::`dynamic initializer for 'prop_TeamColor''
// SYNTHETIC: WEBSERVICE 0x10224c40
// `dynamic atexit destructor for 'RBX::SpawnLocation::prop_AllowTeamChangeOnTouch''
// SYNTHETIC: WEBSERVICE 0x10224c60
// `dynamic atexit destructor for 'RBX::SpawnLocation::prop_Neutral''
// SYNTHETIC: WEBSERVICE 0x10224c80
// `dynamic atexit destructor for 'RBX::SpawnLocation::prop_SpawnForcefieldDuration''
// SYNTHETIC: WEBSERVICE 0x10224ca0
// RBX::`dynamic atexit destructor for 'prop_TeamColor''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_SPAWNLOCATION_H
