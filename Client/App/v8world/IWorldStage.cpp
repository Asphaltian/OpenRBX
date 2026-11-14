#include "v8world/IWorldStage.h"

#include "placeholder.h"
#include "v8world/Primitive.h"

namespace RBX {

void IWorldStage::onEdgeAdded(Edge* e)
{
	e->putInPipeline(this);

	static_cast<IWorldStage*>(getDownstream())->onEdgeAdded(e);
}

void IWorldStage::onEdgeRemoving(Edge* e)
{
	static_cast<IWorldStage*>(getDownstream())->onEdgeRemoving(e);

	e->removeFromStage(this);
}

} // namespace RBX
