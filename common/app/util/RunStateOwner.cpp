#include "util/RunStateOwner.h"

namespace RBX {

const char sRunService[] = "RunService";

} // namespace RBX

template class RBX::FactoryProduct<RBX::RunService, RBX::Instance, RBX::sRunService>;
template class RBX::Reflection::Described<
	RBX::RunService,
	RBX::sRunService,
	RBX::FactoryProduct<RBX::RunService, RBX::Instance, RBX::sRunService> >;
template class RBX::DescribedCreatable<RBX::RunService, RBX::Instance, RBX::sRunService>;
