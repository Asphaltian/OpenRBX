#ifndef V8WORLD_BALL_H
#define V8WORLD_BALL_H

#include "v8world/Geometry.h"

namespace RBX {

// VTABLE: WEBSERVICE 0x1023767c
// SIZE 0x14
class Ball : public Geometry
{
public:
	Ball() : realRadius(0) {}

	// SYNTHETIC: WEBSERVICE 0x100a76b0 FOLDED
	// RBX::Ball::`scalar deleting destructor'

	virtual void onSetSize();
	virtual bool hitTest(const Ray& ray, Vector3& hitPoint, bool& inside);

	// FUNCTION: WEBSERVICE 0x100a7780
	virtual float getRadius() const { return realRadius; }

	// FUNCTION: WEBSERVICE 0x100a7790
	virtual Vector3 getCenterToCorner(const Matrix3& rotation) const
	{
		return Vector3(realRadius, realRadius, realRadius);
	}

	// FUNCTION: WEBSERVICE 0x100a77b0
	virtual Matrix3 getMoment(float mass) const { return getMomentSolid(mass); }

	// FUNCTION: WEBSERVICE 0x10108040 FOLDED
	virtual GeometryType getGeometryType() const { return GEOMETRY_BALL; }

	virtual float getGridVolume() const;

	Matrix3 getMomentSolid(float mass) const;

private:
	float realRadius; // 0x10
};

DECOMP_SIZE_ASSERT(Ball, 0x14)

} // namespace RBX

#endif // V8WORLD_BALL_H
