#ifndef HUMANOID_RUNNING_H
#define HUMANOID_RUNNING_H

#include "humanoid/Balancing.h"
#include "placeholder.h"
#include "util/IRenderable.h"
#include "util/Name.h"

#include <G3D/Vector3.h>

namespace RBX {

class Body;

extern const char sRunning[];

class Running : public Named<Balancing, sRunning>, public IRenderable
{
private:
	G3D::Vector3 floorVelocity;
	float floorRotationalVelocity;
	float jumpWarmup;
	bool lastLadderResult;
	double lastLadderTime;
	G3D::Vector3 desiredVelocity;
	float desiredYRotationalVelocity;
	float desiredAltitude;
	float intendedYRotation;

	void rotateWithGround(Body* body);
	void hoverOnFloor(Body* body);
	void move(Body* body);

	static const float manualTurnSpeed;
	static const float autoTurnSpeed;
	static const float kTurnP;
	static const float kTurnAccelMax;
	static const float kMoveP;
	static const float kAltitudeP;
	static const float kAltitudeD;
	static const G3D::Vector3 maxMoveAccelerationGrid;

	bool isFacingStairs();
	bool isFacingLadder(bool buffered);
	void updateGoals(Controller& controller);

	virtual void onComputeForce(const float dt);
	virtual State* onStep(const float stepDt, Controller& controller);

	virtual float getIntendedRotationAboutYAxis() const { return intendedYRotation; }

public:
	Running(Humanoid* humanoid);

	virtual ~Running();

	virtual bool shouldRender2d() const // IRenderable vtable+0x00
	{
		return false;
	}

	virtual bool shouldRender3dAdorn() const // IRenderable vtable+0x04
	{
		return true;
	}

	virtual void render3dAdorn(Adorn* adorn); // IRenderable vtable+0x0c
};

} // namespace RBX

#endif // HUMANOID_RUNNING_H
