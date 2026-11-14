#ifndef V8WORLD_GEOMETRY_H
#define V8WORLD_GEOMETRY_H

#include "placeholder.h"

#include <G3D/Matrix3.h>
#include <G3D/Ray.h>
#include <G3D/Vector3.h>

namespace RBX {

using G3D::Matrix3;
using G3D::Ray;
using G3D::Vector3;

class Geometry
{
public:
	Geometry() {}

	enum GeometryType
	{
		GEOMETRY_BALL = 1,
		GEOMETRY_BLOCK = 2,
		GEOMETRY_NONE = 3,
	};

protected:
	virtual void onSetSize();

public:
	virtual ~Geometry() {}

	static Geometry* nullGeometry();

	virtual GeometryType getGeometryType() const;
	virtual void setGridSize(const Vector3& size);
	const Vector3& getGridSize() const { return gridSize; }
	virtual float getRadius() const;
	virtual Vector3 getCenterToCorner(const Matrix3& rotation) const;
	virtual Matrix3 getMoment(float mass) const;
	virtual float getGridVolume() const;
	virtual bool hitTest(const Ray& ray, Vector3& hitPoint, bool& inside);

protected:
	Vector3 gridSize;
};

inline float Geometry::getRadius() const
{
	return 0;
}

inline float Geometry::getGridVolume() const
{
	return 0;
}

inline void Geometry::setGridSize(const Vector3& _gridSize)
{
	gridSize = _gridSize;
	onSetSize();
}

inline Matrix3 Geometry::getMoment(float mass) const
{
	return Matrix3::zero();
}

inline bool Geometry::hitTest(const Ray& ray, Vector3& hitPoint, bool& inside)
{
	return false;
}

inline Geometry* Geometry::nullGeometry()
{
	static Geometry g;
	return &g;
}

inline Vector3 Geometry::getCenterToCorner(const Matrix3& rotation) const
{
	return Vector3::zero();
}

inline Geometry::GeometryType Geometry::getGeometryType() const
{
	return GEOMETRY_NONE;
}

inline void Geometry::onSetSize()
{
}

} // namespace RBX

#endif // V8WORLD_GEOMETRY_H
