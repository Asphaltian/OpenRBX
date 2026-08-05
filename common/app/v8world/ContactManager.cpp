#include "decomp.h"
#include "v8world/Contact.h"
#include "v8world/SpatialHash.h"
#include "v8world/World.h"

#include <algorithm>
#include <set>

namespace RBX {

// FUNCTION: WEBSERVICE 0x101080d0
ContactManager::ContactManager(World* world) : spatialHash(new SpatialHash(world, this)), world(world)
{
}

// FUNCTION: WEBSERVICE 0x10108130
void ContactManager::getPrimitivesTouchingExtents(
	const Extents& extents,
	const Primitive* ignore,
	G3D::Array<Primitive*>& found
)
{
	spatialHash->getPrimitivesTouchingExtents(extents, ignore, found);
}

// FUNCTION: WEBSERVICE 0x10108140
void ContactManager::onReleasePair(Primitive* p0, Primitive* p1)
{
	Contact* contact = Primitive::getContact(p0, p1);

	world->destroyContact(contact);
}

// FUNCTION: WEBSERVICE 0x10108170
void ContactManager::onPrimitiveAdded(Primitive* primitive)
{
	spatialHash->onPrimitiveAdded(primitive);
}

// FUNCTION: WEBSERVICE 0x10108180
void ContactManager::onPrimitiveRemoved(Primitive* primitive)
{
	spatialHash->onPrimitiveRemoved(primitive);
}

// FUNCTION: WEBSERVICE 0x10108190
void ContactManager::onPrimitiveExtentsChanged(Primitive* primitive)
{
	spatialHash->onPrimitiveExtentsChanged(primitive);
}

// FUNCTION: WEBSERVICE 0x101081a0
void ContactManager::stepWorld()
{
	spatialHash->onAllPrimitivesMoved();
}

// FUNCTION: WEBSERVICE 0x101081b0
ContactManager::~ContactManager()
{
	delete spatialHash;
}

// FUNCTION: WEBSERVICE 0x10108620
bool ContactManager::intersectingOthers(Primitive* check, const std::set<Primitive*>& checkSet, float overlapIgnored)
{
	Contact* c = check->getFirstContact();

	while (c != NULL) {
		Primitive* other = c->otherPrimitive(check);

		if (checkSet.find(other) == checkSet.end() && c->computeIsColliding(overlapIgnored)) {
			return true;
		}

		c = check->getNextContact(c);
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x10108b30
Contact* ContactManager::createContact(Primitive* p0, Primitive* p1)
{
	if (p0->getPrimitiveType() > p1->getPrimitiveType()) {
		std::swap(p0, p1);
	}

	Geometry::GeometryType type0 = p0->getPrimitiveType();
	Geometry::GeometryType type1 = p1->getPrimitiveType();

	switch (type0) {

	case Geometry::GEOMETRY_BALL:

		switch (type1) {

		case Geometry::GEOMETRY_BALL:
			return new BallBallContact(p0, p1);

		case Geometry::GEOMETRY_BLOCK:
			return new BallBlockContact(p0, p1);
		}

	case Geometry::GEOMETRY_BLOCK:
		return new BlockBlockContact(p0, p1);
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x10108c60
void ContactManager::onNewPair(Primitive* p0, Primitive* p1)
{
	Contact* contact = createContact(p0, p1);

	world->insertContact(contact);
}

// FUNCTION: WEBSERVICE 0x10108c90
void ContactManager::onPrimitiveGeometryTypeChanged(Primitive* p)
{
	G3D::Array<Contact*> newContacts;

	Contact* c;

	while ((c = p->getFirstContact()) != NULL) {
		newContacts.append(createContact(c->getPrimitive(0), c->getPrimitive(1)));

		world->destroyContact(c);
	}

	for (int i = 0; i < newContacts.size(); i++) {
		world->insertContact(newContacts[i]);
	}
}

// FUNCTION: WEBSERVICE 0x10108ea0
bool ContactManager::intersectingOthers(const G3D::Array<Primitive*>& check, float overlapIgnored)
{
	std::set<Primitive*> checkSet(check.begin(), check.end());

	for (int i = 0; i < check.size(); i++) {
		if (intersectingOthers(check[i], checkSet, overlapIgnored)) {
			return true;
		}
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x10108fa0
bool ContactManager::intersectingOthers(Primitive* check, float overlapIgnored)
{
	std::set<Primitive*> checkSet;

	checkSet.insert(check);

	return intersectingOthers(check, checkSet, overlapIgnored);
}

} // namespace RBX
