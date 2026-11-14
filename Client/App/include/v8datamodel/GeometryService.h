#ifndef V8DATAMODEL_GEOMETRYSERVICE_H
#define V8DATAMODEL_GEOMETRYSERVICE_H

#include "placeholder.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <G3D/Ray.h>
#include <G3D/Vector3.h>

namespace RBX {

extern char sGeometryService[];

class Workspace;

class GeometryService : public DescribedCreatable<GeometryService, Instance, sGeometryService>, public Service
{
private:
	Workspace* workspace;

public:
	GeometryService();

	G3D::Vector3 getHitLocationFilterDescendants(Instance* ancestor, G3D::Ray ray);

protected:
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
};

} // namespace RBX

#endif // V8DATAMODEL_GEOMETRYSERVICE_H
