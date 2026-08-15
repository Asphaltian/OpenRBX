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

// FUNCTION: WEBSERVICE 0x101efa00
void RenderScene::transparentPass(G3D::RenderDevice* rd)
{
	rd->setDepthTest(G3D::RenderDevice::DEPTH_LEQUAL);
	rd->setBlendFunc(G3D::RenderDevice::BLEND_SRC_ALPHA, G3D::RenderDevice::BLEND_ONE_MINUS_SRC_ALPHA);
	rd->setDepthWrite(false);

	for (int i = 0; i < transparentProxyArray.size(); i++) {
		RenderSurface* p = transparentProxyArray[i];
		const Material::Level* material = p->material;

		rd->setObjectToWorldMatrix(p->cframe);
		rd->setPolygonOffset(p->polygonOffset);
		p->material->configureRenderDevice(rd);
		rd->setColor(G3D::Color4(material->color(), 1.0f - material->transparent()));
		Mesh::sendGeometry(p->mesh, rd);
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

// FUNCTION: WEBSERVICE 0x101effc0
void RenderScene::reflectionPass(G3D::RenderDevice* rd)
{
	if (G3D::GLCaps::supports_GL_EXT_texture_cube_map() && !lighting->environmentMap.isNull() &&
		(lighting->environmentMap->getDimension() == G3D::Texture::DIM_CUBE_MAP ||
		 lighting->environmentMap->getDimension() == G3D::Texture::DIM_CUBE_MAP_NPOT)) {
		rd->pushState();
		rd->setDepthTest(G3D::RenderDevice::DEPTH_LEQUAL);
		rd->setPolygonOffset(-0.2);
		rd->setBlendFunc(G3D::RenderDevice::BLEND_ONE, G3D::RenderDevice::BLEND_ONE);
		rd->disableLighting();
		rd->configureReflectionMap(1, lighting->environmentMap);
		rd->setDepthWrite(false);

		for (int i = 0; i < reflectProxyArray.size(); i++) {
			RenderSurface* p = reflectProxyArray[i];
			const Material::Level* material = p->material;

			rd->setPolygonOffset(p->polygonOffset);
			rd->setObjectToWorldMatrix(p->cframe);
			rd->setTexture(0, material->matte(rd));
			rd->setColor(material->reflect() * G3D::Color3::white());
			Mesh::sendGeometry(p->mesh, rd);
		}

		rd->popState();
	}
}

// FUNCTION: WEBSERVICE 0x101f01b0
void RenderScene::setThrottle(float t, float m, bool s, float c)
{
	t = G3D::clamp(t, 0.0f, 100.0f);

	if (t != shadingQuality || m != meshDetail || s != shadows || c != cameraDistance) {
		shadingQuality = t;
		meshDetail = m;
		cameraDistance = c;
		shadows = s;

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

// STUB: WEBSERVICE 0x101f06a0
void RenderScene::computeShadowVolumeGeometry(
	G3D::Array<unsigned int>& indexArray,
	G3D::Array<G3D::Vector3>& shadowVertex,
	const G3D::GLight& light,
	bool generateLightCap,
	float shadowVertexDistance
) const
{
	renderStats.cpuShadow.tick();

	shadowVertex.resize(0, false);
	indexArray.resize(0, false);

	shadowVertex.append(-light.position.xyz().unit() * shadowVertexDistance);

	G3D::Vector3 worldLight = light.position.xyz().unit();

	for (int i = 0; i < shadowProxyArray.size(); i++) {
		shadowProxyArray[i].fullMesh->computeDirectionalShadowVolume(
			shadowProxyArray[i].cframe,
			worldLight,
			indexArray,
			shadowVertex,
			generateLightCap
		);
	}

	renderStats.cpuShadow.tock();
}

// FUNCTION: WEBSERVICE 0x101f07a0
void RenderScene::updateShadowVAR(const G3D::Array<G3D::Vector3>& shadowVertex)
{
	size_t size = shadowVertex.size() * sizeof(G3D::Vector3) + 16;

	if (shadowVARArea.isNull() || shadowVARArea->totalSize() < size) {
		shadowVARArea = G3D::VARArea::create(size);
	}
	else {
		shadowVARArea->reset();
	}

	shadowVAR = G3D::VAR(shadowVertex, shadowVARArea);
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

// FUNCTION: WEBSERVICE 0x101f0ff0
RenderScene::~RenderScene()
{
}

// FUNCTION: WEBSERVICE 0x101f1200
void RenderScene::presetLighting(
	G3D::ReferenceCountedPointer<G3D::Sky> sky,
	G3D::LightingParameters skyParameters,
	G3D::Color3 ambientTop,
	G3D::Color3 ambientBottom
)
{
	this->sky = sky;
	desiredSkyParameters = skyParameters;

	G3D::LightingRef lighting = G3D::Lighting::create();
	lighting->ambientTop = ambientTop;
	lighting->ambientBottom = ambientBottom;

	G3D::GLight light = G3D::GLight::directional(skyParameters.lightDirection, skyParameters.lightColor * 0.9);
	lighting->shadowedLightArray.append(light);

	if (!sky.isNull()) {
		lighting->environmentMap = sky->getEnvironmentMap();
	}

	setLighting(lighting);
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
