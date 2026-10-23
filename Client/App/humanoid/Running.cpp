#include "humanoid/Running.h"

#include "humanoid/FallingDown.h"
#include "humanoid/Jumping.h"
#include "humanoid/Seated.h"
#include "util/Math.h"
#include "util/Name.h"
#include "v8datamodel/GeometryService.h"
#include "v8kernel/Body.h"
#include "v8world/Geometry.h"
#include "v8world/Primitive.h"
#include "v8world/World.h"

#include <G3D/System.h>

namespace RBX {

const char sRunning[] = "Running";

const float Running::manualTurnSpeed = 4.0f;
const float Running::autoTurnSpeed = 8.0f;
const float Running::kTurnP = 7500.0f;
const float Running::kMoveP = 1250.0f;
const float Running::kAltitudeP = 10000.0f;
const float Running::kAltitudeD = 1250.0f;

// STUB: WEBSERVICE 0x10106260
void Running::onComputeForce(const float dt)
{
	STUB(0x10106260);
}

// STUB: WEBSERVICE 0x10106a80
bool Running::isFacingLadder(bool buffered)
{
	Body* body = humanoid->getTorsoBody();
	if (body == NULL) {
		return false;
	}

	if (buffered) {
		double now = G3D::System::getTick();
		if (now - lastLadderTime < 0.01666) {
			return lastLadderResult;
		}
		lastLadderTime = now;
	}

	CoordinateFrame cf = body->getCoordinateFrame();

	GeometryService* geometry = ServiceProvider::create<GeometryService>(humanoid);

	float first_space = 0.0f;
	float first_step = 0.0f;
	bool look_for_space = true;
	bool lookForStep = false;

	for (int i = 0; i < 21.0f; i++) {
		Ray rayInPart;
		float height = i / 7.0f;
		rayInPart.origin = Vector3(0.0f, height - 3.0f, 0.0f);
		rayInPart.direction = cf.lookVector() * 2.0f;

		Ray caster;
		caster.origin = cf.translation + rayInPart.origin;
		caster.direction = rayInPart.direction;

		Vector3 hitLoc = geometry->getHitLocationFilterDescendants(humanoid->getParent(), caster);
		float distance = (hitLoc - caster.origin).magnitude();

		if (distance < 0.7f) {
			if (look_for_space) {
				first_space = height;
				look_for_space = false;
				lookForStep = true;
			}
		}
		else if (lookForStep) {
			first_step = height - first_space;
			lookForStep = false;
		}
	}

	if (first_space < 2.45f && first_step > 0.0f && first_step < 2.45f) {
		lastLadderResult = true;
		return true;
	}

	lastLadderResult = false;
	return false;
}

// FUNCTION: WEBSERVICE 0x10106d70
Running::Running(Humanoid* humanoid)
	: Named<Balancing, sRunning>(humanoid, 300.0f, 50.0f), floorRotationalVelocity(0.0f), jumpWarmup(-0.45f),
	  lastLadderResult(false), lastLadderTime(0.0), desiredYRotationalVelocity(0.0f), desiredAltitude(0.0f),
	  intendedYRotation(0.0f)
{
	if (humanoid->getLeftLegPrimitive()) {
		humanoid->getLeftLegPrimitive()->setCanCollide(false);
	}
	if (humanoid->getRightLegPrimitive()) {
		humanoid->getRightLegPrimitive()->setCanCollide(false);
	}
	Humanoid::event_Running.fire(humanoid, 0.0f);
}

// FUNCTION: WEBSERVICE 0x10106ea0
Running::~Running()
{
	Humanoid::event_Running.fire(humanoid, 0.0f);

	if (humanoid->getLeftLegPrimitive()) {
		humanoid->getLeftLegPrimitive()->setCanCollide(true);
	}
	if (humanoid->getRightLegPrimitive()) {
		humanoid->getRightLegPrimitive()->setCanCollide(true);
	}
}

// FUNCTION: WEBSERVICE 0x100d15c0 FOLDED
void Running::render3dAdorn(Adorn* adorn)
{
}

// STUB: WEBSERVICE 0x10106f80
void Running::updateGoals(Controller& controller)
{
	float oldMag = desiredVelocity.magnitude();
	desiredVelocity = humanoid->updateWalkDirection() * 16.0f;

	bool climbing = false;
	desiredYRotationalVelocity = 0.0f;
	if (!desiredVelocity.isZero()) {
		float headingError = Math::radWrap(Math::getHeading(desiredVelocity) - humanoid->getTorsoHeading());
		desiredYRotationalVelocity = headingError * autoTurnSpeed;

		if (isFacingLadder(true)) {
			climbing = true;
			float speed = desiredVelocity.magnitude();
			desiredVelocity.x *= 0.7f;
			desiredVelocity.y = speed * 0.7f;
			desiredVelocity.z *= 0.7f;
		}
	}

	float newMag = desiredVelocity.magnitude();
	if (newMag != oldMag) {
		if (climbing) {
			Humanoid::event_Climbing.fire(humanoid, newMag);
		}
		else {
			Humanoid::event_Running.fire(humanoid, newMag);
		}
	}

	intendedYRotation = desiredYRotationalVelocity;
}

// FUNCTION: WEBSERVICE 0x10107110
Humanoid::State* Running::onStep(const float stepDt, Controller& controller)
{
	State* answer = Balancing::onStep(stepDt, controller);
	if (answer != this) {
		return answer;
	}

	G3D::Vector3 yAxis = getYAxis();
	if (yAxis.y < fallCutoff()) {
		return new FallingDown(humanoid, 3.0f);
	}

	if (jumpWarmup >= 0.0f && humanoid->getJump()) {
		humanoid->setJump(false);
		return new Jumping(humanoid);
	}

	humanoid->setJump(false);

	if (humanoid->getSit()) {
		humanoid->setSit(false);
		return new Seated(humanoid);
	}

	if (floorPart) {
		humanoid->getWorld()->ticklePrimitive(floorPart->getPrimitive(), false);
	}

	desiredAltitude = floorTouchInWorld.y;

	if (humanoid->getTorsoPrimitive()) {
		desiredAltitude += humanoid->getTorsoPrimitive()->getGeometry()->getGridSize().y * 0.5f;
	}

	if (humanoid->getLeftLegPrimitive()) {
		desiredAltitude += humanoid->getLeftLegPrimitive()->getGeometry()->getGridSize().y;
	}
	else if (humanoid->getRightLegPrimitive()) {
		desiredAltitude += humanoid->getRightLegPrimitive()->getGeometry()->getGridSize().y;
	}

	const Velocity& floorVel = !floorPart
								   ? Velocity::zero()
								   : floorPart->getPrimitive()->getBody()->getPV().velocityAtPoint(floorTouchInWorld);
	floorVelocity = floorVel.linear;
	floorRotationalVelocity = floorVel.rotational.y;

	updateGoals(controller);

	return this;
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sRunning>();
template void RBX::Name::callDoDeclare<RBX::sRunning>();
