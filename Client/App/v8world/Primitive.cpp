#include "v8world/Primitive.h"

#include "placeholder.h"
#include "util/Face.h"
#include "util/Velocity.h"
#include "v8kernel/Body.h"
#include "v8kernel/Constants.h"
#include "v8world/Anchor.h"
#include "v8world/Assembly2.h"
#include "v8world/Ball.h"
#include "v8world/Block.h"
#include "v8world/Contact.h"
#include "v8world/Controller.h"
#include "v8world/Geometry.h"
#include "v8world/IMoving.h"
#include "v8world/Joint.h"
#include "v8world/RigidJoint.h"
#include "v8world/World.h"

#include <cmath>
#include <cstddef>

namespace RBX {

bool Primitive::disableSleep = false;

void Primitive::setSizeMultiplier(unsigned int value)
{
	if (value != sizeMultiplier && world == NULL) {
		sizeMultiplier = value;
	}
}

const Guid& Primitive::getGuid() const
{
	return guid;
}

void Primitive::setGuid(const Guid& value)
{
	guid.copyDataFrom(value);
	guidSetExternally = true;
}

Primitive* Primitive::downstreamPrimitive(Joint* j)
{
	Primitive* answer = j->getPrimitive(0);
	Primitive* other = j->getPrimitive(1);

	if (other != NULL && answer->clumpDepth <= other->clumpDepth) {
		answer = other;
	}

	return answer;
}

void EdgeList::insertEdge(Primitive* primitive, Edge* edge, EdgeList& list)
{
	Edge* first = list.first;

	edge->setNext(primitive, first);
	list.first = edge;
	list.num++;
}

void EdgeList::removeEdge(Primitive* p, Edge* e, EdgeList& list)
{
	Edge* previous = list.first;

	if (previous == e) {
		list.first = e->getNext(p);
	}
	else {
		while (previous->getNext(p) != e) {
			previous = previous->getNext(p);
		}

		previous->setNext(p, e->getNext(p));
	}

	list.num--;
}

void Primitive::insertEdge(Edge* e)
{
	Primitive* prim0 = e->getPrimitive(0);
	Primitive* prim1 = e->getPrimitive(1);

	if (e->getEdgeType() == Edge::JOINT) {
		EdgeList::insertEdge(prim0, e, e->getPrimitive(0)->joints);

		if (prim1 != NULL) {
			EdgeList::insertEdge(prim1, e, e->getPrimitive(1)->joints);
		}
	}
	else {
		EdgeList::insertEdge(prim0, e, e->getPrimitive(0)->contacts);
		EdgeList::insertEdge(prim1, e, e->getPrimitive(1)->contacts);
	}
}

void Primitive::removeEdge(Edge* e)
{
	Primitive* prim0 = e->getPrimitive(0);
	Primitive* prim1 = e->getPrimitive(1);

	if (e->getEdgeType() == Edge::JOINT) {
		EdgeList::removeEdge(prim0, e, e->getPrimitive(0)->joints);

		if (prim1 != NULL) {
			EdgeList::removeEdge(prim1, e, e->getPrimitive(1)->joints);
		}
	}
	else {
		EdgeList::removeEdge(prim0, e, e->getPrimitive(0)->contacts);
		EdgeList::removeEdge(prim1, e, e->getPrimitive(1)->contacts);
	}
}

Edge* Primitive::getFirstEdge() const
{
	Edge* edge = joints.first;

	if (edge == NULL) {
		edge = contacts.first;
	}

	return edge;
}

Edge* Primitive::getNextEdge(Edge* e) const
{
	Edge* next = e->getNext(this);

	if (next != NULL) {
		return next;
	}

	if (e->getEdgeType() == Edge::JOINT) {
		return contacts.first;
	}

	return NULL;
}

void Primitive::onNewTouch(Primitive* p0, Primitive* p1)
{
	Clump* clump0 = p0->clump;
	Clump* clump1 = p1->clump;

	if (p0->myOwner->reportTouches()) {
		if (clump0->getSleepStatus() == Sim::AWAKE || clump1->getSleepStatus() == Sim::AWAKE) {
			p0->world->onPrimitiveTouched(p0, p1);
		}
	}

	clump0 = p1->clump;
	clump1 = p0->clump;

	if (p1->myOwner->reportTouches()) {
		if (clump0->getSleepStatus() == Sim::AWAKE || clump1->getSleepStatus() == Sim::AWAKE) {
			p1->world->onPrimitiveTouched(p1, p0);
		}
	}
}

void Primitive::setClump(Clump* set)
{
	if (set != clump) {
		clump = set;
	}
}

Assembly* Primitive::getAssembly() const
{
	if (clump != NULL) {
		return clump->getRootAssembly();
	}

	return NULL;
}

float Primitive::computeJointK() const
{
	return Constants::getJointK(getGridSize(), getPrimitiveType() == Geometry::GEOMETRY_BALL);
}

void Primitive::setCanSleep(bool _canSleep)
{
	if (_canSleep != canSleep) {
		canSleep = _canSleep;

		if (world != NULL) {
			world->onPrimitiveCanSleepChanged(this);
		}
	}
}

void Primitive::setCanCollide(bool _canCollide)
{
	bool wasCollideable = !dragging && canCollide;

	if (canCollide != _canCollide) {
		canCollide = _canCollide;

		if (world != NULL) {
			bool isCollideable = !dragging && canCollide;

			if (wasCollideable != isCollideable) {
				world->onPrimitiveCanCollideChanged(this);
			}
		}
	}
}

void Primitive::setFriction(float _friction)
{
	if (_friction != friction) {
		friction = _friction;

		if (world != NULL) {
			world->onPrimitiveContactParametersChanged(this);
		}
	}
}

void Primitive::setElasticity(float _elasticity)
{
	if (_elasticity != elasticity) {
		elasticity = _elasticity;

		if (world != NULL) {
			world->onPrimitiveContactParametersChanged(this);
		}
	}
}

CoordinateFrame Primitive::getFaceCoordInObject(NormalId objectFace)
{
	return CoordinateFrame(normalIdToMatrix3(objectFace), normalIdToVector3(objectFace) * geometry->getGridSize());
}

Face Primitive::getFaceInObject(NormalId objectFace)
{
	Vector3 half = geometry->getGridSize() * 0.5f;

	return Face::fromExtentsSide(Extents(-half, half), objectFace);
}

void Primitive::setVelocity(const Velocity& vel)
{
	body->setVelocity(vel);
}

void Primitive::setSurfaceData(NormalId id, const SurfaceData& newSurfaceData)
{
	if (surfaceData[id] == NULL && newSurfaceData.isEmpty()) {
		return;
	}

	if (surfaceData[id] != NULL && *surfaceData[id] == newSurfaceData) {
		return;
	}

	if (newSurfaceData.isEmpty()) {
		delete surfaceData[id];
		surfaceData[id] = NULL;
	}
	else {
		if (surfaceData[id] == NULL) {
			surfaceData[id] = new SurfaceData();
		}

		*surfaceData[id] = newSurfaceData;
	}
}

void Primitive::setSurfaceType(NormalId id, SurfaceType newSurfaceType)
{
	if (this->surfaceType[id] != newSurfaceType) {
		this->surfaceType[id] = newSurfaceType;
	}
}

Extents Primitive::computeFuzzyExtents() const
{
	const Matrix3& rotation = body->getCoordinateFrame().rotation;
	const Vector3& translation = body->getCoordinateFrame().translation;

	Extents extents = Extents::fromCenterCorner(translation, geometry->getCenterToCorner(rotation));
	extents.expand(0.01f);

	return extents;
}

const Extents& Primitive::getFastFuzzyExtents() const
{
	if (fuzzyExtentsStateId != body->getStateIndex()) {
		fuzzyExtents = computeFuzzyExtents();
		fuzzyExtentsStateId = body->getStateIndex();
	}

	return fuzzyExtents;
}

Joint* Primitive::getFirstJoint() const
{
	return static_cast<Joint*>(joints.first);
}

Contact* Primitive::getFirstContact()
{
	return static_cast<Contact*>(contacts.first);
}

Contact* Primitive::getNextContact(Contact* prev)
{
	return static_cast<Contact*>(prev->getNext(this));
}

Joint* Primitive::getNextJoint(Joint* prev) const
{
	return static_cast<Joint*>(prev->getNext(this));
}

RigidJoint* Primitive::getFirstRigidAt(Edge* start)
{
	while (start != NULL) {
		if (RigidJoint::isRigidJoint(start)) {
			return static_cast<RigidJoint*>(start);
		}

		start = getNextEdge(start);
	}

	return NULL;
}

RigidJoint* Primitive::getFirstRigid()
{
	return getFirstRigidAt(getFirstEdge());
}

RigidJoint* Primitive::getNextRigid(RigidJoint* prev)
{
	return getFirstRigidAt(getNextEdge(prev));
}

Joint* Primitive::getJoint(Primitive* p0, Primitive* p1)
{
	Primitive* shortest = p0->joints.num < p1->joints.num ? p0 : p1;

	for (Joint* joint = shortest->getFirstJoint(); joint != NULL; joint = shortest->getNextJoint(joint)) {
		if ((p0 == joint->getPrimitive(0) && p1 == joint->getPrimitive(1)) ||
			(p0 == joint->getPrimitive(1) && p1 == joint->getPrimitive(0))) {
			return joint;
		}
	}

	return NULL;
}

Contact* Primitive::getContact(Primitive* p0, Primitive* p1)
{
	Primitive* shortest = p0->contacts.num < p1->contacts.num ? p0 : p1;

	for (Contact* contact = shortest->getFirstContact(); contact != NULL; contact = shortest->getNextContact(contact)) {
		if ((p0 == contact->getPrimitive(0) && p1 == contact->getPrimitive(1)) ||
			(p0 == contact->getPrimitive(1) && p1 == contact->getPrimitive(0))) {
			return contact;
		}
	}

	return NULL;
}

Geometry* Primitive::newGeometry(Geometry::GeometryType geometryType)
{
	switch (geometryType) {
	case Geometry::GEOMETRY_BALL:
		return new Ball();
	case Geometry::GEOMETRY_BLOCK:
		return new Block();
	default:
		return Geometry::nullGeometry();
	}
}

Primitive::~Primitive()
{
	if (geometry->getGeometryType() != Geometry::GEOMETRY_NONE) {
		delete geometry;
		delete body;
	}

	for (int i = 0; i < 6; i++) {
		delete surfaceData[i];
	}
}

bool Primitive::hitTest(const Ray& worldRay, Vector3& worldHitPoint, bool& inside)
{
	Ray localRay = body->getCoordinateFrame().toObjectSpace(worldRay);
	Vector3 localHitPoint;

	if (geometry->hitTest(localRay, localHitPoint, inside)) {
		worldHitPoint = body->getCoordinateFrame().pointToWorldSpace(localHitPoint);
		return true;
	}

	return false;
}

void Primitive::setAnchor(bool _anchored)
{
	bool have = anchorObject != NULL;

	anchored = _anchored;

	bool wanted = _anchored || dragging;

	if (wanted != have) {
		if (wanted) {
			anchorObject = new Anchor(this);

			if (world != NULL) {
				world->onPrimitiveAddedAnchor(this);
			}
		}
		else {
			delete anchorObject;
			anchorObject = NULL;

			if (world != NULL) {
				world->onPrimitiveRemovedAnchor(this);
			}
		}
	}
}

Face Primitive::getFaceInWorld(NormalId objectFace)
{
	return getFaceInObject(objectFace).toWorldSpace(body->getCoordinateFrame());
}

void Primitive::setCoordinateFrame(const CoordinateFrame& cFrame)
{
	if (cFrame != body->getCoordinateFrame()) {
		Assembly* assembly = getAssembly();

		if (assembly == NULL) {
			body->setCoordinateFrame(cFrame);
			myOwner->notifyMoved();

			if (world != NULL) {
				world->onPrimitiveExtentsChanged(this);
			}
		}
		else if (assembly->getAssemblyPrimitive() == this) {
			body->setCoordinateFrame(cFrame);
			assembly->notifyMoved();
			world->onAssemblyExtentsChanged(assembly);

			if (!anchored) {
				world->ticklePrimitive(this, false);
			}
		}
	}
}

const CoordinateFrame& Primitive::getCoordinateFrame() const
{
	if (FrameInterpolator::isRendering()) {
		return renderInterpolator.sample(body->getCoordinateFrame());
	}

	return body->getCoordinateFrame();
}

CoordinateFrame Primitive::getGridCorner() const
{
	const CoordinateFrame& gridCorner = body->getCoordinateFrame();

	return CoordinateFrame(gridCorner.rotation, gridCorner.pointToWorldSpace(-(geometry->getGridSize() * 0.5f)));
}

Primitive::Primitive(Geometry::GeometryType geometryType)
	: guidSetExternally(false), sizeMultiplier(1), worldIndex(-1), world(NULL), clump(NULL), clumpDepth(-1),
	  traverseId(-1), spatialNodes(NULL), oldSpatialMin(), oldSpatialMax(), fuzzyExtents(), fuzzyExtentsStateId(-2),
	  geometry(newGeometry(geometryType)), body(new Body()), myOwner(NULL), anchorObject(NULL), dragging(false),
	  anchored(false), canCollide(true), canSleep(true), friction(0), elasticity(0.75f),
	  controller(NullController::getStaticNullController()), JointK(this, &Primitive::computeJointK)
{
	for (int i = 0; i < 6; i++) {
		surfaceType[i] = NO_SURFACE;
		surfaceData[i] = NULL;
	}
}

Vector3 Primitive::clipToSafeSize(const Vector3& newSize)
{
	Vector3 answer = newSize.min(Vector3(512.0f, 512.0f, 512.0f));

	if (answer.x * answer.y * answer.z > 1000000.0f) {
		answer.y = floor(1000000.0f / (answer.x * answer.z));
	}

	return answer;
}

void Primitive::setGridSize(const Vector3& gridSize)
{
	Vector3 protectedSize = clipToSafeSize(gridSize);

	if (protectedSize != geometry->getGridSize()) {
		fuzzyExtentsStateId = -2;
		geometry->setGridSize(protectedSize);

		float volume = geometry->getGridVolume();
		body->setMass(volume);
		body->setMoment(geometry->getMoment(volume));

		JointK.setDirty();

		if (world != NULL) {
			world->onPrimitiveExtentsChanged(this);
		}

		JointK.setDirty();
	}
}

void Primitive::setDragging(bool _dragging)
{
	if (dragging != _dragging) {
		bool wasCollideable = !dragging && canCollide;

		dragging = _dragging;
		setAnchor(anchored);

		if (world != NULL) {
			bool isCollideable = !dragging && canCollide;

			if (isCollideable != wasCollideable) {
				world->onPrimitiveCanCollideChanged(this);
			}
		}
	}
}

void Primitive::setGridCorner(const CoordinateFrame& gridCorner)
{
	Vector3 gridCenter = geometry->getGridSize() * 0.5f;

	setCoordinateFrame(CoordinateFrame(gridCorner.rotation, gridCorner.pointToWorldSpace(gridCenter)));
}

void Primitive::setPV(const PV& pv)
{
	body->setVelocity(pv.velocity);
	setCoordinateFrame(pv.position);
}

void Primitive::setController(Controller* _controller)
{
	if (_controller == NULL) {
		_controller = NullController::getStaticNullController();
	}

	if (controller != _controller) {
		controller = _controller;
	}
}

void Primitive::setPrimitiveType(Geometry::GeometryType geometryType)
{
	if (geometry->getGeometryType() != geometryType) {
		Vector3 oldSize = geometry->getGridSize();

		delete geometry;
		geometry = newGeometry(geometryType);

		if (world != NULL) {
			world->onPrimitiveGeometryTypeChanged(this);
		}

		setGridSize(oldSize);
		JointK.setDirty();
	}
}

} // namespace RBX
