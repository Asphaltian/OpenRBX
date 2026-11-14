#include "v8datamodel/Gyro.h"

#include "network/Players.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Workspace.h"
#include "v8kernel/Body.h"
#include "v8kernel/Kernel.h"
#include "v8kernel/SimBody.h"
#include "v8world/Primitive.h"
#include "v8world/World.h"

namespace RBX {

char sRocket[] = "RocketPropulsion";

char sBodyVelocity[] = "BodyVelocity";

char sBodyPosition[] = "BodyPosition";

char sBodyThrust[] = "BodyThrust";

char sBodyForce[] = "BodyForce";

char sBodyGyro[] = "BodyGyro";

Reflection::RefPropDescriptor<Rocket, PartInstance> Rocket::prop_Target(
	"Target", "Goals", &Rocket::getTarget, &Rocket::setTarget
);

Reflection::BoundProp<G3D::Vector3, 1> Rocket::prop_targetOffset(
	"TargetOffset", "Goals", &Rocket::targetOffset, &Rocket::onGoalChanged
);

Reflection::BoundProp<float, 1> Rocket::prop_targetRadius(
	"TargetRadius", "Goals", &Rocket::targetRadius, &Rocket::onGoalChanged
);

Reflection::BoundProp<float, 1> Rocket::prop_MaxSpeed("MaxSpeed", "Thrust", &Rocket::maxSpeed);
Reflection::BoundProp<float, 1> Rocket::prop_MaxThrust("MaxThrust", "Thrust", &Rocket::maxThrust);
Reflection::BoundProp<float, 1> Rocket::prop_ThrustP("ThrustP", "Thrust", &Rocket::kThrustP);
Reflection::BoundProp<float, 1> Rocket::prop_ThrustD("ThrustD", "Thrust", &Rocket::kThrustD);
Reflection::BoundProp<float, 1> Rocket::prop_TurnP("TurnP", "Turn", &Rocket::kTurnP);
Reflection::BoundProp<float, 1> Rocket::prop_TurnD("TurnD", "Turn", &Rocket::kTurnD);
Reflection::BoundProp<G3D::Vector3, 1> Rocket::prop_MaxTorque("MaxTorque", "Turn", &Rocket::maxTorque);
Reflection::BoundProp<float, 1> Rocket::prop_CartoonFactor("CartoonFactor", "Goals", &Rocket::cartoonFactor);
Reflection::BoundFuncDesc<Rocket, void(), 0> Rocket::func_Fire(
	&Rocket::fire, "Fire", Reflection::FunctionDescriptor::AnyCaller
);

Reflection::BoundFuncDesc<Rocket, void(), 0> Rocket::func_Abort(
	&Rocket::abort, "Abort", Reflection::FunctionDescriptor::AnyCaller
);

Reflection::SignalDesc<Rocket, void()> Rocket::event_ReachedTarget("ReachedTarget");

Reflection::BoundProp<float, 1> BodyGyro::prop_kP("P", "Goals", &BodyGyro::kP);
Reflection::BoundProp<float, 1> BodyGyro::prop_kD("D", "Goals", &BodyGyro::kD);
Reflection::BoundProp<G3D::Vector3, 1> BodyGyro::prop_maxTorque("maxTorque", "Goals", &BodyGyro::maxTorque);
Reflection::BoundProp<G3D::CoordinateFrame, 1> BodyGyro::prop_cframe("cframe", "Goals", &BodyGyro::cframe);

Reflection::BoundProp<float, 1> BodyPosition::prop_kP("P", "Goals", &BodyPosition::kP);
Reflection::BoundProp<float, 1> BodyPosition::prop_kD("D", "Goals", &BodyPosition::kD);
Reflection::BoundProp<G3D::Vector3, 1> BodyPosition::prop_maxForce("maxForce", "Goals", &BodyPosition::maxForce);
Reflection::BoundProp<G3D::Vector3, 1> BodyPosition::prop_position("position", "Goals", &BodyPosition::position);

} // namespace RBX

RBX::Reflection::BoundFuncDesc<RBX::BodyPosition, G3D::Vector3(), 0> func_getLastForceOld(
	&RBX::BodyPosition::getLastForce, "lastForce", RBX::Reflection::FunctionDescriptor::AnyCaller
);

RBX::Reflection::BoundFuncDesc<RBX::BodyPosition, G3D::Vector3(), 0> func_getLastForce(
	&RBX::BodyPosition::getLastForce, "GetLastForce", RBX::Reflection::FunctionDescriptor::AnyCaller
);

namespace RBX {

Reflection::BoundProp<float, 1> BodyVelocity::prop_kP("P", "Goals", &BodyVelocity::kP);
Reflection::BoundProp<G3D::Vector3, 1> BodyVelocity::prop_maxForce("maxForce", "Goals", &BodyVelocity::maxForce);
Reflection::BoundProp<G3D::Vector3, 1> BodyVelocity::prop_velocity("velocity", "Goals", &BodyVelocity::velocity);

} // namespace RBX

RBX::Reflection::BoundFuncDesc<RBX::BodyVelocity, G3D::Vector3(), 0> func_getLastForceVOld(
	&RBX::BodyVelocity::getLastForce, "lastForce", RBX::Reflection::FunctionDescriptor::AnyCaller
);

RBX::Reflection::BoundFuncDesc<RBX::BodyVelocity, G3D::Vector3(), 0> func_getLastForceV(
	&RBX::BodyVelocity::getLastForce, "GetLastForce", RBX::Reflection::FunctionDescriptor::AnyCaller
);

namespace RBX {

Reflection::BoundProp<G3D::Vector3, 1> BodyForce::prop_Force("force", "Goals", &BodyForce::force);

Reflection::BoundProp<G3D::Vector3, 1> BodyThrust::prop_force("force", "Goals", &BodyThrust::force);
Reflection::BoundProp<G3D::Vector3, 1> BodyThrust::prop_location("location", "Goals", &BodyThrust::location);

bool BodyMover::askSetParent(const Instance* instance) const
{
	return dynamic_cast<const PartInstance*>(instance) != NULL;
}

void Rocket::fire()
{
	if (!Workspace::findWorkspace(this)) {
		throw std::runtime_error("Rocket:Fire may only be called when the Rocket is in the Workspace");
	}

	if (Network::Players::clientIsPresent(this, true)) {
		throw std::runtime_error("Rocket:Fire may not be called from a client");
	}

	active = true;
}

void Rocket::abort()
{
	active = false;
}

BodyMover::BodyMover(const char* name) : Instance(name), world(NULL), part(NULL)
{
}

BodyMover::~BodyMover()
{
}

void BodyMover::onEvent(const RunService* source, Stepped event)
{
	if (preventBodySleep()) {
		world->ticklePrimitive(part->getPrimitive(), true);
	}
}

Vector3 Rocket::turn(Body* body, const Vector3& targetDir)
{
	Vector3 targetDirBody = body->getPV().position.vectorToObjectSpace(targetDir);

	Body* root = body->getRoot();

	Vector3 oldTorqueWorld = root->getBranchTorque();

	const float desiredTorqueX = kTurnP * root->getBranchIBodyV3().x * targetDirBody.y;
	const float desiredTorqueY = -kTurnP * root->getBranchIBodyV3().y * targetDirBody.x;

	Vector3 torqueBody = body->getPV().position.vectorToObjectSpace(oldTorqueWorld);

	if (fabs(desiredTorqueX - torqueBody.x) < root->getBranchIBodyV3().x * maxTorque.x) {
		torqueBody.x = desiredTorqueX;
	}
	else {
		torqueBody.x += desiredTorqueX;
	}

	if (fabs(desiredTorqueY - torqueBody.y) < root->getBranchIBodyV3().y * maxTorque.y) {
		torqueBody.y = desiredTorqueY;
	}
	else {
		torqueBody.y += desiredTorqueY;
	}

	Vector3 angVelBody = body->getPV().position.vectorToObjectSpace(body->getPV().velocity.rotational);

	torqueBody.x -= kTurnD * (root->getBranchIBodyV3().x * angVelBody.x);
	torqueBody.y -= kTurnD * (root->getBranchIBodyV3().y * angVelBody.y);

	Vector3 addedTorque = body->getPV().position.vectorToWorldSpace(torqueBody) - oldTorqueWorld;

	root->accumulateTorque(addedTorque);

	return targetDirBody;
}

void BodyGyro::computeOrientation(Body* body)
{
	if (!G3D::fuzzyEq(maxTorque.y, 0.0f)) {
		Body* root = body->getRoot();

		Vector3 oldTorqueWorld = root->getBranchTorque();

		Vector3 localZNegAxis = body->getPV().position.vectorToObjectSpace(cframe.lookVector());

		const float desiredTorqueY = -kP * root->getBranchIBodyV3().y * localZNegAxis.x;

		Vector3 torqueBody = body->getPV().position.vectorToObjectSpace(oldTorqueWorld);

		if (fabs(desiredTorqueY - torqueBody.y) < root->getBranchIBodyV3().y * maxTorque.y) {
			torqueBody.y = desiredTorqueY;
		}
		else {
			torqueBody.y += desiredTorqueY;
		}

		Vector3 angVelBody = body->getPV().position.vectorToObjectSpace(body->getPV().velocity.rotational);

		torqueBody.y -= kD * root->getBranchIBodyV3().y * angVelBody.y;

		Vector3 addedTorque = body->getPV().position.vectorToWorldSpace(torqueBody) - oldTorqueWorld;
		root->accumulateTorque(addedTorque);
	}
}

void BodyGyro::computeBalance(Body* body)
{
	if (!G3D::fuzzyEq(maxTorque.x, 0.0f) || !G3D::fuzzyEq(maxTorque.z, 0.0f)) {
		Body* root = body->getRoot();

		Vector3 oldTorqueWorld = root->getBranchTorque();

		Vector3 localYAxis = body->getPV().position.vectorToObjectSpace(cframe.upVector());

		const float desiredTorqueX = kP * root->getBranchIBodyV3().x * localYAxis.z;
		const float desiredTorqueZ = -kP * root->getBranchIBodyV3().z * localYAxis.x;

		Vector3 torqueBody = body->getPV().position.vectorToObjectSpace(oldTorqueWorld);

		if (fabs(desiredTorqueX - torqueBody.x) < root->getBranchIBodyV3().x * maxTorque.x) {
			torqueBody.x = desiredTorqueX;
		}
		else {
			torqueBody.x += desiredTorqueX;
		}

		if (fabs(desiredTorqueZ - torqueBody.z) < root->getBranchIBodyV3().z * maxTorque.z) {
			torqueBody.z = desiredTorqueZ;
		}
		else {
			torqueBody.z += desiredTorqueZ;
		}

		Vector3 angVelBody = body->getPV().position.vectorToObjectSpace(body->getPV().velocity.rotational);

		torqueBody.x -= kD * root->getBranchIBodyV3().x * angVelBody.x;
		torqueBody.z -= kD * root->getBranchIBodyV3().z * angVelBody.z;

		Vector3 addedTorque = body->getPV().position.vectorToWorldSpace(torqueBody) - oldTorqueWorld;

		root->accumulateTorque(addedTorque);
	}
}

void BodyPosition::computeForce(float dt, bool throttling)
{
	Body* body = part->getPrimitive()->getBody();

	Vector3 pAccel = (position - body->getPV().position.translation) * kP;

	lastForce = body->getRoot()->getBranchMass() * (pAccel + body->getPV().velocity.linear * -kD);

	lastForce = lastForce.clamp(-maxForce, maxForce);

	body->getRoot()->accumulateForceAtBranchCofm(lastForce);
}

void BodyVelocity::computeForce(float dt, bool throttling)
{
	Body* body = part->getPrimitive()->getBody();

	Vector3 pAccel = (velocity - body->getPV().velocity.linear) * kP;

	lastForce = pAccel * body->getRoot()->getBranchMass();

	lastForce = lastForce.clamp(-maxForce, maxForce);

	body->getRoot()->accumulateForceAtBranchCofm(lastForce);
}

void BodyForce::computeForce(float dt, bool throttling)
{
	part->getPrimitive()->getBody()->getRoot()->accumulateForceAtBranchCofm(force);
}

void BodyThrust::computeForce(float dt, bool throttling)
{
	Body* body = part->getPrimitive()->getBody();

	Vector3 worldForce = body->getPV().position.vectorToWorldSpace(force);
	Vector3 worldPos = body->getPV().position.pointToWorldSpace(location);

	body->accumulateForce(worldForce, worldPos);
}

void Rocket::computeForce(float dt, bool throttling)
{
	if (active) {
		Body* body = part->getPrimitive()->getBody();

		Vector3 targetPos = target ? target->getCoordinateFrame().pointToWorldSpace(targetOffset) : targetOffset;
		targetPos = targetPos - body->getPV().position.translation;

		Body* root = body->getRoot();

		Vector3 targetDir = targetPos.direction();
		Vector3 pAccel = targetPos * kThrustP;

		Vector3 force = pAccel * root->getBranchMass();

		if (root->getPV().velocity.linear.squaredLength() > maxSpeed * maxSpeed) {
			Vector3 velDir = root->getPV().velocity.linear.direction();
			float dot = velDir.dot(force);

			if (dot > 0.0f) {
				force -= velDir * dot;
				force -= root->getBranchMass() * (root->getPV().velocity.linear - velDir * maxSpeed) / dt;
			}
		}

		force -= root->getBranchMass() * kThrustD * root->getPV().velocity.linear;
		force -= body->getBranchForce();

		Math::fixDenorm(force);

		force = force.clamp(Vector3(-maxThrust, -maxThrust, -maxThrust), Vector3(maxThrust, maxThrust, maxThrust));

		root->accumulateForceAtBranchCofm(force);

		const Vector3& forceDir = force.squaredLength() > 1e-6f ? force.direction() : Vector3::zero();

		turn(body, (targetDir * cartoonFactor + forceDir * (1.0f - cartoonFactor)).direction());
	}
}

void BodyGyro::computeForce(float dt, bool throttling)
{
	Body* body = part->getPrimitive()->getBody();

	computeBalance(body);
	computeOrientation(body);
}

void registerBodyMovers()
{
	BodyGyro::className();
	BodyPosition::className();
	BodyVelocity::className();
	BodyForce::className();
	BodyThrust::className();
	Rocket::className();
}

BodyGyro::BodyGyro()
	: DescribedCreatable<BodyGyro, BodyMover, sBodyGyro>(sBodyGyro), kP(3000.0f), kD(500.0f),
	  maxTorque(400000.0f, 0.0f, 400000.0f)
{
}

BodyPosition::BodyPosition()
	: DescribedCreatable<BodyPosition, BodyMover, sBodyPosition>(sBodyPosition), kP(10000.0f), kD(1250.0f),
	  maxForce(4000.0f, 4000.0f, 4000.0f), position(0.0f, 50.0f, 0.0f)
{
}

BodyVelocity::BodyVelocity()
	: DescribedCreatable<BodyVelocity, BodyMover, sBodyVelocity>(sBodyVelocity), kP(1250.0f),
	  maxForce(4000.0f, 4000.0f, 4000.0f), velocity(0.0f, 2.0f, 0.0f)
{
}

bool BodyVelocity::preventBodySleep() const
{
	return velocity != G3D::Vector3::zero();
}

BodyForce::BodyForce() : DescribedCreatable<BodyForce, BodyMover, sBodyForce>(sBodyForce), force(G3D::Vector3::unitY())
{
}

bool BodyForce::preventBodySleep() const
{
	return force != G3D::Vector3::zero();
}

bool BodyThrust::preventBodySleep() const
{
	return force != G3D::Vector3::zero();
}

BodyThrust::BodyThrust()
	: DescribedCreatable<BodyThrust, BodyMover, sBodyThrust>(sBodyThrust), force(G3D::Vector3::unitY()),
	  location(G3D::Vector3::zero())
{
}

Rocket::Rocket()
	: DescribedCreatable<Rocket, BodyMover, sRocket>(sRocket), active(false), targetRadius(4.0f), firedEvent(false),
	  maxThrust(4000.0f), kThrustP(5.0f), kThrustD(0.001f), maxSpeed(30.0f), kTurnP(3000.0f), kTurnD(500.0f),
	  maxTorque(400000.0f, 400000.0f, 0.0f), cartoonFactor(0.7f)
{
}

void BodyMover::updateWorld()
{
	World* newWorld = part ? Workspace::getWorldIfInWorkspace(this) : NULL;

	if (world != newWorld) {
		if (world) {
			world->getKernel().removeConnector2ndPass(this);
		}

		world = newWorld;

		if (world) {
			world->getKernel().insertConnector2ndPass(this);

			startListening(ServiceProvider::create<RunService>(this), this);
		}
	}
}

void BodyMover::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	stopListening(oldProvider ? oldProvider->create<RunService>() : NULL, this);

	Instance::onServiceProvider(oldProvider, newProvider);

	updateWorld();
}

void BodyMover::onAncestorChanged(const AncestorChanged& event)
{
	Instance::onAncestorChanged(event);

	if (event.child == this) {
		part = dynamic_cast<PartInstance*>(event.newParent);
	}

	updateWorld();
}

void Rocket::onEvent(const RunService* source, Stepped event)
{
	BodyMover::onEvent(source, event);

	if (active && !firedEvent && part) {
		G3D::Vector3 pos = part->getCoordinateFrame().translation;
		G3D::Vector3 goal = target ? target->getCoordinateFrame().pointToWorldSpace(targetOffset) : targetOffset;
		G3D::Vector3 delta = pos - goal;

		if (delta.squaredLength() <= targetRadius * targetRadius) {
			event_ReachedTarget.fire(this);
			firedEvent = true;
		}
	}
}

void Rocket::setTarget(PartInstance* value)
{
	if (target.get() != value) {
		target = shared_from(value);
		firedEvent = false;
		raisePropertyChanged(prop_Target);
	}
}

} // namespace RBX

template const RBX::FactoryProduct<RBX::BodyGyro, RBX::BodyMover, RBX::sBodyGyro>::Creator
	RBX::FactoryProduct<RBX::BodyGyro, RBX::BodyMover, RBX::sBodyGyro>::creator;
template const RBX::FactoryProduct<RBX::BodyForce, RBX::BodyMover, RBX::sBodyForce>::Creator
	RBX::FactoryProduct<RBX::BodyForce, RBX::BodyMover, RBX::sBodyForce>::creator;
template const RBX::FactoryProduct<RBX::BodyThrust, RBX::BodyMover, RBX::sBodyThrust>::Creator
	RBX::FactoryProduct<RBX::BodyThrust, RBX::BodyMover, RBX::sBodyThrust>::creator;
template const RBX::FactoryProduct<RBX::BodyPosition, RBX::BodyMover, RBX::sBodyPosition>::Creator
	RBX::FactoryProduct<RBX::BodyPosition, RBX::BodyMover, RBX::sBodyPosition>::creator;
template const RBX::FactoryProduct<RBX::BodyVelocity, RBX::BodyMover, RBX::sBodyVelocity>::Creator
	RBX::FactoryProduct<RBX::BodyVelocity, RBX::BodyMover, RBX::sBodyVelocity>::creator;
template const RBX::FactoryProduct<RBX::Rocket, RBX::BodyMover, RBX::sRocket>::Creator
	RBX::FactoryProduct<RBX::Rocket, RBX::BodyMover, RBX::sRocket>::creator;
