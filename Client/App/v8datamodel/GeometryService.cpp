#include "v8datamodel/GeometryService.h"

namespace RBX {

char sGeometryService[] = "Geometry";

// STUB: WEBSERVICE 0x100e9b90
G3D::Vector3 GeometryService::getHitLocationFilterDescendants(Instance* ancestor, G3D::Ray ray)
{
	STUB(0x100e9b90);
	return G3D::Vector3::zero();
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::GeometryService, RBX::Instance, RBX::sGeometryService>;
template class RBX::Reflection::Described<
	RBX::GeometryService,
	RBX::sGeometryService,
	RBX::FactoryProduct<RBX::GeometryService, RBX::Instance, RBX::sGeometryService> >;
template class RBX::DescribedCreatable<RBX::GeometryService, RBX::Instance, RBX::sGeometryService>;
