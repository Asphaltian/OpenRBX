#include "v8world/Primitive.h"

#include "decomp.h"
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

// FUNCTION: WEBSERVICE 0x100a77d0
void Primitive::setSizeMultiplier(unsigned int multiplier)
{
	if (multiplier != sizeMultiplier && world == NULL) {
		sizeMultiplier = multiplier;
	}
}

// FUNCTION: WEBSERVICE 0x100a77f0
const Guid& Primitive::getGuid() const
{
	return guid;
}

// FUNCTION: WEBSERVICE 0x100a7800
void Primitive::setGuid(const Guid& value)
{
	guid.assign(value.data);
	guidSetExternally = true;
}

// FUNCTION: WEBSERVICE 0x100a7820
Primitive* Primitive::downstreamPrimitive(Joint* joint)
{
	Primitive* answer = joint->getPrimitive(0);
	Primitive* other = joint->getPrimitive(1);

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

// FUNCTION: WEBSERVICE 0x100a7840
void EdgeList::removeEdge(Primitive* primitive, Edge* edge, EdgeList& list)
{
	Edge* previous = list.first;

	if (previous == edge) {
		list.first = edge->getNext(primitive);
	}
	else {
		while (previous->getNext(primitive) != edge) {
			previous = previous->getNext(primitive);
		}

		previous->setNext(primitive, edge->getNext(primitive));
	}

	list.num--;
}

// STUB: WEBSERVICE 0x100a78c0
void Primitive::insertEdge(Edge* edge)
{
	Primitive* prim0 = edge->getPrimitive(0);
	Primitive* prim1 = edge->getPrimitive(1);

	if (edge->getEdgeType() == Edge::JOINT) {
		EdgeList::insertEdge(prim0, edge, edge->getPrimitive(0)->joints);

		if (prim1 != NULL) {
			EdgeList::insertEdge(prim1, edge, edge->getPrimitive(1)->joints);
		}
	}
	else {
		EdgeList::insertEdge(prim0, edge, edge->getPrimitive(0)->contacts);
		EdgeList::insertEdge(prim1, edge, edge->getPrimitive(1)->contacts);
	}
}

// FUNCTION: WEBSERVICE 0x100a7960
void Primitive::removeEdge(Edge* edge)
{
	Primitive* prim0 = edge->getPrimitive(0);
	Primitive* prim1 = edge->getPrimitive(1);

	if (edge->getEdgeType() == Edge::JOINT) {
		EdgeList::removeEdge(prim0, edge, edge->getPrimitive(0)->joints);

		if (prim1 != NULL) {
			EdgeList::removeEdge(prim1, edge, edge->getPrimitive(1)->joints);
		}
	}
	else {
		EdgeList::removeEdge(prim0, edge, edge->getPrimitive(0)->contacts);
		EdgeList::removeEdge(prim1, edge, edge->getPrimitive(1)->contacts);
	}
}

// FUNCTION: WEBSERVICE 0x100a79d0
Edge* Primitive::getFirstEdge() const
{
	Edge* edge = joints.first;

	if (edge == NULL) {
		edge = contacts.first;
	}

	return edge;
}

// FUNCTION: WEBSERVICE 0x100a79e0
Edge* Primitive::getNextEdge(Edge* edge) const
{
	Edge* next = edge->getNext(this);

	if (next != NULL) {
		return next;
	}

	if (edge->getEdgeType() == Edge::JOINT) {
		return contacts.first;
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x100a7a10
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

// FUNCTION: WEBSERVICE 0x100a7a90
void Primitive::setClump(Clump* value)
{
	if (value != clump) {
		clump = value;
	}
}

// FUNCTION: WEBSERVICE 0x100a7aa0
Assembly* Primitive::getAssembly() const
{
	if (clump != NULL) {
		return clump->getRootAssembly();
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x100a7ab0
float Primitive::computeJointK() const
{
	bool isBall = geometry->getGeometryType() == Geometry::GEOMETRY_BALL;

	return Constants::getJointK(geometry->getGridSize(), isBall);
}

// FUNCTION: WEBSERVICE 0x100a7ae0
void Primitive::setCanSleep(bool value)
{
	if (value != canSleep) {
		canSleep = value;

		if (world != NULL) {
			world->onPrimitiveCanSleepChanged(this);
		}
	}
}

// FUNCTION: WEBSERVICE 0x100a7b10
void Primitive::setCanCollide(bool value)
{
	bool wasCollideable = !dragging && canCollide;

	if (value != canCollide) {
		canCollide = value;

		if (world != NULL) {
			bool isCollideable = !dragging && canCollide;

			if (wasCollideable != isCollideable) {
				world->onPrimitiveCanCollideChanged(this);
			}
		}
	}
}

// FUNCTION: WEBSERVICE 0x100a7b70
void Primitive::setFriction(float value)
{
	if (value != friction) {
		friction = value;

		if (world != NULL) {
			world->onPrimitiveContactParametersChanged(this);
		}
	}
}

// FUNCTION: WEBSERVICE 0x100a7bb0
void Primitive::setElasticity(float value)
{
	if (value != elasticity) {
		elasticity = value;

		if (world != NULL) {
			world->onPrimitiveContactParametersChanged(this);
		}
	}
}

// FUNCTION: WEBSERVICE 0x100a7cd0
void Primitive::setVelocity(const Velocity& velocity)
{
	body->setVelocity(velocity);
}

// FUNCTION: WEBSERVICE 0x100a7bf0
CoordinateFrame Primitive::getFaceCoordInObject(NormalId normalId)
{
	return CoordinateFrame(normalIdToMatrix3(normalId), normalIdToVector3(normalId) * geometry->getGridSize());
}

// FUNCTION: WEBSERVICE 0x100a7c60
Face Primitive::getFaceInObject(NormalId normalId)
{
	Vector3 half = geometry->getGridSize() * 0.5f;

	return Face::fromExtentsSide(Extents(-half, half), normalId);
}

// FUNCTION: WEBSERVICE 0x100a7ce0
void Primitive::setSurfaceData(NormalId normalId, const SurfaceData& value)
{
	if (surfaceData[normalId] == NULL && value.isEmpty()) {
		return;
	}

	if (surfaceData[normalId] != NULL && *surfaceData[normalId] == value) {
		return;
	}

	if (value.isEmpty()) {
		delete surfaceData[normalId];
		surfaceData[normalId] = NULL;
	}
	else {
		if (surfaceData[normalId] == NULL) {
			surfaceData[normalId] = new SurfaceData();
		}

		*surfaceData[normalId] = value;
	}
}

// FUNCTION: WEBSERVICE 0x100a7dc0
void Primitive::setSurfaceType(NormalId normalId, SurfaceType surfaceType)
{
	if (this->surfaceType[normalId] != surfaceType) {
		this->surfaceType[normalId] = surfaceType;
	}
}

// FUNCTION: WEBSERVICE 0x100a7e80
Extents Primitive::computeFuzzyExtents() const
{
	const Matrix3& rotation = body->getCoordinateFrame().rotation;
	const Vector3& translation = body->getCoordinateFrame().translation;

	Extents extents = Extents::fromCenterCorner(translation, geometry->getCenterToCorner(rotation));
	extents.expand(0.01f);

	return extents;
}

// FUNCTION: WEBSERVICE 0x100a7f40
const Extents& Primitive::getFastFuzzyExtents() const
{
	if (fuzzyExtentsStateId != body->getStateIndex()) {
		fuzzyExtents = computeFuzzyExtents();
		fuzzyExtentsStateId = body->getStateIndex();
	}

	return fuzzyExtents;
}

// FUNCTION: WEBSERVICE 0x100a7fb0 FOLDED
Joint* Primitive::getFirstJoint() const
{
	return static_cast<Joint*>(joints.first);
}

// FUNCTION: WEBSERVICE 0x100a7fc0
Contact* Primitive::getFirstContact() const
{
	return static_cast<Contact*>(contacts.first);
}

// FUNCTION: WEBSERVICE 0x100a7fd0 FOLDED
Contact* Primitive::getNextContact(Contact* contact) const
{
	return static_cast<Contact*>(contact->getNext(this));
}

// FUNCTION: WEBSERVICE 0x100a7fd0 FOLDED
Joint* Primitive::getNextJoint(Joint* joint) const
{
	return static_cast<Joint*>(joint->getNext(this));
}

// FUNCTION: WEBSERVICE 0x100a7ff0
RigidJoint* Primitive::getFirstRigidAt(Edge* edge) const
{
	while (edge != NULL) {
		if (RigidJoint::isRigidJoint(edge)) {
			return static_cast<RigidJoint*>(edge);
		}

		edge = getNextEdge(edge);
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x100a8060
RigidJoint* Primitive::getFirstRigid() const
{
	return getFirstRigidAt(getFirstEdge());
}

// FUNCTION: WEBSERVICE 0x100a8080
RigidJoint* Primitive::getNextRigid(RigidJoint* joint) const
{
	return getFirstRigidAt(getNextEdge(joint));
}

// FUNCTION: WEBSERVICE 0x100a80c0
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

// FUNCTION: WEBSERVICE 0x100a8110
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

// FUNCTION: WEBSERVICE 0x100a8160
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

// FUNCTION: WEBSERVICE 0x100a81d0
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

// FUNCTION: WEBSERVICE 0x100a8250
bool Primitive::hitTest(const Ray& ray, Vector3& hitPoint, bool& inside)
{
	Ray objectRay = body->getCoordinateFrame().toObjectSpace(ray);
	Vector3 objectHitPoint;

	if (geometry->hitTest(objectRay, objectHitPoint, inside)) {
		hitPoint = body->getCoordinateFrame().pointToWorldSpace(objectHitPoint);
		return true;
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x100a8320
void Primitive::setAnchor(bool value)
{
	bool have = anchorObject != NULL;

	anchored = value;

	bool wanted = value || dragging;

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

// FUNCTION: WEBSERVICE 0x100a83b0
Face Primitive::getFaceInWorld(NormalId normalId)
{
	return getFaceInObject(normalId).toWorldSpace(body->getCoordinateFrame());
}

// FUNCTION: WEBSERVICE 0x100a83f0
void Primitive::setCoordinateFrame(const CoordinateFrame& value)
{
	if (value != body->getCoordinateFrame()) {
		Assembly* assembly = getAssembly();

		if (assembly == NULL) {
			body->setCoordinateFrame(value);
			myOwner->notifyMoved();

			if (world != NULL) {
				world->onPrimitiveExtentsChanged(this);
			}
		}
		else if (assembly->getAssemblyPrimitive() == this) {
			body->setCoordinateFrame(value);
			assembly->notifyMoved();
			world->onAssemblyExtentsChanged(assembly);

			if (!anchored) {
				world->ticklePrimitive(this, false);
			}
		}
	}
}

// FUNCTION: WEBSERVICE 0x100a8490
const CoordinateFrame& Primitive::getCoordinateFrame() const
{
	return body->getCoordinateFrame();
}

// FUNCTION: WEBSERVICE 0x100a84b0
CoordinateFrame Primitive::getGridCorner() const
{
	const CoordinateFrame& coord = body->getCoordinateFrame();

	return CoordinateFrame(coord.rotation, coord.pointToWorldSpace(-(geometry->getGridSize() * 0.5f)));
}

// STUB: WEBSERVICE 0x100a8760
Vector3 Primitive::clipToSafeSize(const Vector3& size)
{
	Vector3 answer(G3D::min(512.0f, size.x), G3D::min(512.0f, size.y), G3D::min(512.0f, size.z));

	if (answer.x * answer.y * answer.z > 1000000.0f) {
		answer.y = sqrt(1000000.0f / (answer.x * answer.z));
	}

	return answer;
}

// FUNCTION: WEBSERVICE 0x100a8810
void Primitive::setGridSize(const Vector3& size)
{
	Vector3 clipped = clipToSafeSize(size);

	if (clipped != geometry->getGridSize()) {
		fuzzyExtentsStateId = -2;
		geometry->setGridSize(clipped);

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

// FUNCTION: WEBSERVICE 0x100a88d0
void Primitive::setDragging(bool value)
{
	if (value != dragging) {
		bool wasCollideable = !dragging && canCollide;

		dragging = value;
		setAnchor(anchored);

		if (world != NULL) {
			bool isCollideable = !dragging && canCollide;

			if (wasCollideable != isCollideable) {
				world->onPrimitiveCanCollideChanged(this);
			}
		}
	}
}

// FUNCTION: WEBSERVICE 0x100a8940
void Primitive::setGridCorner(const CoordinateFrame& corner)
{
	Vector3 half = geometry->getGridSize() * 0.5f;

	setCoordinateFrame(CoordinateFrame(corner.rotation, corner.pointToWorldSpace(half)));
}

// FUNCTION: WEBSERVICE 0x100a89f0
void Primitive::setController(Controller* value)
{
	if (value == NULL) {
		value = NullController::getStaticNullController();
	}

	if (value != controller) {
		controller = value;
	}
}

// FUNCTION: WEBSERVICE 0x100a8a20
void Primitive::setPrimitiveType(Geometry::GeometryType geometryType)
{
	if (geometry->getGeometryType() != geometryType) {
		Vector3 size = geometry->getGridSize();

		delete geometry;
		geometry = newGeometry(geometryType);

		if (world != NULL) {
			world->onPrimitiveGeometryTypeChanged(this);
		}

		setGridSize(size);
		JointK.setDirty();
	}
}

} // namespace RBX
