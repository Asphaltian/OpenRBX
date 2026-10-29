#include "v8datamodel/GeometryService.h"

#include "v8datamodel/Filters.h"
#include "v8datamodel/Workspace.h"
#include "v8world/SpatialHash.h"
#include "v8world/World.h"

namespace RBX {

char sGeometryService[] = "Geometry";

// FUNCTION: WEBSERVICE 0x100e9b90
G3D::Vector3 GeometryService::getHitLocationFilterDescendants(Instance* ancestor, G3D::Ray ray)
{
	if (workspace == NULL) {
		return G3D::Vector3::ZERO;
	}

	IgnoreDescendantsOf filter(ancestor);

	ContactManager* contactManager = &workspace->getWorld()->getContactManager();
	G3D::Vector3 hitPoint;
	std::vector<const Primitive*> dummy;

	Primitive* hit = contactManager->getHit(ray, &dummy, &filter, hitPoint);

	if (hit == NULL) {
		hitPoint = ray.unit().origin + ray.direction;
	}

	return hitPoint;
}

// FUNCTION: WEBSERVICE 0x100e9d00
void GeometryService::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	DescribedCreatable<GeometryService, Instance, sGeometryService>::onServiceProvider(oldProvider, newProvider);

	if (newProvider) {
		workspace = newProvider->find<Workspace>();
	}
}

// FUNCTION: WEBSERVICE 0x100e9e10
inline GeometryService::GeometryService()
	: DescribedCreatable<GeometryService, Instance, sGeometryService>(sGeometryService), workspace(NULL)
{
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::GeometryService, RBX::Instance, RBX::sGeometryService>;
template class RBX::Reflection::Described<
	RBX::GeometryService,
	RBX::sGeometryService,
	RBX::FactoryProduct<RBX::GeometryService, RBX::Instance, RBX::sGeometryService> >;
template class RBX::DescribedCreatable<RBX::GeometryService, RBX::Instance, RBX::sGeometryService>;
