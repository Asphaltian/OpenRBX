#include "v8datamodel/TimerService.h"

namespace RBX {

char sTimerService[] = "Timer";

// STUB: WEBSERVICE 0x100fbb60
void TimerService::onEvent(const RunService* source, Heartbeat event)
{
	STUB(0x100fbb60);
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::TimerService, RBX::Instance, RBX::sTimerService>;
template class RBX::Reflection::Described<
	RBX::TimerService,
	RBX::sTimerService,
	RBX::FactoryProduct<RBX::TimerService, RBX::Instance, RBX::sTimerService> >;
template class RBX::DescribedCreatable<RBX::TimerService, RBX::Instance, RBX::sTimerService>;
