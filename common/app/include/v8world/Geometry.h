#ifndef V8WORLD_GEOMETRY_H
#define V8WORLD_GEOMETRY_H

#include "decomp.h"

#include <G3D/Matrix3.h>
#include <G3D/Ray.h>
#include <G3D/Vector3.h>

namespace RBX {

using G3D::Matrix3;
using G3D::Ray;
using G3D::Vector3;

// SIZE 0x10
class Geometry
{
public:
	enum GeometryType
	{
		GEOMETRY_BALL = 1,
		GEOMETRY_BLOCK = 2,
		GEOMETRY_NONE = 3,
	};

	virtual void onSetSize() = 0; // vtable+0x00
	virtual ~Geometry() {}        // vtable+0x04

	static Geometry* nullGeometry();

	virtual GeometryType getGeometryType() const = 0; // vtable+0x08
	virtual void setGridSize(const Vector3& size);    // vtable+0x0c
	const Vector3& getGridSize() const { return gridSize; }
	virtual float getRadius() const = 0;                                       // vtable+0x10
	virtual Vector3 getCenterToCorner(const Matrix3& rotation) const;          // vtable+0x14
	virtual Matrix3 getMoment(float mass) const;                               // vtable+0x18
	virtual float getGridVolume() const = 0;                                   // vtable+0x1c
	virtual bool hitTest(const Ray& ray, Vector3& hitPoint, bool& inside) = 0; // vtable+0x20

protected:
	Vector3 gridSize; // 0x04
};

DECOMP_SIZE_ASSERT(Geometry, 0x10)

// FUNCTION: WEBSERVICE 0x100a7660
inline void Geometry::setGridSize(const Vector3& size)
{
	gridSize = size;
	onSetSize();
}

// FUNCTION: WEBSERVICE 0x100a7680
inline Matrix3 Geometry::getMoment(float mass) const
{
	return Matrix3::zero();
}

// FUNCTION: WEBSERVICE 0x100a7e30
inline Vector3 Geometry::getCenterToCorner(const Matrix3& rotation) const
{
	return Vector3::zero();
}

} // namespace RBX

#endif // V8WORLD_GEOMETRY_H
