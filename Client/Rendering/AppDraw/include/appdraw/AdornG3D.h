#ifndef APPDRAW_ADORNG3D_H
#define APPDRAW_ADORNG3D_H

#include "decomp.h"
#include "rbxgraphics/Adorn.h"

#include <GLG3D/RenderDevice.h>
#include <GLG3D/TextureManager.h>

namespace RBX {

// VTABLE: WEBSERVICE 0x1025bb44
// SIZE 0x20
class AdornG3D : public Adorn
{
private:
	G3D::RenderDevice* renderDevice;    // 0x04
	G3D::TextureManager textureManager; // 0x08

public:
	G3D::RenderDevice* getRenderDevice() { return renderDevice; }

	virtual G3D::ReferenceCountedPointer<TextureProxyBase> createTextureProxy(const std::string& filename);

	virtual G3D::Rect2D getViewport() const;
	virtual int getWidth() const;
	virtual int getHeight() const;

	virtual void setTexture(int id, G3D::ReferenceCountedPointer<TextureProxyBase> _texture);
	virtual G3D::Rect2D getTextureSize(G3D::ReferenceCountedPointer<TextureProxyBase> _texture) const;

	virtual void line2d(const G3D::Vector2& p0, const G3D::Vector2& p1, const G3D::Color4& color) const;
	virtual void outlineRect2d(const G3D::Rect2D& rect, float thick, const G3D::Color4& color) const;
	virtual void rect2d(const G3D::Rect2D& rect, const G3D::Color4& color) const;

	virtual G3D::Vector2 get2DStringBounds(const std::string& s, double size, Spacing spacing) const;
	virtual G3D::Vector2 drawFont2D(
		const std::string& s,
		const G3D::Vector2& pos2d,
		double size,
		const G3D::Color4& color,
		const G3D::Color4& outline,
		XAlign xalign,
		YAlign yalign,
		Spacing spacing
	) const;

	virtual void setObjectToWorldMatrix(const G3D::CoordinateFrame& c);
	virtual void setColor(const G3D::Color4& color);

	virtual void box(const G3D::AABox& box, const G3D::Color4& solidColor, const G3D::Color4& wireColor);
	virtual void sphere(const G3D::Sphere& sphere, const G3D::Color4& solidColor, const G3D::Color4& wireColor);
	virtual void explosion(const G3D::Sphere& sph, const G3D::Color4& solidColor, const G3D::Color4& wireColor);

	virtual void cylinderAlongX(
		float radius,
		float length,
		const G3D::Color4& solidColor,
		const G3D::Color4& wireColor
	);

	virtual void ray(const G3D::Ray& ray, const G3D::Color4& color, float scale);
	virtual void lineSegment(const G3D::LineSegment& lineSegment, const G3D::Color4& color, float scale);
	virtual void axes(const G3D::Color4& xColor, const G3D::Color4& yColor, const G3D::Color4& zColor, float scale);

	virtual void quad(
		const G3D::Vector3& v0,
		const G3D::Vector3& v1,
		const G3D::Vector3& v2,
		const G3D::Vector3& v3,
		const G3D::Color4& color
	);
};

DECOMP_SIZE_ASSERT(AdornG3D, 0x20)

} // namespace RBX

#endif // APPDRAW_ADORNG3D_H
