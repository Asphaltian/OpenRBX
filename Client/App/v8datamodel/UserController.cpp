#include "v8datamodel/UserController.h"

namespace RBX {

char sControllerService[] = "ControllerService";

// STUB: WEBSERVICE 0x100c6f00
G3D::ReferenceCountedPointer<Controller> ControllerService::getController(
	Controller::ControllerType type,
	const PVInstance* pvInstance
)
{
	STUB(0x100c6f00);
	return G3D::ReferenceCountedPointer<Controller>();
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::ControllerService, RBX::Instance, RBX::sControllerService>;
template class RBX::Reflection::Described<
	RBX::ControllerService,
	RBX::sControllerService,
	RBX::FactoryProduct<RBX::ControllerService, RBX::Instance, RBX::sControllerService> >;
template class RBX::DescribedCreatable<RBX::ControllerService, RBX::Instance, RBX::sControllerService>;
