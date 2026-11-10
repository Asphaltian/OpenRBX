#include "appdraw/AdornRbxGfx.h"

#include "appdraw/RbxEntity.h"
#include "appdraw/RbxFont.h"
#include "appdraw/RbxSceneNode.h"
#include "appdraw/RbxSubEntity.h"
#include "appdraw/VisualEngine.h"
#include "renderlibnew/TextureProxy.h"
#include "util/ContentProvider.h"

#include <OgreLogManager.h>
#include <OgreMaterialManager.h>
#include <OgreMeshManager.h>
#include <OgreParticleEmitter.h>
#include <OgreParticleSystem.h>
#include <OgreParticleSystemManager.h>
#include <OgreSceneManager.h>
#include <OgreStringConverter.h>
#include <OgreTechnique.h>
#include <OgreTextureManager.h>
#include <OgreViewport.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

namespace RBX {

// GLOBAL: ROBLOX 0x00949c7c
Ogre::SharedPtr<Ogre::RbxFont> g_unk0x00949c7c;

// GLOBAL: ROBLOX 0x009612a8
int g_unk0x009612a8;

// GLOBAL: ROBLOX 0x009612c8
std::string g_unk0x009612c8 = "material explosion/explosionMatl\n"
							  "{\n"
							  "\ttechnique\n"
							  "\t{\n"
							  "\t\tpass\n"
							  "\t\t{\n"
							  "\t\t\tlighting off\n"
							  "\t\t\tdepth_write off\n"
							  "\t\t\tscene_blend add\n"
							  "\n"
							  "\t\t\ttexture_unit\n"
							  "\t\t\t{\n"
							  "\t\t\t\ttexture explosion.png\n"
							  "\t\t\t\tcolour_op modulate\n"
							  "\t\t\t}\n"
							  "\t\t}\n"
							  "\t}\n"
							  "}\n";

// GLOBAL: ROBLOX 0x009612e4
std::string g_unk0x009612e4 = "explosion/explosion1\n"
							  "{\n"
							  "\tquota\t400\n"
							  "\tmaterial\texplosion/explosionMatl\n"
							  "\tparticle_width\t5\n"
							  "\tparticle_height\t5\n"
							  "\tcull_each\tfalse\n"
							  "\trenderer\tbillboard\n"
							  "\tsorted\tfalse\n"
							  "\tlocal_space\tfalse\n"
							  "\titeration_interval\t0\n"
							  "\tnonvisible_update_timeout\t0\n"
							  "\tbillboard_type\tpoint\n"
							  "\tbillboard_origin\tcenter\n"
							  "\tbillboard_rotation_type\tvertex\n"
							  "\tcommon_up_vector\t0 1 0\n"
							  "\tpoint_rendering\tfalse\n"
							  "\taccurate_facing\tfalse\n"
							  "\n"
							  "\temitter Point\n"
							  "\t{\n"
							  "\t\tangle\t180\n"
							  "\t\tcolour\t0.3 0.3 0.3 .6\n"
							  "\t\tcolour_range_start\t0.25 0.25 0.25 1\n"
							  "\t\tcolour_range_end\t0.5 0.5 0.5 1\n"
							  "\t\tdirection\t0 -1 0\n"
							  "\t\temission_rate\t20000\n"
							  "\t\tposition\t0 0 0\n"
							  "\t\tvelocity\t30\n"
							  "\t\tvelocity_min\t20\n"
							  "\t\tvelocity_max\t30\n"
							  "\t\ttime_to_live\t0.3\n"
							  "\t\ttime_to_live_min\t0.25\n"
							  "\t\ttime_to_live_max\t0.35\n"
							  "\t\tduration\t1.2\n"
							  "\t\tduration_min\t1.2\n"
							  "\t\tduration_max\t1.2\n"
							  "\t\trepeat_delay\t10000\n"
							  "\t\trepeat_delay_min\t10000\n"
							  "\t\trepeat_delay_max\t10000\n"
							  "\t}\n"
							  "\n"
							  "\taffector DeflectorPlane\n"
							  "\t{\n"
							  "\t\tplane_point\t0 0 0\n"
							  "\t\tplane_normal\t0 1 0\n"
							  "\t\tbounce\t0\n"
							  "\t}\n"
							  "\n"
							  "\taffector DirectionRandomiser\n"
							  "\t{\n"
							  "\t\trandomness\t0\n"
							  "\t\tscope\t1\n"
							  "\t\tkeep_velocity\tfalse\n"
							  "\t}\n"
							  "}\n";

// GLOBAL: ROBLOX 0x00961334
int g_unk0x00961334;

inline static void setExplosionScale(Ogre::ParticleSystem* particleSystem, Ogre::ParticleEmitter* emitter, float scale)
{
	particleSystem->setParticleQuota((size_t) (atoi("400") * scale));
	emitter->setMinParticleVelocity(pow(scale, 1.0f / 3.0f) * atoi("20"));
	emitter->setMaxParticleVelocity(pow(scale, 1.0f / 3.0f) * atoi("30"));
	emitter->setMinDuration((scale - 1.0) * 0.4f + atoi("1.2"));
	emitter->setMaxDuration((scale - 1.0) * 0.4f + atoi("1.2"));
}

// FUNCTION: ROBLOX 0x0066a0d0
int AdornRbxGfx::getWidth() const
{
	return m_unk0x04->getViewport()->getActualWidth();
}

// FUNCTION: ROBLOX 0x0066a0e0
int AdornRbxGfx::getHeight() const
{
	return m_unk0x04->getViewport()->getActualHeight();
}

// FUNCTION: ROBLOX 0x0066a0f0
void AdornRbxGfx::rect2d(const G3D::Rect2D& rect, const G3D::Color4& color) const
{
	m_unk0x04->m_unk0x14->FUN_0067d9e0(
		m_unk0x08,
		color,
		rect.x0(),
		getHeight() - 1 - rect.y0(),
		rect.x1(),
		getHeight() - 1 - rect.y1(),
		0.0f,
		0.0f,
		1.0f,
		1.0f
	);
}

// FUNCTION: ROBLOX 0x0066a1b0
void AdornRbxGfx::outlineRect2d(const G3D::Rect2D& rect, float thickness, const G3D::Color4& color) const
{
	float y1 = rect.y1();
	float x1 = rect.x1();
	float y0 = rect.y0();
	float x0 = rect.x0();
	Ogre::TwoDManager* twoDManager = m_unk0x04->m_unk0x14;
	twoDManager->FUN_0067dcc0(2, x0, getHeight() - 1 - y0, x1, getHeight() - 1 - y1, color);
}

// FUNCTION: ROBLOX 0x0066a250
void AdornRbxGfx::line2d(const G3D::Vector2& from, const G3D::Vector2& to, const G3D::Color4& color) const
{
	Ogre::TwoDManager* twoDManager = m_unk0x04->m_unk0x14;
	twoDManager->FUN_0067dd70(from.x, getHeight() - 1 - from.y, to.x, getHeight() - 1 - to.y, color);
}

// STUB: ROBLOX 0x0066a580
G3D::Vector2 AdornRbxGfx::drawFont2D(
	const std::string& s,
	const G3D::Vector2& position,
	double size,
	const G3D::Color4& color,
	const G3D::Color4& outline,
	XAlign xAlign,
	YAlign yAlign,
	Spacing spacing
) const
{
	float x = position.x;
	float y = getHeight() - 1 - position.y;
	Ogre::Vector2 extent =
		g_unk0x00949c7c->FUN_0067e1e0(m_unk0x04, s, Ogre::Vector2(x, y), size, color, outline, xAlign, yAlign, spacing);
	return G3D::Vector2(extent.x, extent.y);
}

// FUNCTION: ROBLOX 0x0066a620
G3D::Vector2 AdornRbxGfx::get2DStringBounds(const std::string& s, double size, Spacing spacing) const
{
	Ogre::Vector2 bounds = g_unk0x00949c7c->FUN_0067e0f0(s, size, spacing);
	return G3D::Vector2(bounds.x, bounds.y);
}

// FUNCTION: ROBLOX 0x0066a660
void AdornRbxGfx::setObjectToWorldMatrix(const G3D::CoordinateFrame& coord)
{
	m_unk0x94 = coord;
}

// STUB: ROBLOX 0x0066a760
G3D::Rect2D AdornRbxGfx::getViewport() const
{
	Ogre::Viewport* viewport = m_unk0x04->getViewport();
	float left = viewport->getActualLeft();
	float top = viewport->getActualTop();
	float width = viewport->getActualWidth();
	float height = viewport->getActualHeight();
	return G3D::Rect2D::xywh(left, top, width, height);
}

// FUNCTION: ROBLOX 0x0066a810
void AdornRbxGfx::setTexture(int stage, G3D::ReferenceCountedPointer<TextureProxyBase> texture)
{
	if (texture.isNull()) {
		m_unk0x08 = "";
		return;
	}

	RenderNew::TextureProxy* proxy = static_cast<RenderNew::TextureProxy*>(texture.pointer());
	m_unk0x08 = proxy->filename;
	proxy->resolve(reinterpret_cast<G3D::RenderDevice*>(m_unk0x04));
}

// FUNCTION: ROBLOX 0x0066a8f0
G3D::Rect2D AdornRbxGfx::getTextureSize(G3D::ReferenceCountedPointer<TextureProxyBase> texture) const
{
	RenderNew::TextureProxy* proxy = static_cast<RenderNew::TextureProxy*>(texture.pointer());
	proxy->resolve(reinterpret_cast<G3D::RenderDevice*>(m_unk0x04));

	Ogre::TexturePtr ogreTexture = Ogre::TextureManager::getSingleton().getByName(proxy->filename);
	size_t width = ogreTexture->getWidth();
	size_t height = ogreTexture->getHeight();
	return G3D::Rect2D(G3D::Vector2((float) width, (float) height));
}

// FUNCTION: ROBLOX 0x0066aa60
G3D::ReferenceCountedPointer<TextureProxyBase> AdornRbxGfx::createTextureProxy(const std::string& name)
{
	return new RenderNew::TextureProxy(name);
}

// FUNCTION: ROBLOX 0x0066b510
void AdornRbxGfx::prepareRenderPass()
{
	int total = 0;
	std::map<std::string, Ogre::SceneNode*>::iterator it;
	for (it = m_unk0x88.begin(); it != m_unk0x88.end(); ++it) {
		Ogre::ParticleSystem* particleSystem =
			static_cast<Ogre::ParticleSystem*>(it->second->getAttachedObject((unsigned short) 0));
		total += particleSystem->getParticleQuota();
	}

	float quota = total;
	if (quota > 11000.0f) {
		float scale = 10000.0 / quota;
		for (it = m_unk0x88.begin(); it != m_unk0x88.end(); ++it) {
			Ogre::ParticleSystem* particleSystem =
				static_cast<Ogre::ParticleSystem*>(it->second->getAttachedObject((unsigned short) 0));
			Ogre::ParticleEmitter* emitter = particleSystem->getEmitter(0);
			int particleQuota = particleSystem->getParticleQuota();
			float minVelocity = emitter->getMinParticleVelocity();
			float maxVelocity = emitter->getMaxParticleVelocity();
			float duration = emitter->getDuration();
			particleSystem->setParticleQuota((size_t) (particleQuota * scale));
			emitter->setMinParticleVelocity(pow(scale, 1.0f / 3.0f) * minVelocity);
			emitter->setMaxParticleVelocity(pow(scale, 1.0f / 3.0f) * maxVelocity);
			emitter->setMinDuration((scale - 1.0) * 0.4f + duration);
			emitter->setMaxDuration((scale - 1.0) * 0.4f + duration);
		}
	}
}

// FUNCTION: ROBLOX 0x0066c0b0
Ogre::RbxSubEntity* AdornRbxGfx::FUN_0066c0b0()
{
	if (m_unk0x44.size() != 0) {
		Ogre::RbxSubEntity* subEntity = m_unk0x44[m_unk0x44.size() - 1];
		m_unk0x44.pop_back();
		m_unk0x54.push_back(subEntity);
		return subEntity;
	}

	Ogre::RbxSubEntity* subEntity = new Ogre::RbxSubEntity(m_unk0x70);
	m_unk0x54.push_back(subEntity);
	return subEntity;
}

// FUNCTION: ROBLOX 0x0066c190
void AdornRbxGfx::FUN_0066c190()
{
	for (int i = 0; i < (int) m_unk0x54.size(); i++) {
		m_unk0x44.push_back(m_unk0x54[i]);
	}
	m_unk0x54.clear();
}

// FUNCTION: ROBLOX 0x0066c2b0
void AdornRbxGfx::finishRenderPass()
{
	m_unk0x30->removeAndDestroyAllChildren();
	m_unk0x04->m_unk0x04->destroyAllManualObjects();
	g_unk0x009612a8 = 0;
	m_unk0x70->FUN_00675c90();
	FUN_0066c190();
}

// STUB: ROBLOX 0x0066c2f0
void AdornRbxGfx::box(const G3D::AABox& box, const G3D::Color4& solid, const G3D::Color4& wire)
{
	STUB(0x66c2f0);
}

// STUB: ROBLOX 0x0066ca20
void AdornRbxGfx::cylinderAlongX(float radius, float length, const G3D::Color4& solid, const G3D::Color4& wire)
{
	STUB(0x66ca20);
}

// STUB: ROBLOX 0x0066cf50
void AdornRbxGfx::sphere(const G3D::Sphere& sphere, const G3D::Color4& solid, const G3D::Color4& wire)
{
	STUB(0x66cf50);
}

// STUB: ROBLOX 0x0066d350
void AdornRbxGfx::quad(
	const G3D::Vector3& v0,
	const G3D::Vector3& v1,
	const G3D::Vector3& v2,
	const G3D::Vector3& v3,
	const G3D::Color4& color
)
{
	STUB(0x66d350);
}

// STUB: ROBLOX 0x0066da00
void AdornRbxGfx::ray(const G3D::Ray& ray, const G3D::Color4& color, float scale)
{
	STUB(0x66da00);
}

// STUB: ROBLOX 0x0066df30
AdornRbxGfx::~AdornRbxGfx()
{
	for (std::map<std::string, Ogre::MeshPtr>::const_iterator it = m_unk0x64.begin(), end = m_unk0x64.end(); it != end;
		 ++it) {
		const Ogre::MeshPtr& mesh = it->second;
		Ogre::MeshManager::getSingleton().remove(mesh->getHandle());
	}
	m_unk0x64.clear();

	m_unk0x04->m_unk0x04->getRootSceneNode()->removeAndDestroyChild(m_unk0x30->getName());
	m_unk0x04->m_unk0x04->getRootSceneNode()->removeChild(m_unk0x34);
	delete m_unk0x34;
	delete m_unk0x70;

	for (int i = 0; i < (int) m_unk0x78.size(); i++) {
		m_unk0x04->m_unk0x04->destroyParticleSystem(
			static_cast<Ogre::ParticleSystem*>(m_unk0x78[i]->getAttachedObject((unsigned short) 0))->getName()
		);
		m_unk0x04->m_unk0x04->destroySceneNode(m_unk0x78[i]->getName());
	}
	if (m_unk0x74) {
		m_unk0x04->m_unk0x04->destroySceneNode(m_unk0x74->getName());
	}
	m_unk0x78.clear();
	m_unk0x88.clear();
}

// STUB: ROBLOX 0x0066e3b0
AdornRbxGfx::AdornRbxGfx(Ogre::VisualEngine* visualEngine) : m_unk0x08(""), m_unk0x74(NULL)
{
	m_unk0x04 = visualEngine;
	if (g_unk0x00949c7c.isNull()) {
		try {
			g_unk0x00949c7c =
				Ogre::RbxFont::FUN_0067e9d0(ContentProvider::singleton().getAssetFile("Fonts\\comics.fnt"));
		}
		catch (...) {
		}
	}

	char name[8];
	_snprintf(name, sizeof(name), "a3dr%d", g_unk0x00961334++);
	m_unk0x30 = m_unk0x04->m_unk0x04->createSceneNode(name);
	m_unk0x04->m_unk0x04->getRootSceneNode()->addChild(m_unk0x30);

	m_unk0x34 = new Ogre::RbxSceneNode(visualEngine->m_unk0x04, std::string(name) + "opt");
	m_unk0x04->m_unk0x04->getRootSceneNode()->addChild(m_unk0x34);

	m_unk0x70 = new Ogre::RbxEntity();
	m_unk0x70->m_unk0x244 = false;
	m_unk0x70->m_unk0x246 = false;
	m_unk0x70->FUN_00674e30(&m_unk0x04->m_unk0x18);
	m_unk0x34->attachObject(m_unk0x70);

	Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().getByName("rbxdummymesh");
	if (mesh.isNull()) {
		mesh = Ogre::MeshManager::getSingleton().create("rbxdummymesh", "General");
	}
	m_unk0x70->FUN_00674e40(mesh);

	_snprintf(name, sizeof(name), "loff%d", g_unk0x00961334 - 1);
	m_unk0x38 = Ogre::MaterialManager::getSingleton().create(name, "General");
	m_unk0x38->setLightingEnabled(false);
}

// STUB: ROBLOX 0x0066e8e0
void AdornRbxGfx::explosion(const G3D::Sphere& sphere, const G3D::Color4& solid, const G3D::Color4& wire)
{
	Ogre::Vector3 position;
	{
		Ogre::Quaternion orientation;
		orientation.FromRotationMatrix(reinterpret_cast<const Ogre::Matrix3&>(m_unk0x94.rotation));
		position =
			m_unk0x30->createChildSceneNode(reinterpret_cast<const Ogre::Vector3&>(m_unk0x94.translation), orientation)
				->getPosition();
	}

	char key[64];
	_snprintf(key, sizeof(key), "%.3f %.3f %.3f", position.x, position.y, position.z);
	Ogre::LogManager::getSingleton().logMessage(
		std::string("RBX expl ") + " r=" + Ogre::StringConverter::toString(sphere.radius)
	);

	if (m_unk0x88.find(key) != m_unk0x88.end()) {
		Ogre::ParticleSystem* particleSystem =
			static_cast<Ogre::ParticleSystem*>(m_unk0x88[key]->getAttachedObject((unsigned short) 0));
		if (particleSystem->getNumParticles() == 0) {
			Ogre::ParticleEmitter* emitter = particleSystem->getEmitter(0);
			setExplosionScale(particleSystem, emitter, 1.0f);
			emitter->setEnabled(false);
			m_unk0x88.erase(m_unk0x88.find(key));
		}
		return;
	}

	std::vector<std::map<std::string, Ogre::SceneNode*>::iterator> retired;
	for (std::map<std::string, Ogre::SceneNode*>::iterator it = m_unk0x88.begin(); it != m_unk0x88.end(); ++it) {
		Ogre::ParticleSystem* particleSystem =
			static_cast<Ogre::ParticleSystem*>(it->second->getAttachedObject((unsigned short) 0));
		if (particleSystem->getNumParticles() == 0) {
			retired.push_back(it);
		}
	}

	for (unsigned int i = 0; i < retired.size(); i++) {
		Ogre::ParticleSystem* particleSystem =
			static_cast<Ogre::ParticleSystem*>(retired[i]->second->getAttachedObject((unsigned short) 0));
		Ogre::ParticleEmitter* emitter = particleSystem->getEmitter(0);
		emitter->setEnabled(false);
		m_unk0x88.erase(retired[i]);
		Ogre::LogManager::getSingleton().logMessage("RBX retired " + particleSystem->getName());
		setExplosionScale(particleSystem, emitter, 1.0f);
	}

	float scale = sphere.radius * 0.25;
	if (scale > 3.0f) {
		scale = 3.0f;
	}

	Ogre::SceneNode* node;
	unsigned int i;
	for (i = 0; i < m_unk0x78.size(); i++) {
		Ogre::ParticleSystem* particleSystem =
			static_cast<Ogre::ParticleSystem*>(m_unk0x78[i]->getAttachedObject((unsigned short) 0));
		if (!particleSystem->getEmitter(0)->getEnabled()) {
			Ogre::SceneNode* sceneNode = m_unk0x78[i];
			node = sceneNode;
			m_unk0x88[key] = sceneNode;
			sceneNode->setPosition(position.x, position.y, position.z);
			particleSystem = static_cast<Ogre::ParticleSystem*>(sceneNode->getAttachedObject((unsigned short) 0));
			Ogre::ParticleEmitter* emitter = particleSystem->getEmitter(0);
			setExplosionScale(particleSystem, emitter, scale);
			emitter->setEnabled(true);
			particleSystem->_update(0.001f);
			Ogre::LogManager::getSingleton().logMessage("RBX use " + particleSystem->getName());
			break;
		}
	}

	if (i == m_unk0x78.size()) {
		if (!Ogre::MaterialManager::getSingleton().resourceExists("explosion/explosionMatl")) {
			Ogre::DataStreamPtr materialStream(new Ogre::MemoryDataStream(
				const_cast<char*>(g_unk0x009612c8.c_str()), g_unk0x009612c8.size() + 1, false
			));
			Ogre::MaterialManager::getSingleton().parseScript(materialStream, "General");
			Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName("explosion/explosionMatl");
			material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(
				ContentProvider::singleton().getAssetFile("textures\\explosion.png"), Ogre::TEX_TYPE_2D
			);
			Ogre::DataStreamPtr particleStream(new Ogre::MemoryDataStream(
				const_cast<char*>(g_unk0x009612e4.c_str()), g_unk0x009612e4.size() + 1, false
			));
			Ogre::ParticleSystemManager::getSingleton().parseScript(particleStream, "General");
		}

		if (!m_unk0x74) {
			m_unk0x74 = m_unk0x04->m_unk0x04->getRootSceneNode()->createChildSceneNode(
				m_unk0x04->m_unk0x04->getName() + "rbxexpl", Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY
			);
		}

		Ogre::ParticleSystem* particleSystem = m_unk0x04->m_unk0x04->createParticleSystem(
			m_unk0x04->m_unk0x04->getName() + "expl" + Ogre::StringConverter::toString(i), "explosion/explosion1"
		);
		if (!particleSystem) {
			Ogre::LogManager::getSingleton().logMessage("RBX Error creating psys");
			return;
		}

		Ogre::ParticleEmitter* emitter = particleSystem->getEmitter(0);
		setExplosionScale(particleSystem, emitter, scale);
		Ogre::SceneNode* sceneNode = m_unk0x74->createChildSceneNode(Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY);
		node = sceneNode;
		sceneNode->attachObject(particleSystem);
		m_unk0x78.push_back(sceneNode);
		m_unk0x88[key] = sceneNode;
		Ogre::LogManager::getSingleton().logMessage("RBX created " + particleSystem->getName());
	}

	node->setPosition(position.x, position.y, position.z);
}

void AdornRbxGfx::lineSegment(const G3D::LineSegment& segment, const G3D::Color4& color, float scale)
{
}

void AdornRbxGfx::axes(const G3D::Color4& x, const G3D::Color4& y, const G3D::Color4& z, float scale)
{
}

} // namespace RBX
