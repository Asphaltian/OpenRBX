#ifndef TOOL_RUNDRAGGER_H
#define TOOL_RUNDRAGGER_H

#include "decomp.h"
#include "util/Face.h"
#include "util/NormalId.h"
#include "v8datamodel/Workspace.h"
#include "v8world/Primitive.h"
#include "v8world/SpatialHash.h"
#include "v8world/World.h"

#include <G3D/Array.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/Ray.h>
#include <G3D/Vector3.h>
#include <boost/weak_ptr.hpp>

namespace RBX {

class Contact;
class PartInstance;
class Primitive;

// SIZE 0x70
class RunDragger
{
private:
	// SIZE 0x20
	class SnapInfo
	{
	public:
		Primitive* snap;           // 0x00
		NormalId surface;          // 0x04
		G3D::Vector3 hitWorld;     // 0x08
		G3D::Vector3 lastDragSnap; // 0x14

		SnapInfo()
			: snap(NULL), surface(NORM_UNDEFINED), hitWorld(G3D::Vector3::inf()), lastDragSnap(G3D::Vector3::inf())
		{
		}

		void updateHitFromSurface(const G3D::Ray& mouseRay);
		void updateSurfaceFromHit();
		float hitOutsideExtents();
	};

	boost::weak_ptr<PartInstance> dragPart; // 0x00
	boost::weak_ptr<PartInstance> snapPart; // 0x08
	Workspace* workspace;                   // 0x10
	Primitive* drag;                        // 0x14
	G3D::Vector3 dragPointLocal;            // 0x18
	SnapInfo snapInfo;                      // 0x24
	G3D::Ray mouseRay;                      // 0x44
	NormalId dragSurface;                   // 0x60
	G3D::Vector3 dragHitLocal;              // 0x64

	SnapInfo createSnapSurface(Primitive* snap, G3D::Array<NormalId>* ignore);
	bool moveDragPart();
	bool snapDragPart();
	void findSafeY();
	bool notTried(Primitive* check, const G3D::Array<Primitive*>& tried);
	bool adjacent(Primitive* p0, Primitive* p1);
	SnapInfo rayHitsPart(const G3D::Array<Primitive*>& triedSnap, bool forceAdjacent);
	SnapInfo bestProximatePart(const G3D::Array<Primitive*>& triedSnap, bool (Contact::*proximityTest)(float));

	bool fallOffEdge()
	{
		Face snapFace = snapInfo.snap->getFaceInWorld(snapInfo.surface);
		Face dragFace = drag->getFaceInWorld(dragSurface);
		return !Face::hasOverlap(snapFace, dragFace, -0.01f);
	}

	bool fallOffPart(bool& snapped);

	bool colliding() { return workspace->getWorld()->getContactManager().intersectingOthers(drag, 0.01f); }

	bool rayHitsCloserPart();
	bool tooCloseToCamera();
	SnapInfo findSnap(const G3D::Array<Primitive*>& triedSnap);
	void findNoSnapPosition(const G3D::CoordinateFrame& original);
	void smartPtrHackPre();
	void smartPtrHackPost();

public:
	RunDragger();
	~RunDragger();

	void init(Workspace* _workspace, boost::weak_ptr<PartInstance> _dragPart, const G3D::Vector3& _dragPointWorld);
	bool snap(const G3D::Ray& _mouseRay);

	static void turnUpright(PartInstance* part);
	static void rotatePart(PartInstance* part);
	static void tiltPart(PartInstance* part, const G3D::CoordinateFrame& camera);
};

DECOMP_SIZE_ASSERT(RunDragger, 0x70)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1012a910
// RBX::RunDragger::SnapInfo::operator=
// FUNCTION: WEBSERVICE 0x1012aaa0
// RBX::RunDragger::SnapInfo::SnapInfo
// clang-format on

} // namespace RBX

#endif // TOOL_RUNDRAGGER_H
