#ifndef V8DATAMODEL_SPAWNLOCATION_H
#define V8DATAMODEL_SPAWNLOCATION_H

#include "placeholder.h"
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

class SpawnLocation : public DescribedCreatable<SpawnLocation, PartInstance, sSpawnLocation>
{
private:
	BrickColor teamColor;
	boost::signals::scoped_connection spawnerTouched;

	void onEvent_spawnerTouched(boost::shared_ptr<Instance> other);

public:
	SpawnLocation();

	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

	bool neutral;
	bool allowTeamChangeOnTouch;
	int forcefieldDuration;

	static Reflection::BoundProp<bool, 1> prop_Neutral;
	static Reflection::BoundProp<bool, 1> prop_AllowTeamChangeOnTouch;
	static Reflection::BoundProp<int, 1> prop_SpawnForcefieldDuration;

	BrickColor getTeamColor() const;
	void setTeamColor(BrickColor value);
};

class SpawnerService : public DescribedCreatable<SpawnerService, Instance, sSpawnerService>, public Service
{
public:
	SpawnerService();
	virtual ~SpawnerService();

	void RegisterSpawner(SpawnLocation* spawner) { spawners.push_back(spawner); }
	void UnregisterSpawner(SpawnLocation* spawner) { spawners.remove(spawner); }
	void SpawnPlayer(boost::shared_ptr<ModelInstance> model, Network::Player* p);

private:
	std::list<SpawnLocation*> spawners;
};

} // namespace RBX

#endif // V8DATAMODEL_SPAWNLOCATION_H
