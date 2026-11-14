#include "v8kernel/Connector.h"

#include "placeholder.h"
#include "util/Math.h"
#include "v8kernel/Body.h"
#include "v8kernel/SimBody.h"

#include <G3D/g3dmath.h>
#include <cmath>
#include <cstddef>

namespace RBX {

float Connector::potentialEnergy()
{
	return 0.0f;
}

bool Connector::canThrottle() const
{
	return false;
}

bool Connector::getBroken()
{
	return false;
}

RotateConnector::RotateConnector(Point* base0, Point* ray0, Point* ref0, Point* ref1, float kValue, float armLength)
	: k(kValue * armLength * armLength), base0(base0), ray0(ray0), ref0(ref0), ref1(ref1), lastRotation(0.0f),
	  windings(0)
{
}

float PointToPointBreakConnector::potentialEnergy()
{
	Vector3 delta = point1->getWorldPos() - point0->getWorldPos();

	float length = delta.magnitude();

	return k * length * length * 0.5f;
}

bool ContactConnector::canThrottle() const
{
	return geoPair.getBody(0)->getCanThrottle() && geoPair.getBody(1)->getCanThrottle();
}

void RotateConnector::computeParams(Vector3& normal, float& rotation, float& rotVel)
{
	normal = ray0->getWorldPos() - base0->getWorldPos();
	normal.unitize();

	Vector3 rayRef0 = ref0->getWorldPos() - base0->getWorldPos();
	Vector3 rayRef1 = ref1->getWorldPos() - base0->getWorldPos();

	float d0 = rayRef0.dot(normal);
	float d1 = rayRef1.dot(normal);

	Vector3 rayP0plane = rayRef0 - normal * d0;
	Vector3 rayP1plane = rayRef1 - normal * d1;

	Vector3 cross = rayP0plane.cross(rayP1plane);

	float angle = atan2f(cross.dot(normal), rayP1plane.dot(rayP0plane));

	int lastWindings = windings;

	if (lastRotation > 1.5707963267949) {
		if (angle < -1.5707963267949) {
			windings = lastWindings + 1;
		}
	}
	else if (lastRotation < -1.5707963267949 && angle > 1.5707963267949) {
		windings = lastWindings - 1;
	}

	rotation = windings * 6.283185f + angle;
	rotVel = rotation - (lastWindings * 6.283185f + lastRotation);

	lastRotation = angle;
}

void RotateConnector::computeForce(float dt, bool throttling)
{
	Vector3 normal(0.0f, 0.0f, 0.0f);
	float rotation;
	float rotVel;

	computeParams(normal, rotation, rotVel);

	KernelInput& input = getKernelInput();

	if (input.latchToZero) {
		input.latchToZero = false;
		input.currentGoal = (input.currentGoal + rotation) * 0.5f;
	}

	float torqueMag = (input.get() - rotation) * k;

	Vector3 torque = normal * torqueMag;

	ref0->getBody()->accumulateTorque(-torque);
	ref1->getBody()->accumulateTorque(torque);
}

void PointToPointBreakConnector::computeForce(float dt, bool throttling)
{
	if (!broken) {
		Vector3 force = (point1->getWorldPos() - point0->getWorldPos()) * -k;

		broken = breakForce < Math::taxiCabMagnitude(force);

		forceToPoints(force);
	}
}

void NormalBreakConnector::computeForce(float dt, bool throttling)
{
	if (!broken) {
		const CoordinateFrame& coord0 = point0->getBody()->getCoordinateFrame();

		Vector3 normal = Math::getWorldNormal(normalIdBody0, coord0);

		Vector3 force = (point1->getWorldPos() - point0->getWorldPos()) * -k;

		broken = breakForce < -force.dot(normal);

		forceToPoints(force);
	}
}

void ContactConnector::computeForce(const float dt, bool throttling)
{
	PairParams params;

	params.position = params.normal = Vector3(0.0f, 0.0f, 0.0f);
	geoPair.computeLengthNormalPosition(params);

	if (params.length < 0.0f) {
		float normalVel;
		Vector3 perpVel(0.0f, 0.0f, 0.0f);

		geoPair.computeNormalPerpVel(normalVel, perpVel, params);

		frictionOffset += dt * perpVel;

		float kApplied = k * 0.2f;
		float magnitude = frictionOffset.magnitude(), frictionForce = magnitude * kApplied;
		float maxFriction = kFriction * forceMagLast;

		if (frictionForce > maxFriction && frictionForce > 1e-08) {
			frictionOffset *= maxFriction / frictionForce;
		}

		frictionOffset -= params.normal * frictionOffset.dot(params.normal);

		threshold =
			threshold != 0.0f ? (threshold + 0.01f) * 0.999f - 0.01f : (firstApproach == 0.0f ? 0.0f : params.length);

		firstApproach = firstApproach != 0.0f ? (firstApproach + 0.01f) * 0.999f - 0.01f : params.length;

		forceMagLast = (firstApproach - params.length) * (normalVel < 0.0f ? k : kNeg);
		forceMagLast = params.length <= threshold ? forceMagLast : 0.0f;

		Vector3 force = forceMagLast * params.normal - frictionOffset * kApplied;

		geoPair.forceToBodies(force, params.position);
	}
	else {
		forceMagLast = threshold = firstApproach = 0.0f;
	}
}

bool PointToPointBreakConnector::getBroken()
{
	return broken;
}

} // namespace RBX
