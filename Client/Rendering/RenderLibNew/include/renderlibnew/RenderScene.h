#ifndef RENDERLIBNEW_RENDERSCENE_H
#define RENDERLIBNEW_RENDERSCENE_H

#include "decomp.h"
#include "renderlib/RenderStats.h"

#include <G3D/Array.h>
#include <G3D/Color4.h>
#include <G3D/GCamera.h>
#include <G3D/ReferenceCount.h>
#include <GLG3D/LightingParameters.h>
#include <GLG3D/VAR.h>
#include <GLG3D/VARArea.h>
#include <OgreMaterial.h>
#include <OgreMesh.h>

namespace G3D {
class RenderDevice;
} // namespace G3D

namespace RBX {
namespace RenderNew {

class Chunk;

// SIZE 0x510
class RenderScene
{
	friend class SceneManager;

private:
	undefined4* m_unk0x00;                                       // 0x000
	undefined4* m_unk0x04;                                       // 0x004
	Ogre::MaterialPtr m_unk0x08;                                 // 0x008
	Ogre::MeshPtr m_unk0x14;                                     // 0x014
	G3D::Array<undefined4> m_unk0x20;                            // 0x020
	G3D::Array<undefined4> m_unk0x2c;                            // 0x02c
	G3D::Array<undefined4> m_unk0x38;                            // 0x038
	G3D::Array<undefined4> m_unk0x44;                            // 0x044
	G3D::Array<undefined4> m_unk0x50;                            // 0x050
	G3D::Array<undefined4> m_unk0x5c;                            // 0x05c
	G3D::VAR shadowVAR;                                          // 0x068
	G3D::ReferenceCountedPointer<G3D::VARArea> shadowVARArea;    // 0x088
	undefined m_unk0x8c[0xa0 - 0x8c];                            // 0x08c
	G3D::ReferenceCountedPointer<G3D::Lighting> desiredLighting; // 0x0a0
	G3D::ReferenceCountedPointer<G3D::Lighting> lighting;        // 0x0a4
	float shadingQuality;                                        // 0x0a8
	float meshDetail;                                            // 0x0ac
	bool shadows;                                                // 0x0b0
	float cameraDistance;                                        // 0x0b4
	G3D::Array<G3D::ReferenceCountedPointer<Chunk> > m_unk0xb8;  // 0x0b8
	G3D::Array<unsigned int> shadowIndexArray;                   // 0x0c4

public:
	mutable RenderStats renderStats; // 0x0d0
	bool debugShadowVolumes;         // 0x308
	G3D::Color4 colorClearValue;     // 0x30c

private:
	undefined4 m_unk0x31c; // 0x31c

public:
	G3D::LightingParameters desiredSkyParameters; // 0x320
	G3D::LightingParameters skyParameters;        // 0x418

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

	void FUN_0052dcf0(const G3D::ReferenceCountedPointer<Chunk>& chunk);
};

DECOMP_SIZE_ASSERT(SceneManager, 0x08)

} // namespace RenderNew
} // namespace RBX

#endif // RENDERLIBNEW_RENDERSCENE_H
