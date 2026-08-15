#include "renderlib/RenderScene.h"

namespace RBX {
namespace Render {

// FUNCTION: WEBSERVICE 0x101ef3f0
static float detailLevel(float x, float lo, float hi)
{
	return G3D::clamp(1.0f - (x - lo) / (hi - lo), 0.0f, 1.0f);
}

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
	G3D::CoordinateFrame cameraCFrame;
	camera.getCoordinateFrame(cameraCFrame);

	float farZ = G3D::max(effectSettings.farCullZ(), camera.getFarPlaneZ());
	float nearZ = camera.getNearPlaneZ();

	for (int i = 0; i < chunkArray.size(); i++) {
		Chunk* chunk = chunkArray[i].getPointer();

		if (chunk == NULL) {
			continue;
		}

		if (chunk->getMesh().isNull() || chunk->getMaterial().isNull()) {
			continue;
		}

		const G3D::CoordinateFrame& cframe = chunk->cframe();
		G3D::Vector3 p = cameraCFrame.pointToObjectSpace(cframe.translation);

		bool farCulled = p.z + chunk->radius < farZ && chunk->cullable();
		bool nearCulled = p.z - chunk->radius > nearZ;
		bool shadowCulled = p.z + chunk->radius < farZ * 1.2f && chunk->cullable();
		bool castsShadow = chunk->castsShadows() && !shadowCulled && !chunk->getMaterial()->veryTransparent();

		if (castsShadow) {
			if (chunk->cachesShadows()) {
				shadowCachingChunkArray.append(chunkArray[i]);
			}
			else {
				shadowProxyArray.resize(shadowProxyArray.size() + 1, false);

				RenderSurface& shadowProxy = shadowProxyArray.last();
				shadowProxy.material = NULL;
				shadowProxy.cframe = cframe;
				shadowProxy.mesh = chunk->getMesh()->dropShadowMesh().getPointer();
				shadowProxy.fullMesh = chunk->getMesh();
			}
		}

		if (!farCulled && !nearCulled) {
			RenderSurface proxy;
			proxy.cframe = cframe;
			proxy.polygonOffset = chunk->polygonOffset;
			proxy.z = p.z - chunk->polygonOffset;
			proxy.fullMesh = chunk->getMesh();

			float detail = detailLevel(fabs(proxy.z), -nearZ, -farZ) + effectSettings.LODShift();
			float lod = G3D::clamp(detail, 0.0f, 1.0f);

			proxy.material = chunk->getMaterial()->detailLevel(detail);

			if (proxy.material != NULL) {
				proxy.material->baseTexture(rd);
				proxy.mesh = chunk->getMesh()->detailLevel(lod).getPointer();
				proxyArray.append(proxy);
			}
		}
	}
}

// FUNCTION: WEBSERVICE 0x101f1fd0
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
