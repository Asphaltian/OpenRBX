#include "renderlibnew/RenderScene.h"

#include "appdraw/RbxEntity.h"
#include "appdraw/VisualEngine.h"
#include "renderlibnew/Chunk.h"
#include "util/Utilities.h"

#include <G3D/Color4.h>
#include <G3D/g3dmath.h>
#include <OgreLight.h>
#include <OgreLogManager.h>
#include <OgreSceneManager.h>

namespace RBX {
namespace RenderNew {

// STUB: ROBLOX 0x0052c9b0
void RenderScene::FUN_0052c9b0(Ogre::VisualEngine* engine, bool p_unk0x8e)
{
	Ogre::SceneManager* sceneManager = engine->m_unk0x04;
	float radius = 1000.0f;
	Ogre::Vector3 center(0.0f, 0.0f, 0.0f);

	int i;
	for (i = 0; i < lighting->lightArray.size(); i++) {
		const G3D::GLight& source = lighting->lightArray[i];
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
		light->setCastShadows(m_unk0x8c);

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

	if (lighting->lightArray.size() == 1) {
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
	if (lighting->lightArray.size() > 0) {
		G3D::Vector3 direction = lighting->lightArray[0].position.xyz();
	}
}

// STUB: ROBLOX 0x0052d400
void RenderScene::setThrottle(float t, float m, bool s, float c)
{
	STUB(0x52d400);
}

// STUB: ROBLOX 0x0052d7e0
RenderScene::~RenderScene()
{
	STUB(0x52d7e0);
}

// FUNCTION: ROBLOX 0x0052dcf0
void SceneManager::addToScene(const G3D::ReferenceCountedPointer<Chunk>& chunk)
{
	renderScene->chunkArray.append(chunk);
}

// FUNCTION: ROBLOX 0x0052dd00
RenderScene::RenderScene()
	: m_unk0x04(NULL), shadingQuality(-1.0f), meshDetail(-1.0f), shadows(false), cameraDistance((float) G3D::inf()),
	  debugShadowVolumes(false), colorClearValue(0.5f, 0.5f, 1.0f, 1.0f)
{
	desiredLighting = lighting = G3D::Lighting::create();
	m_unk0x00 = NULL;
}

// STUB: ROBLOX 0x0052df80
void RenderScene::render(G3D::RenderDevice* rd, const G3D::GCamera& camera)
{
	STUB(0x52df80);
}

} // namespace RenderNew
} // namespace RBX
