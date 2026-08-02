#include "v8datamodel/UserController.h"

namespace RBX {

char sControllerService[] = "ControllerService";

} // namespace RBX

template class RBX::FactoryProduct<RBX::ControllerService, RBX::Instance, RBX::sControllerService>;
template class RBX::Reflection::Described<
	RBX::ControllerService,
	RBX::sControllerService,
	RBX::FactoryProduct<RBX::ControllerService, RBX::Instance, RBX::sControllerService> >;
template class RBX::DescribedCreatable<RBX::ControllerService, RBX::Instance, RBX::sControllerService>;
