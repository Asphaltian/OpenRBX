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

// VTABLE: WEBSERVICE 0x1023762c
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

	virtual void onSetSize(); // vtable+0x00
	virtual ~Geometry() {}    // vtable+0x04

	static Geometry* nullGeometry();

	virtual GeometryType getGeometryType() const;  // vtable+0x08
	virtual void setGridSize(const Vector3& size); // vtable+0x0c
	const Vector3& getGridSize() const { return gridSize; }
	virtual float getRadius() const;                                       // vtable+0x10
	virtual Vector3 getCenterToCorner(const Matrix3& rotation) const;      // vtable+0x14
	virtual Matrix3 getMoment(float mass) const;                           // vtable+0x18
	virtual float getGridVolume() const;                                   // vtable+0x1c
	virtual bool hitTest(const Ray& ray, Vector3& hitPoint, bool& inside); // vtable+0x20

protected:
	Vector3 gridSize; // 0x04
};

DECOMP_SIZE_ASSERT(Geometry, 0x10)

// FUNCTION: WEBSERVICE 0x100a0710 FOLDED
inline float Geometry::getRadius() const
{
	return 0;
}

// FUNCTION: WEBSERVICE 0x100a0710 FOLDED
inline float Geometry::getGridVolume() const
{
	return 0;
}

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

// FUNCTION: WEBSERVICE 0x100a76a0 FOLDED
inline bool Geometry::hitTest(const Ray& ray, Vector3& hitPoint, bool& inside)
{
	return false;
}

// FUNCTION: WEBSERVICE 0x100a7de0
inline Geometry* Geometry::nullGeometry()
{
	static Geometry g;
	return &g;
}

// FUNCTION: WEBSERVICE 0x100a7e30
inline Vector3 Geometry::getCenterToCorner(const Matrix3& rotation) const
{
	return Vector3::zero();
}

// FUNCTION: WEBSERVICE 0x100d7a70 FOLDED
inline Geometry::GeometryType Geometry::getGeometryType() const
{
	return GEOMETRY_NONE;
}

// FUNCTION: WEBSERVICE 0x1015b610 FOLDED
inline void Geometry::onSetSize()
{
}

// SYNTHETIC: WEBSERVICE 0x102235a0
// `RBX::Geometry::nullGeometry'::`2'::`dynamic atexit destructor for 'g''

} // namespace RBX

#endif // V8WORLD_GEOMETRY_H
