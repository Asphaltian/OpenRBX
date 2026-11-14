#ifndef RENDERLIB_AGGREGATINGSCENEMANAGER_H
#define RENDERLIB_AGGREGATINGSCENEMANAGER_H

#include "renderlib/Chunk.h"
#include "renderlib/Material.h"
#include "renderlib/RenderScene.h"
#include "placeholder.h"

#include <G3D/ReferenceCount.h>
#include <boost/noncopyable.hpp>
#include <map>
#include <set>
#include <vector>

namespace RBX {
namespace Render {

class AggregatingSceneManager : public SceneManager
{
public:
	struct BucketKey
	{
		bool castsShadows;
		bool cullable;
		float polygonOffset;
		G3D::ReferenceCountedPointer<Material> material;

		BucketKey(const G3D::ReferenceCountedPointer<Chunk>& chunk)
			: castsShadows(chunk->castsShadows()), cullable(chunk->cullable()), polygonOffset(chunk->polygonOffset),
			  material(chunk->getMaterial())
		{
		}

		bool operator<(const BucketKey& that) const;
	};

	class Bucket : public G3D::ReferenceCountedObject, public boost::noncopyable
	{
	public:
		bool dequeueSleepingChunk(const G3D::ReferenceCountedPointer<Chunk>& chunk);
		void addToQueue(const G3D::ReferenceCountedPointer<Chunk>& chunk);
		void clear() { queue.clear(); }
		void optimize(AggregatingSceneManager& sceneManager, double maxTime);

		typedef std::vector<G3D::ReferenceCountedPointer<Chunk> > Queue;

	private:
		Queue queue;

	public:
	};

	typedef G3D::ReferenceCountedPointer<Bucket> BucketRef;

private:
	std::map<BucketKey, BucketRef> buckets;
	std::set<G3D::WeakReferenceCountedPointer<Chunk> > invalidSleepingModels;
	std::set<G3D::WeakReferenceCountedPointer<Chunk> > invalidMovingModels;

public:
	static unsigned int aggregationSize;

	AggregatingSceneManager(RenderScene* renderScene);
	virtual ~AggregatingSceneManager();

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

public:
};

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_AGGREGATINGSCENEMANAGER_H
