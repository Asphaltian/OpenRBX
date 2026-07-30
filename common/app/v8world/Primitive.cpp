#include "v8world/Primitive.h"

#include "v8world/Geometry.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x100a77d0
void Primitive::setSizeMultiplier(unsigned int multiplier)
{
	if (multiplier != sizeMultiplier && world == 0) {
		sizeMultiplier = multiplier;
	}
}

// FUNCTION: WEBSERVICE 0x100a77f0
const Guid& Primitive::getGuid() const
{
	return guid;
}

// FUNCTION: WEBSERVICE 0x100a7820
Primitive* Primitive::downstreamPrimitive(Edge* edge)
{
	Primitive* answer = edge->getPrimitive(0);
	Primitive* other = edge->getPrimitive(1);

	if (other != 0 && answer->clumpDepth <= other->clumpDepth) {
		answer = other;
	}

	return answer;
}

// FUNCTION: WEBSERVICE 0x100a79d0
Edge* Primitive::getFirstEdge() const
{
	Edge* edge = joints.first;

	if (edge == 0) {
		edge = contacts.first;
	}

	return edge;
}

// FUNCTION: WEBSERVICE 0x100a79e0
Edge* Primitive::getNextEdge(Edge* edge) const
{
	Edge* next = edge->getNext(this);

	if (next != 0) {
		return next;
	}

	if (edge->getEdgeType() == Edge::JOINT) {
		return contacts.first;
	}

	return 0;
}

// FUNCTION: WEBSERVICE 0x100a7a90
void Primitive::setClump(Clump* value)
{
	if (value != clump) {
		clump = value;
	}
}

// FUNCTION: WEBSERVICE 0x100a7fc0
Edge* Primitive::getFirstContact() const
{
	return contacts.first;
}

} // namespace RBX
