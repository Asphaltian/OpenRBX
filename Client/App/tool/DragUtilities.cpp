#include "tool/DragUtilities.h"

#include "decomp.h"

namespace RBX {

// STUB: WEBSERVICE 0x100a6970
void DragUtilities::moveAndClean(PartInstance* part, const G3D::Vector3& delta)
{
	STUB(0x100a6970);
}

// STUB: WEBSERVICE 0x100a6b10
void DragUtilities::pvsToParts(
	const std::vector<PVInstance*>& pvInstances,
	std::vector<boost::weak_ptr<PartInstance> >& parts
)
{
	STUB(0x100a6b10);
}

// STUB: WEBSERVICE 0x100a6b70
bool DragUtilities::hitObjectOrPlane(
	const ContactManager& contactManager,
	const G3D::Ray& unitMouseRay,
	const G3D::Array<Primitive*>* ignore,
	G3D::Vector3& hitPoint
)
{
	STUB(0x100a6b70);
	return false;
}

// STUB: WEBSERVICE 0x100a6cd0
bool DragUtilities::anyPartAlive(const std::vector<boost::weak_ptr<PartInstance> >& parts)
{
	STUB(0x100a6cd0);
	return false;
}

// STUB: WEBSERVICE 0x100a6d50
World* DragUtilities::partsToPrimitives(
	const std::vector<boost::weak_ptr<PartInstance> >& parts,
	G3D::Array<Primitive*>& primitives
)
{
	STUB(0x100a6d50);
	return NULL;
}

// STUB: WEBSERVICE 0x100a7070
void DragUtilities::unJoinAndSetDragging(const std::vector<boost::weak_ptr<PartInstance> >& parts)
{
	STUB(0x100a7070);
}

// STUB: WEBSERVICE 0x100a7180
void DragUtilities::joinAndStopDragging(const std::vector<boost::weak_ptr<PartInstance> >& parts)
{
	STUB(0x100a7180);
}

// STUB: WEBSERVICE 0x100a7300
void DragUtilities::clean(const std::vector<boost::weak_ptr<PartInstance> >& parts)
{
	STUB(0x100a7300);
}

// STUB: WEBSERVICE 0x100a7440
void DragUtilities::move(
	const std::vector<boost::weak_ptr<PartInstance> >& parts,
	G3D::CoordinateFrame start,
	G3D::CoordinateFrame end
)
{
	STUB(0x100a7440);
}

// STUB: WEBSERVICE 0x100a7550
Extents DragUtilities::computeExtents(const std::vector<boost::weak_ptr<PartInstance> >& parts)
{
	STUB(0x100a7550);
	return Extents();
}

} // namespace RBX
