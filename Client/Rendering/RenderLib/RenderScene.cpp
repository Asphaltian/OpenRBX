#include "renderlib/RenderScene.h"

namespace RBX {
namespace Render {

// FUNCTION: WEBSERVICE 0x101ef990
void RenderScene::sendDiffuseProxyMeshGeometry(G3D::RenderDevice* rd) const
{
	for (int i = diffuseProxyArray.size() - 1; i >= 0; i--) {
		RenderSurface* proxy = diffuseProxyArray[i];

		rd->setObjectToWorldMatrix(proxy->cframe);
		rd->setPolygonOffset(proxy->polygonOffset);
		proxy->material->configureRenderDevice(rd);
		Mesh::sendGeometry(proxy->mesh, rd);
	}
}

// FUNCTION: WEBSERVICE 0x101eff30
void RenderScene::renderShadowVolumeGeometry(
	G3D::RenderDevice* rd,
	const G3D::GLight& light,
	bool caps,
	float shadowVertexDistance
)
{
	rd->beginIndexedPrimitives();
	rd->setVertexArray(shadowVAR);
	rd->sendIndices(G3D::RenderDevice::TRIANGLES, shadowIndexArray);
	rd->endIndexedPrimitives();

	for (int i = 0; i < shadowCachingChunkArray.size(); i++) {
		shadowCachingChunkArray[i]->renderShadows(rd, light, caps, shadowVertexDistance);
	}
}

// FUNCTION: WEBSERVICE 0x101f0320
void RenderScene::setLighting(const G3D::ReferenceCountedPointer<G3D::Lighting>& L)
{
	desiredLighting = L;
	lighting = effectSettings.update(
		shadingQuality,
		meshDetail,
		shadows,
		cameraDistance,
		desiredLighting,
		desiredSkyParameters,
		skyParameters
	);
}

// FUNCTION: WEBSERVICE 0x101f0900
void RenderScene::classifyProxies()
{
	for (int i = 0; i < proxyArray.size(); i++) {
		RenderSurface* ptr = &proxyArray[i];

		if (ptr->material != NULL) {
			if (ptr->material->transparent() > 0) {
				transparentProxyArray.append(ptr);
			}
			else {
				diffuseProxyArray.append(ptr);

				if (ptr->material->reflect() > 0) {
					reflectProxyArray.append(ptr);
				}
			}
		}
	}
}

// STUB: WEBSERVICE 0x101f1960
void RenderScene::allocateProxies(G3D::RenderDevice* rd, const G3D::GCamera& camera)
{
	STUB(0x101f1960);
}

// STUB: WEBSERVICE 0x101f1fd0
void RenderScene::computeProxyArrays(G3D::RenderDevice* rd, const G3D::GCamera& camera)
{
	renderStats.computeProxyArrays.tick();

	clearProxyArrays();
	allocateProxies(rd, camera);
	classifyProxies();
	sortProxies();

	renderStats.computeProxyArrays.tock();

	renderStats.diffuseProxyCount = diffuseProxyArray.size();
}

} // namespace Render
} // namespace RBX
