#ifndef HUMANOID_RUNNING_H
#define HUMANOID_RUNNING_H

#include "decomp.h"
#include "humanoid/Balancing.h"
#include "util/IRenderable.h"
#include "util/Name.h"

#include <G3D/Vector3.h>

namespace RBX {

class Body;

extern const char sRunning[];

// SIZE 0x70
class Running : public Named<Balancing, sRunning>, public IRenderable
{
private:
	G3D::Vector3 floorVelocity;       // 0x38
	float floorRotationalVelocity;    // 0x44
	float jumpWarmup;                 // 0x48
	bool lastLadderResult;            // 0x4c
	double lastLadderTime;            // 0x50
	G3D::Vector3 desiredVelocity;     // 0x58
	float desiredYRotationalVelocity; // 0x64
	float desiredAltitude;            // 0x68
	float intendedYRotation;          // 0x6c

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

	virtual void onComputeForce(const float dt);                       // vtable+0x08
	virtual State* onStep(const float stepDt, Controller& controller); // vtable+0x0c

	// FUNCTION: WEBSERVICE 0x10106e90
	virtual float getIntendedRotationAboutYAxis() const // vtable+0x10
	{
		return intendedYRotation;
	}

public:
	Running(Humanoid* humanoid);

	virtual ~Running(); // vtable+0x04

	// FUNCTION: WEBSERVICE 0x100eb790 FOLDED
	virtual bool shouldRender2d() const // IRenderable vtable+0x00
	{
		return false;
	}

	// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
	virtual bool shouldRender3dAdorn() const // IRenderable vtable+0x04
	{
		return true;
	}

	virtual void render3dAdorn(Adorn* adorn); // IRenderable vtable+0x0c

	// SYNTHETIC: WEBSERVICE 0x101070f0
	// RBX::Running::`scalar deleting destructor'
};

// clang-format off
// SYNTHETIC: WEBSERVICE 0x101231d0
// RBX::Named<RBX::Balancing,&RBX::sRunning>::~Named<RBX::Balancing,&RBX::sRunning>
// clang-format on

DECOMP_SIZE_ASSERT(Running, 0x70)

} // namespace RBX

#endif // HUMANOID_RUNNING_H
