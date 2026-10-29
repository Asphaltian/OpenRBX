#include "renderlib/AggregatingSceneManager.h"

#include "Clusterer.h"

#include <G3D/System.h>
#include <G3D/g3dmath.h>
#include <algorithm>

namespace RBX {
namespace Render {

unsigned int AggregatingSceneManager::aggregationSize = 40;

// FUNCTION: WEBSERVICE 0x101f2640
// FUNCTION: ROBLOX 0x00495610
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

// FUNCTION: WEBSERVICE 0x101f26b0
// FUNCTION: ROBLOX 0x0052fa10
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

// FUNCTION: WEBSERVICE 0x101f29f0
// FUNCTION: ROBLOX 0x0052fb30
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

// FUNCTION: WEBSERVICE 0x101f3200
void AggregatingSceneManager::addModel(const G3D::ReferenceCountedPointer<Chunk>& chunk)
{
	addToScene(chunk);
}

// FUNCTION: WEBSERVICE 0x101f4570
// FUNCTION: ROBLOX 0x00530be0
bool AggregatingSceneManager::Bucket::dequeueSleepingChunk(const G3D::ReferenceCountedPointer<Chunk>& chunk)
{
	Queue::iterator iter = std::find(queue.begin(), queue.end(), chunk);
	if (iter != queue.end()) {
		queue.erase(iter);
		return true;
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x101f4600
// FUNCTION: ROBLOX 0x00496e30
void AggregatingSceneManager::invalidateModel(const G3D::ReferenceCountedPointer<Chunk>& chunk, bool isSleeping)
{
	if (isSleeping) {
		invalidSleepingModels.insert(chunk);
	}
	else {
		invalidMovingModels.insert(chunk);
	}
}

// FUNCTION: WEBSERVICE 0x101f4d10
// FUNCTION: ROBLOX 0x004974a0
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

// FUNCTION: WEBSERVICE 0x101f4d90
// FUNCTION: ROBLOX 0x00497520
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

// FUNCTION: WEBSERVICE 0x101f4ea0
// FUNCTION: ROBLOX 0x00497630
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

// FUNCTION: WEBSERVICE 0x101f5460
// FUNCTION: ROBLOX 0x00497bb0
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

// FUNCTION: WEBSERVICE 0x101f5610
// FUNCTION: ROBLOX 0x00497d60
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

// FUNCTION: WEBSERVICE 0x101f5850
// FUNCTION: ROBLOX 0x00497fa0
AggregatingSceneManager::~AggregatingSceneManager()
{
}

// FUNCTION: WEBSERVICE 0x101f5930
// FUNCTION: ROBLOX 0x00498080
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

// FUNCTION: WEBSERVICE 0x101f5ae0
// FUNCTION: ROBLOX 0x00498230
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

// FUNCTION: WEBSERVICE 0x101f5cc0
// FUNCTION: ROBLOX 0x00498410
AggregatingSceneManager::AggregatingSceneManager(RenderScene* renderScene) : SceneManager(renderScene)
{
}

// FUNCTION: WEBSERVICE 0x101f5df0
// FUNCTION: ROBLOX 0x00498540
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

// FUNCTION: WEBSERVICE 0x101f62c0
// FUNCTION: ROBLOX 0x00498a10
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

} // namespace Render
} // namespace RBX
