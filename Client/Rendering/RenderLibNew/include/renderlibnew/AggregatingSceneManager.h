#ifndef RENDERLIBNEW_AGGREGATINGSCENEMANAGER_H
#define RENDERLIBNEW_AGGREGATINGSCENEMANAGER_H

#include "decomp.h"
#include "renderlibnew/RenderScene.h"

namespace RBX {
namespace RenderNew {

// SIZE 0x30
class AggregatingSceneManager : public SceneManager
{
private:
	undefined m_unk0x08[0x30 - 0x08]; // 0x08

public:
	AggregatingSceneManager(RenderScene* renderScene);
	virtual ~AggregatingSceneManager();

	virtual void invalidateModel(const G3D::ReferenceCountedPointer<Chunk>& chunk, bool isSleeping);
	virtual void addModel(const G3D::ReferenceCountedPointer<Chunk>& chunk);
	virtual void removeModel(const G3D::ReferenceCountedPointer<Chunk>& chunk);
	virtual void clear();
	virtual void setSleeping(const G3D::ReferenceCountedPointer<Chunk>& chunk, bool sleeping);
	virtual void prerender(double optimizationTime);
};

DECOMP_SIZE_ASSERT(AggregatingSceneManager, 0x30)

} // namespace RenderNew
} // namespace RBX

#endif // RENDERLIBNEW_AGGREGATINGSCENEMANAGER_H
