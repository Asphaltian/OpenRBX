#ifndef RENDERLIBNEW_RENDERSCENE_H
#define RENDERLIBNEW_RENDERSCENE_H

#include "renderlib/RenderStats.h"
#include "renderlibnew/EffectSettings.h"
#include "renderlibnew/RenderSurface.h"
#include "placeholder.h"

#include <G3D/Array.h>
#include <G3D/Color4.h>
#include <G3D/GCamera.h>
#include <G3D/ReferenceCount.h>
#include <GLG3D/LightingParameters.h>
#include <GLG3D/Sky.h>
#include <GLG3D/VAR.h>
#include <GLG3D/VARArea.h>
#include <OgreMaterial.h>
#include <OgreMesh.h>

namespace G3D {
class RenderDevice;
} // namespace G3D

namespace Ogre {
class RbxEntity;
class RbxSceneNode;
class VisualEngine;
} // namespace Ogre

namespace RBX {
namespace RenderNew {

class Chunk;

class RenderScene
{
	friend class SceneManager;

private:
	Ogre::RbxSceneNode* m_unk0x00;
	Ogre::RbxEntity* m_unk0x04;
	Ogre::MaterialPtr m_unk0x08;
	Ogre::MeshPtr m_unk0x14;
	G3D::Array<RenderSurface> proxyArray;
	G3D::Array<RenderSurface*> diffuseProxyArray;
	G3D::Array<RenderSurface*> reflectProxyArray;
	G3D::Array<RenderSurface*> transparentProxyArray;
	G3D::Array<RenderSurface> shadowProxyArray;
	G3D::Array<G3D::ReferenceCountedPointer<Chunk> > shadowCachingChunkArray;
	G3D::VAR shadowVAR;
	G3D::ReferenceCountedPointer<G3D::VARArea> shadowVARArea;
	EffectSettings effectSettings;
	G3D::ReferenceCountedPointer<G3D::Lighting> desiredLighting;
	G3D::ReferenceCountedPointer<G3D::Lighting> lighting;
	float shadingQuality;
	float meshDetail;
	bool shadows;
	float cameraDistance;
	G3D::Array<G3D::ReferenceCountedPointer<Chunk> > chunkArray;
	G3D::Array<unsigned int> shadowIndexArray;

public:
	mutable RenderStats renderStats;
	bool debugShadowVolumes;
	G3D::Color4 colorClearValue;

private:
	undefined4 m_unk0x31c;

public:
	G3D::LightingParameters desiredSkyParameters;
	G3D::LightingParameters skyParameters;

	RenderScene();
	~RenderScene();

	void FUN_0052c9b0(Ogre::VisualEngine* engine, bool p_unk0x8e);
	void setThrottle(float t, float m, bool s, float c);
	void setLighting(const G3D::ReferenceCountedPointer<G3D::Lighting>& L);
	void presetLighting(
		G3D::ReferenceCountedPointer<G3D::Sky> sky,
		G3D::LightingParameters skyParameters,
		G3D::Color3 ambientTop,
		G3D::Color3 ambientBottom
	);

	float getShadingQuality() const { return shadingQuality; }
	float getMeshDetail() const { return meshDetail; }

	void render(G3D::RenderDevice* rd, const G3D::GCamera& camera);
};

class SceneManager
{
public:
	virtual void invalidateModel(const G3D::ReferenceCountedPointer<Chunk>& chunk, bool isSleeping) = 0;
	virtual void addModel(const G3D::ReferenceCountedPointer<Chunk>& chunk) = 0;
	virtual void removeModel(const G3D::ReferenceCountedPointer<Chunk>& chunk) = 0;
	virtual void clear() = 0;
	virtual void setSleeping(const G3D::ReferenceCountedPointer<Chunk>& chunk, bool sleeping) = 0;
	virtual void prerender(double optimizationTime) = 0;

	virtual ~SceneManager() {}

	RenderScene* renderScene;

protected:
	SceneManager(RenderScene* renderScene) : renderScene(renderScene) {}

	void clearScene() { renderScene->chunkArray.resize(0); }

	void addToScene(const G3D::ReferenceCountedPointer<Chunk>& chunk);

	void removeFromScene(const G3D::ReferenceCountedPointer<Chunk>& chunk)
	{
		int index = renderScene->chunkArray.findIndex(chunk);
		renderScene->chunkArray.fastRemove(index);
	}
};

} // namespace RenderNew
} // namespace RBX

#endif // RENDERLIBNEW_RENDERSCENE_H
