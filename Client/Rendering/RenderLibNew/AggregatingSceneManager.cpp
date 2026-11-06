#include "renderlibnew/AggregatingSceneManager.h"

#include "renderlibnew/Chunk.h"

#include <G3D/g3dmath.h>
#include <algorithm>

namespace RBX {
namespace RenderNew {

static bool isFinite(const G3D::Matrix3& m)
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (!G3D::isFinite(m[i][j])) {
				return false;
			}
		}
	}

	return true;
}

static bool isReasonable(const G3D::Vector3& v)
{
	if (!v.isFinite()) {
		return false;
	}

	for (int i = 0; i < 3; ++i) {
		if (G3D::abs(v[i]) > 1e10) {
			return false;
		}
	}

	return true;
}

bool AggregatingSceneManager::BucketKey::operator<(const BucketKey& that) const
{
	if (material < that.material) {
		return true;
	}
	if (material > that.material) {
		return false;
	}

	if (castsShadows < that.castsShadows) {
		return true;
	}
	if (castsShadows > that.castsShadows) {
		return false;
	}

	if (polygonOffset < that.polygonOffset) {
		return true;
	}
	if (polygonOffset > that.polygonOffset) {
		return false;
	}

	if (cullable < that.cullable) {
		return true;
	}

	return false;
}

bool AggregatingSceneManager::Bucket::dequeueSleepingChunk(const G3D::ReferenceCountedPointer<Chunk>& chunk)
{
	Queue::iterator iter = std::find(queue.begin(), queue.end(), chunk);
	if (iter != queue.end()) {
		queue.erase(iter);
		return true;
	}

	return false;
}

// FUNCTION: ROBLOX 0x0052fa00
void AggregatingSceneManager::addModel(const G3D::ReferenceCountedPointer<Chunk>& chunk)
{
	addToScene(chunk);
}

// FUNCTION: ROBLOX 0x00530c70
void AggregatingSceneManager::invalidateModel(const G3D::ReferenceCountedPointer<Chunk>& chunk, bool isSleeping)
{
	chunk->VTable0x20();

	if (isSleeping) {
		invalidSleepingModels.insert(chunk);
	}
	else {
		invalidMovingModels.insert(chunk);
	}
}

// FUNCTION: ROBLOX 0x00530e20
void AggregatingSceneManager::dequeueSleepingChunk(const G3D::ReferenceCountedPointer<Chunk>& chunk)
{
	std::map<BucketKey, BucketRef>::iterator iter = buckets.begin();
	std::map<BucketKey, BucketRef>::iterator end = buckets.end();
	while (iter != end) {
		if (iter->second->dequeueSleepingChunk(chunk)) {
			return;
		}
		++iter;
	}
}

// FUNCTION: ROBLOX 0x00530ea0
void AggregatingSceneManager::clear()
{
	invalidSleepingModels.clear();
	invalidMovingModels.clear();

	std::map<BucketKey, BucketRef>::iterator iter = buckets.begin();
	std::map<BucketKey, BucketRef>::iterator end = buckets.end();
	while (iter != end) {
		iter->second->clear();
		++iter;
	}

	clearScene();
}

// FUNCTION: ROBLOX 0x00530fb0
void AggregatingSceneManager::deconstructAggregate(G3D::ReferenceCountedPointer<AggregateChunk> aggregate)
{
	AggregateChunk::Components::iterator iter = aggregate->components.begin();
	AggregateChunk::Components::iterator end = aggregate->components.end();

	while (iter != end) {
		const G3D::ReferenceCountedPointer<Chunk>& chunk = *iter;

		chunk->aggregate = NULL;
		RenderStats::aggregatedChunkCount--;

		addToScene(chunk);

		setSleeping(chunk, true);

		++iter;
	}

	removeFromScene(aggregate);

	aggregate->components.clear();
}

// FUNCTION: ROBLOX 0x00531230
void AggregatingSceneManager::removeModel(const G3D::ReferenceCountedPointer<Chunk>& chunk)
{
	invalidSleepingModels.erase(chunk);
	invalidMovingModels.erase(chunk);

	G3D::ReferenceCountedPointer<AggregateChunk> aggr = chunk->aggregate.createStrongPtr();
	if (aggr.notNull()) {
		deconstructAggregate(aggr);
	}

	removeFromScene(chunk);

	dequeueSleepingChunk(chunk);
}

// FUNCTION: ROBLOX 0x00531350
void AggregatingSceneManager::Bucket::addToQueue(const G3D::ReferenceCountedPointer<Chunk>& chunk)
{
	if (chunk->aggregate.createStrongPtr().notNull()) {
		return;
	}

	if (chunk->getMesh().isNull()) {
		return;
	}

	if (chunk->getMesh()->detailLevel(1.0f)->primitive != G3D::RenderDevice::QUADS) {
		return;
	}

	const G3D::CoordinateFrame& cframe = chunk->cframe();
	if (!isReasonable(cframe.translation) || !isFinite(cframe.rotation)) {
		return;
	}

	if (std::find(queue.begin(), queue.end(), chunk) != queue.end()) {
		return;
	}

	queue.push_back(chunk);
}

// FUNCTION: ROBLOX 0x00531550
AggregatingSceneManager::~AggregatingSceneManager()
{
}

// FUNCTION: ROBLOX 0x00531630
void AggregatingSceneManager::queueSleepingChunk(const G3D::ReferenceCountedPointer<Chunk>& chunk)
{
	BucketRef queue;

	{
		BucketKey key(chunk);
		std::map<BucketKey, BucketRef>::iterator iter = buckets.find(key);
		if (iter == buckets.end()) {
			queue = new Bucket();
			buckets[key] = queue;
		}
		else {
			queue = iter->second;
		}
	}

	queue->addToQueue(chunk);
}

// FUNCTION: ROBLOX 0x005317e0
void AggregatingSceneManager::setSleeping(const G3D::ReferenceCountedPointer<Chunk>& chunk, bool sleeping)
{
	if (sleeping) {
		if (invalidMovingModels.erase(chunk) == 1) {
			invalidSleepingModels.insert(chunk);
		}

		queueSleepingChunk(chunk);
	}
	else {
		if (invalidSleepingModels.erase(chunk) == 1) {
			invalidMovingModels.insert(chunk);
		}

		G3D::ReferenceCountedPointer<AggregateChunk> aggr = chunk->aggregate.createStrongPtr();
		if (aggr.notNull()) {
			deconstructAggregate(aggr);
		}

		dequeueSleepingChunk(chunk);
	}
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
