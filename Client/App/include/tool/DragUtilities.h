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
class Instance;
class PartInstance;
class Primitive;
class PVInstance;
class World;

class DragUtilities
{
public:
	static void unJoinFromOutsiders(const std::vector<boost::weak_ptr<PartInstance> >& parts);
	static void joinToOutsiders(const std::vector<boost::weak_ptr<PartInstance> >& parts);
	static void join(const std::vector<boost::weak_ptr<PartInstance> >& parts);
	static void unJoinAndSetDragging(const std::vector<boost::weak_ptr<PartInstance> >& parts);
	static void joinAndStopDragging(const std::vector<boost::weak_ptr<PartInstance> >& parts);
	static void clean(PartInstance* part);
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
	static void instancesToParts(
		const std::vector<Instance*>& instances,
		std::vector<boost::weak_ptr<PartInstance> >& parts
	);
	static World* partsToPrimitives(
		const std::vector<boost::weak_ptr<PartInstance> >& parts,
		G3D::Array<Primitive*>& primitives
	);
	static Extents computeExtents(const std::vector<boost::weak_ptr<PartInstance> >& parts);
	static bool hitObjectOrPlane(
		const ContactManager& contactManager,
		const G3D::Ray& unitSearchRay,
		const G3D::Array<Primitive*>* ignorePrims,
		G3D::Vector3& hit
	);
	static bool anyPartAlive(const std::vector<boost::weak_ptr<PartInstance> >& parts);
	static void getPrimitivesConst(const Instance* instance, std::vector<const Primitive*>& primitives);
};

} // namespace RBX

#endif // TOOL_DRAGUTILITIES_H
