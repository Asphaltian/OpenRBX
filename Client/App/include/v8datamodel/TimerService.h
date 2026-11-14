#ifndef V8DATAMODEL_TIMERSERVICE_H
#define V8DATAMODEL_TIMERSERVICE_H

#include "placeholder.h"
#include "util/Handle.h"
#include "util/RunStateOwner.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <list>

namespace RBX {

extern char sTimerService[];
class TimerService : public DescribedCreatable<TimerService, Instance, sTimerService>,
					 public Service,
					 public Listener<RunService, Heartbeat>
{
private:
	boost::shared_ptr<RunService> runService;

	class Item
	{
	public:
		double time;
		boost::function0<void> func;
	};

	std::list<Item> items;

public:
	TimerService();

	void delay(boost::function0<void> func, double seconds);

protected:
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
	virtual void onEvent(const RunService* source, Heartbeat event);

public:
};

} // namespace RBX

#endif // V8DATAMODEL_TIMERSERVICE_H
