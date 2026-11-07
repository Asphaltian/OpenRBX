#ifndef APPDRAW_ADORNRBXGFX_H
#define APPDRAW_ADORNRBXGFX_H

#include "decomp.h"
#include "rbxgraphics/Adorn.h"

#include <G3D/CoordinateFrame.h>
#include <OgreMaterial.h>
#include <OgreMesh.h>
#include <map>
#include <string>
#include <vector>

namespace Ogre {
class RbxEntity;
class RbxSceneNode;
class SceneNode;
class VisualEngine;
} // namespace Ogre

namespace RBX {

// VTABLE: ROBLOX 0x0083cfd4
// SIZE 0xc4
class AdornRbxGfx : public Adorn
{
private:
	Ogre::VisualEngine* m_unk0x04;                  // 0x04
	std::string m_unk0x08;                          // 0x08
	Ogre::MaterialPtr m_unk0x24;                    // 0x24
	Ogre::SceneNode* m_unk0x30;                     // 0x30
	Ogre::RbxSceneNode* m_unk0x34;                  // 0x34
	Ogre::MaterialPtr m_unk0x38;                    // 0x38
	std::vector<undefined4> m_unk0x44;              // 0x44
	std::vector<undefined4> m_unk0x54;              // 0x54
	std::map<std::string, Ogre::MeshPtr> m_unk0x64; // 0x64
	Ogre::RbxEntity* m_unk0x70;                     // 0x70
	Ogre::SceneNode* m_unk0x74;                     // 0x74
	std::vector<Ogre::SceneNode*> m_unk0x78;        // 0x78
	std::map<std::string, undefined4> m_unk0x88;    // 0x88
	G3D::CoordinateFrame m_unk0x94;                 // 0x94

public:
	AdornRbxGfx(Ogre::VisualEngine* visualEngine);
	virtual ~AdornRbxGfx(); // vtable+0x00

	// SYNTHETIC: ROBLOX 0x0066e390
	// RBX::AdornRbxGfx::`scalar deleting destructor'

	virtual G3D::ReferenceCountedPointer<TextureProxyBase> createTextureProxy(const std::string& name);
	virtual void prepareRenderPass();
	virtual void finishRenderPass();
	virtual G3D::Rect2D getViewport() const;
	virtual int getWidth() const;
	virtual int getHeight() const;
	virtual void setTexture(int stage, G3D::ReferenceCountedPointer<TextureProxyBase> texture);
	virtual G3D::Rect2D getTextureSize(G3D::ReferenceCountedPointer<TextureProxyBase> texture) const;
	virtual void line2d(const G3D::Vector2& from, const G3D::Vector2& to, const G3D::Color4& color) const;
	virtual void outlineRect2d(const G3D::Rect2D& rect, float thickness, const G3D::Color4& color) const;
	virtual void rect2d(const G3D::Rect2D& rect, const G3D::Color4& color) const;
	virtual G3D::Vector2 get2DStringBounds(const std::string& s, double size, Spacing spacing) const;
	virtual G3D::Vector2 drawFont2D(
		const std::string& s,
		const G3D::Vector2& position,
		double size,
		const G3D::Color4& color,
		const G3D::Color4& outline,
		XAlign xAlign,
		YAlign yAlign,
		Spacing spacing
	) const;
	virtual void setObjectToWorldMatrix(const G3D::CoordinateFrame& coord);
	virtual void setColor(const G3D::Color4& color) {}
	virtual void box(const G3D::AABox& box, const G3D::Color4& solid, const G3D::Color4& wire);
	virtual void sphere(const G3D::Sphere& sphere, const G3D::Color4& solid, const G3D::Color4& wire);
	virtual void explosion(const G3D::Sphere& sphere, const G3D::Color4& solid, const G3D::Color4& wire);
	virtual void cylinderAlongX(float radius, float length, const G3D::Color4& solid, const G3D::Color4& wire);
	virtual void ray(const G3D::Ray& ray, const G3D::Color4& color, float scale);
	virtual void lineSegment(const G3D::LineSegment& segment, const G3D::Color4& color, float scale);
	virtual void axes(const G3D::Color4& x, const G3D::Color4& y, const G3D::Color4& z, float scale);
	virtual void quad(
		const G3D::Vector3& v0,
		const G3D::Vector3& v1,
		const G3D::Vector3& v2,
		const G3D::Vector3& v3,
		const G3D::Color4& color
	);
};

DECOMP_SIZE_ASSERT(AdornRbxGfx, 0xc4)

} // namespace RBX

#endif // APPDRAW_ADORNRBXGFX_H
