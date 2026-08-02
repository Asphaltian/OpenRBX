#include "v8world/IWorldStage.h"

#include "decomp.h"
#include "v8world/Primitive.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x10126da0
void IWorldStage::onEdgeAdded(Edge* edge)
{
	edge->putInPipeline(this);

	static_cast<IWorldStage*>(getDownstream())->onEdgeAdded(edge);
}

// FUNCTION: WEBSERVICE 0x10126dc0
void IWorldStage::onEdgeRemoving(Edge* edge)
{
	static_cast<IWorldStage*>(getDownstream())->onEdgeRemoving(edge);

	edge->removeFromStage(this);
}

} // namespace RBX
