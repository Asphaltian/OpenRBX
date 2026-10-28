#ifndef V8DATAMODEL_GYRO_H
#define V8DATAMODEL_GYRO_H

#include "decomp.h"
#include "reflection/function.h"
#include "reflection/property.h"
#include "reflection/signal.h"
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
protected:
	BodyMover(const char* name);

public:
	virtual ~BodyMover();
	virtual bool askSetParent(const Instance* instance) const;

protected:
	// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
	virtual bool preventBodySleep() const { return true; } // vtable+0x40

	virtual void onAncestorChanged(const AncestorChanged& event);
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
	virtual void onEvent(const RunService* source, Stepped event);

private:
	void updateWorld();

protected:
	World* world;       // 0x104
	PartInstance* part; // 0x108
};

DECOMP_SIZE_ASSERT(BodyMover, 0x10c)

// SIZE 0x118
class BodyForce : public DescribedCreatable<BodyForce, BodyMover, sBodyForce>
{
public:
	BodyForce();

	static Reflection::BoundProp<G3D::Vector3, 1> prop_Force;

	virtual void computeForce(float dt, bool throttling);

protected:
	virtual bool preventBodySleep() const;

private:
	G3D::Vector3 force; // 0x10c
};

DECOMP_SIZE_ASSERT(BodyForce, 0x118)

// SIZE 0x150
class BodyGyro : public DescribedCreatable<BodyGyro, BodyMover, sBodyGyro>
{
public:
	BodyGyro();
	static Reflection::BoundProp<float, 1> prop_kP;
	static Reflection::BoundProp<float, 1> prop_kD;
	static Reflection::BoundProp<G3D::Vector3, 1> prop_maxTorque;
	static Reflection::BoundProp<G3D::CoordinateFrame, 1> prop_cframe;

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
	BodyPosition();
	// FUNCTION: WEBSERVICE 0x100e9f50
	G3D::Vector3 getLastForce() { return lastForce; }

	static Reflection::BoundProp<float, 1> prop_kP;
	static Reflection::BoundProp<float, 1> prop_kD;
	static Reflection::BoundProp<G3D::Vector3, 1> prop_maxForce;
	static Reflection::BoundProp<G3D::Vector3, 1> prop_position;

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
	BodyThrust();
	static Reflection::BoundProp<G3D::Vector3, 1> prop_force;
	static Reflection::BoundProp<G3D::Vector3, 1> prop_location;

	virtual void computeForce(float dt, bool throttling);

protected:
	virtual bool preventBodySleep() const;

private:
	G3D::Vector3 force;    // 0x10c
	G3D::Vector3 location; // 0x118
};

DECOMP_SIZE_ASSERT(BodyThrust, 0x124)

// SIZE 0x134
class BodyVelocity : public DescribedCreatable<BodyVelocity, BodyMover, sBodyVelocity>
{
public:
	BodyVelocity();
	// FUNCTION: WEBSERVICE 0x100e9f80
	G3D::Vector3 getLastForce() { return lastForce; }

	static Reflection::BoundProp<float, 1> prop_kP;
	static Reflection::BoundProp<G3D::Vector3, 1> prop_maxForce;
	static Reflection::BoundProp<G3D::Vector3, 1> prop_velocity;

	virtual void computeForce(float dt, bool throttling);

protected:
	virtual bool preventBodySleep() const;

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
	Rocket();
	void fire();
	void abort();

	static Reflection::RefPropDescriptor<Rocket, PartInstance> prop_Target;
	static Reflection::BoundProp<G3D::Vector3, 1> prop_targetOffset;
	static Reflection::BoundProp<float, 1> prop_targetRadius;
	static Reflection::BoundProp<float, 1> prop_MaxSpeed;
	static Reflection::BoundProp<float, 1> prop_MaxThrust;
	static Reflection::BoundProp<float, 1> prop_ThrustP;
	static Reflection::BoundProp<float, 1> prop_ThrustD;
	static Reflection::BoundProp<float, 1> prop_TurnP;
	static Reflection::BoundProp<float, 1> prop_TurnD;
	static Reflection::BoundProp<G3D::Vector3, 1> prop_MaxTorque;
	static Reflection::BoundProp<float, 1> prop_CartoonFactor;
	static Reflection::BoundFuncDesc<Rocket, void(), 0> func_Fire;
	static Reflection::BoundFuncDesc<Rocket, void(), 0> func_Abort;
	static Reflection::SignalDesc<Rocket, void()> event_ReachedTarget;

	virtual void computeForce(float dt, bool throttling);

protected:
	virtual void onEvent(const RunService* source, Stepped event);

private:
	G3D::Vector3 turn(Body* body, const G3D::Vector3& targetDir);

	PartInstance* getTarget() const { return target.get(); }
	void setTarget(PartInstance* value);

	// FUNCTION: WEBSERVICE 0x100e9fb0
	void onGoalChanged(const Reflection::PropertyDescriptor&) { firedEvent = false; }

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

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021cf20
// `dynamic initializer for 'RBX::Rocket::prop_Target''
// SYNTHETIC: WEBSERVICE 0x1021cf60
// `dynamic initializer for 'RBX::Rocket::prop_targetOffset''
// SYNTHETIC: WEBSERVICE 0x1021cf90
// `dynamic initializer for 'RBX::Rocket::prop_targetRadius''
// SYNTHETIC: WEBSERVICE 0x1021cfc0
// `dynamic initializer for 'RBX::Rocket::prop_MaxSpeed''
// SYNTHETIC: WEBSERVICE 0x1021cff0
// `dynamic initializer for 'RBX::Rocket::prop_MaxThrust''
// SYNTHETIC: WEBSERVICE 0x1021d020
// `dynamic initializer for 'RBX::Rocket::prop_ThrustP''
// SYNTHETIC: WEBSERVICE 0x1021d050
// `dynamic initializer for 'RBX::Rocket::prop_ThrustD''
// SYNTHETIC: WEBSERVICE 0x1021d080
// `dynamic initializer for 'RBX::Rocket::prop_TurnP''
// SYNTHETIC: WEBSERVICE 0x1021d0b0
// `dynamic initializer for 'RBX::Rocket::prop_TurnD''
// SYNTHETIC: WEBSERVICE 0x1021d0e0
// `dynamic initializer for 'RBX::Rocket::prop_MaxTorque''
// SYNTHETIC: WEBSERVICE 0x1021d110
// `dynamic initializer for 'RBX::Rocket::prop_CartoonFactor''
// SYNTHETIC: WEBSERVICE 0x1021d140
// `dynamic initializer for 'RBX::Rocket::func_Fire''
// SYNTHETIC: WEBSERVICE 0x1021d170
// `dynamic initializer for 'RBX::Rocket::func_Abort''
// SYNTHETIC: WEBSERVICE 0x1021d1a0
// `dynamic initializer for 'RBX::Rocket::event_ReachedTarget''
// SYNTHETIC: WEBSERVICE 0x1021d1d0
// `dynamic initializer for 'RBX::BodyGyro::prop_kP''
// SYNTHETIC: WEBSERVICE 0x1021d200
// `dynamic initializer for 'RBX::BodyGyro::prop_kD''
// SYNTHETIC: WEBSERVICE 0x1021d230
// `dynamic initializer for 'RBX::BodyGyro::prop_maxTorque''
// SYNTHETIC: WEBSERVICE 0x1021d260
// `dynamic initializer for 'RBX::BodyGyro::prop_cframe''
// SYNTHETIC: WEBSERVICE 0x1021d290
// `dynamic initializer for 'RBX::BodyPosition::prop_kP''
// SYNTHETIC: WEBSERVICE 0x1021d2c0
// `dynamic initializer for 'RBX::BodyPosition::prop_kD''
// SYNTHETIC: WEBSERVICE 0x1021d2f0
// `dynamic initializer for 'RBX::BodyPosition::prop_maxForce''
// SYNTHETIC: WEBSERVICE 0x1021d320
// `dynamic initializer for 'RBX::BodyPosition::prop_position''
// SYNTHETIC: WEBSERVICE 0x1021d350
// `dynamic initializer for 'func_getLastForceOld''
// SYNTHETIC: WEBSERVICE 0x1021d380
// `dynamic initializer for 'func_getLastForce''
// SYNTHETIC: WEBSERVICE 0x1021d3b0
// `dynamic initializer for 'RBX::BodyVelocity::prop_kP''
// SYNTHETIC: WEBSERVICE 0x1021d3e0
// `dynamic initializer for 'RBX::BodyVelocity::prop_maxForce''
// SYNTHETIC: WEBSERVICE 0x1021d410
// `dynamic initializer for 'RBX::BodyVelocity::prop_velocity''
// SYNTHETIC: WEBSERVICE 0x1021d440
// `dynamic initializer for 'func_getLastForceVOld''
// SYNTHETIC: WEBSERVICE 0x1021d470
// `dynamic initializer for 'func_getLastForceV''
// SYNTHETIC: WEBSERVICE 0x1021d4a0
// `dynamic initializer for 'RBX::BodyForce::prop_Force''
// SYNTHETIC: WEBSERVICE 0x1021d4d0
// `dynamic initializer for 'RBX::BodyThrust::prop_force''
// SYNTHETIC: WEBSERVICE 0x1021d500
// `dynamic initializer for 'RBX::BodyThrust::prop_location''
// SYNTHETIC: WEBSERVICE 0x10224380
// `dynamic atexit destructor for 'RBX::Rocket::prop_Target''
// SYNTHETIC: WEBSERVICE 0x102243a0
// `dynamic atexit destructor for 'RBX::Rocket::prop_MaxThrust''
// SYNTHETIC: WEBSERVICE 0x102243c0
// `dynamic atexit destructor for 'RBX::Rocket::prop_ThrustP''
// SYNTHETIC: WEBSERVICE 0x102243e0
// `dynamic atexit destructor for 'RBX::Rocket::prop_ThrustD''
// SYNTHETIC: WEBSERVICE 0x10224400
// `dynamic atexit destructor for 'RBX::Rocket::prop_TurnP''
// SYNTHETIC: WEBSERVICE 0x10224420
// `dynamic atexit destructor for 'RBX::Rocket::prop_TurnD''
// SYNTHETIC: WEBSERVICE 0x10224440
// `dynamic atexit destructor for 'RBX::Rocket::prop_CartoonFactor''
// SYNTHETIC: WEBSERVICE 0x10224460
// `dynamic atexit destructor for 'RBX::Rocket::prop_MaxTorque''
// SYNTHETIC: WEBSERVICE 0x10224480
// `dynamic atexit destructor for 'RBX::BodyGyro::prop_kD''
// SYNTHETIC: WEBSERVICE 0x102244a0
// `dynamic atexit destructor for 'RBX::BodyGyro::prop_kP''
// SYNTHETIC: WEBSERVICE 0x102244c0
// `dynamic atexit destructor for 'RBX::BodyGyro::prop_maxTorque''
// SYNTHETIC: WEBSERVICE 0x102244e0
// `dynamic atexit destructor for 'RBX::BodyPosition::prop_kD''
// SYNTHETIC: WEBSERVICE 0x10224500
// `dynamic atexit destructor for 'RBX::BodyPosition::prop_kP''
// SYNTHETIC: WEBSERVICE 0x10224520
// `dynamic atexit destructor for 'RBX::BodyPosition::prop_position''
// SYNTHETIC: WEBSERVICE 0x10224540
// `dynamic atexit destructor for 'RBX::BodyPosition::prop_maxForce''
// SYNTHETIC: WEBSERVICE 0x10224560
// `dynamic atexit destructor for 'RBX::BodyVelocity::prop_velocity''
// SYNTHETIC: WEBSERVICE 0x10224580
// `dynamic atexit destructor for 'RBX::BodyVelocity::prop_maxForce''
// SYNTHETIC: WEBSERVICE 0x102245a0
// `dynamic atexit destructor for 'RBX::BodyVelocity::prop_kP''
// SYNTHETIC: WEBSERVICE 0x102245c0
// `dynamic atexit destructor for 'RBX::BodyForce::prop_Force''
// SYNTHETIC: WEBSERVICE 0x102245e0
// `dynamic atexit destructor for 'RBX::BodyThrust::prop_location''
// SYNTHETIC: WEBSERVICE 0x10224600
// `dynamic atexit destructor for 'RBX::BodyThrust::prop_force''
// SYNTHETIC: WEBSERVICE 0x10224620
// `dynamic atexit destructor for 'RBX::Rocket::prop_MaxSpeed''
// SYNTHETIC: WEBSERVICE 0x10224640
// `dynamic atexit destructor for 'RBX::Rocket::prop_targetRadius''
// SYNTHETIC: WEBSERVICE 0x10224660
// `dynamic atexit destructor for 'RBX::Rocket::prop_targetOffset''
// SYNTHETIC: WEBSERVICE 0x10224680
// `dynamic atexit destructor for 'RBX::BodyGyro::prop_cframe''
// SYNTHETIC: WEBSERVICE 0x10224700
// `dynamic atexit destructor for 'func_getLastForce''
// SYNTHETIC: WEBSERVICE 0x10224710
// `dynamic atexit destructor for 'func_getLastForceV''
// SYNTHETIC: WEBSERVICE 0x10224720
// `dynamic atexit destructor for 'RBX::Rocket::func_Abort''
// SYNTHETIC: WEBSERVICE 0x10224730
// `dynamic atexit destructor for 'func_getLastForceOld''
// SYNTHETIC: WEBSERVICE 0x10224740
// `dynamic atexit destructor for 'func_getLastForceVOld''
// SYNTHETIC: WEBSERVICE 0x10224750
// `dynamic atexit destructor for 'RBX::Rocket::func_Fire''
// SYNTHETIC: WEBSERVICE 0x10224760
// `dynamic atexit destructor for 'RBX::Rocket::event_ReachedTarget''
// clang-format on

// clang-format off
// SYNTHETIC: WEBSERVICE 0x100ee090
// RBX::Rocket::~Rocket
// SYNTHETIC: WEBSERVICE 0x100ee110
// RBX::Rocket::`scalar deleting destructor'
// clang-format on

void registerBodyMovers();

} // namespace RBX

#endif // V8DATAMODEL_GYRO_H
