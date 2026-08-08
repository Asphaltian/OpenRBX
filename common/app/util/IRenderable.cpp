#include "util/IRenderable.h"

namespace RBX {

IRenderable::~IRenderable()
{
}

// FUNCTION: WEBSERVICE 0x100a66c0
IRenderableBucket::~IRenderableBucket()
{
}

// FUNCTION: WEBSERVICE 0x100a6800
void IRenderableBucket::recomputeShouldRender(IRenderable* renderable)
{
	if (renderable->shouldRender2d()) {
		if (renderable->index2d < 0) {
			renderable2ds.fastAppend(renderable);
		}
	}
	else if (renderable->index2d >= 0) {
		renderable2ds.fastRemove(renderable);
	}

	if (renderable->shouldRender3dAdorn()) {
		if (renderable->index3d < 0) {
			renderable3ds.fastAppend(renderable);
		}
	}
	else if (renderable->index3d >= 0) {
		renderable3ds.fastRemove(renderable);
	}
}

// FUNCTION: WEBSERVICE 0x100a6960
void IRenderable::shouldRenderSetDirty()
{
	if (bucket != NULL) {
		bucket->recomputeShouldRender(this);
	}
}

} // namespace RBX
