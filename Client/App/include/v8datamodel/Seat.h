#ifndef V8DATAMODEL_SEAT_H
#define V8DATAMODEL_SEAT_H

#include "decomp.h"
#include "v8datamodel/PartInstance.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>

namespace RBX {

class Humanoid;
class Weld;

extern char sSeat[];
// SIZE 0x2e4
class Seat : public DescribedCreatable<Seat, PartInstance, sSeat>
{
private:
	boost::signals::scoped_connection seatTouched;    // 0x290
	boost::signals::scoped_connection humanoidJumped; // 0x2a4

	void onEvent_seatTouched(boost::shared_ptr<Instance> other);
	void onEvent_humanoidJumped(bool active);
	void seatCharacter(Humanoid* h);
	void unseatCharacter();

	boost::shared_ptr<Weld> weld; // 0x2b8

	bool charSeated();

	double sleepTime; // 0x2c0

public:
	Seat();

	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

	// SYNTHETIC: WEBSERVICE 0x100fd430
	// RBX::Seat::~Seat
};

DECOMP_SIZE_ASSERT(Seat, 0x2e4)

} // namespace RBX

#endif // V8DATAMODEL_SEAT_H
