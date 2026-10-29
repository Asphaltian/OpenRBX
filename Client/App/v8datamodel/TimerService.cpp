#include "v8datamodel/TimerService.h"

#include "util/StandardOut.h"

#include <G3D/System.h>

namespace RBX {

char sTimerService[] = "Timer";

// FUNCTION: WEBSERVICE 0x100fba10
// FUNCTION: ROBLOX 0x005e8670
void TimerService::delay(boost::function0<void> func, double seconds)
{
	Item item;
	item.time = G3D::System::getLocalTime() + seconds;
	item.func = func;

	bool inserted = false;
	for (std::list<Item>::iterator iter = items.begin(); iter != items.end(); ++iter) {
		if (iter->time > item.time) {
			items.insert(iter, item);
			inserted = true;
			break;
		}
	}

	if (!inserted) {
		items.push_back(item);
	}
}

// FUNCTION: WEBSERVICE 0x100fbb60
// FUNCTION: ROBLOX 0x005e87c0
void TimerService::onEvent(const RunService* source, Heartbeat event)
{
	double time = G3D::System::getLocalTime();

	while (items.size() > 0) {
		Item& item = items.front();

		if (time >= item.time) {
			try {
				item.func();
			}
			catch (std::exception& e) {
				StandardOut::singleton()->print(MESSAGE_WARNING, e);
			}

			items.pop_front();
		}
		else {
			break;
		}
	}
}

// FUNCTION: WEBSERVICE 0x100fbd30
// FUNCTION: ROBLOX 0x005e8980
TimerService::TimerService()
{
}

// FUNCTION: WEBSERVICE 0x100fbf40
// FUNCTION: ROBLOX 0x005e8b90
void TimerService::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	stopListening(runService, this);

	DescribedCreatable<TimerService, Instance, sTimerService>::onServiceProvider(oldProvider, newProvider);

	runService = shared_from(newProvider ? newProvider->create<RunService>() : NULL);

	startListening(runService, this);
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::TimerService, RBX::Instance, RBX::sTimerService>;
template class RBX::Reflection::Described<
	RBX::TimerService,
	RBX::sTimerService,
	RBX::FactoryProduct<RBX::TimerService, RBX::Instance, RBX::sTimerService> >;
template class RBX::DescribedCreatable<RBX::TimerService, RBX::Instance, RBX::sTimerService>;
