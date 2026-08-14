#ifndef APPDRAW_DRAWPRIMITIVES_H
#define APPDRAW_DRAWPRIMITIVES_H

#include <G3D/AABox.h>
#include <G3D/Color4.h>
#include <G3D/Vector2.h>

namespace G3D {
class RenderDevice;
}

namespace RBX {

class Rect;

class DrawPrimitives
{
public:
	static void rawBox(const G3D::AABox& box, G3D::RenderDevice* rd);

	static void rawSphere(float radius, G3D::RenderDevice* rd);

	static void rawCylinderAlongX(float radius, float axis, G3D::RenderDevice* rd);

	static void rect2d(const Rect& rect, G3D::RenderDevice* rd, const G3D::Color4& color);

	static void line2d(const G3D::Vector2& p0, const G3D::Vector2& p1, G3D::RenderDevice* rd, const G3D::Color4& color);

	static void outlineRect2d(const Rect& rect, float thick, G3D::RenderDevice* rd, const G3D::Color4& color);
};

} // namespace RBX

#endif // APPDRAW_DRAWPRIMITIVES_H
