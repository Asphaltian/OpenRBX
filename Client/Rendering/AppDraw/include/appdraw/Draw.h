#ifndef APPDRAW_DRAW_H
#define APPDRAW_DRAW_H

#include "util/IRenderable.h"

#include <G3D/Color3.h>
#include <G3D/Color4.h>

namespace RBX {

class Adorn;
class Part;

class Draw
{
	static void spokes(float radius, float axis, Adorn* adorn);

	static void adornSurfaces(const Part& part, Adorn* adorn, const G3D::Color3& controllerColor);

	static void frameBox(const Part& part, Adorn* adorn, const G3D::Color4& selectColor);

	static void constraint(const Part& part, Adorn* adorn, int face, const G3D::Color3& controllerColor);

public:
	static const G3D::Color3& selectColor();

	static void partAdorn(const Part& part, Adorn* adorn, const G3D::Color3& controllerColor);

	static void selectionBox(const Part& part, Adorn* adorn, const G3D::Color4& selectColor);
	static void selectionBox(const Part& part, Adorn* adorn, SelectState selectState);
};

} // namespace RBX

#endif // APPDRAW_DRAW_H
