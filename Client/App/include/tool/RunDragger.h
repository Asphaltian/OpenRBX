#ifndef TOOL_RUNDRAGGER_H
#define TOOL_RUNDRAGGER_H

#include "placeholder.h"
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

class RunDragger
{
private:
	class SnapInfo
	{
	public:
		Primitive* snap;
		NormalId surface;
		G3D::Vector3 hitWorld;
		G3D::Vector3 lastDragSnap;

		SnapInfo()
			: snap(NULL), surface(NORM_UNDEFINED), hitWorld(G3D::Vector3::inf()), lastDragSnap(G3D::Vector3::inf())
		{
		}

		void updateHitFromSurface(const G3D::Ray& mouseRay);
		void updateSurfaceFromHit();
		float hitOutsideExtents();
	};

	boost::weak_ptr<PartInstance> dragPart;
	boost::weak_ptr<PartInstance> snapPart;
	Workspace* workspace;
	Primitive* drag;
	G3D::Vector3 dragPointLocal;
	SnapInfo snapInfo;
	G3D::Ray mouseRay;
	NormalId dragSurface;
	G3D::Vector3 dragHitLocal;

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

} // namespace RBX

#endif // TOOL_RUNDRAGGER_H
