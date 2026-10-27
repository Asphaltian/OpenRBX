#ifndef V8DATAMODEL_FLAG_H
#define V8DATAMODEL_FLAG_H

#include "decomp.h"
#include "v8datamodel/BrickColor.h"
#include "v8datamodel/Tool.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>

namespace RBX {

class TimerService;

extern const char sFlag[];

// SIZE 0x260
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
	boost::shared_ptr<Instance> evilClone;         // 0x230
	FlagState state;                               // 0x238
	boost::signals::scoped_connection flagTouched; // 0x23c

	void onEvent_flagTouched(boost::shared_ptr<Instance> other);

	TimerService* timer; // 0x250

	void doUglyPeriodicCloneHack();

protected:
	// FUNCTION: WEBSERVICE 0x100eb790 FOLDED
	virtual bool canUnequip() { return false; }

	virtual bool canBePickedUpByPlayer(Network::Player* p);

public:
	BrickColor teamColor; // 0x254

	BrickColor getTeamColor() const;

	void setTeamColor(BrickColor value);

	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

	Flag();
	virtual ~Flag();

	// SYNTHETIC: WEBSERVICE 0x100cd790
	// RBX::Flag::`scalar deleting destructor'

	virtual void onChildAdded(Instance* child);
};

DECOMP_SIZE_ASSERT(Flag, 0x260)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021c3e0
// RBX::`dynamic initializer for 'prop_Color''
// SYNTHETIC: WEBSERVICE 0x10223d60
// RBX::`dynamic atexit destructor for 'prop_Color''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_FLAG_H
