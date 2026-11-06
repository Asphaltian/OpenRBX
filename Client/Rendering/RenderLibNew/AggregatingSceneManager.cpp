#include "renderlibnew/AggregatingSceneManager.h"

namespace RBX {
namespace RenderNew {

// FUNCTION: ROBLOX 0x0052fa00
void AggregatingSceneManager::addModel(const G3D::ReferenceCountedPointer<Chunk>& chunk)
{
	FUN_0052dcf0(chunk);
}

// STUB: ROBLOX 0x00530c70
void AggregatingSceneManager::invalidateModel(const G3D::ReferenceCountedPointer<Chunk>& chunk, bool isSleeping)
{
	STUB(0x530c70);
}

// STUB: ROBLOX 0x00530ea0
void AggregatingSceneManager::clear()
{
	STUB(0x530ea0);
}

// STUB: ROBLOX 0x00531230
void AggregatingSceneManager::removeModel(const G3D::ReferenceCountedPointer<Chunk>& chunk)
{
	STUB(0x531230);
}

// FUNCTION: ROBLOX 0x00531550
AggregatingSceneManager::~AggregatingSceneManager()
{
}

// STUB: ROBLOX 0x005317e0
void AggregatingSceneManager::setSleeping(const G3D::ReferenceCountedPointer<Chunk>& chunk, bool sleeping)
{
	STUB(0x5317e0);
}

// FUNCTION: ROBLOX 0x00531980
AggregatingSceneManager::AggregatingSceneManager(RenderScene* renderScene) : SceneManager(renderScene)
{
}

// STUB: ROBLOX 0x00531f50
void AggregatingSceneManager::prerender(double optimizationTime)
{
	STUB(0x531f50);
}

} // namespace RenderNew
} // namespace RBX
