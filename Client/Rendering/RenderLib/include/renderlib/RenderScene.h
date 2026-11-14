#ifndef RENDERLIB_RENDERSCENE_H
#define RENDERLIB_RENDERSCENE_H

#include "renderlib/Chunk.h"
#include "renderlib/EffectSettings.h"
#include "renderlib/RenderStats.h"
#include "renderlib/RenderSurface.h"
#include "placeholder.h"

#include <G3D/Array.h>
#include <G3D/Color3.h>
#include <G3D/Color4.h>
#include <G3D/GCamera.h>
#include <G3D/ReferenceCount.h>
#include <G3D/Vector3.h>
#include <GLG3D/LightingParameters.h>
#include <GLG3D/Sky.h>
#include <GLG3D/VAR.h>
#include <GLG3D/VARArea.h>

namespace RBX {
namespace Render {

class RenderScene
{
	friend class SceneManager;

private:
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

	void updateShadowVAR(const G3D::Array<G3D::Vector3>& shadowVertex);

	void computeShadowVolumeGeometry(
		G3D::Array<unsigned int>& indexArray,
		G3D::Array<G3D::Vector3>& shadowVertex,
		const G3D::GLight& light,
		bool generateLightCap,
		float shadowVertexDistance
	) const;

	void clearProxyArrays()
	{
		diffuseProxyArray.resize(0, false);
		shadowProxyArray.resize(0, false);
		reflectProxyArray.resize(0, false);
		transparentProxyArray.resize(0, false);
		proxyArray.resize(0, false);
		shadowCachingChunkArray.resize(0, false);
	}

	void allocateProxies(G3D::RenderDevice* rd, const G3D::GCamera& camera);
	void classifyProxies();

	void sortProxies()
	{
		renderStats.sort.tick();
		sortByMaterialAndDepth(diffuseProxyArray);
		sortByMaterial(reflectProxyArray);
		sortByDepth(transparentProxyArray);
		renderStats.sort.tock();
	}
	void computeProxyArrays(G3D::RenderDevice* rd, const G3D::GCamera& camera);
	void sendDiffuseProxyMeshGeometry(G3D::RenderDevice* rd) const;
	void markStencilShadows(G3D::RenderDevice* rd, const G3D::GCamera& camera, const G3D::GLight& light);
	void sendShadowProxyMeshGeometry(G3D::RenderDevice* rd, const G3D::GLight& light) const;
	void turnOnLights(G3D::RenderDevice* rd, bool allLights) const;
	void diffusePass(G3D::RenderDevice* rd);
	void reflectionPass(G3D::RenderDevice* rd);
	void transparentPass(G3D::RenderDevice* rd);
	void debugShowTextures(G3D::RenderDevice* rd, const G3D::GCamera& camera);

	void renderShadowVolumeGeometry(
		G3D::RenderDevice* rd, const G3D::GLight& light, bool caps, float shadowVertexDistance
	);

	G3D::Array<G3D::ReferenceCountedPointer<Chunk> > chunkArray;
	G3D::Array<unsigned int> shadowIndexArray;

public:
	mutable RenderStats renderStats;
	bool debugShadowVolumes;
	G3D::ReferenceCountedPointer<G3D::Sky> sky;
	G3D::Color4 colorClearValue;
	G3D::LightingParameters desiredSkyParameters;
	G3D::LightingParameters skyParameters;

	RenderScene();
	~RenderScene();

	void presetLighting(
		G3D::ReferenceCountedPointer<G3D::Sky> sky,
		G3D::LightingParameters skyParameters,
		G3D::Color3 ambientTop,
		G3D::Color3 ambientBottom
	);

	void setLighting(const G3D::ReferenceCountedPointer<G3D::Lighting>& L);
	void setThrottle(float t, float m, bool s, float c);

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

	void addToScene(const G3D::ReferenceCountedPointer<Chunk>& chunk) { renderScene->chunkArray.append(chunk); }

	void removeFromScene(const G3D::ReferenceCountedPointer<Chunk>& chunk)
	{
		int index = renderScene->chunkArray.findIndex(chunk);
		renderScene->chunkArray.fastRemove(index);
	}
};

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_RENDERSCENE_H
