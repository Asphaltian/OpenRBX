#include "v8datamodel/Explosion.h"

#include "rbxgraphics/Adorn.h"
#include "reflection/signal.h"
#include "util/Extents.h"
#include "v8datamodel/ForceField.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Workspace.h"
#include "v8kernel/Body.h"
#include "v8kernel/Constants.h"
#include "v8world/Primitive.h"
#include "v8world/SpatialHash.h"
#include "v8world/World.h"

#include <G3D/Sphere.h>

namespace RBX {

char sExplosion[] = "Explosion";

static Reflection::PropDescriptor<Explosion, float> propBlastRadius(
	"BlastRadius", "Data", &Explosion::getBlastRadius, &Explosion::setBlastRadius
);

Reflection::BoundProp<float, 1> Explosion::propBlastPressure("BlastPressure", "Data", &Explosion::blastPressure);
Reflection::BoundProp<G3D::Vector3, 1> Explosion::propPosition("Position", "Data", &Explosion::position);
Reflection::SignalDesc<Explosion, void(boost::shared_ptr<Instance>, float)> signal_Hit("Hit", "part", "distance");

// FUNCTION: WEBSERVICE 0x100e1c60
void Explosion::doBlast(const G3D::Array<Primitive*>& primitives)
{
	if (blastPressure > 0.0f) {
		World* world = Workspace::getMyWorldFast(this);

		for (int i = 0; i < primitives.size(); i++) {
			Primitive* primitive = primitives[i];

			if (primitive->getRadius() < blastMaxObjectRadius()) {
				PartInstance* part = PartInstance::fromPrimitive(primitive);

				if (!ForceField::partInForceField(part)) {
					part->destroyJoints();
				}
			}
		}

		world->update();

		for (int i = 0; i < primitives.size(); i++) {
			Primitive* primitive = primitives[i];

			if (primitive->getRadius() < blastMaxObjectRadius()) {
				G3D::Vector3 delta = primitive->getCoordinateFrame().translation - position;

				G3D::Vector3 normal = delta == G3D::Vector3::zero() ? G3D::Vector3::unitY() : delta.direction();

				float radius = primitive->getRadius();
				float area = radius * radius;
				G3D::Vector3 impulse = normal * blastPressure * area * 0.00021929825f;
				G3D::Vector3 force = impulse * Constants::kernelStepsPerSec();
				world->ticklePrimitive(primitive, false);
				primitive->getBody()->accumulateForce(force, primitive->getCoordinateFrame().translation);
				primitive->getBody()->accumulateTorque(force * 0.5f * radius);
				world->addedBodyForce();
			}
		}
	}
}

// FUNCTION: WEBSERVICE 0x100e1f70
void Explosion::render3dAdorn(Adorn* adorn)
{
	adorn->setObjectToWorldMatrix(CoordinateFrame(position));
	G3D::Sphere sphere(G3D::Vector3::zero(), blastRadius);

	adorn->explosion(sphere, G3D::Color4(G3D::Color3::red()), G3D::Color4::clear());
}

// FUNCTION: WEBSERVICE 0x100e2ea0
Explosion::Explosion() : age(0.0f), blastRadius(4.0f), blastPressure(500000.0f)
{
	setName("Explosion");
}

// FUNCTION: WEBSERVICE 0x100e3120
void Explosion::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	stopListening(oldProvider ? oldProvider->find<RunService>() : NULL, this);

	DescribedCreatable<Explosion, Instance, sExplosion>::onServiceProvider(oldProvider, newProvider);

	startListening(newProvider ? newProvider->find<RunService>() : NULL, this);
}

// FUNCTION: WEBSERVICE 0x100e3700
void Explosion::setBlastRadius(float _blastRadius)
{
	_blastRadius = G3D::clamp(_blastRadius, 0.0f, 100.0f);

	if (blastRadius != _blastRadius) {
		blastRadius = _blastRadius;
		raisePropertyChanged(propBlastRadius);
	}
}

// FUNCTION: WEBSERVICE 0x100e3750
void Explosion::signalBlast(const G3D::Array<Primitive*>& primitives)
{
	if (!signal_Hit.empty(this)) {
		for (int i = 0; i < primitives.size(); i++) {
			Primitive* primitive = primitives[i];

			if (primitive->getRadius() < blastMaxObjectRadius()) {
				PartInstance* part = PartInstance::fromPrimitive(primitive);

				if (part) {
					G3D::Vector3 delta = primitive->getCoordinateFrame().translation - position;
					signal_Hit.fire(this, shared_from<Instance>(part), delta.magnitude());
				}
			}
		}
	}
}

// STUB: WEBSERVICE 0x100e3840
void Explosion::onEvent(const RunService* source, Stepped event)
{
	age += event.step;

	if (age > renderTime()) {
		if (blastRadius > 0.0f) {
			G3D::Array<Primitive*> primitives;
			World* world = Workspace::getMyWorldFast(this);

			float radius = blastRadius;
			Extents blastExtents = Extents::fromCenterCorner(position, G3D::Vector3(radius, radius, radius));

			world->getContactManager()->getPrimitivesTouchingExtents(blastExtents, NULL, primitives);

			signalBlast(primitives);
			doBlast(primitives);
		}

		setParent(NULL);
	}
}

// FUNCTION: WEBSERVICE 0x100e44b0 FOLDED
bool Explosion::askSetParent(const Instance* instance) const
{
	return true;
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::Explosion, RBX::Instance, RBX::sExplosion>;
template class RBX::Reflection::
	Described<RBX::Explosion, RBX::sExplosion, RBX::FactoryProduct<RBX::Explosion, RBX::Instance, RBX::sExplosion> >;
template class RBX::DescribedCreatable<RBX::Explosion, RBX::Instance, RBX::sExplosion>;
