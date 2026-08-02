#include "v8datamodel/TimerService.h"

namespace RBX {

char sTimerService[] = "Timer";

} // namespace RBX

template class RBX::FactoryProduct<RBX::TimerService, RBX::Instance, RBX::sTimerService>;
template class RBX::Reflection::Described<
	RBX::TimerService,
	RBX::sTimerService,
	RBX::FactoryProduct<RBX::TimerService, RBX::Instance, RBX::sTimerService> >;
template class RBX::DescribedCreatable<RBX::TimerService, RBX::Instance, RBX::sTimerService>;
