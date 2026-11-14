#ifndef APPDRAW_PART_H
#define APPDRAW_PART_H

#include "placeholder.h"
#include "util/SurfaceType.h"
#include "util/Vector6.h"

#include <G3D/Color4.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/Vector3.h>

namespace RBX {

class Part
{
public:
	enum PartType
	{
		BALL_PART = 0,
		BLOCK_PART = 1,
		CYLINDER_PART = 2,
	};

	Part(PartType _type, const G3D::Vector3& _gridSize, G3D::Color4 _color, const G3D::CoordinateFrame& c)
		: type(_type), gridSize(_gridSize), color(_color), surfaceType(NO_SURFACE), coordinateFrame(c)
	{
	}

	Part(
		PartType type,
		const G3D::Vector3& gridSize,
		G3D::Color4 color,
		const Vector6<SurfaceType>& surfaceType,
		const G3D::CoordinateFrame& c
	)
		: type(type), gridSize(gridSize), color(color), surfaceType(surfaceType), coordinateFrame(c)
	{
	}

	Part() {}

	PartType type;
	G3D::Vector3 gridSize;
	G3D::Color4 color;
	Vector6<SurfaceType> surfaceType;
	G3D::CoordinateFrame coordinateFrame;
};

} // namespace RBX

#endif // APPDRAW_PART_H
