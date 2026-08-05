#include "v8world/IWorldStage.h"

#include "decomp.h"
#include "v8world/Primitive.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x10126da0
void IWorldStage::onEdgeAdded(Edge* e)
{
	e->putInPipeline(this);

	static_cast<IWorldStage*>(getDownstream())->onEdgeAdded(e);
}

// FUNCTION: WEBSERVICE 0x10126dc0
void IWorldStage::onEdgeRemoving(Edge* e)
{
	static_cast<IWorldStage*>(getDownstream())->onEdgeRemoving(e);

	e->removeFromStage(this);
}

} // namespace RBX
