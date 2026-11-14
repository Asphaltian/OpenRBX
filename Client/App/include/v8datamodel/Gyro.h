#ifndef V8DATAMODEL_GYRO_H
#define V8DATAMODEL_GYRO_H

#include "placeholder.h"
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
class BodyMover : public Instance, public Connector, public Listener<RunService, Stepped>
{
protected:
	BodyMover(const char* name);

public:
	virtual ~BodyMover();
	virtual bool askSetParent(const Instance* instance) const;

protected:
	virtual bool preventBodySleep() const { return true; }

	virtual void onAncestorChanged(const AncestorChanged& event);
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
	virtual void onEvent(const RunService* source, Stepped event);

private:
	void updateWorld();

protected:
	World* world;
	PartInstance* part;
};

class BodyForce : public DescribedCreatable<BodyForce, BodyMover, sBodyForce>
{
public:
	BodyForce();

	static Reflection::BoundProp<G3D::Vector3, 1> prop_Force;

	virtual void computeForce(float dt, bool throttling);

protected:
	virtual bool preventBodySleep() const;

private:
	G3D::Vector3 force;
};

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

	float kP;
	float kD;
	G3D::Vector3 maxTorque;
	G3D::CoordinateFrame cframe;
};

class BodyPosition : public DescribedCreatable<BodyPosition, BodyMover, sBodyPosition>
{
public:
	BodyPosition();
	G3D::Vector3 getLastForce() { return lastForce; }

	static Reflection::BoundProp<float, 1> prop_kP;
	static Reflection::BoundProp<float, 1> prop_kD;
	static Reflection::BoundProp<G3D::Vector3, 1> prop_maxForce;
	static Reflection::BoundProp<G3D::Vector3, 1> prop_position;

	virtual void computeForce(float dt, bool throttling);

private:
	float kP;
	float kD;
	G3D::Vector3 maxForce;
	G3D::Vector3 position;
	G3D::Vector3 lastForce;
};

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
	G3D::Vector3 force;
	G3D::Vector3 location;
};

class BodyVelocity : public DescribedCreatable<BodyVelocity, BodyMover, sBodyVelocity>
{
public:
	BodyVelocity();
	G3D::Vector3 getLastForce() { return lastForce; }

	static Reflection::BoundProp<float, 1> prop_kP;
	static Reflection::BoundProp<G3D::Vector3, 1> prop_maxForce;
	static Reflection::BoundProp<G3D::Vector3, 1> prop_velocity;

	virtual void computeForce(float dt, bool throttling);

protected:
	virtual bool preventBodySleep() const;

private:
	float kP;
	G3D::Vector3 maxForce;
	G3D::Vector3 velocity;
	G3D::Vector3 lastForce;
};

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

	void onGoalChanged(const Reflection::PropertyDescriptor&) { firedEvent = false; }

	bool active;
	boost::shared_ptr<PartInstance> target;
	G3D::Vector3 targetOffset;
	float targetRadius;
	bool firedEvent;
	float maxThrust;
	float kThrustP;
	float kThrustD;
	float maxSpeed;
	float kTurnP;
	float kTurnD;
	G3D::Vector3 maxTorque;
	float cartoonFactor;
};

void registerBodyMovers();

} // namespace RBX

#endif // V8DATAMODEL_GYRO_H
