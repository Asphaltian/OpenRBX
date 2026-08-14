#ifndef UTIL_IRENDERABLE_H
#define UTIL_IRENDERABLE_H

#include "decomp.h"
#include "util/IndexArray.h"

namespace RBX {

class Adorn;
class IRenderableBucket;

enum SelectState
{
	SELECT_NORMAL = 0,
	SELECT_LIMIT = 1
};

// SIZE 0x10
class __declspec(novtable) IRenderable
{
protected:
	virtual bool shouldRender2d() const { return false; }      // vtable+0x00
	virtual bool shouldRender3dAdorn() const { return false; } // vtable+0x04

public:
	virtual void render2d(Adorn* adorn) {}                                // vtable+0x08
	virtual void render3dAdorn(Adorn* adorn) {}                           // vtable+0x0c
	virtual void render3dSelect(Adorn* adorn, SelectState selectState) {} // vtable+0x10

	IRenderable() : index2d(-1), index3d(-1), bucket(NULL) {}

	~IRenderable();

	void shouldRenderSetDirty();

private:
	int index2d; // 0x04
	int index3d; // 0x08

	int& indexFunc2d() { return index2d; }
	int& indexFunc3d() { return index3d; }

	IRenderableBucket* bucket; // 0x0c

	friend class IRenderableBucket;
};

DECOMP_SIZE_ASSERT(IRenderable, 0x10)

// SIZE 0x18
class IRenderableBucket
{
private:
	IndexArray<IRenderable, &IRenderable::indexFunc2d> renderable2ds; // 0x00
	IndexArray<IRenderable, &IRenderable::indexFunc3d> renderable3ds; // 0x0c

protected:
	void onAdded(IRenderable* renderable);
	void onRemoving(IRenderable* renderable);
	void recomputeShouldRender(IRenderable* renderable);

public:
	~IRenderableBucket();

	void render2dItems(Adorn* adorn);
	void render3dAdornItems(Adorn* adorn);

	friend class IRenderable;
};

DECOMP_SIZE_ASSERT(IRenderableBucket, 0x18)

} // namespace RBX

#endif // UTIL_IRENDERABLE_H
