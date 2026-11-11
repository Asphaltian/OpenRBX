#include "appdraw/AdornRbxGfx.h"

#include "appdraw/RbxEntity.h"
#include "appdraw/RbxFont.h"
#include "appdraw/RbxSceneNode.h"
#include "appdraw/RbxSubEntity.h"
#include "appdraw/Unknown0067eb00.h"
#include "appdraw/VisualEngine.h"
#include "renderlibnew/TextureProxy.h"
#include "util/ContentProvider.h"
#include "util/IndexBox.h"
#include "util/Utilities.h"

#include <OgreLogManager.h>
#include <OgreManualObject.h>
#include <OgreMaterialManager.h>
#include <OgreMeshManager.h>
#include <OgreParticleEmitter.h>
#include <OgreParticleSystem.h>
#include <OgreParticleSystemManager.h>
#include <OgreResourceGroupManager.h>
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

// GLOBAL: ROBLOX 0x009612ac
std::string g_unk0x009612ac = "q0adorn";

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

// GLOBAL: ROBLOX 0x00961318
std::string g_unk0x00961318 = "c01adorn";

// GLOBAL: ROBLOX 0x00961300
Ogre::MeshPtr g_unk0x00961300;

// GLOBAL: ROBLOX 0x0096130c
Ogre::MeshPtr g_unk0x0096130c;

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

// FUNCTION: ROBLOX 0x0066a2d0
void AdornRbxGfx::FUN_0066a2d0(
	Ogre::Matrix4& transform, const Ogre::Vector3& offset, const Ogre::Vector3& scale, const Ogre::Quaternion& rotation
)
{
	Ogre::Quaternion orientation;
	orientation.FromRotationMatrix(reinterpret_cast<const Ogre::Matrix3&>(m_unk0x94.rotation));
	orientation = orientation * rotation;
	transform.makeTransform(reinterpret_cast<const Ogre::Vector3&>(m_unk0x94.translation) + offset, scale, orientation);
}

// FUNCTION: ROBLOX 0x0066a360
void AdornRbxGfx::FUN_0066a360(Ogre::Matrix4& transform)
{
	Ogre::Quaternion orientation;
	orientation.FromRotationMatrix(reinterpret_cast<const Ogre::Matrix3&>(m_unk0x94.rotation));
	transform.makeTransform(
		reinterpret_cast<const Ogre::Vector3&>(m_unk0x94.translation), Ogre::Vector3(1.0f, 1.0f, 1.0f), orientation
	);
}

// FUNCTION: ROBLOX 0x0066a3c0
std::string AdornRbxGfx::FUN_0066a3c0()
{
	int id = 0;
	id = g_unk0x009612a8++;
	return StringConverter<int>::convertToString(id);
}

// FUNCTION: ROBLOX 0x0066a580
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
	Ogre::Vector2 pos(x, y);
	Ogre::Vector2 extent =
		g_unk0x00949c7c->FUN_0067e1e0(m_unk0x04, s, pos, size, color, outline, xAlign, yAlign, spacing);
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

// FUNCTION: ROBLOX 0x0066aae0
void AdornRbxGfx::FUN_0066aae0(const G3D::Color4& color, int p_unk0x08)
{
	char name[64];
	_snprintf(name, sizeof(name), "setColor%.3f%.3f%.3f%.3f%d", color.r, color.g, color.b, color.a, p_unk0x08);
	m_unk0x24 = Ogre::MaterialManager::getSingleton().getByName(name);
	if (m_unk0x24.isNull()) {
		m_unk0x24 = Ogre::MaterialManager::getSingleton().create(name, "adorn3d");
		Ogre::Pass* pass = m_unk0x24->getTechnique(0)->getPass(0);
		pass->setAmbient(color.r, color.g, color.b);
		pass->setDiffuse(color.r, color.g, color.b, color.a);
		pass->setSpecular(color.r, color.g, color.b, color.a);
		pass->setShininess(10.0f);
		m_unk0x24->setLightingEnabled(true);
	}
}

// FUNCTION: ROBLOX 0x0066b450
Ogre::MeshPtr AdornRbxGfx::FUN_0066b450(std::string name)
{
	std::map<std::string, Ogre::MeshPtr>::const_iterator it = m_unk0x64.find(name);
	if (it != m_unk0x64.end()) {
		return it->second;
	}

	return Ogre::MeshPtr();
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

// FUNCTION: ROBLOX 0x0066bf20
void AdornRbxGfx::FUN_0066bf20(const std::string& name, const Ogre::MeshPtr& mesh)
{
	m_unk0x64[name] = mesh;
	Ogre::Sphere sphere(mesh->getBounds().getCenter(), mesh->getBoundingSphereRadius());
	for (int i = 0; i < mesh->getNumSubMeshes(); i++) {
		m_unk0x34->FUN_0067b2b0(mesh->getSubMesh(i), sphere);
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

// FUNCTION: ROBLOX 0x0066c220
void AdornRbxGfx::FUN_0066c220(
	const Ogre::MeshPtr& mesh, const Ogre::Matrix4& transform, const Ogre::MaterialPtr& material
)
{
	for (int i = 0; i < mesh->getNumSubMeshes(); i++) {
		Ogre::RbxSubEntity* subEntity = FUN_0066c0b0();
		subEntity->FUN_0067b3d0(mesh->getSubMesh(i));
		subEntity->m_unk0xdc = transform;
		subEntity->FUN_0067b4b0(material);
		m_unk0x70->FUN_00676730(subEntity);
	}
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

// FUNCTION: ROBLOX 0x0066c2f0
void AdornRbxGfx::box(const G3D::AABox& box, const G3D::Color4& solid, const G3D::Color4& wire)
{
	IndexBox indexBox(G3D::Vector3(-1.0f, -1.0f, -1.0f), G3D::Vector3(1.0f, 1.0f, 1.0f));

	char key[64];
	_snprintf(key, sizeof(key), "c%d%d%d", (int) (solid.r * 255.0), (int) (solid.g * 255.0), (int) (solid.b * 255.0));
	Ogre::MeshPtr mesh = FUN_0066b450(key);
	if (mesh.isNull()) {
		Ogre::ManualObject* manual = m_unk0x04->getSceneManager()->createManualObject("box" + FUN_0066a3c0());
		manual->begin("", Ogre::RenderOperation::OT_TRIANGLE_LIST);
		manual->colour(Ogre::FUN_00671cd0(solid));

		int index = 0;
		for (int face = 0; face < 6; face++) {
			G3D::Vector3 v0;
			G3D::Vector3 v1;
			G3D::Vector3 v2;
			G3D::Vector3 v3;
			indexBox.getFaceCorners(face, v0, v1, v2, v3);
			manual->normal(Ogre::FUN_005e0b40(indexBox.getFaceNormal(face)));
			manual->position(Ogre::FUN_005e0b40(v0));
			manual->index(index++);
			manual->position(Ogre::FUN_005e0b40(v1));
			manual->index(index++);
			manual->position(Ogre::FUN_005e0b40(v2));
			manual->index(index++);
			manual->position(Ogre::FUN_005e0b40(v0));
			manual->index(index++);
			manual->position(Ogre::FUN_005e0b40(v2));
			manual->index(index++);
			manual->position(Ogre::FUN_005e0b40(v3));
			manual->index(index++);
		}

		manual->end();
		manual->setMaterialName(0, m_unk0x38->getName());
		mesh = manual->convertToMesh(
			m_unk0x04->m_unk0x04->getName() + "bmsh" + Ogre::StringConverter::toString(m_unk0x64.size()),
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
		);
		FUN_0066bf20(std::string(key), mesh);
		Ogre::Sphere bounds(mesh->getBounds().getCenter(), mesh->getBoundingSphereRadius());
		m_unk0x34->FUN_0067b2b0(mesh->getSubMesh(0), bounds);
	}

	Ogre::Matrix4 local;
	Ogre::Matrix4 transform;
	FUN_0066a2d0(transform, Ogre::Vector3(0.0f, 0.0f, 0.0f), Ogre::Vector3(1.0f, 1.0f, 1.0f), Ogre::Quaternion());
	local.makeTransform(
		Ogre::FUN_005e0b40(box.center()),
		Ogre::Vector3(
			(box.high().x - box.low().x) * 0.5, (box.high().y - box.low().y) * 0.5, (box.high().z - box.low().z) * 0.5
		),
		Ogre::Quaternion()
	);

	FUN_0066c220(mesh, transform * local, m_unk0x38);
}

// FUNCTION: ROBLOX 0x0066ca20
void AdornRbxGfx::cylinderAlongX(float radius, float length, const G3D::Color4& solid, const G3D::Color4& wire)
{
	Ogre::Sphere bounds(
		Ogre::Vector3(m_unk0x94.translation + G3D::Vector3(length * 0.5, 0.0f, 0.0f)),
		radius > length ? radius * 1.4f : length * 1.4f
	);
	if (!m_unk0x04->m_unk0x08->isVisible(bounds, NULL)) {
		return;
	}

	FUN_0066aae0(solid, 0);
	Ogre::Unknown0067eb00 quadric;
	if (g_unk0x00961300.isNull()) {
		Ogre::ManualObject* manual =
			quadric.FUN_0067ecc0(m_unk0x04->getSceneManager(), "cyl" + FUN_0066a3c0(), NULL, 1.0f, 1.0f, 1.0f, 12, 1);
		quadric.FUN_0067eae0(Ogre::Vector3(0.0f, 0.0f, 1.0f));
		quadric.FUN_00685580(m_unk0x04->getSceneManager(), "dsk" + FUN_0066a3c0(), manual, 0.0f, 1.0f, 12, 1);
		quadric.FUN_0067eae0(Ogre::Vector3(0.0f, 0.0f, 0.0f));
		quadric.m_unk0x08 = 1;
		quadric.FUN_00685580(m_unk0x04->getSceneManager(), "dsk" + FUN_0066a3c0(), manual, 0.0f, 1.0f, 12, 1);
		manual->setMaterialName(0, m_unk0x24->getName());
		manual->setMaterialName(1, m_unk0x24->getName());
		manual->setMaterialName(2, m_unk0x24->getName());
		g_unk0x00961300 = manual->convertToMesh(
			m_unk0x04->getSceneManager()->getName() + g_unk0x00961318,
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
		);
		Ogre::Sphere meshBounds(g_unk0x00961300->getBounds().getCenter(), g_unk0x00961300->getBoundingSphereRadius());
		for (int i = 0; i < g_unk0x00961300->getNumSubMeshes(); i++) {
			m_unk0x34->FUN_0067b2b0(g_unk0x00961300->getSubMesh(i), meshBounds);
		}
	}

	Ogre::Matrix4 world;
	FUN_0066a360(world);

	Ogre::Matrix4 local;
	local.makeTransform(
		Ogre::Vector3(-length * 0.5, 0.0f, 0.0f),
		Ogre::Vector3(radius, radius, length),
		Ogre::Quaternion(Ogre::Radian(Ogre::Degree(90.0f)), Ogre::Vector3(0.0f, 1.0f, 0.0f))
	);

	FUN_0066c220(g_unk0x00961300, world * local, m_unk0x24);
}

// FUNCTION: ROBLOX 0x0066cf50
void AdornRbxGfx::sphere(const G3D::Sphere& sphere, const G3D::Color4& solid, const G3D::Color4& wire)
{
	Ogre::Unknown0067eb00 quadric;
	FUN_0066aae0(G3D::Color4(0.8f * solid.r, 0.8f * solid.g, 0.8f * solid.b, 0.8f * solid.a), 0);

	std::string key("sph0001");
	Ogre::MeshPtr mesh = FUN_0066b450(key);
	if (mesh.isNull()) {
		Ogre::ManualObject* manual =
			quadric.FUN_00682ad0(m_unk0x04->getSceneManager(), "sph" + FUN_0066a3c0(), NULL, 1.0f, 17, 17);
		for (int i = 0; i < (int) manual->getNumSections(); i++) {
			manual->setMaterialName(i, m_unk0x24->getName());
		}
		mesh = manual->convertToMesh(
			m_unk0x04->m_unk0x04->getName() + "smsh" + Ogre::StringConverter::toString(m_unk0x64.size()),
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
		);
		m_unk0x64[key] = mesh;
		Ogre::Sphere bounds(mesh->getBounds().getCenter(), mesh->getBoundingSphereRadius());
		for (int i = 0; i < mesh->getNumSubMeshes(); i++) {
			m_unk0x34->FUN_0067b2b0(mesh->getSubMesh(i), bounds);
		}
	}

	Ogre::Matrix4 transform;
	FUN_0066a2d0(
		transform,
		Ogre::Vector3(sphere.center.x, sphere.center.y, sphere.center.z),
		Ogre::Vector3(sphere.radius, sphere.radius, sphere.radius),
		Ogre::Quaternion()
	);
	FUN_0066c220(mesh, transform, m_unk0x24);
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
	float height = v0.y - v2.y;
	float depth = v0.z - v2.z;
	if (height < 0.0f) {
		height = -height;
	}
	if (depth < 0.0f) {
		depth = -depth;
	}
	float size = depth < height ? height : depth;

	G3D::Vector3 center(v0.x, (v0.y + v1.y + v2.y + v3.y) * 0.25, (v0.z + v1.z + v2.z + v3.z) * 0.25);
	Ogre::Sphere bounds(Ogre::Vector3(m_unk0x94.translation + center), size * 1.4f);
	Ogre::Vector3 offset =
		reinterpret_cast<const Ogre::Vector3&>(m_unk0x94.translation) - m_unk0x04->m_unk0x08->getDerivedPosition();
	if (!m_unk0x04->m_unk0x08->isVisible(bounds, NULL)) {
		return;
	}

	if (offset.squaredLength() > size * size * 60.0 * 60.0) {
		return;
	}

	FUN_0066aae0(color, 0);
	if (v0.x != v1.x || v1.x != v2.x || v2.x != v3.x) {
		throw std::runtime_error("Bad: quad not handled");
	}

	if (g_unk0x0096130c.isNull()) {
		Ogre::ManualObject* manual = m_unk0x04->getSceneManager()->createManualObject("ln" + FUN_0066a3c0());
		manual->begin("", Ogre::RenderOperation::OT_TRIANGLE_LIST);
		manual->position(0.0f, 0.5f, 0.5f);
		manual->normal(Ogre::Vector3(1.0f, 0.0f, 0.0f));
		manual->index(0);
		manual->position(0.0f, -0.5f, 0.5f);
		manual->normal(Ogre::Vector3(1.0f, 0.0f, 0.0f));
		manual->index(1);
		manual->position(0.0f, -0.5f, -0.5f);
		manual->normal(Ogre::Vector3(1.0f, 0.0f, 0.0f));
		manual->index(2);
		manual->position(0.0f, 0.5f, 0.5f);
		manual->normal(Ogre::Vector3(1.0f, 0.0f, 0.0f));
		manual->index(3);
		manual->position(0.0f, -0.5f, -0.5f);
		manual->normal(Ogre::Vector3(1.0f, 0.0f, 0.0f));
		manual->index(4);
		manual->position(0.0f, 0.5f, -0.5f);
		manual->normal(Ogre::Vector3(1.0f, 0.0f, 0.0f));
		manual->index(5);
		manual->end();
		g_unk0x0096130c = manual->convertToMesh(
			m_unk0x04->getSceneManager()->getName() + g_unk0x009612ac,
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
		);
		Ogre::Sphere meshBounds(g_unk0x0096130c->getBounds().getCenter(), g_unk0x0096130c->getBoundingSphereRadius());
		m_unk0x34->FUN_0067b2b0(g_unk0x0096130c->getSubMesh(0), meshBounds);
	}

	Ogre::Matrix4 transform;
	FUN_0066a360(transform);
	Ogre::Matrix4 local;
	local.makeTransform(
		reinterpret_cast<const Ogre::Vector3&>(center),
		Ogre::Vector3(1.0f, v2.y - v0.y, v2.z - v0.z),
		Ogre::Quaternion()
	);
	transform = transform * local;
	FUN_0066c220(g_unk0x0096130c, transform, m_unk0x24);
}

// FUNCTION: ROBLOX 0x0066da00
void AdornRbxGfx::ray(const G3D::Ray& ray, const G3D::Color4& color, float scale)
{
	FUN_0066aae0(color, 0);
	Ogre::Unknown0067eb00 quadric;

	std::string key("ray0001");
	Ogre::MeshPtr mesh = FUN_0066b450(key);
	if (mesh.isNull()) {
		Ogre::Unknown0067eb00 quadric;
		Ogre::ManualObject* manual =
			quadric.FUN_0067ecc0(m_unk0x04->getSceneManager(), FUN_0066a3c0(), NULL, 0.05f, 0.05f, 1.0f, 12, 1);
		quadric.FUN_0067eae0(Ogre::Vector3(0.0f, 0.0f, 1.0f));
		quadric.FUN_0067ecc0(m_unk0x04->getSceneManager(), FUN_0066a3c0(), manual, 0.15f, 0.0f, 0.3f, 12, 1);
		quadric.m_unk0x08 = 1;
		quadric.FUN_00685580(m_unk0x04->getSceneManager(), FUN_0066a3c0(), manual, 0.0f, 0.15f, 12, 1);
		quadric.FUN_0067eae0(Ogre::Vector3(0.0f, 0.0f, 0.0f));
		quadric.FUN_00685580(m_unk0x04->getSceneManager(), FUN_0066a3c0(), manual, 0.0f, 0.05f, 12, 1);
		for (int i = 0; i < 4; i++) {
			manual->setMaterialName(i, m_unk0x24->getName());
		}
		mesh = manual->convertToMesh(
			m_unk0x04->m_unk0x04->getName() + "amsh" + Ogre::StringConverter::toString(m_unk0x64.size()),
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
		);
		m_unk0x64[key] = mesh;
		Ogre::Sphere bounds(mesh->getBounds().getCenter(), mesh->getBoundingSphereRadius());
		for (int i = 0; i < mesh->getNumSubMeshes(); i++) {
			m_unk0x34->FUN_0067b2b0(mesh->getSubMesh(i), bounds);
		}
	}

	float length = ray.direction.magnitude() / 1.3f;
	Ogre::Quaternion rotation =
		Ogre::Vector3(0.0f, 0.0f, 1.0f).getRotationTo(Ogre::Vector3(ray.direction.x, ray.direction.y, ray.direction.z));
	Ogre::Matrix4 transform;
	FUN_0066a2d0(
		transform,
		Ogre::Vector3(ray.origin.x, ray.origin.y, ray.origin.z),
		Ogre::Vector3(length * 0.5, length * 0.5, length),
		rotation
	);
	FUN_0066c220(mesh, transform, m_unk0x24);
}

// FUNCTION: ROBLOX 0x0066df30
AdornRbxGfx::~AdornRbxGfx()
{
	for (std::map<std::string, Ogre::MeshPtr>::const_iterator it = m_unk0x64.begin(); it != m_unk0x64.end(); ++it) {
		const Ogre::MeshPtr& mesh = it->second;
		Ogre::MeshManager::getSingleton().remove(mesh->getHandle());
	}
	m_unk0x64.clear();

	m_unk0x04->getSceneManager()->getRootSceneNode()->removeAndDestroyChild(m_unk0x30->getName());
	m_unk0x04->getSceneManager()->getRootSceneNode()->removeChild(m_unk0x34);
	delete m_unk0x34;
	delete m_unk0x70;

	for (int i = 0; i < (int) m_unk0x78.size(); i++) {
		m_unk0x04->getSceneManager()->destroyParticleSystem(
			static_cast<Ogre::ParticleSystem*>(m_unk0x78[i]->getAttachedObject((unsigned short) 0))->getName()
		);
		m_unk0x04->getSceneManager()->destroySceneNode(m_unk0x78[i]->getName());
	}
	if (m_unk0x74) {
		m_unk0x04->getSceneManager()->destroySceneNode(m_unk0x74->getName());
	}
	m_unk0x78.clear();
	m_unk0x88.clear();
}

// FUNCTION: ROBLOX 0x0066e3b0
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
	m_unk0x30 = m_unk0x04->getSceneManager()->createSceneNode(name);
	m_unk0x04->getSceneManager()->getRootSceneNode()->addChild(m_unk0x30);

	m_unk0x34 = new Ogre::RbxSceneNode(visualEngine->getSceneManager(), std::string(name) + "opt");
	m_unk0x04->getSceneManager()->getRootSceneNode()->addChild(m_unk0x34);

	m_unk0x70 = new Ogre::RbxEntity();
	m_unk0x70->m_unk0x244 = false;
	m_unk0x70->m_unk0x246 = false;
	Ogre::VisualEngine* engine = m_unk0x04;
	m_unk0x70->FUN_00674e30(&engine->getUnk0x18());
	m_unk0x34->attachObject(m_unk0x70);

	Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().getByName("rbxdummymesh");
	if (mesh.isNull()) {
		mesh = Ogre::MeshManager::getSingleton().createOrRetrieve("rbxdummymesh", "General").first;
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
			m_unk0x74 = m_unk0x04->getSceneManager()->getRootSceneNode()->createChildSceneNode(
				m_unk0x04->m_unk0x04->getName() + "rbxexpl", Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY
			);
		}

		Ogre::ParticleSystem* particleSystem = m_unk0x04->getSceneManager()->createParticleSystem(
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
