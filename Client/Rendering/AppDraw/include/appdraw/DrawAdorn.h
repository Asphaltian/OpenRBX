#ifndef APPDRAW_DRAWADORN_H
#define APPDRAW_DRAWADORN_H

#include "rbxgraphics/Adorn.h"

#include <G3D/Color3.h>
#include <G3D/Color4.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/GCamera.h>
#include <G3D/Vector3.h>

namespace RBX {

class Part;

enum HandleType
{
	HANDLE_RESIZE = 0,
	HANDLE_MOVE = 1,
	HANDLE_ROTATE = 2,
	HANDLE_VELOCITY = 3
};

class DrawAdorn
{
private:
	static void surfaceBorder(
		Adorn* adorn, const G3D::Vector3& halfRealSize, float highlight, int surfaceId, const G3D::Color4& color
	);

public:
	static const G3D::Color3& resizeColor()
	{
		static G3D::Color3 c(0.1f, 0.6f, 1.0f);
		return c;
	}

	static void cylinder(
		Adorn* adorn, const G3D::CoordinateFrame& worldC, int axis, float length, float radius, const G3D::Color3& color
	);

	static void partSurface(const Part& part, int surfaceId, Adorn* adorn, const G3D::Color4& color);
	static void handles3d(const G3D::Vector3&, const G3D::CoordinateFrame&, Adorn*, HandleType, const G3D::Vector3&);
	static void handles2d(
		const G3D::Vector3&, const G3D::CoordinateFrame&, const G3D::GCamera&, Adorn*, const G3D::Color3&
	);
};

} // namespace RBX

#endif // APPDRAW_DRAWADORN_H
