#include "v8datamodel/GeometryService.h"

namespace RBX {

char sGeometryService[] = "Geometry";

} // namespace RBX

template class RBX::FactoryProduct<RBX::GeometryService, RBX::Instance, RBX::sGeometryService>;
template class RBX::Reflection::Described<
	RBX::GeometryService,
	RBX::sGeometryService,
	RBX::FactoryProduct<RBX::GeometryService, RBX::Instance, RBX::sGeometryService> >;
template class RBX::DescribedCreatable<RBX::GeometryService, RBX::Instance, RBX::sGeometryService>;
