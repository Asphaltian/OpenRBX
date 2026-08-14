#ifndef RENDERLIB_RENDERSCENE_H
#define RENDERLIB_RENDERSCENE_H

#include "decomp.h"
#include "renderlib/Chunk.h"
#include "renderlib/EffectSettings.h"
#include "renderlib/RenderStats.h"
#include "renderlib/RenderSurface.h"

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

// SIZE 0x4f0
class RenderScene
{
private:
	G3D::Array<RenderSurface> proxyArray;                                     // 0x000
	G3D::Array<RenderSurface*> diffuseProxyArray;                             // 0x00c
	G3D::Array<RenderSurface*> reflectProxyArray;                             // 0x018
	G3D::Array<RenderSurface*> transparentProxyArray;                         // 0x024
	G3D::Array<RenderSurface> shadowProxyArray;                               // 0x030
	G3D::Array<G3D::ReferenceCountedPointer<Chunk> > shadowCachingChunkArray; // 0x03c
	G3D::VAR shadowVAR;                                                       // 0x048
	G3D::ReferenceCountedPointer<G3D::VARArea> shadowVARArea;                 // 0x068
	EffectSettings effectSettings;                                            // 0x06c
	G3D::ReferenceCountedPointer<G3D::Lighting> desiredLighting;              // 0x080
	G3D::ReferenceCountedPointer<G3D::Lighting> lighting;                     // 0x084
	float shadingQuality;                                                     // 0x088
	float meshDetail;                                                         // 0x08c
	bool shadows;                                                             // 0x090
	float cameraDistance;                                                     // 0x094

	void updateShadowVAR(const G3D::Array<G3D::Vector3>& shadowVertex);

	void computeShadowVolumeGeometry(
		G3D::Array<unsigned int>& shadowIndexArray,
		G3D::Array<G3D::Vector3>& shadowVertex,
		const G3D::GLight& light,
		bool caps,
		float shadowVertexDistance
	) const;

	void clearProxyArrays();
	void allocateProxies(G3D::RenderDevice* renderDevice, const G3D::GCamera& camera);
	void classifyProxies();
	void sortProxies();
	void computeProxyArrays(G3D::RenderDevice* renderDevice, const G3D::GCamera& camera);
	void sendDiffuseProxyMeshGeometry(G3D::RenderDevice* renderDevice) const;
	void markStencilShadows(G3D::RenderDevice* renderDevice, const G3D::GCamera& camera, const G3D::GLight& light);
	void sendShadowProxyMeshGeometry(G3D::RenderDevice* renderDevice, const G3D::GLight& light) const;
	void turnOnLights(G3D::RenderDevice* renderDevice, bool shadowPass) const;
	void diffusePass(G3D::RenderDevice* renderDevice);
	void reflectionPass(G3D::RenderDevice* renderDevice);
	void transparentPass(G3D::RenderDevice* renderDevice);
	void debugShowTextures(G3D::RenderDevice* renderDevice, const G3D::GCamera& camera);

	void renderShadowVolumeGeometry(
		G3D::RenderDevice* renderDevice,
		const G3D::GLight& light,
		bool caps,
		float shadowVertexDistance
	);

	G3D::Array<G3D::ReferenceCountedPointer<Chunk> > chunkArray; // 0x098
	G3D::Array<unsigned int> shadowIndexArray;                   // 0x0a4

public:
	RenderStats renderStats;                      // 0x0b0
	bool debugShadowVolumes;                      // 0x2e8
	G3D::ReferenceCountedPointer<G3D::Sky> sky;   // 0x2ec
	G3D::Color4 colorClearValue;                  // 0x2f0
	G3D::LightingParameters desiredSkyParameters; // 0x300
	G3D::LightingParameters skyParameters;        // 0x3f8

	RenderScene();
	~RenderScene();

	void presetLighting(
		G3D::ReferenceCountedPointer<G3D::Sky> sky,
		G3D::LightingParameters skyParameters,
		G3D::Color3 ambient,
		G3D::Color3 diffuse
	);

	void setLighting(const G3D::ReferenceCountedPointer<G3D::Lighting>& lighting);
	void setThrottle(float shadingQuality, float meshDetail, bool shadows, float cameraDistance);

	float getShadingQuality() const;
	float getMeshDetail() const;

	void render(G3D::RenderDevice* renderDevice, const G3D::GCamera& camera);
};

DECOMP_SIZE_ASSERT(RenderScene, 0x4f0)

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_RENDERSCENE_H
