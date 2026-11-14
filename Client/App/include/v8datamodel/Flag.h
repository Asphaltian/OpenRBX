#ifndef V8DATAMODEL_FLAG_H
#define V8DATAMODEL_FLAG_H

#include "placeholder.h"
#include "v8datamodel/BrickColor.h"
#include "v8datamodel/Tool.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>

namespace RBX {

class TimerService;

extern const char sFlag[];

class Flag : public DescribedCreatable<Flag, Tool, sFlag>
{
public:
	enum FlagState
	{
		NOTHING = 0,
		LOOSE = 1,
		IN_STAND = 2,
		EQUIPPED = 3
	};

private:
	boost::shared_ptr<Instance> evilClone;
	FlagState state;
	boost::signals::scoped_connection flagTouched;

	void onEvent_flagTouched(boost::shared_ptr<Instance> other);

	TimerService* timer;

	void doUglyPeriodicCloneHack();

protected:
	virtual bool canUnequip() { return false; }

	virtual bool canBePickedUpByPlayer(Network::Player* p);

public:
	BrickColor teamColor;

	BrickColor getTeamColor() const;

	void setTeamColor(BrickColor value);

	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

	Flag();
	virtual ~Flag();

	virtual void onChildAdded(Instance* child);
};

} // namespace RBX

#endif // V8DATAMODEL_FLAG_H
