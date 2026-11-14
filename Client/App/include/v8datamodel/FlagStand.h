#ifndef V8DATAMODEL_FLAGSTAND_H
#define V8DATAMODEL_FLAGSTAND_H

#include "placeholder.h"
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

class FlagStand : public DescribedCreatable<FlagStand, PartInstance, sFlagStand>
{
private:
	boost::signals::scoped_connection standTouched;

	void onEvent_standTouched(boost::shared_ptr<Instance> other);

public:
	FlagStand();

	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

	void AffixFlag(Flag* flag);
	bool HasFlag() { return GetFlag() != NULL; }
	Flag* GetFlag();

	BrickColor teamColor;

	BrickColor getTeamColor() const;
	void setTeamColor(BrickColor value);
};

class FlagStandService : public DescribedCreatable<FlagStandService, Instance, sFlagStandService>, public Service
{
private:
	std::list<FlagStand*> flagStands;

public:
	FlagStandService();
	virtual ~FlagStandService();

	FlagStand* FindClosestEmptyStandForFlag(Flag* flag);
	FlagStand* FindStandWithFlag(Flag* flag);
	void RegisterFlagStand(FlagStand* flagStand) { flagStands.push_back(flagStand); }
	void UnregisterFlagStand(FlagStand* flagStand) { flagStands.remove(flagStand); }
};

} // namespace RBX

#endif // V8DATAMODEL_FLAGSTAND_H
