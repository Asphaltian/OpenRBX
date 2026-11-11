#include "appdraw/RbxSky.h"

#include "appdraw/VisualEngine.h"

#include <G3D/BinaryInput.h>
#include <G3D/fileutils.h>
#include <G3D/g3dmath.h>
#include <GLG3D/LightingParameters.h>
#include <OgreBillboard.h>
#include <OgreBillboardSet.h>
#include <OgreCamera.h>
#include <OgreManualObject.h>
#include <OgreMaterialManager.h>
#include <OgreMeshManager.h>
#include <OgrePass.h>
#include <OgreRenderQueue.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreStringConverter.h>
#include <OgreSubMesh.h>
#include <OgreTechnique.h>
#include <OgreTextureManager.h>
#include <OgreTextureUnitState.h>
#include <OgreViewport.h>
#include <algorithm>

namespace Ogre {

// FUNCTION: ROBLOX 0x00677eb0
void RbxSky::FUN_00677eb0(VisualEngine* engine, const G3D::LightingParameters& parameters)
{
	if (parameters.moonPosition.y > -0.3f) {
		m_unk0x85 = true;

		MeshPtr mesh = MeshManager::getSingleton().getByName("starsMesh");
		if (!mesh.isNull()) {
			if (0.3f < parameters.skyAmbient.r && parameters.skyAmbient.r < 0.7f) {
				for (int i = 0; i < 3; i++) {
					MaterialPtr material =
						MaterialManager::getSingleton().getByName(mesh->getSubMesh(i)->getMaterialName());
					material->setSceneBlending(SBF_ONE_MINUS_DEST_COLOUR, SBF_ZERO);
				}
			}
			else {
				for (int i = 0; i < 3; i++) {
					MaterialPtr material =
						MaterialManager::getSingleton().getByName(mesh->getSubMesh(i)->getMaterialName());
					material->setSceneBlending(SBT_REPLACE);
				}
			}
		}
	}
	else {
		m_unk0x85 = false;
	}

	G3D::Vector3 position = parameters.physicallyCorrect ? parameters.trueMoonPosition : parameters.moonPosition;
	float brightness = std::min(1.0f - parameters.skyAmbient.g + 0.1f, 1.0f);
	position *= 2700.0;
	m_unk0x44->setPosition(position.x, position.y, position.z);
	m_unk0x38->getBillboard(0)->setColour(ColourValue(brightness, brightness, brightness, 1.0f));
	m_unk0xb8 = Vector3(position.x, position.y, position.z);
}

// STUB: ROBLOX 0x00678210
void RbxSky::FUN_00678210(VisualEngine* engine, const G3D::LightingParameters& parameters)
{
	G3D::Vector3 position = parameters.physicallyCorrect ? parameters.trueSunPosition : parameters.sunPosition;
	G3D::Color3 colour = parameters.emissiveScale * 0.8;

	if (position.y < 0.0f) {
		float fade = std::max(0.0f, (float) ((0.1f + position.y) * 10.0));
		colour *= fade;
	}

	position *= 2700.0;
	m_unk0x40->setPosition(position.x, position.y, position.z);
	m_unk0x34->getBillboard(0)->setColour(ColourValue(colour.r, colour.g, colour.b, 1.0f));
	m_unk0x34->getBillboard(1)->setColour(ColourValue(colour.r, colour.g, colour.b, 1.0f));
	m_unk0x34->getBillboard(2)->setColour(ColourValue(colour.r, colour.g, colour.b, 1.0f));
	m_unk0xac = Vector3(position.x, position.y, position.z);

	if (position.x > 0.0f && position.y >= -0.1) {
		float rays = std::max(0.0f, std::min((float) (1.0f - position.y * 2.0 / sqrt(2.0)), 1.0f));
		float alpha = rays * 0.256f;
		m_unk0x3c->getBillboard(0)->setColour(ColourValue(alpha, alpha, alpha, alpha));
		m_unk0x84 = true;
	}
	else {
		m_unk0x84 = false;
	}
}

// FUNCTION: ROBLOX 0x00678540
void RbxSky::FUN_00678540(VisualEngine* engine, const G3D::LightingParameters& parameters)
{
	engine->getViewport()->setBackgroundColour(
		ColourValue(parameters.skyAmbient.r, parameters.skyAmbient.g, parameters.skyAmbient.b, 1.0f)
	);
	if (m_unk0xa8) {
		FUN_00677eb0(engine, parameters);
		FUN_00678210(engine, parameters);
	}
}

// FUNCTION: ROBLOX 0x00678610
void RbxSky::FUN_00678610(Camera* cam, RenderQueue* queue, uint8 queueGroupId)
{
	Vector3 position = cam->getDerivedPosition();
	m_unk0x44->setPosition(m_unk0xb8 + position);
	m_unk0x40->setPosition(m_unk0xac + position);
	m_unk0x48->setPosition(position);

	queue->getQueueGroup(RENDER_QUEUE_1)->setShadowsEnabled(false);
	queue->getQueueGroup(RENDER_QUEUE_2)->setShadowsEnabled(false);

	m_unk0x38->setRenderQueueGroup(RENDER_QUEUE_1);
	m_unk0x38->_notifyCurrentCamera(cam);
	m_unk0x38->_updateRenderQueue(queue);

	m_unk0x34->setRenderQueueGroup(RENDER_QUEUE_2);
	m_unk0x34->_notifyCurrentCamera(cam);
	m_unk0x34->_updateRenderQueue(queue);

	if (m_unk0x84) {
		m_unk0x3c->setRenderQueueGroup(RENDER_QUEUE_2);
		m_unk0x3c->_notifyCurrentCamera(cam);
		m_unk0x3c->_updateRenderQueue(queue);
	}

	if (m_unk0x85) {
		SceneNode::ObjectIterator it = m_unk0x48->getAttachedObjectIterator();
		while (it.hasMoreElements()) {
			ManualObject* stars = dynamic_cast<ManualObject*>(it.getNext());
			stars->setRenderQueueGroup(RENDER_QUEUE_1);
			stars->_notifyCurrentCamera(cam);
			stars->_updateRenderQueue(queue);
		}
	}
}

// FUNCTION: ROBLOX 0x00678b20
RbxSky::~RbxSky()
{
}

// STUB: ROBLOX 0x00678d30
RbxSky::RbxSky(SceneManager* sceneManager, const String& folder, bool p_unk0x0c) : m_unk0x84(false)
{
	m_unk0xa8 = p_unk0x0c;
	if (!m_unk0xa8) {
		return;
	}

	const String& name = sceneManager->getName();

	m_unk0x10 = TextureManager::getSingleton().load(folder + "moon.jpg", "General");
	m_unk0x04 = TextureManager::getSingleton().load(folder + "sun.jpg", "General");
	m_unk0x28 = TextureManager::getSingleton().load(folder + "sun-rays.jpg", "General");

	m_unk0x54 = MaterialManager::getSingleton().create(name + "sunMatl", "General");
	m_unk0x54->setSceneBlending(SBF_ONE, SBF_ONE);
	TextureUnitState* sunUnit = m_unk0x54->getTechnique(0)->getPass(0)->createTextureUnitState();
	sunUnit->setColourOperationEx(LBX_MODULATE, LBS_TEXTURE, LBS_CURRENT);
	sunUnit->setAlphaOperation(LBX_MODULATE, LBS_TEXTURE, LBS_CURRENT);
	sunUnit->setTextureName(m_unk0x04->getName());
	m_unk0x54->setDepthWriteEnabled(false);
	m_unk0x54->setLightingEnabled(false);

	m_unk0x60 = m_unk0x54->clone(name + "sunRaysMatl");
	m_unk0x60->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(m_unk0x28->getName());

	m_unk0x6c = MaterialManager::getSingleton().create(name + "moonMatl", "General");
	m_unk0x6c->setSceneBlending(SBF_SOURCE_ALPHA, SBF_ONE);
	TextureUnitState* moonUnit = m_unk0x6c->getTechnique(0)->getPass(0)->createTextureUnitState();
	moonUnit->setColourOperationEx(LBX_MODULATE, LBS_TEXTURE, LBS_CURRENT);
	moonUnit->setAlphaOperation(LBX_MODULATE, LBS_TEXTURE, LBS_CURRENT);
	moonUnit->setTextureName(m_unk0x10->getName());
	moonUnit->setTextureName(m_unk0x10->getName());
	m_unk0x6c->setDepthWriteEnabled(false);
	m_unk0x6c->setLightingEnabled(false);

	m_unk0x34 = sceneManager->createBillboardSet(name + "sun", 1);
	m_unk0x34->setDefaultDimensions(1000.0f, 1000.0f);
	m_unk0x34->createBillboard(Vector3::ZERO, ColourValue::White);
	m_unk0x34->createBillboard(Vector3::ZERO, ColourValue::White);
	m_unk0x34->createBillboard(Vector3::ZERO, ColourValue::White);
	m_unk0x34->setMaterialName(name + "sunMatl");

	m_unk0x3c = sceneManager->createBillboardSet(name + "sunRays", 1);
	m_unk0x3c->setDefaultDimensions(1000.0f, 1000.0f);
	m_unk0x3c->createBillboard(Vector3::ZERO, ColourValue::White);
	m_unk0x3c->setMaterialName(name + "sunRaysMatl");

	m_unk0x38 = sceneManager->createBillboardSet(name + "moon", 1);
	m_unk0x38->setDefaultDimensions(500.0f, 500.0f);
	m_unk0x38->createBillboard(Vector3::ZERO, ColourValue::White);
	m_unk0x38->setMaterialName(name + "moonMatl");

	m_unk0x4c = sceneManager->createSceneNode(name + "skyNode");
	m_unk0x40 = m_unk0x4c->createChildSceneNode(name + "sunNode");
	m_unk0x44 = m_unk0x4c->createChildSceneNode(name + "moonNode");
	m_unk0x40->attachObject(m_unk0x34);
	m_unk0x40->attachObject(m_unk0x3c);
	m_unk0x44->attachObject(m_unk0x38);
	m_unk0x34->setCastShadows(false);
	m_unk0x3c->setCastShadows(false);
	m_unk0x38->setCastShadows(false);

	if (G3D::fileExists(folder + "real.str")) {
		G3D::BinaryInput in(folder + "real.str", G3D::G3D_LITTLE_ENDIAN, true);
		in.readString(5);

		int count = in.readInt16();
		m_unk0x88.resize(count);
		m_unk0x98.resize(count, 0.0f);

		for (int i = 0; i < count; i++) {
			float x = (in.readInt16() * 2.0 + 1.0) * (1.0 / 65535.0);
			float y = (in.readInt16() * 2.0 + 1.0) * (1.0 / 65535.0);
			float z = (in.readInt16() * 2.0 + 1.0) * (1.0 / 65535.0);
			m_unk0x88[i] = Vector3(x * 1000.0, y * 1000.0, z * 1000.0);
			m_unk0x98[i] = G3D::square((in.readInt16() * 2.0 + 1.0) * (1.0 / 65535.0)) + 0.3f;
		}
	}
	else {
		m_unk0x88.resize(3000);
		m_unk0x98.resize(m_unk0x88.size(), 0.0f);

		for (int i = m_unk0x88.size() - 1; i >= 0; --i) {
			G3D::Vector3 direction = G3D::Vector3::random();
			m_unk0x88[i] = Vector3(direction.x * 1000.0, direction.y * 1000.0, direction.z * 1000.0);
			m_unk0x98[i] = G3D::square(G3D::uniformRandom()) + 0.3f;
		}
	}

	m_unk0x48 = sceneManager->createSceneNode(name + "starsNode");

	for (int i = 1; i <= 3; i++) {
		m_unk0x78 =
			MaterialManager::getSingleton().create(name + "starsMatl" + StringConverter::toString(i), "General");
		m_unk0x78->setDepthWriteEnabled(false);
		m_unk0x78->setLightingEnabled(false);

		Real brightness = i;
		m_unk0x78->setPointSize(brightness + 0.2f);

		ManualObject* stars = new ManualObject(name + "st" + StringConverter::toString(i));
		stars->begin(m_unk0x78->getName(), RenderOperation::OT_POINT_LIST);
		for (int j = 0; j < (int) m_unk0x88.size(); j++) {
			if (m_unk0x98[j] * 3.0 > i - 1 && m_unk0x98[j] * 3.0 <= brightness) {
				stars->position(m_unk0x88[j].x, m_unk0x88[j].y, m_unk0x88[j].z);
			}
		}
		stars->end();
		stars->setMaterialName(0, m_unk0x78->getName());
		m_unk0x48->attachObject(stars);
		stars->setCastShadows(false);
	}
}

} // namespace Ogre
