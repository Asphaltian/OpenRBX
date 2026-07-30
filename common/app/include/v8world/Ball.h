#ifndef V8WORLD_BALL_H
#define V8WORLD_BALL_H

#include "v8world/Geometry.h"

namespace RBX {

class Ball : public Geometry
{
public:
	virtual void onSetSize();

	// FUNCTION: WEBSERVICE 0x100a7780
	virtual float getRadius() const { return realRadius; }

	// FUNCTION: WEBSERVICE 0x100a7790
	virtual Vector3 getCenterToCorner() const { return Vector3(realRadius, realRadius, realRadius); }

	// FUNCTION: WEBSERVICE 0x100a77b0
	virtual Matrix3 getMoment(float mass) const { return getMomentSolid(mass); }

	virtual float getGridVolume() const;

	Matrix3 getMomentSolid(float mass) const;

private:
	float realRadius; // 0x10
};

} // namespace RBX

#endif // V8WORLD_BALL_H
