#ifndef V8DATAMODEL_TIMERSERVICE_H
#define V8DATAMODEL_TIMERSERVICE_H

#include "decomp.h"
#include "util/Handle.h"
#include "util/RunStateOwner.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <list>

namespace RBX {

extern char sTimerService[];
// SIZE 0x110
class TimerService : public DescribedCreatable<TimerService, Instance, sTimerService>,
					 public Service,
					 public Listener<RunService, Heartbeat>
{
private:
	boost::shared_ptr<RunService> runService; // 0xfc

	// SIZE 0x28
	class Item
	{
	public:
		double time;                 // 0x00
		boost::function0<void> func; // 0x08
	};

	std::list<Item> items; // 0x104

public:
	TimerService();

	void delay(boost::function0<void> func, double seconds);

protected:
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
	virtual void onEvent(const RunService* source, Heartbeat event);

public:
	// SYNTHETIC: WEBSERVICE 0x100fbe80
	// RBX::TimerService::~TimerService

	// SYNTHETIC: WEBSERVICE 0x100fbf20
	// RBX::TimerService::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(TimerService, 0x110)

} // namespace RBX

#endif // V8DATAMODEL_TIMERSERVICE_H
