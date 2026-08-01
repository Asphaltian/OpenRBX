#include "v8world/Clump2.h"

#include "decomp.h"
#include "v8kernel/Body.h"
#include "v8world/Assembly2.h"
#include "v8world/IPipelined.h"
#include "v8world/Joint.h"
#include "v8world/MotorJoint.h"
#include "v8world/Primitive.h"
#include "v8world/RigidJoint.h"

#include <cstddef>

namespace RBX {

// FUNCTION: WEBSERVICE 0x10117da0
Edge* EdgeIterator::getNextExternalUtil(Primitive* primitive, Edge* edge)
{
	while (edge != NULL) {
		Primitive* other = edge->getPrimitive(0);

		if (primitive == other) {
			other = edge->getPrimitive(1);
		}

		if (other != NULL && primitive->getAssembly() != other->getAssembly()) {
			return edge;
		}

		edge = primitive->getNextEdge(edge);
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x10117df0
bool PrimIterator::isParent(Primitive* parent, Primitive* child, Joint* joint, SearchType searchType)
{
	if (parent == NULL || child == NULL) {
		return false;
	}

	bool jointed;

	if (searchType == IN_CLUMP) {
		jointed = RigidJoint::isRigidJoint(joint);
	}
	else {
		jointed = RigidJoint::isRigidJoint(joint) || MotorJoint::isMotorJoint(joint);
	}

	if (!jointed) {
		return false;
	}

	if (child->getBody()->getParent() != parent->getBody()) {
		return false;
	}

	bool sameClump = parent->getClump() == child->getClump();

	if (searchType == IN_CLUMP) {
		return sameClump;
	}

	return sameClump || parent->getAssembly() == child->getAssembly();
}

// FUNCTION: WEBSERVICE 0x10117ea0
Primitive* PrimIterator::findParent(Primitive* primitive, SearchType searchType)
{
	Joint* joint = primitive->getFirstJoint();

	while (joint != NULL) {
		Primitive* other = primitive == joint->getPrimitive(0) ? joint->getPrimitive(1) : joint->getPrimitive(0);

		if (isParent(other, primitive, joint, searchType)) {
			return other;
		}

		joint = primitive->getNextJoint(joint);
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x10117f00
Primitive* PrimIterator::findFirstChild(Primitive* primitive, SearchType searchType)
{
	Joint* joint = primitive->getFirstJoint();

	while (joint != NULL) {
		Primitive* other = primitive == joint->getPrimitive(0) ? joint->getPrimitive(1) : joint->getPrimitive(0);

		if (isParent(primitive, other, joint, searchType)) {
			return other;
		}

		joint = primitive->getNextJoint(joint);
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x10117f60
Primitive* PrimIterator::findNextSibling(Primitive* parent, Primitive* child, SearchType searchType)
{
	bool found = false;

	for (Joint* joint = parent->getFirstJoint(); joint != NULL; joint = parent->getNextJoint(joint)) {
		Primitive* other = parent == joint->getPrimitive(0) ? joint->getPrimitive(1) : joint->getPrimitive(0);

		if (isParent(parent, other, joint, searchType)) {
			if (found) {
				return other;
			}

			if (other == child) {
				found = true;
			}
		}
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x10117fc0
Primitive* PrimIterator::findNextRelative(Primitive* parent, Primitive* child, SearchType searchType)
{
	if (parent == NULL) {
		return NULL;
	}

	Primitive* sibling = findNextSibling(parent, child, searchType);

	if (sibling != NULL) {
		return sibling;
	}

	return findNextRelative(findParent(parent, searchType), parent, searchType);
}

// STUB: WEBSERVICE 0x10118000
PrimIterator& PrimIterator::operator++()
{
	Primitive* current = primitive;
	SearchType type = searchType;

	Primitive* next = findFirstChild(current, type);

	if (next == NULL) {
		next = findNextRelative(findParent(current, type), current, type);
	}

	primitive = next;

	return *this;
}

// STUB: WEBSERVICE 0x10118040
void EdgeIterator::findEdgeOnNextPrimitive()
{
	while (edge == NULL) {
		PrimIterator next(primitive, IN_ASSEMBLY);
		++next;

		primitive = *next;

		if (primitive == NULL) {
			return;
		}

		edge = getNextExternalUtil(primitive, primitive->getFirstEdge());
	}
}

// FUNCTION: WEBSERVICE 0x101180c0
EdgeIterator& EdgeIterator::operator++()
{
	Primitive* current = primitive;

	edge = getNextExternalUtil(current, current->getNextEdge(edge));

	if (edge == NULL) {
		findEdgeOnNextPrimitive();
	}

	return *this;
}

// FUNCTION: WEBSERVICE 0x101180f0
EdgeIterator EdgeIterator::begin(Primitive* primitive)
{
	EdgeIterator answer(primitive, getNextExternalUtil(primitive, primitive->getFirstEdge()));

	if (answer.edge == NULL) {
		answer.findEdgeOnNextPrimitive();
	}

	return answer;
}

// FUNCTION: WEBSERVICE 0x10118120
bool IPipelined::downstreamOfStage(IStage* stage)
{
	return currentStage->getStageType() > stage->getStageType();
}

} // namespace RBX
