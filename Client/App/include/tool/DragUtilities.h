#ifndef TOOL_DRAGUTILITIES_H
#define TOOL_DRAGUTILITIES_H

#include "util/Extents.h"

#include <G3D/Array.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/Ray.h>
#include <G3D/Vector3.h>
#include <boost/weak_ptr.hpp>
#include <vector>

namespace RBX {

class ContactManager;
class PartInstance;
class Primitive;
class PVInstance;
class World;

class DragUtilities
{
public:
	static void unJoinAndSetDragging(const std::vector<boost::weak_ptr<PartInstance> >& parts);
	static void joinAndStopDragging(const std::vector<boost::weak_ptr<PartInstance> >& parts);
	static void clean(const std::vector<boost::weak_ptr<PartInstance> >& parts);
	static void move(
		const std::vector<boost::weak_ptr<PartInstance> >& parts,
		G3D::CoordinateFrame start,
		G3D::CoordinateFrame end
	);
	static void moveAndClean(PartInstance* part, const G3D::Vector3& delta);
	static void pvsToParts(
		const std::vector<PVInstance*>& pvInstances,
		std::vector<boost::weak_ptr<PartInstance> >& parts
	);
	static World* partsToPrimitives(
		const std::vector<boost::weak_ptr<PartInstance> >& parts,
		G3D::Array<Primitive*>& primitives
	);
	static Extents computeExtents(const std::vector<boost::weak_ptr<PartInstance> >& parts);
	static bool hitObjectOrPlane(
		const ContactManager& contactManager,
		const G3D::Ray& unitMouseRay,
		const G3D::Array<Primitive*>* ignore,
		G3D::Vector3& hitPoint
	);
	static bool anyPartAlive(const std::vector<boost::weak_ptr<PartInstance> >& parts);
};

} // namespace RBX

#endif // TOOL_DRAGUTILITIES_H
