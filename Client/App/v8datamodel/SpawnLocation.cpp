#include "v8datamodel/SpawnLocation.h"

#include "humanoid/Humanoid.h"
#include "network/Player.h"
#include "network/Players.h"
#include "reflection/property.h"

#include <boost/bind.hpp>

namespace RBX {

const char sSpawnerService[] = "SpawnerService";

char sSpawnLocation[] = "SpawnLocation";

static Reflection::PropDescriptor<SpawnLocation, BrickColor> prop_TeamColor(
	"TeamColor",
	"Teams",
	&RBX::SpawnLocation::getTeamColor,
	&RBX::SpawnLocation::setTeamColor
);

Reflection::BoundProp<int, 1> SpawnLocation::prop_SpawnForcefieldDuration(
	"Duration",
	"Forcefield",
	&SpawnLocation::forcefieldDuration
);
Reflection::BoundProp<bool, 1> SpawnLocation::prop_Neutral("Neutral", "Teams", &SpawnLocation::neutral);
Reflection::BoundProp<bool, 1> SpawnLocation::prop_AllowTeamChangeOnTouch(
	"AllowTeamChangeOnTouch",
	"Teams",
	&SpawnLocation::allowTeamChangeOnTouch
);

// FUNCTION: WEBSERVICE 0x100fd500
BrickColor SpawnLocation::getTeamColor() const
{
	return teamColor;
}

// FUNCTION: WEBSERVICE 0x100fdc00
void SpawnLocation::onEvent_spawnerTouched(boost::shared_ptr<Instance> other)
{
	if (!allowTeamChangeOnTouch) {
		return;
	}

	Instance* character = other->getParent();
	if (character) {
		Humanoid* humanoid = character->findFirstChildOfType<Humanoid>();
		if (humanoid) {
			Network::Player* player = Network::Players::getPlayerFromCharacter(character);
			if (player) {
				player->setTeamColor(getTeamColor());
				player->setNeutral(neutral);
			}
		}
	}
}

// FUNCTION: WEBSERVICE 0x100fdd80
SpawnerService::~SpawnerService()
{
}

// FUNCTION: WEBSERVICE 0x100febd0
SpawnerService::SpawnerService()
{
	setName(sSpawnerService);
	propArchivable.setValue(this, false);
}

// FUNCTION: WEBSERVICE 0x100fef90
SpawnLocation::SpawnLocation() : neutral(true), allowTeamChangeOnTouch(false), forcefieldDuration(10)
{
	setName(sSpawnLocation);
}

// FUNCTION: WEBSERVICE 0x100ff2b0
void SpawnLocation::setTeamColor(BrickColor color)
{
	teamColor = color;
	raisePropertyChanged(prop_TeamColor);
}

// FUNCTION: WEBSERVICE 0x100ff2d0
void SpawnLocation::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	Instance::onServiceProvider(oldProvider, newProvider);

	if (oldProvider == NULL) {
		spawnerTouched = PartInstance::event_Touched
							 .connect(this, boost::bind(&SpawnLocation::onEvent_spawnerTouched, this, _1), 10);

		SpawnerService* spawnerService = newProvider ? newProvider->create<SpawnerService>() : NULL;
		spawnerService->RegisterSpawner(this);
	}

	if (newProvider == NULL) {
		spawnerTouched.disconnect();

		SpawnerService* spawnerService = oldProvider ? oldProvider->find<SpawnerService>() : NULL;
		spawnerService->UnregisterSpawner(this);
	}
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::SpawnerService, RBX::Instance, RBX::sSpawnerService>;
template class RBX::Reflection::Described<
	RBX::SpawnerService,
	RBX::sSpawnerService,
	RBX::FactoryProduct<RBX::SpawnerService, RBX::Instance, RBX::sSpawnerService> >;
template class RBX::DescribedCreatable<RBX::SpawnerService, RBX::Instance, RBX::sSpawnerService>;

template class RBX::FactoryProduct<RBX::SpawnLocation, RBX::PartInstance, RBX::sSpawnLocation>;
template class RBX::Reflection::Described<
	RBX::SpawnLocation,
	RBX::sSpawnLocation,
	RBX::FactoryProduct<RBX::SpawnLocation, RBX::PartInstance, RBX::sSpawnLocation> >;
template class RBX::DescribedCreatable<RBX::SpawnLocation, RBX::PartInstance, RBX::sSpawnLocation>;
