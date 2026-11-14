#include "renderlibnew/RenderScene.h"

#include "appdraw/RbxEntity.h"
#include "appdraw/RbxSceneNode.h"
#include "appdraw/RbxSubEntity.h"
#include "appdraw/VisualEngine.h"
#include "renderlibnew/Chunk.h"
#include "util/ContentProvider.h"
#include "util/Utilities.h"

#include <G3D/Color4.h>
#include <G3D/g3dmath.h>
#include <OgreCamera.h>
#include <OgreImage.h>
#include <OgreLight.h>
#include <OgreLogManager.h>
#include <OgreMaterialManager.h>
#include <OgreMeshManager.h>
#include <OgrePass.h>
#include <OgreRenderTarget.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreSubMesh.h>
#include <OgreTechnique.h>
#include <OgreTextureManager.h>
#include <OgreTextureUnitState.h>
#include <OgreTimer.h>
#include <stdio.h>

namespace RBX {
namespace RenderNew {

static int g_unk0x00958700;

void RenderScene::FUN_0052c9b0(Ogre::VisualEngine* engine, bool p_unk0x8e)
{
	Ogre::SceneManager* sceneManager = engine->m_unk0x04;
	float radius = 1000.0f;
	Ogre::Vector3 center(0.0f, 0.0f, 0.0f);

	int i;
	for (i = 0; i < lighting->shadowedLightArray.size(); i++) {
		const G3D::GLight& source = lighting->shadowedLightArray[i];
		std::string name = "rbxlight" + StringConverter<int>::convertToString(i);

		Ogre::Light* light;
		if (sceneManager->hasLight(name)) {
			light = sceneManager->getLight(name);
		}
		else {
			light = sceneManager->createLight(name);
			if (light) {
				Ogre::LogManager::getSingleton().logMessage("RbxGfx: created light" + name);
			}
			else {
				Ogre::LogManager::getSingleton().logMessage("RbxGfx : FAILED to create light" + name);
			}
		}
		light->setCastShadows(effectSettings.stencilShadows());

		if (m_unk0x04 && !m_unk0x04->m_unk0x248.isNull()) {
			const Ogre::AxisAlignedBox& box = m_unk0x04->m_unk0x248;
			Ogre::Vector3 size = box.getSize();
			float extent = size.x > size.y ? size.x : size.y;
			if (extent < size.z) {
				extent = size.z;
			}
			radius = extent * 1.717 * 0.5 * 1.3;
			Ogre::Vector3 boxCenter = box.getCenter();
			center.x = boxCenter.x;
			center.z = boxCenter.z;
			center.y = box.getMinimum().y;
		}

		light->setType(Ogre::Light::LT_POINT);
		light->setDiffuseColour(source.color.r, source.color.g, source.color.b);
		light->setPosition(
			radius * source.position.x + center.x,
			source.position.y * radius + center.y,
			center.z + radius * source.position.z
		);
		light->setSpecularColour(source.color.r, source.color.g, source.color.b);
	}

	for (;; i++) {
		std::string name = "rbxlight" + StringConverter<int>::convertToString(i);
		if (!sceneManager->hasLight(name)) {
			break;
		}
		sceneManager->destroyLight(sceneManager->getLight(name));
	}

	if (lighting->shadowedLightArray.size() == 1) {
		int index = 0;
		std::string name = "rbxlight" + StringConverter<int>::convertToString(index);
		Ogre::Light* light = sceneManager->getLight(name);

		Ogre::Vector3 position;
		if (light->getType() == Ogre::Light::LT_DIRECTIONAL) {
			position = -light->getDirection() * 2000.0;
		}
		else {
			position = light->getPosition();
		}

		G3D::Color3 average = (lighting->ambientBottom + lighting->ambientTop) / 2.0f;

		if (average != lighting->ambientBottom) {
			std::string fillName = "rbxfl";
			Ogre::Light* fill;
			if (sceneManager->hasLight(fillName)) {
				fill = sceneManager->getLight(fillName);
			}
			else {
				fill = sceneManager->createLight(fillName);
				fill->setCastShadows(false);
				fill->setType(Ogre::Light::LT_POINT);
			}
			fill->setPosition(center * 2.0 - position);
			fill->setDiffuseColour(Ogre::FUN_00671cd0(G3D::Color4(lighting->ambientBottom - average, 1.0f)));
		}

		if (average != lighting->ambientTop) {
			std::string fillName = "rbxfl2";
			Ogre::Light* fill;
			if (sceneManager->hasLight(fillName)) {
				fill = sceneManager->getLight(fillName);
			}
			else {
				fill = sceneManager->createLight(fillName);
				fill->setCastShadows(false);
				fill->setType(Ogre::Light::LT_POINT);
			}
			fill->setPosition(Ogre::Vector3(center.x, center.y + radius, center.z));
			fill->setDiffuseColour(Ogre::FUN_00671cd0(G3D::Color4(lighting->ambientTop - average, 1.0f)));
		}
	}

	G3D::Color3 ambient = (lighting->ambientBottom + lighting->ambientTop) / 2.0f;
	if (lighting->shadowedLightArray.size() > 0) {
		G3D::Vector3 direction = lighting->shadowedLightArray[0].position.xyz();
	}
}

void RenderScene::setThrottle(float t, float m, bool s, float c)
{
	t = G3D::clamp(t, 0.0f, 100.0f);

	if (t != shadingQuality || m != meshDetail || s != shadows || c != cameraDistance) {
		shadingQuality = t;
		meshDetail = m;
		cameraDistance = c;
		shadows = s;

		lighting = effectSettings.update(
			shadingQuality, meshDetail, shadows, cameraDistance, desiredLighting, desiredSkyParameters, skyParameters
		);
	}
}

void RenderScene::setLighting(const G3D::ReferenceCountedPointer<G3D::Lighting>& L)
{
	desiredLighting = L;
	lighting = effectSettings.update(
		shadingQuality, meshDetail, shadows, cameraDistance, desiredLighting, desiredSkyParameters, skyParameters
	);
}

RenderScene::~RenderScene()
{
	if (m_unk0x00) {
		m_unk0x00->getCreator()->getRootSceneNode()->removeChild(m_unk0x00->getName());
		delete m_unk0x00;
	}
	delete m_unk0x04;
}

void RenderScene::presetLighting(
	G3D::ReferenceCountedPointer<G3D::Sky> sky,
	G3D::LightingParameters skyParameters,
	G3D::Color3 ambientTop,
	G3D::Color3 ambientBottom
)
{
	desiredSkyParameters = skyParameters;

	G3D::LightingRef lighting = G3D::Lighting::create();
	lighting->ambientTop = ambientTop;
	lighting->ambientBottom = ambientBottom;

	G3D::GLight sun = G3D::GLight::directional(skyParameters.lightDirection, skyParameters.lightColor * 0.9);
	lighting->shadowedLightArray.append(sun);

	setLighting(lighting);
}

void SceneManager::addToScene(const G3D::ReferenceCountedPointer<Chunk>& chunk)
{
	renderScene->chunkArray.append(chunk);
}

RenderScene::RenderScene()
	: m_unk0x04(NULL), shadingQuality(-1.0f), meshDetail(-1.0f), shadows(false), cameraDistance((float) G3D::inf()),
	  debugShadowVolumes(false), colorClearValue(0.5f, 0.5f, 1.0f, 1.0f)
{
	desiredLighting = lighting = G3D::Lighting::create();
	m_unk0x00 = NULL;
}

void RenderScene::render(G3D::RenderDevice* rd, const G3D::GCamera& camera)
{
	Ogre::VisualEngine* engine = reinterpret_cast<Ogre::VisualEngine*>(rd);
	Ogre::Camera* ogreCamera = engine->m_unk0x08;
	G3D::CoordinateFrame cameraFrame = camera.getCoordinateFrame();

	if (effectSettings.stencilShadows()) {
		engine->m_unk0x04->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
		engine->m_unk0x04->setShadowColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f, 1.0f));
	}
	else {
		engine->m_unk0x04->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
	}

	Ogre::Quaternion orientation(1.0f, 0.0f, 0.0f, 0.0f);
	orientation.FromRotationMatrix(reinterpret_cast<const Ogre::Matrix3&>(cameraFrame.rotation));
	ogreCamera->setOrientation(orientation);
	ogreCamera->setPosition(cameraFrame.translation.x, cameraFrame.translation.y, cameraFrame.translation.z);
	engine->m_unk0x08->setNearClipDistance(-camera.getNearPlaneZ());
	engine->m_unk0x08->setFarClipDistance(-camera.getFarPlaneZ());
	engine->m_unk0x08->setFOVy(Ogre::Radian((float) camera.getFieldOfView()));

	Ogre::Timer timer;
	timer.reset();

	if (m_unk0x00 == NULL) {
		m_unk0x00 = new Ogre::RbxSceneNode(engine->m_unk0x04, "rbxroot");
		engine->m_unk0x04->getRootSceneNode()->addChild(m_unk0x00);
	}

	FUN_0052c9b0(engine, effectSettings.useAllLightsInUnshadowedPass());
	bool hardwareMeshes = engine->m_unk0x7f5;

	if (m_unk0x08.isNull()) {
		m_unk0x08 = Ogre::MaterialManager::getSingleton().getByName("BaseWhite");
	}

	if (m_unk0x14.isNull()) {
		m_unk0x14 = Ogre::MeshManager::getSingleton().createOrRetrieve("rbxdummymesh", "General").first;
	}

	if (m_unk0x04 == NULL && chunkArray.size() != 0) {
		m_unk0x04 = new Ogre::RbxEntity();
		m_unk0x00->attachObject(m_unk0x04);
		m_unk0x04->FUN_00674e40(m_unk0x14);
	}

	for (int i = 0; i < chunkArray.size(); i++) {
		Chunk* chunk = chunkArray[i].getPointer();
		if (chunk) {
			chunk->getMesh();
		}
	}

	if (Mesh::varDirty && chunkArray.size() != 0 && hardwareMeshes) {
		Mesh::FUN_00532380(engine);
		m_unk0x04->FUN_00674e40(Mesh::g_unk0x0095877c);
		m_unk0x04->FUN_0052c8b0(Mesh::g_unk0x0095877c->getBounds());
	}

	G3D::CoordinateFrame viewFrame;
	camera.getCoordinateFrame(viewFrame);

	for (int i = 0; i < chunkArray.size(); i++) {
		Chunk* chunk = chunkArray[i].getPointer();
		if (chunk == NULL) {
			continue;
		}

		bool hasMesh = chunk->getMesh().notNull();
		bool hasMaterial = chunk->getMaterial().notNull();
		if (!hasMesh || !hasMaterial) {
			continue;
		}

		const Material::Level* materialLevel = chunk->getMaterial()->detailLevel(0.0f);
		if (materialLevel == NULL) {
			continue;
		}

		G3D::ReferenceCountedPointer<Mesh> mesh = chunk->getMesh();
		if (effectSettings.stencilShadows() && mesh->m_unk0x38 == NULL) {
			mesh->FUN_005324e0(engine);
		}

		int lod = 0;
		if (mesh->levels.size() == 2) {
			G3D::Vector3 local = viewFrame.pointToObjectSpace(chunk->cframe().translation);
			lod = 0;
			if (-local.z >= 80.0f) {
				lod = 1;
			}
		}

		Mesh::LevelRef meshLevel = mesh->detailLevel((float) lod);
		bool rebuilt = false;

		if (mesh->m_unk0x3c == 4 && !engine->m_unk0x806 && meshLevel->m_unk0x1c) {
			Mesh::LevelRef base = mesh->detailLevel(0.0f);
			if (base->primitive == G3D::RenderDevice::QUADS && base->indexArray.size() == 4) {
				const G3D::Vector3& normal = mesh->getNormal(base->indexArray[0]);
				for (int j = 0; j < 4; j++) {
					mesh->getVertex(base->indexArray[j]) += normal * 0.06f;
				}
			}
		}

		if (!hardwareMeshes) {
			if (meshLevel->m_unk0x1c) {
				Ogre::AxisAlignedBox box;
				meshLevel->FUN_00533160(engine, box);
				float radius = box.getHalfSize().length();
				Ogre::Sphere sphere(box.getCenter(), radius);
				m_unk0x00->FUN_0067b2b0(meshLevel->m_unk0x24, sphere);
				rebuilt = true;
			}
		}
		else if (meshLevel->m_unk0x1c) {
			Ogre::AxisAlignedBox box;
			meshLevel->makeHWSubMesh(engine, box);
			float radius = box.getHalfSize().length();
			Ogre::Sphere sphere(box.getCenter(), radius);
			Ogre::SubMesh* subMesh = !meshLevel->m_unk0x1c && meshLevel->m_unk0x20 >= 0
										 ? Mesh::g_unk0x0095877c->getSubMesh(meshLevel->m_unk0x20)
										 : NULL;
			m_unk0x00->FUN_0067b2b0(subMesh, sphere);
			rebuilt = true;
		}

		bool updateMaterial = false;
		materialLevel->baseTexture(rd);

		if (materialLevel->m_unk0x28.isNull()) {
			char name[8];
			_snprintf(name, sizeof(name), "m%d", g_unk0x00958700++);
			materialLevel->m_unk0x28 = Ogre::MaterialManager::getSingleton().create(name, "General");

			const G3D::Color3& color = materialLevel->color();
			Ogre::Pass* pass = materialLevel->m_unk0x28->getTechnique(0)->getPass(0);
			pass->setDiffuse(color.r, color.g, color.b, 1.0f);
			pass->setAmbient(color.r, color.g, color.b);
			pass->setShininess(materialLevel->shiny());
			pass->setSpecular(materialLevel->specular(), materialLevel->specular(), materialLevel->specular(), 1.0f);

			const Ogre::TexturePtr& texture = materialLevel->mBaseTexture->m_unk0x4c;
			if (!texture.isNull()) {
				Ogre::TextureUnitState* unit = pass->createTextureUnitState();
				unit->setTextureFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_LINEAR);
				unit->setTextureAddressingMode(
					Ogre::TextureUnitState::TAM_WRAP, Ogre::TextureUnitState::TAM_WRAP, Ogre::TextureUnitState::TAM_WRAP
				);
				unit->setTextureBorderColour(Ogre::ColourValue(color.r, color.g, color.b, 1.0f));
				unit->setTextureName(texture->getName(), Ogre::TEX_TYPE_2D);
			}

			bool hasAlpha = !texture.isNull() && texture->hasAlpha();

			if (0.0f < materialLevel->transparent() || hasAlpha) {
				pass->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
				pass->setDepthWriteEnabled(false);
				pass->setDiffuse(color.r, color.g, color.b, 1.0f - materialLevel->transparent());
				if (pass->getNumTextureUnitStates() != 0) {
					pass->getTextureUnitState(0)->setColourOperation(Ogre::LBO_MODULATE);
				}
			}

			if (!(0.0f < materialLevel->transparent()) && hasAlpha &&
				meshLevel->primitive == G3D::RenderDevice::TRIANGLES) {
				pass->setSceneBlending(Ogre::SBT_REPLACE);
				pass->setDepthWriteEnabled(true);
				pass->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 1);
			}

			if (0.0f < materialLevel->reflect()) {
				Ogre::TexturePtr sky = Ogre::TextureManager::getSingleton().getByName("skysphmap");
				if (sky.isNull()) {
					sky = Ogre::TextureManager::getSingleton().create("skysphmap", "General");
					Ogre::Image image;
					std::string path = ContentProvider::singleton().assetFolder() + "Sky\\" + "skyspheremap.jpg";
					image.load(path, "General");
					sky->loadImage(image);
				}

				Ogre::Pass* reflection = materialLevel->m_unk0x28->getTechnique(0)->createPass();
				reflection->setDiffuse(0.0f, 0.0f, 0.0f, 1.0f);
				reflection->setAmbient(Ogre::ColourValue(2.0f, 2.0f, 2.0f, 2.0f) * materialLevel->reflect());
				reflection->setSceneBlending(Ogre::SBF_ONE, Ogre::SBF_ONE);
				Ogre::TextureUnitState* unit = reflection->createTextureUnitState();
				unit->setTextureName(sky->getName(), Ogre::TEX_TYPE_2D);
				unit->setEnvironmentMap(true, Ogre::TextureUnitState::ENV_CURVED);
			}

			pass->setDepthBias(-chunk->polygonOffset * 100.0, 0.0f);
			Ogre::LogManager::getSingleton().logMessage(std::string("RBX create ") + name);
			updateMaterial = true;
		}

		if (chunk->m_unk0x10 == NULL) {
			Ogre::SubMesh* subMesh;
			Ogre::RbxSubEntity* subEntity;
			if (hardwareMeshes) {
				if (meshLevel->m_unk0x1c || meshLevel->m_unk0x20 < 0 ||
					!Mesh::g_unk0x0095877c->getSubMesh(meshLevel->m_unk0x20)) {
					throw std::exception("Please restart ROBLOX, if you changed your settings but have not restarted");
				}
				subMesh = !meshLevel->m_unk0x1c && meshLevel->m_unk0x20 >= 0
							  ? Mesh::g_unk0x0095877c->getSubMesh(meshLevel->m_unk0x20)
							  : NULL;
				subEntity = m_unk0x04->FUN_00676680(subMesh, lod);
				subEntity->m_unk0x128 = false;
			}
			else {
				subMesh = meshLevel->m_unk0x24;
				if (subMesh == NULL) {
					throw std::exception("Please restart ROBLOX, if you changed your settings but have not restarted");
				}
				subEntity = m_unk0x04->FUN_00676680(subMesh, lod);
			}

			subEntity->FUN_0067bcc0(chunk->VTable0x28());
			subEntity->m_unk0x12c = mesh->m_unk0x38;
			if (mesh->m_unk0x38 == NULL && mesh->m_unk0x3c == 1) {
				subEntity->m_unk0x128 = false;
			}
			if (mesh->m_unk0x3c == 4) {
				subEntity->m_unk0x128 = false;
			}

			chunk->m_unk0x10 = subEntity;
			chunk->m_unk0x0c = m_unk0x04;

			const G3D::Vector3& translation = chunk->cframe().translation;
			Ogre::AxisAlignedBox bounds(m_unk0x04->m_unk0x248);
			if (fabs(translation.x) < 10000.0f && fabs(translation.y) < 10000.0f && fabs(translation.z) < 10000.0f) {
				if (subMesh == meshLevel->m_unk0x24) {
					const Ogre::AxisAlignedBox& meshBounds =
						static_cast<Ogre::Unknown00671c70*>(meshLevel->m_unk0x24)->m_unk0x8c;
					Ogre::Vector3 offset(translation.x, translation.y, translation.z);
					bounds.merge(
						Ogre::AxisAlignedBox(meshBounds.getMinimum() + offset, meshBounds.getMaximum() + offset)
					);
					m_unk0x04->FUN_0052c8b0(bounds);
				}
				else {
					const Ogre::Sphere& sphere = m_unk0x00->FUN_0067af60(subMesh);
					float radius = sphere.getRadius();
					const G3D::Vector3& position = chunk->cframe().translation;
					Ogre::AxisAlignedBox merged(m_unk0x04->m_unk0x248);
					Ogre::Vector3 center(
						position.x + sphere.getCenter().x,
						position.y + sphere.getCenter().y,
						position.z + sphere.getCenter().z
					);
					Ogre::Vector3 extent(radius, radius, radius);
					merged.merge(Ogre::AxisAlignedBox(center - extent, center + extent));
					m_unk0x04->FUN_0052c8b0(merged);
				}
			}
		}
		else if (rebuilt || !chunk->m_unk0x10->FUN_0067b4e0(lod)) {
			Ogre::SubMesh* subMesh = NULL;
			if (hardwareMeshes) {
				if (!meshLevel->m_unk0x1c && meshLevel->m_unk0x20 >= 0 &&
					Mesh::g_unk0x0095877c->getSubMesh(meshLevel->m_unk0x20)) {
					subMesh = Mesh::g_unk0x0095877c->getSubMesh(meshLevel->m_unk0x20);
				}
			}
			else {
				subMesh = meshLevel->m_unk0x24;
			}
			if (subMesh) {
				chunk->m_unk0x10->FUN_0067b4c0(lod, subMesh);
			}
		}

		chunk->m_unk0x10->FUN_0067b500(lod);

		Ogre::RbxSubEntity* subEntity = chunk->m_unk0x10;
		if (updateMaterial || subEntity->getMaterial() == m_unk0x08) {
			if (!materialLevel->m_unk0x28.isNull()) {
				subEntity->setMaterialName(materialLevel->m_unk0x28->getName());
			}
		}

		Ogre::Quaternion rotation(1.0f, 0.0f, 0.0f, 0.0f);
		rotation.FromRotationMatrix(reinterpret_cast<const Ogre::Matrix3&>(chunk->cframe().rotation));
		const G3D::Vector3& position = chunk->cframe().translation;
		Ogre::Matrix4 transform;
		transform.makeTransform(
			Ogre::Vector3(position.x, position.y, position.z), Ogre::Vector3(1.0f, 1.0f, 1.0f), rotation
		);
		subEntity->m_unk0xdc = transform;
	}

	if (hardwareMeshes && Mesh::varDirty) {
		Mesh::FUN_00532380(engine);
		if (m_unk0x04) {
			m_unk0x04->FUN_00674e40(Mesh::g_unk0x0095877c);
		}
	}

	timer.getMicroseconds();
	timer.reset();

	if (m_unk0x04) {
		m_unk0x04->FUN_00674e30(&engine->m_unk0x18);
		m_unk0x04->m_unk0x245 = false;
		m_unk0x04->setVisible(true);
		if (effectSettings.stencilShadows()) {
			m_unk0x04->m_unk0x268 = 15.0f;
			m_unk0x04->m_unk0x26c = 600;
		}
		else {
			m_unk0x04->m_unk0x268 = 20.0f;
			m_unk0x04->m_unk0x26c = 650;
		}
	}

	if (Ogre::g_unk0x0096134c->_fireFrameStarted()) {
		engine->m_unk0x0c->update();
		Ogre::g_unk0x0096134c->_fireFrameEnded();
		timer.getMicroseconds();
	}
}

} // namespace RenderNew
} // namespace RBX
