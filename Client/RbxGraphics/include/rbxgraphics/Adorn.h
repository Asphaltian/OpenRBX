#ifndef RBXGRAPHICS_ADORN_H
#define RBXGRAPHICS_ADORN_H

#include "rbxgraphics/TextureProxyBase.h"
#include "placeholder.h"

#include <G3D/AABox.h>
#include <G3D/Color4.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/LineSegment.h>
#include <G3D/Ray.h>
#include <G3D/Rect2D.h>
#include <G3D/ReferenceCount.h>
#include <G3D/Sphere.h>
#include <G3D/Vector2.h>
#include <G3D/Vector3.h>
#include <string>

namespace RBX {

class __declspec(novtable) Adorn
{
public:
	Adorn() {}

	enum XAlign
	{
		XALIGN_RIGHT = 0,
		XALIGN_LEFT = 1,
		XALIGN_CENTER = 2
	};

	enum YAlign
	{
		YALIGN_TOP = 0,
		YALIGN_BASELINE = 1,
		YALIGN_CENTER = 2,
		YALIGN_BOTTOM = 3
	};

	enum Spacing
	{
		PROPORTIONAL_SPACING = 0,
		FIXED_SPACING = 1
	};

	virtual ~Adorn() {}

	virtual G3D::ReferenceCountedPointer<TextureProxyBase> createTextureProxy(const std::string& name) = 0;
	virtual void prepareRenderPass() {}
	virtual void finishRenderPass() {}
	virtual G3D::Rect2D getViewport() const = 0;
	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;

	virtual void setTexture(int stage, G3D::ReferenceCountedPointer<TextureProxyBase> texture) = 0;

	virtual G3D::Rect2D getTextureSize(G3D::ReferenceCountedPointer<TextureProxyBase> texture) const = 0;

	virtual void line2d(const G3D::Vector2& from, const G3D::Vector2& to, const G3D::Color4& color) const = 0;

	virtual void outlineRect2d(const G3D::Rect2D& rect, float thickness, const G3D::Color4& color) const = 0;

	virtual void rect2d(const G3D::Rect2D& rect, const G3D::Color4& color) const = 0;

	virtual G3D::Vector2 get2DStringBounds(const std::string& s, double size, Spacing spacing) const = 0;

	virtual G3D::Vector2 drawFont2D(
		const std::string& s,
		const G3D::Vector2& position,
		double size,
		const G3D::Color4& color,
		const G3D::Color4& outline,
		XAlign xAlign,
		YAlign yAlign,
		Spacing spacing
	) const = 0;

	virtual void setObjectToWorldMatrix(const G3D::CoordinateFrame& coord) = 0;
	virtual void setColor(const G3D::Color4& color) = 0;

	virtual void box(const G3D::AABox& box, const G3D::Color4& solid, const G3D::Color4& wire) = 0;
	virtual void sphere(const G3D::Sphere& sphere, const G3D::Color4& solid, const G3D::Color4& wire) = 0;
	virtual void explosion(const G3D::Sphere& sphere, const G3D::Color4& solid, const G3D::Color4& wire) = 0;

	virtual void cylinderAlongX(float radius, float length, const G3D::Color4& solid, const G3D::Color4& wire) = 0;

	virtual void ray(const G3D::Ray& ray, const G3D::Color4& color, float scale) = 0;
	virtual void lineSegment(const G3D::LineSegment& segment, const G3D::Color4& color, float scale) = 0;
	virtual void axes(const G3D::Color4& x, const G3D::Color4& y, const G3D::Color4& z, float scale) = 0;

	virtual void quad(
		const G3D::Vector3& v0,
		const G3D::Vector3& v1,
		const G3D::Vector3& v2,
		const G3D::Vector3& v3,
		const G3D::Color4& color
	) = 0;
};

} // namespace RBX

#endif // RBXGRAPHICS_ADORN_H
