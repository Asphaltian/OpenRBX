#ifndef RENDERLIB_AGGREGATINGSCENEMANAGER_H
#define RENDERLIB_AGGREGATINGSCENEMANAGER_H

#include "decomp.h"
#include "renderlib/Chunk.h"
#include "renderlib/Material.h"
#include "renderlib/RenderScene.h"

#include <G3D/ReferenceCount.h>
#include <boost/noncopyable.hpp>
#include <map>
#include <set>
#include <vector>

namespace RBX {
namespace Render {

// VTABLE: WEBSERVICE 0x10259d60
// VTABLE: ROBLOX 0x008107b0
// SIZE 0x2c
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

		// SYNTHETIC: WEBSERVICE 0x101f2b70
		// SYNTHETIC: ROBLOX 0x0052fc70
		// RBX::Render::AggregatingSceneManager::BucketKey::~BucketKey
	};

	// VTABLE: WEBSERVICE 0x10259d58
	// VTABLE: ROBLOX 0x008107a8
	// SIZE 0x1c
	class Bucket : public G3D::ReferenceCountedObject, public boost::noncopyable
	{
	public:
		bool dequeueSleepingChunk(const G3D::ReferenceCountedPointer<Chunk>& chunk);
		void addToQueue(const G3D::ReferenceCountedPointer<Chunk>& chunk);
		void clear() { queue.clear(); }
		void optimize(AggregatingSceneManager& sceneManager, double maxTime);

		typedef std::vector<G3D::ReferenceCountedPointer<Chunk> > Queue;

	private:
		Queue queue; // 0x0c

	public:
		// SYNTHETIC: WEBSERVICE 0x101f44a0
		// SYNTHETIC: ROBLOX 0x00496d60
		// RBX::Render::AggregatingSceneManager::Bucket::Bucket
		// SYNTHETIC: WEBSERVICE 0x101f44f0
		// SYNTHETIC: ROBLOX 0x00496db0
		// RBX::Render::AggregatingSceneManager::Bucket::~Bucket
		// SYNTHETIC: WEBSERVICE 0x101f4cf0
		// SYNTHETIC: ROBLOX 0x00497480
		// RBX::Render::AggregatingSceneManager::Bucket::`scalar deleting destructor'
	};

	typedef G3D::ReferenceCountedPointer<Bucket> BucketRef;

private:
	std::map<BucketKey, BucketRef> buckets;                                   // 0x08
	std::set<G3D::WeakReferenceCountedPointer<Chunk> > invalidSleepingModels; // 0x14
	std::set<G3D::WeakReferenceCountedPointer<Chunk> > invalidMovingModels;   // 0x20

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
	// SYNTHETIC: WEBSERVICE 0x101f5d90
	// SYNTHETIC: ROBLOX 0x004984e0
	// RBX::Render::AggregatingSceneManager::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(AggregatingSceneManager, 0x2c)
DECOMP_SIZE_ASSERT(AggregatingSceneManager::BucketKey, 0x0c)
DECOMP_SIZE_ASSERT(AggregatingSceneManager::Bucket, 0x1c)

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_AGGREGATINGSCENEMANAGER_H
