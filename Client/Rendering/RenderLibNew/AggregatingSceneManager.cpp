#include "renderlibnew/AggregatingSceneManager.h"

#include "Clusterer.h"
#include "renderlibnew/Chunk.h"

#include <G3D/System.h>
#include <G3D/g3dmath.h>
#include <algorithm>

namespace RBX {
namespace RenderNew {

// GLOBAL: ROBLOX 0x0092b7c4
unsigned int AggregatingSceneManager::aggregationSize = 40;

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

// FUNCTION: ROBLOX 0x00531a70
void AggregatingSceneManager::Bucket::optimize(AggregatingSceneManager& sceneManager, double maxTime)
{
	if (queue.size() * 4 < aggregationSize) {
		return;
	}

	Clusterer clusterer(std::max(1U, queue.size() / aggregationSize));

	clusterer.addSamples(queue.begin(), queue.end());
	queue.clear();

	Clusterer::Clusters* clusters = clusterer.go(5);

	Clusterer::Clusters::iterator iter = clusters->begin();
	Clusterer::Clusters::iterator end = clusters->end();
	for (; iter != end; ++iter) {
		Clusterer::Samples::iterator iter2 = iter->samples.begin();
		Clusterer::Samples::iterator end2 = iter->samples.end();

		if (end2 - iter2 >= 4) {
			G3D::ReferenceCountedPointer<AggregateChunk> aggregateChunk;

			for (; iter2 != end2; ++iter2) {
				G3D::ReferenceCountedPointer<Chunk> chunkToAggregate = *iter2;
				if (aggregateChunk.isNull()) {
					aggregateChunk = new AggregateChunk(chunkToAggregate);
				}

				aggregateChunk->components.push_back(chunkToAggregate);
				chunkToAggregate->aggregate = aggregateChunk;
				RenderStats::aggregatedChunkCount++;
				sceneManager.removeFromScene(chunkToAggregate);
			}

			if (aggregateChunk.notNull()) {
				aggregateChunk->makeMesh();
				sceneManager.addToScene(aggregateChunk);
			}
		}
	}
}

// FUNCTION: ROBLOX 0x00531f50
void AggregatingSceneManager::prerender(double optimizationTime)
{
	const double timeUp = G3D::System::getTick() + optimizationTime * 1000.0;

	std::set<G3D::WeakReferenceCountedPointer<Chunk> >::iterator iter = invalidSleepingModels.begin();
	std::set<G3D::WeakReferenceCountedPointer<Chunk> >::iterator end = invalidSleepingModels.end();
	while (iter != end) {
		G3D::ReferenceCountedPointer<Chunk> chunk = iter->createStrongPtr();
		if (chunk.notNull()) {
			chunk->getMesh();
			chunk->getMaterial();

			G3D::ReferenceCountedPointer<AggregateChunk> aggr = chunk->aggregate.createStrongPtr();
			if (aggr.notNull()) {
				deconstructAggregate(aggr);
			}
			else {
				dequeueSleepingChunk(chunk);
				queueSleepingChunk(chunk);
			}
		}
		++iter;
	}

	invalidSleepingModels.clear();
	invalidMovingModels.clear();

	if (aggregationSize > 1) {
		std::map<BucketKey, BucketRef>::iterator iter = buckets.begin();
		std::map<BucketKey, BucketRef>::iterator end = buckets.end();
		while (timeUp > G3D::System::getTick() && iter != end) {
			iter->second->optimize(*this, optimizationTime);
			++iter;
		}
	}
}

} // namespace RenderNew
} // namespace RBX
