#ifndef V8DATAMODEL_SEAT_H
#define V8DATAMODEL_SEAT_H

#include "placeholder.h"
#include "v8datamodel/PartInstance.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>

namespace RBX {

class Humanoid;
class Weld;

extern char sSeat[];
class Seat : public DescribedCreatable<Seat, PartInstance, sSeat>
{
private:
	boost::signals::scoped_connection seatTouched;
	boost::signals::scoped_connection humanoidJumped;

	void onEvent_seatTouched(boost::shared_ptr<Instance> other);
	void onEvent_humanoidJumped(bool active);
	void seatCharacter(Humanoid* h);
	void unseatCharacter();

	boost::shared_ptr<Weld> weld;

	bool charSeated();

	double sleepTime;

public:
	Seat();

	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
};

} // namespace RBX

#endif // V8DATAMODEL_SEAT_H
