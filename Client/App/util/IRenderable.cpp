#include "util/IRenderable.h"

namespace RBX {

IRenderable::~IRenderable()
{
}

// FUNCTION: WEBSERVICE 0x100a66c0
IRenderableBucket::~IRenderableBucket()
{
}

// FUNCTION: WEBSERVICE 0x100a6730
void IRenderableBucket::render2dItems(Adorn* adorn)
{
	for (int i = 0; i < renderable2ds.size(); i++) {
		renderable2ds[i]->render2d(adorn);
	}
}

// FUNCTION: WEBSERVICE 0x100a6760
void IRenderableBucket::render3dAdornItems(Adorn* adorn)
{
	for (int i = 0; i < renderable3ds.size(); i++) {
		renderable3ds[i]->render3dAdorn(adorn);
	}
}

// FUNCTION: WEBSERVICE 0x100a6800
void IRenderableBucket::recomputeShouldRender(IRenderable* iR)
{
	if (iR->shouldRender2d()) {
		if (iR->index2d < 0) {
			renderable2ds.fastAppend(iR);
		}
	}
	else if (iR->index2d >= 0) {
		renderable2ds.fastRemove(iR);
	}

	if (iR->shouldRender3dAdorn()) {
		if (iR->index3d < 0) {
			renderable3ds.fastAppend(iR);
		}
	}
	else if (iR->index3d >= 0) {
		renderable3ds.fastRemove(iR);
	}
}

// FUNCTION: WEBSERVICE 0x100a68d0
void IRenderableBucket::onAdded(IRenderable* iR)
{
	iR->bucket = this;

	recomputeShouldRender(iR);
}

// FUNCTION: WEBSERVICE 0x100a68e0
void IRenderableBucket::onRemoving(IRenderable* iR)
{
	if (iR->index2d >= 0) {
		renderable2ds.fastRemove(iR);
	}

	if (iR->index3d >= 0) {
		renderable3ds.fastRemove(iR);
	}

	iR->bucket = NULL;
}

// FUNCTION: WEBSERVICE 0x100a6960
void IRenderable::shouldRenderSetDirty()
{
	if (bucket != NULL) {
		bucket->recomputeShouldRender(this);
	}
}

} // namespace RBX
