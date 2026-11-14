#ifndef V8WORLD_BALL_H
#define V8WORLD_BALL_H

#include "v8world/Geometry.h"

namespace RBX {

class Ball : public Geometry
{
private:
	Matrix3 getMomentSolid(float mass) const;
	G3D::Matrix3 getMomentHollow(float) const;

	virtual void onSetSize();

public:
	virtual bool hitTest(const Ray& ray, Vector3& hitPoint, bool& inside);

	virtual GeometryType getGeometryType() const { return GEOMETRY_BALL; }

	virtual float getRadius() const { return realRadius; }

	virtual Vector3 getCenterToCorner(const Matrix3& rotation) const
	{
		return Vector3(realRadius, realRadius, realRadius);
	}

	virtual Matrix3 getMoment(float mass) const { return getMomentSolid(mass); }

	virtual float getGridVolume() const;

	Ball() : realRadius(0) {}
	virtual ~Ball() {}

private:
	float realRadius;
};

} // namespace RBX

#endif // V8WORLD_BALL_H
