#ifndef RENDERLIBNEW_AGGREGATINGSCENEMANAGER_H
#define RENDERLIBNEW_AGGREGATINGSCENEMANAGER_H

#include "decomp.h"
#include "renderlibnew/Chunk.h"
#include "renderlibnew/Material.h"
#include "renderlibnew/RenderScene.h"

#include <G3D/ReferenceCount.h>
#include <boost/noncopyable.hpp>
#include <map>
#include <set>
#include <vector>

namespace RBX {
namespace RenderNew {

// SIZE 0x30
class AggregatingSceneManager : public SceneManager
{
public:
	// SIZE 0x0c
	struct BucketKey
	{
		bool castsShadows;                               // 0x00
		bool cullable;                                   // 0x01
		float polygonOffset;                             // 0x04
		G3D::ReferenceCountedPointer<Material> material; // 0x08

		BucketKey(const G3D::ReferenceCountedPointer<Chunk>& chunk)
			: castsShadows(chunk->castsShadows()), cullable(chunk->cullable()), polygonOffset(chunk->polygonOffset),
			  material(chunk->getMaterial())
		{
		}

		bool operator<(const BucketKey& that) const;
	};

	// SIZE 0x1c
	class Bucket : public G3D::ReferenceCountedObject, public boost::noncopyable
	{
	public:
		bool dequeueSleepingChunk(const G3D::ReferenceCountedPointer<Chunk>& chunk);
		void addToQueue(const G3D::ReferenceCountedPointer<Chunk>& chunk);
		void optimize(AggregatingSceneManager& sceneManager, double maxTime);
		void clear() { queue.clear(); }

		typedef std::vector<G3D::ReferenceCountedPointer<Chunk> > Queue;

	private:
		Queue queue; // 0x0c

	public:
		// SYNTHETIC: ROBLOX 0x00530b90
		// RBX::RenderNew::AggregatingSceneManager::Bucket::Bucket
	};

	typedef G3D::ReferenceCountedPointer<Bucket> BucketRef;

private:
	undefined4 m_unk0x08;                                                     // 0x08
	std::map<BucketKey, BucketRef> buckets;                                   // 0x0c
	std::set<G3D::WeakReferenceCountedPointer<Chunk> > invalidSleepingModels; // 0x18
	std::set<G3D::WeakReferenceCountedPointer<Chunk> > invalidMovingModels;   // 0x24

public:
	static unsigned int aggregationSize;

	AggregatingSceneManager(RenderScene* renderScene);
	virtual ~AggregatingSceneManager();

	// SYNTHETIC: ROBLOX 0x00531a50
	// RBX::RenderNew::AggregatingSceneManager::`scalar deleting destructor'

	virtual void invalidateModel(const G3D::ReferenceCountedPointer<Chunk>& chunk, bool isSleeping);
	virtual void addModel(const G3D::ReferenceCountedPointer<Chunk>& chunk);
	virtual void removeModel(const G3D::ReferenceCountedPointer<Chunk>& chunk);
	virtual void clear();
	virtual void setSleeping(const G3D::ReferenceCountedPointer<Chunk>& chunk, bool sleeping);
	virtual void prerender(double optimizationTime);

private:
	void dequeueSleepingChunk(const G3D::ReferenceCountedPointer<Chunk>& chunk);
	void queueSleepingChunk(const G3D::ReferenceCountedPointer<Chunk>& chunk);
	void deconstructAggregate(G3D::ReferenceCountedPointer<AggregateChunk> aggregate);
};

DECOMP_SIZE_ASSERT(AggregatingSceneManager, 0x30)
DECOMP_SIZE_ASSERT(AggregatingSceneManager::BucketKey, 0x0c)
DECOMP_SIZE_ASSERT(AggregatingSceneManager::Bucket, 0x1c)

} // namespace RenderNew
} // namespace RBX

#endif // RENDERLIBNEW_AGGREGATINGSCENEMANAGER_H
