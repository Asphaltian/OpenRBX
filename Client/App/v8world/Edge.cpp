#include "v8world/Primitive.h"

#include <cstddef>

namespace RBX {

Edge::Edge(Primitive* prim0, Primitive* prim1)
	: prim0(prim0), prim1(prim1), edgeState(Sim::UNDEFINED), next0(NULL), next1(NULL), inEdgeList(false)
{
}

void Edge::setPrimitive(int i, Primitive* p)
{
	if (i == 0) {
		prim0 = p;
	}
	else {
		prim1 = p;
	}
}

} // namespace RBX
