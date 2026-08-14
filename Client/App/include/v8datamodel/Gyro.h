#ifndef V8DATAMODEL_GYRO_H
#define V8DATAMODEL_GYRO_H

#include "decomp.h"
#include "util/RunStateOwner.h"
#include "v8kernel/Connector.h"
#include "v8tree/Instance.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/Vector3.h>

namespace RBX {

class Body;
class World;
class PartInstance;

extern char sRocket[];

extern char sBodyVelocity[];

extern char sBodyPosition[];

extern char sBodyThrust[];

extern char sBodyForce[];

extern char sBodyGyro[];
// SIZE 0x10c
class BodyMover : public Instance, public Connector, public Listener<RunService, Stepped>
{
public:
	virtual ~BodyMover();

protected:
	// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
	virtual bool preventBodySleep() { return true; } // vtable+0x40

	virtual void onEvent(const RunService* source, Stepped event);

	World* world;       // 0x104
	PartInstance* part; // 0x108
};

DECOMP_SIZE_ASSERT(BodyMover, 0x10c)

// SIZE 0x118
class BodyForce : public DescribedCreatable<BodyForce, BodyMover, sBodyForce>
{
public:
	BodyForce();

	virtual bool preventBodySleep();

	virtual void computeForce(float dt, bool throttling);

private:
	G3D::Vector3 force; // 0x10c
};

DECOMP_SIZE_ASSERT(BodyForce, 0x118)

// SIZE 0x150
class BodyGyro : public DescribedCreatable<BodyGyro, BodyMover, sBodyGyro>
{
public:
	virtual void computeForce(float dt, bool throttling);

private:
	void computeOrientation(Body* body);
	void computeBalance(Body* body);

	float kP;                    // 0x10c
	float kD;                    // 0x110
	G3D::Vector3 maxTorque;      // 0x114
	G3D::CoordinateFrame cframe; // 0x120
};

DECOMP_SIZE_ASSERT(BodyGyro, 0x150)

// SIZE 0x138
class BodyPosition : public DescribedCreatable<BodyPosition, BodyMover, sBodyPosition>
{
public:
	virtual void computeForce(float dt, bool throttling);

private:
	float kP;               // 0x10c
	float kD;               // 0x110
	G3D::Vector3 maxForce;  // 0x114
	G3D::Vector3 position;  // 0x120
	G3D::Vector3 lastForce; // 0x12c
};

DECOMP_SIZE_ASSERT(BodyPosition, 0x138)

// SIZE 0x124
class BodyThrust : public DescribedCreatable<BodyThrust, BodyMover, sBodyThrust>
{
public:
	virtual bool preventBodySleep();

	virtual void computeForce(float dt, bool throttling);

private:
	G3D::Vector3 force;    // 0x10c
	G3D::Vector3 location; // 0x118
};

DECOMP_SIZE_ASSERT(BodyThrust, 0x124)

// SIZE 0x134
class BodyVelocity : public DescribedCreatable<BodyVelocity, BodyMover, sBodyVelocity>
{
public:
	virtual bool preventBodySleep();

	virtual void computeForce(float dt, bool throttling);

private:
	float kP;               // 0x10c
	G3D::Vector3 maxForce;  // 0x110
	G3D::Vector3 velocity;  // 0x11c
	G3D::Vector3 lastForce; // 0x128
};

DECOMP_SIZE_ASSERT(BodyVelocity, 0x134)

// SIZE 0x154
class Rocket : public DescribedCreatable<Rocket, BodyMover, sRocket>
{
public:
	virtual void computeForce(float dt, bool throttling);

private:
	bool active;                            // 0x10c
	boost::shared_ptr<PartInstance> target; // 0x110
	G3D::Vector3 targetOffset;              // 0x118
	float targetRadius;                     // 0x124
	bool firedEvent;                        // 0x128
	float maxThrust;                        // 0x12c
	float kThrustP;                         // 0x130
	float kThrustD;                         // 0x134
	float maxSpeed;                         // 0x138
	float kTurnP;                           // 0x13c
	float kTurnD;                           // 0x140
	G3D::Vector3 maxTorque;                 // 0x144
	float cartoonFactor;                    // 0x150
};

DECOMP_SIZE_ASSERT(Rocket, 0x154)

} // namespace RBX

#endif // V8DATAMODEL_GYRO_H
