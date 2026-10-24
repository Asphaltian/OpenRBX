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
	"Target",
	"Goals",
	&RBX::Rocket::getTarget,
	&RBX::Rocket::setTarget
);
Reflection::BoundProp<G3D::Vector3, 1> Rocket::prop_targetOffset(
	"TargetOffset",
	"Goals",
	&RBX::Rocket::targetOffset,
	&RBX::Rocket::onGoalChanged
);
Reflection::BoundProp<float, 1> Rocket::prop_targetRadius(
	"TargetRadius",
	"Goals",
	&RBX::Rocket::targetRadius,
	&RBX::Rocket::onGoalChanged
);
Reflection::BoundProp<float, 1> Rocket::prop_MaxSpeed("MaxSpeed", "Thrust", &RBX::Rocket::maxSpeed);
Reflection::BoundProp<float, 1> Rocket::prop_MaxThrust("MaxThrust", "Thrust", &RBX::Rocket::maxThrust);
Reflection::BoundProp<float, 1> Rocket::prop_ThrustP("ThrustP", "Thrust", &RBX::Rocket::kThrustP);
Reflection::BoundProp<float, 1> Rocket::prop_ThrustD("ThrustD", "Thrust", &RBX::Rocket::kThrustD);
Reflection::BoundProp<float, 1> Rocket::prop_TurnP("TurnP", "Turn", &RBX::Rocket::kTurnP);
Reflection::BoundProp<float, 1> Rocket::prop_TurnD("TurnD", "Turn", &RBX::Rocket::kTurnD);
Reflection::BoundProp<G3D::Vector3, 1> Rocket::prop_MaxTorque("MaxTorque", "Turn", &RBX::Rocket::maxTorque);
Reflection::BoundProp<float, 1> Rocket::prop_CartoonFactor("CartoonFactor", "Goals", &RBX::Rocket::cartoonFactor);
Reflection::BoundFuncDesc<Rocket, void(), 0> Rocket::func_Fire(
	&RBX::Rocket::fire,
	"Fire",
	Reflection::FunctionDescriptor::AnyCaller
);
Reflection::BoundFuncDesc<Rocket, void(), 0> Rocket::func_Abort(
	&RBX::Rocket::abort,
	"Abort",
	Reflection::FunctionDescriptor::AnyCaller
);
Reflection::SignalDesc<Rocket, void()> Rocket::event_ReachedTarget("ReachedTarget");

Reflection::BoundProp<float, 1> BodyGyro::prop_kP("P", "Goals", &RBX::BodyGyro::kP);
Reflection::BoundProp<float, 1> BodyGyro::prop_kD("D", "Goals", &RBX::BodyGyro::kD);
Reflection::BoundProp<G3D::Vector3, 1> BodyGyro::prop_maxTorque("maxTorque", "Goals", &RBX::BodyGyro::maxTorque);
Reflection::BoundProp<G3D::CoordinateFrame, 1> BodyGyro::prop_cframe("cframe", "Goals", &RBX::BodyGyro::cframe);

Reflection::BoundProp<float, 1> BodyPosition::prop_kP("P", "Goals", &RBX::BodyPosition::kP);
Reflection::BoundProp<float, 1> BodyPosition::prop_kD("D", "Goals", &RBX::BodyPosition::kD);
Reflection::BoundProp<G3D::Vector3, 1> BodyPosition::prop_maxForce("maxForce", "Goals", &RBX::BodyPosition::maxForce);
Reflection::BoundProp<G3D::Vector3, 1> BodyPosition::prop_position("position", "Goals", &RBX::BodyPosition::position);

} // namespace RBX

RBX::Reflection::BoundFuncDesc<RBX::BodyPosition, G3D::Vector3(), 0> func_getLastForceOld(
	&RBX::BodyPosition::getLastForce,
	"lastForce",
	RBX::Reflection::FunctionDescriptor::AnyCaller
);
RBX::Reflection::BoundFuncDesc<RBX::BodyPosition, G3D::Vector3(), 0> func_getLastForce(
	&RBX::BodyPosition::getLastForce,
	"GetLastForce",
	RBX::Reflection::FunctionDescriptor::AnyCaller
);

namespace RBX {

Reflection::BoundProp<float, 1> BodyVelocity::prop_kP("P", "Goals", &RBX::BodyVelocity::kP);
Reflection::BoundProp<G3D::Vector3, 1> BodyVelocity::prop_maxForce("maxForce", "Goals", &RBX::BodyVelocity::maxForce);
Reflection::BoundProp<G3D::Vector3, 1> BodyVelocity::prop_velocity("velocity", "Goals", &RBX::BodyVelocity::velocity);

} // namespace RBX

RBX::Reflection::BoundFuncDesc<RBX::BodyVelocity, G3D::Vector3(), 0> func_getLastForceVOld(
	&RBX::BodyVelocity::getLastForce,
	"lastForce",
	RBX::Reflection::FunctionDescriptor::AnyCaller
);
RBX::Reflection::BoundFuncDesc<RBX::BodyVelocity, G3D::Vector3(), 0> func_getLastForceV(
	&RBX::BodyVelocity::getLastForce,
	"GetLastForce",
	RBX::Reflection::FunctionDescriptor::AnyCaller
);

namespace RBX {

Reflection::BoundProp<G3D::Vector3, 1> BodyForce::prop_Force("force", "Goals", &RBX::BodyForce::force);

Reflection::BoundProp<G3D::Vector3, 1> BodyThrust::prop_force("force", "Goals", &RBX::BodyThrust::force);
Reflection::BoundProp<G3D::Vector3, 1> BodyThrust::prop_location("location", "Goals", &RBX::BodyThrust::location);

// FUNCTION: WEBSERVICE 0x100ea070 FOLDED
bool BodyMover::askSetParent(const Instance* instance) const
{
	return dynamic_cast<const PartInstance*>(instance) != NULL;
}

// FUNCTION: WEBSERVICE 0x100ea0a0
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

// FUNCTION: WEBSERVICE 0x100ea170
void Rocket::abort()
{
	active = false;
}

// FUNCTION: WEBSERVICE 0x100ea200
BodyMover::BodyMover(const char* name) : Instance(name), world(NULL), part(NULL)
{
}

// FUNCTION: WEBSERVICE 0x100ea340
BodyMover::~BodyMover()
{
}

// FUNCTION: WEBSERVICE 0x100ea3b0
void BodyMover::onEvent(const RunService* source, Stepped event)
{
	if (preventBodySleep()) {
		world->ticklePrimitive(part->getPrimitive(), true);
	}
}

// STUB: WEBSERVICE 0x100ea7c0
void BodyGyro::computeOrientation(Body* body)
{
	STUB(0x100ea7c0);
}

// STUB: WEBSERVICE 0x100eaaf0
void BodyGyro::computeBalance(Body* body)
{
	STUB(0x100eaaf0);
}

// STUB: WEBSERVICE 0x100eaf60
void BodyPosition::computeForce(float dt, bool throttling)
{
	Body* body = part->getPrimitive()->getBody();

	Vector3 positionForce = (position - body->getPV().position.translation) * kP;

	Vector3 goalForce = positionForce + body->getPV().velocity.linear * -kD;

	lastForce = goalForce * body->getRoot()->getBranchMass();

	lastForce = lastForce.clamp(-maxForce, maxForce);

	body->getRoot()->accumulateForceAtBranchCofm(lastForce);
}

// FUNCTION: WEBSERVICE 0x100eb0c0
void BodyVelocity::computeForce(float dt, bool throttling)
{
	Body* body = part->getPrimitive()->getBody();

	Vector3 goalForce = (velocity - body->getPV().velocity.linear) * kP;

	lastForce = goalForce * body->getRoot()->getBranchMass();

	lastForce = lastForce.clamp(-maxForce, maxForce);

	body->getRoot()->accumulateForceAtBranchCofm(lastForce);
}

// FUNCTION: WEBSERVICE 0x100eb1e0
void BodyForce::computeForce(float dt, bool throttling)
{
	part->getPrimitive()->getBody()->getRoot()->accumulateForceAtBranchCofm(force);
}

// FUNCTION: WEBSERVICE 0x100eb240
void BodyThrust::computeForce(float dt, bool throttling)
{
	Body* body = part->getPrimitive()->getBody();

	Vector3 worldForce = body->getPV().position.vectorToWorldSpace(force);
	Vector3 worldPos = body->getPV().position.pointToWorldSpace(location);

	body->accumulateForce(worldForce, worldPos);
}

// STUB: WEBSERVICE 0x100ebb10
void Rocket::computeForce(float dt, bool throttling)
{
	STUB(0x100ebb10);
}

// STUB: WEBSERVICE 0x100ebfb0
void BodyGyro::computeForce(float dt, bool throttling)
{
	Body* body = part->getPrimitive()->getBody();

	computeBalance(body);
	computeOrientation(body);
}

// FUNCTION: WEBSERVICE 0x100ed460
BodyGyro::BodyGyro()
	: DescribedCreatable<BodyGyro, BodyMover, sBodyGyro>(sBodyGyro), kP(3000.0f), kD(500.0f),
	  maxTorque(400000.0f, 0.0f, 400000.0f)
{
}

// FUNCTION: WEBSERVICE 0x100ed570
BodyPosition::BodyPosition()
	: DescribedCreatable<BodyPosition, BodyMover, sBodyPosition>(sBodyPosition), kP(10000.0f), kD(1250.0f),
	  maxForce(4000.0f, 4000.0f, 4000.0f), position(0.0f, 50.0f, 0.0f)
{
}

// FUNCTION: WEBSERVICE 0x100ed6a0
BodyVelocity::BodyVelocity()
	: DescribedCreatable<BodyVelocity, BodyMover, sBodyVelocity>(sBodyVelocity), kP(1250.0f),
	  maxForce(4000.0f, 4000.0f, 4000.0f), velocity(0.0f, 2.0f, 0.0f)
{
}

// FUNCTION: WEBSERVICE 0x100ed7b0
bool BodyVelocity::preventBodySleep() const
{
	return velocity != G3D::Vector3::zero();
}

// FUNCTION: WEBSERVICE 0x100ed840
BodyForce::BodyForce() : DescribedCreatable<BodyForce, BodyMover, sBodyForce>(sBodyForce), force(G3D::Vector3::unitY())
{
}

// FUNCTION: WEBSERVICE 0x100ed950 FOLDED
bool BodyForce::preventBodySleep() const
{
	return force != G3D::Vector3::zero();
}

// FUNCTION: WEBSERVICE 0x100ed950 FOLDED
bool BodyThrust::preventBodySleep() const
{
	return force != G3D::Vector3::zero();
}

// FUNCTION: WEBSERVICE 0x100ed9e0
BodyThrust::BodyThrust()
	: DescribedCreatable<BodyThrust, BodyMover, sBodyThrust>(sBodyThrust), force(G3D::Vector3::unitY()),
	  location(G3D::Vector3::zero())
{
}

// FUNCTION: WEBSERVICE 0x100eded0
Rocket::Rocket()
	: DescribedCreatable<Rocket, BodyMover, sRocket>(sRocket), active(false), targetRadius(4.0f), firedEvent(false),
	  maxThrust(4000.0f), kThrustP(5.0f), kThrustD(0.001f), maxSpeed(30.0f), kTurnP(3000.0f), kTurnD(500.0f),
	  maxTorque(400000.0f, 400000.0f, 0.0f), cartoonFactor(0.7f)
{
}

// FUNCTION: WEBSERVICE 0x100ee130
void BodyMover::updateWorld()
{
	World* newWorld = part ? Workspace::getWorldIfInWorkspace(this) : NULL;

	if (world != newWorld) {
		if (world) {
			world->getKernel()->removeConnector2ndPass(this);
		}

		world = newWorld;

		if (world) {
			world->getKernel()->insertConnector2ndPass(this);

			startListening(ServiceProvider::create<RunService>(this), this);
		}
	}
}

// FUNCTION: WEBSERVICE 0x100ee1b0
void BodyMover::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	stopListening(oldProvider ? oldProvider->create<RunService>() : NULL, this);

	Instance::onServiceProvider(oldProvider, newProvider);

	updateWorld();
}

// FUNCTION: WEBSERVICE 0x100ee200
void BodyMover::onAncestorChanged(const AncestorChanged& event)
{
	Instance::onAncestorChanged(event);

	if (event.child == this) {
		part = dynamic_cast<PartInstance*>(event.newParent);
	}

	updateWorld();
}

// FUNCTION: WEBSERVICE 0x100ee350
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
