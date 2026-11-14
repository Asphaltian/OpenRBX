#ifndef UTIL_IRENDERABLE_H
#define UTIL_IRENDERABLE_H

#include "placeholder.h"
#include "util/IndexArray.h"

namespace RBX {

class Adorn;
class IRenderableBucket;

enum SelectState
{
	SELECT_NORMAL = 0,
	SELECT_LIMIT = 1
};

class __declspec(novtable) IRenderable
{
protected:
	virtual bool shouldRender2d() const { return false; }
	virtual bool shouldRender3dAdorn() const { return false; }

public:
	virtual void render2d(Adorn* adorn) {}
	virtual void render3dAdorn(Adorn* adorn) {}
	virtual void render3dSelect(Adorn* adorn, SelectState selectState) {}

	IRenderable() : index2d(-1), index3d(-1), bucket(NULL) {}

	~IRenderable();

	void shouldRenderSetDirty();

private:
	int index2d;
	int index3d;

	int& indexFunc2d() { return index2d; }
	int& indexFunc3d() { return index3d; }

	IRenderableBucket* bucket;

	friend class IRenderableBucket;
};

class IRenderableBucket
{
public:
	IRenderableBucket() {}

private:
	IndexArray<IRenderable, &IRenderable::indexFunc2d> renderable2ds;
	IndexArray<IRenderable, &IRenderable::indexFunc3d> renderable3ds;

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

} // namespace RBX

#endif // UTIL_IRENDERABLE_H
