#ifndef RENDERLIBNEW_RENDERSCENE_H
#define RENDERLIBNEW_RENDERSCENE_H

#include "decomp.h"
#include "renderlib/RenderStats.h"

#include <G3D/Color4.h>
#include <G3D/GCamera.h>
#include <G3D/ReferenceCount.h>

namespace G3D {
class RenderDevice;
} // namespace G3D

namespace RBX {
namespace RenderNew {

class Chunk;

// SIZE 0x510
class RenderScene
{
private:
	undefined m_unk0x00[0xa8];        // 0x000
	float shadingQuality;             // 0x0a8
	float meshDetail;                 // 0x0ac
	undefined m_unk0xb0[0xd0 - 0xb0]; // 0x0b0

public:
	mutable RenderStats renderStats; // 0x0d0

private:
	undefined m_unk0x308[0x30c - 0x308]; // 0x308

public:
	G3D::Color4 colorClearValue; // 0x30c

private:
	undefined m_unk0x31c[0x510 - 0x31c]; // 0x31c

public:
	RenderScene();
	~RenderScene();

	void setThrottle(float t, float m, bool s, float c);

	float getShadingQuality() const { return shadingQuality; }
	float getMeshDetail() const { return meshDetail; }

	void render(G3D::RenderDevice* rd, const G3D::GCamera& camera);
};

DECOMP_SIZE_ASSERT(RenderScene, 0x510)

// VTABLE: ROBLOX 0x0081adfc
// SIZE 0x08
class SceneManager
{
public:
	virtual void invalidateModel(const G3D::ReferenceCountedPointer<Chunk>& chunk, bool isSleeping) = 0; // vtable+0x00
	virtual void addModel(const G3D::ReferenceCountedPointer<Chunk>& chunk) = 0;                         // vtable+0x04
	virtual void removeModel(const G3D::ReferenceCountedPointer<Chunk>& chunk) = 0;                      // vtable+0x08
	virtual void clear() = 0;                                                                            // vtable+0x0c
	virtual void setSleeping(const G3D::ReferenceCountedPointer<Chunk>& chunk, bool sleeping) = 0;       // vtable+0x10
	virtual void prerender(double optimizationTime) = 0;                                                 // vtable+0x14

	virtual ~SceneManager() // vtable+0x18
	{
	}

	// SYNTHETIC: ROBLOX 0x0052f9e0
	// RBX::RenderNew::SceneManager::`scalar deleting destructor'

	RenderScene* renderScene; // 0x04

protected:
	SceneManager(RenderScene* renderScene) : renderScene(renderScene) {}
};

DECOMP_SIZE_ASSERT(SceneManager, 0x08)

} // namespace RenderNew
} // namespace RBX

#endif // RENDERLIBNEW_RENDERSCENE_H
