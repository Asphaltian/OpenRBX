#include "appdraw/AdornRbxGfx.h"

#include "appdraw/RbxEntity.h"
#include "appdraw/RbxFont.h"
#include "appdraw/RbxSceneNode.h"
#include "appdraw/VisualEngine.h"
#include "renderlibnew/TextureProxy.h"
#include "util/ContentProvider.h"

#include <OgreMaterialManager.h>
#include <OgreMeshManager.h>
#include <OgreSceneManager.h>
#include <OgreTextureManager.h>
#include <OgreViewport.h>
#include <stdio.h>

namespace RBX {

// GLOBAL: ROBLOX 0x00949c7c
Ogre::SharedPtr<Ogre::RbxFont> g_unk0x00949c7c;

// GLOBAL: ROBLOX 0x009612a8
int g_unk0x009612a8;

// GLOBAL: ROBLOX 0x00961334
int g_unk0x00961334;

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

// STUB: ROBLOX 0x0066a8f0
G3D::Rect2D AdornRbxGfx::getTextureSize(G3D::ReferenceCountedPointer<TextureProxyBase> texture) const
{
	RenderNew::TextureProxy* proxy = static_cast<RenderNew::TextureProxy*>(texture.pointer());
	proxy->resolve(reinterpret_cast<G3D::RenderDevice*>(m_unk0x04));

	Ogre::TexturePtr ogreTexture = Ogre::TextureManager::getSingleton().getByName(proxy->filename);
	float width = ogreTexture->getWidth();
	float height = ogreTexture->getHeight();
	return G3D::Rect2D::xywh(0.0f, 0.0f, width, height);
}

// FUNCTION: ROBLOX 0x0066aa60
G3D::ReferenceCountedPointer<TextureProxyBase> AdornRbxGfx::createTextureProxy(const std::string& name)
{
	return new RenderNew::TextureProxy(name);
}

// STUB: ROBLOX 0x0066b510
void AdornRbxGfx::prepareRenderPass()
{
	STUB(0x66b510);
}

// STUB: ROBLOX 0x0066c190
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
	for (std::map<std::string, Ogre::MeshPtr>::const_iterator it = m_unk0x64.begin(); it != m_unk0x64.end(); ++it) {
		Ogre::MeshManager::getSingleton().remove(it->second->getHandle());
	}
	m_unk0x64.clear();

	m_unk0x04->m_unk0x04->getRootSceneNode()->removeAndDestroyChild(m_unk0x30->getName());
	m_unk0x04->m_unk0x04->getRootSceneNode()->removeChild(m_unk0x34);
	delete m_unk0x34;
	delete m_unk0x70;

	for (int i = 0; i < (int) m_unk0x78.size(); i++) {
		Ogre::Entity* entity = static_cast<Ogre::Entity*>(m_unk0x78[i]->detachObject((unsigned short) 0));
		m_unk0x04->m_unk0x04->destroyEntity(entity->getName());
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
	STUB(0x66e8e0);
}

void AdornRbxGfx::lineSegment(const G3D::LineSegment& segment, const G3D::Color4& color, float scale)
{
}

void AdornRbxGfx::axes(const G3D::Color4& x, const G3D::Color4& y, const G3D::Color4& z, float scale)
{
}

} // namespace RBX
