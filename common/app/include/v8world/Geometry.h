#ifndef V8WORLD_GEOMETRY_H
#define V8WORLD_GEOMETRY_H

#include <G3D/Matrix3.h>
#include <G3D/Vector3.h>

namespace RBX {

using G3D::Matrix3;
using G3D::Vector3;

class Geometry
{
public:
	enum GeometryType
	{
		GEOMETRY_BALL = 0,
		GEOMETRY_BLOCK = 1,
	};

	virtual void onSetSize() = 0;
	virtual ~Geometry();
	virtual GeometryType getGeometryType() const = 0;

	// FUNCTION: WEBSERVICE 0x100a7660
	virtual void setGridSize(const Vector3& size)
	{
		gridSize = size;
		onSetSize();
	}

	virtual float getRadius() const = 0;
	virtual Vector3 getCenterToCorner() const;

	// FUNCTION: WEBSERVICE 0x100a7680
	virtual Matrix3 getMoment(float mass) const { return Matrix3::zero(); }

	virtual float getGridVolume() const = 0;
	virtual bool hitTest(const Vector3& point, Vector3& normal) = 0;

	const Vector3& getGridSize() const { return gridSize; }

protected:
	Vector3 gridSize; // 0x04
};

} // namespace RBX

#endif // V8WORLD_GEOMETRY_H
