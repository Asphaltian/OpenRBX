#include "v8datamodel/Camera.h"

namespace RBX {

char sCamera[] = "Camera";

} // namespace RBX

template class RBX::FactoryProduct<RBX::Camera, RBX::Instance, RBX::sCamera>;
template class RBX::Reflection::
	Described<RBX::Camera, RBX::sCamera, RBX::FactoryProduct<RBX::Camera, RBX::Instance, RBX::sCamera> >;
template class RBX::DescribedCreatable<RBX::Camera, RBX::Instance, RBX::sCamera>;
