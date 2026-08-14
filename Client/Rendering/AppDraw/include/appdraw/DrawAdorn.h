#ifndef APPDRAW_DRAWADORN_H
#define APPDRAW_DRAWADORN_H

#include "rbxgraphics/Adorn.h"

#include <G3D/Color3.h>
#include <G3D/Color4.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/Vector3.h>

namespace RBX {

class Part;

class DrawAdorn
{
public:
	static void cylinder(
		Adorn* adorn,
		const G3D::CoordinateFrame& worldC,
		int axis,
		float length,
		float radius,
		const G3D::Color3& color
	);

	static void partSurface(const Part& part, int surfaceId, Adorn* adorn, const G3D::Color4& color);

private:
	static void surfaceBorder(
		Adorn* adorn,
		const G3D::Vector3& halfRealSize,
		float highlight,
		int surfaceId,
		const G3D::Color4& color
	);
};

} // namespace RBX

#endif // APPDRAW_DRAWADORN_H
