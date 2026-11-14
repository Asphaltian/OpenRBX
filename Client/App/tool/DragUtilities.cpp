#include "tool/DragUtilities.h"

#include "tool/Dragger.h"
#include "placeholder.h"
#include "util/Math.h"
#include "v8datamodel/MouseCommand.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Workspace.h"
#include "v8world/SpatialHash.h"
#include "v8world/World.h"

namespace RBX {

void DragUtilities::moveAndClean(PartInstance* part, const G3D::Vector3& delta)
{
	G3D::CoordinateFrame coord = part->getCoordinateFrame();
	coord.translation += delta;

	if (part->aligned()) {
		coord = Math::snapToGrid(coord, 0.1f);
	}

	part->setCoordinateFrame(coord);
}

void DragUtilities::pvsToParts(
	const std::vector<PVInstance*>& pvInstances, std::vector<boost::weak_ptr<PartInstance> >& parts
)
{
	for (unsigned int i = 0; i < pvInstances.size(); i++) {
		PartInstance::findParts(pvInstances[i], parts);
	}
}

void DragUtilities::instancesToParts(
	const std::vector<Instance*>& instances, std::vector<boost::weak_ptr<PartInstance> >& parts
)
{
	for (unsigned int i = 0; i < instances.size(); i++) {
		PartInstance::findParts(instances[i], parts);
	}
}

bool DragUtilities::hitObjectOrPlane(
	const ContactManager& contactManager,
	const G3D::Ray& unitSearchRay,
	const G3D::Array<Primitive*>* ignorePrims,
	G3D::Vector3& hit
)
{
	G3D::Ray searchRay = MouseCommand::getSearchRay(unitSearchRay);

	if (!contactManager.getHit(searchRay, (const G3D::Array<const Primitive*>*) ignorePrims, NULL, hit)) {
		if (!Math::intersectRayPlane(searchRay, G3D::Plane(G3D::Vector3::unitY(), G3D::Vector3::zero()), hit)) {
			hit = G3D::Vector3::zero();
			return false;
		}
	}

	hit = Dragger::toGrid(hit);
	return true;
}

bool DragUtilities::anyPartAlive(const std::vector<boost::weak_ptr<PartInstance> >& parts)
{
	for (unsigned int i = 0; i < parts.size(); i++) {
		if (PartInstance::nonNullInWorkspace(parts[i].lock())) {
			return true;
		}
	}

	return false;
}

World* DragUtilities::partsToPrimitives(
	const std::vector<boost::weak_ptr<PartInstance> >& parts, G3D::Array<Primitive*>& primitives
)
{
	World* answer = NULL;

	for (unsigned int i = 0; i < parts.size(); i++) {
		boost::shared_ptr<PartInstance> part = parts[i].lock();

		if (PartInstance::nonNullInWorkspace(part)) {
			primitives.append(part->getPrimitive());

			if (answer == NULL) {
				answer = Workspace::getMyWorldFast(part.get());
			}
		}
	}

	return answer;
}

void DragUtilities::unJoinFromOutsiders(const std::vector<boost::weak_ptr<PartInstance> >& parts)
{
	G3D::Array<Primitive*> primitives;
	World* world = partsToPrimitives(parts, primitives);

	if (world) {
		world->destroyJointsToWorld(primitives);
	}
}

void DragUtilities::joinToOutsiders(const std::vector<boost::weak_ptr<PartInstance> >& parts)
{
	G3D::Array<Primitive*> primitives;
	World* world = partsToPrimitives(parts, primitives);

	if (world) {
		world->createJointsToWorld(primitives);
	}
}

void DragUtilities::join(const std::vector<boost::weak_ptr<PartInstance> >& parts)
{
	for (unsigned int i = 0; i < parts.size(); i++) {
		boost::shared_ptr<PartInstance> part = parts[i].lock();

		if (PartInstance::nonNullInWorkspace(part)) {
			part->join();
		}
	}
}

void DragUtilities::unJoinAndSetDragging(const std::vector<boost::weak_ptr<PartInstance> >& parts)
{
	unJoinFromOutsiders(parts);

	for (unsigned int i = 0; i < parts.size(); i++) {
		boost::shared_ptr<PartInstance> part = parts[i].lock();

		if (PartInstance::nonNullInWorkspace(part)) {
			part->setDragging(true);
		}
	}
}

void DragUtilities::joinAndStopDragging(const std::vector<boost::weak_ptr<PartInstance> >& parts)
{
	joinToOutsiders(parts);

	for (unsigned int i = 0; i < parts.size(); i++) {
		boost::shared_ptr<PartInstance> part = parts[i].lock();

		if (PartInstance::nonNullInWorkspace(part)) {
			part->setDragging(false);

			if (parts.size() == 1) {
				part->setLinearVelocity(G3D::Vector3::zero());
				part->setRotationalVelocity(G3D::Vector3::zero());
			}
		}
	}
}

void DragUtilities::clean(PartInstance* part)
{
	if (part->aligned()) {
		const G3D::CoordinateFrame& coord = part->getCoordinateFrame();
		G3D::CoordinateFrame snapped = Math::snapToGrid(coord, 0.1f);

		if (snapped != coord) {
			part->setCoordinateFrame(snapped);
		}
	}
}

void DragUtilities::clean(const std::vector<boost::weak_ptr<PartInstance> >& parts)
{
	for (unsigned int i = 0; i < parts.size(); i++) {
		boost::shared_ptr<PartInstance> part = parts[i].lock();

		if (part) {
			clean(part.get());
		}
	}
}

void DragUtilities::move(
	const std::vector<boost::weak_ptr<PartInstance> >& parts, G3D::CoordinateFrame from, G3D::CoordinateFrame to
)
{
	for (unsigned int i = 0; i < parts.size(); i++) {
		boost::shared_ptr<PartInstance> part = parts[i].lock();

		if (part) {
			G3D::CoordinateFrame meInFrom = from.toObjectSpace(part->getCoordinateFrame());
			Math::orthonormalizeIfNecessary(meInFrom.rotation);
			part->setCoordinateFrame(to * meInFrom);
		}
	}
}

Extents DragUtilities::computeExtents(const std::vector<boost::weak_ptr<PartInstance> >& parts)
{
	G3D::Array<Primitive*> primitives;
	partsToPrimitives(parts, primitives);

	return Dragger::computeExtents(primitives);
}

void DragUtilities::getPrimitivesConst(const Instance* instance, std::vector<const Primitive*>& primitives)
{
	if (const PartInstance* part = dynamic_cast<const PartInstance*>(instance)) {
		primitives.push_back(part->getPrimitive());
	}

	for (unsigned int i = 0; i < instance->numChildren(); i++) {
		getPrimitivesConst(instance->getChild(i), primitives);
	}
}

void DragUtilities::getPrimitives(const Instance* instance, std::vector<Primitive*>& primitives)
{
	getPrimitivesConst(instance, reinterpret_cast<std::vector<const Primitive*>&>(primitives));
}

} // namespace RBX
