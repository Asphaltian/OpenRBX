#include "v8world/RotateJoint.h"

#include "decomp.h"
#include "util/Math.h"
#include "v8kernel/Body.h"
#include "v8kernel/Connector.h"
#include "v8kernel/Constants.h"
#include "v8kernel/Kernel.h"
#include "v8world/Controller.h"
#include "v8world/Geometry.h"
#include "v8world/Primitive.h"
#include "v8world/SurfaceData.h"
#include "v8world/Tolerance.h"
#include "v8world/World.h"

#include <algorithm>
#include <cmath>
#include <cstddef>

namespace RBX {

// FUNCTION: WEBSERVICE 0x1011edc0
RotateJoint::RotateJoint() : MultiJoint(2), rotateConnector(NULL)
{
}

// FUNCTION: WEBSERVICE 0x1011ede0
RotateJoint::RotateJoint(
	Primitive* prim0,
	Primitive* prim1,
	const CoordinateFrame& coord0,
	const CoordinateFrame& coord1
)
	: MultiJoint(prim0, prim1, coord0, coord1, 2), rotateConnector(NULL)
{
}

// FUNCTION: WEBSERVICE 0x1011ee20
RotateJoint::~RotateJoint()
{
}

// FUNCTION: WEBSERVICE 0x1011ee30
void RotateJoint::removeFromKernel()
{
	rotateConnector = NULL;

	MultiJoint::removeFromKernel();
}

// FUNCTION: WEBSERVICE 0x1011ee40
float RotateJoint::getChannelValue(int frameCount)
{
	const SurfaceData& surfaceData = getAxlePrim()->getSurfaceData(getAxleId());

	float value = getAxlePrim()->getController()->getValue(surfaceData.inputType);
	float paramA = surfaceData.paramA;
	float paramB = surfaceData.paramB;
	float time = Constants::uiDt() * frameCount;

	switch (surfaceData.inputType) {
	case Controller::CONSTANT_INPUT:
		return paramB;
	case Controller::SIN_INPUT:
		return sin(time * paramB) * paramA;
	case Controller::NO_INPUT:
		break;
	case Controller::LEFT_TRACK_INPUT:
		if (value < -0.1) {
			return fabs(paramA) * value;
		}
		if (0.1 < value) {
			return fabs(paramB) * value;
		}
		break;
	case Controller::RIGHT_TRACK_INPUT:
		if (value < -0.1) {
			return -(fabs(paramA) * value);
		}
		if (0.1 < value) {
			return -(fabs(paramB) * value);
		}
		break;
	default:
		if (value < -0.1) {
			return -(value * paramA);
		}
		if (0.1 < value) {
			return value * paramB;
		}
		break;
	}

	return 0.0f;
}

// FUNCTION: WEBSERVICE 0x1011f030
RotatePJoint::RotatePJoint(
	Primitive* prim0,
	Primitive* prim1,
	const CoordinateFrame& coord0,
	const CoordinateFrame& coord1
)
	: RotateJoint(prim0, prim1, coord0, coord1)
{
}

// FUNCTION: WEBSERVICE 0x1011f070
RotateVJoint::RotateVJoint(
	Primitive* prim0,
	Primitive* prim1,
	const CoordinateFrame& coord0,
	const CoordinateFrame& coord1
)
	: RotateJoint(prim0, prim1, coord0, coord1)
{
}

// FUNCTION: WEBSERVICE 0x1011f0e0
RotateJoint* RotateJoint::surfaceTypeToJoint(
	SurfaceType surfaceType,
	Primitive* prim0,
	Primitive* prim1,
	const CoordinateFrame& coord0,
	const CoordinateFrame& coord1
)
{
	switch (surfaceType) {
	case ROTATE:
		return new RotateJoint(prim0, prim1, coord0, coord1);
	case ROTATE_P:
		return new RotatePJoint(prim0, prim1, coord0, coord1);
	case ROTATE_V:
		return new RotateVJoint(prim0, prim1, coord0, coord1);
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x1011f200
RotateJoint* RotateJoint::canBuildJoint(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1)
{
	SurfaceType surfaceType0 = prim0->getSurfaceType(normalId0);
	SurfaceType surfaceType1 = prim1->getSurfaceType(normalId1);

	if (surfaceType0 < ROTATE && surfaceType1 < ROTATE) {
		return NULL;
	}

	const CoordinateFrame& coord0 = prim0->getCoordinateFrame();
	const CoordinateFrame& coord1 = prim1->getCoordinateFrame();

	Vector3 axis0 = Math::getWorldNormal(normalId0, coord0);
	Vector3 axis1 = Math::getWorldNormal(normalId1, coord1);

	if (0.025f < Math::angle(axis0, -axis1)) {
		return NULL;
	}

	Face face0 = prim0->getFaceInWorld(normalId0);
	Face face1 = prim1->getFaceInWorld(normalId1);

	if (!Face::hasOverlap(face0, face1, 0.35f)) {
		return NULL;
	}

	if (!Face::overlapWithinPlanes(face0, face1, 0.05f)) {
		return NULL;
	}

	Vector3 center0 = face0.center();
	Vector3 center1 = face1.center();

	bool contains0 = face1.fuzzyContainsInExtrusion(center0, 0.05f);
	bool contains1 = face0.fuzzyContainsInExtrusion(center1, 0.05f);

	bool axle0 = surfaceType0 >= ROTATE && contains0;
	bool axle1 = surfaceType1 >= ROTATE && contains1;

	if (!axle0 && !axle1) {
		return NULL;
	}

	if (!axle0 || (axle1 && prim1->getGeometry()->getGridSize().sum() > prim0->getGeometry()->getGridSize().sum())) {
		surfaceType0 = surfaceType1;
		std::swap(prim0, prim1);
		std::swap(normalId0, normalId1);
		std::swap(center0, center1);
		std::swap(axis0, axis1);
	}

	CoordinateFrame jointCoord0 = prim0->getFaceCoordInObject(normalId0);

	Vector3 worldPos0 = prim0->getCoordinateFrame().pointToWorldSpace(jointCoord0.translation);
	Vector3 objectPos = prim1->getCoordinateFrame().pointToObjectSpace(worldPos0);

	NormalId oppositeId = normalIdOpposite(normalId1);

	CoordinateFrame jointCoord1(normalIdToMatrix3(oppositeId), Math::toGrid(objectPos, 0.1f));

	Vector3 worldPos1 = prim1->getCoordinateFrame().pointToWorldSpace(jointCoord1.translation);

	if (Tolerance::pointsUnaligned(worldPos0, worldPos1)) {
		return NULL;
	}

	for (int i = 0; i < 2; ++i) {
		float offset = i == 0 ? 1.0f : -1.0f;
		Vector3 p0 = worldPos0 - axis0 * offset;
		Vector3 p1 = worldPos1 + axis1 * offset;

		if (Tolerance::pointsUnaligned(p0, p1)) {
			return NULL;
		}
	}

	return surfaceTypeToJoint(surfaceType0, prim0, prim1, jointCoord0, jointCoord1);
}

// FUNCTION: WEBSERVICE 0x1011f690
void RotatePJoint::stepUi(int frameCount)
{
	float value = getChannelValue(frameCount);

	if (rotateConnector != NULL) {
		rotateConnector->getKernelInput().setGoal(value);
	}

	if (value != 0.0f) {
		World* world = getWorld();

		world->ticklePrimitive(getPrimitive(0), false);
		world->ticklePrimitive(getPrimitive(1), false);
	}
}

// FUNCTION: WEBSERVICE 0x1011f730
void RotateVJoint::stepUi(int frameCount)
{
	float value = getChannelValue(frameCount);

	if (rotateConnector != NULL) {
		rotateConnector->getKernelInput().setDelta(value);
	}

	if (value != 0.0f) {
		World* world = getWorld();

		world->ticklePrimitive(getPrimitive(0), false);
		world->ticklePrimitive(getPrimitive(1), false);
	}
}

// FUNCTION: WEBSERVICE 0x1011f7d0
float RotateJoint::getTorqueArmLength()
{
	Vector3 axleSize = getAxlePrim()->getGeometry()->getGridSize();
	Vector3 holeSize = getHolePrim()->getGeometry()->getGridSize();

	NormalId axleId = getAxleId();
	NormalId holeId = getHoleId();

	int holeIndex0 = (holeId + 1) % 3;
	int holeIndex1 = (holeId + 2) % 3;

	float axleArm = std::max(axleSize[(axleId + 1) % 3], axleSize[(axleId + 2) % 3]);
	float holeArm = std::max(holeSize[holeIndex0], holeSize[holeIndex1]);

	return std::min(axleArm, holeArm) * 0.1f;
}

// FUNCTION: WEBSERVICE 0x1011f8c0
void RotateJoint::putInKernel(Kernel* kernel)
{
	MultiJoint::putInKernel(kernel);

	Body* body0 = getAxlePrim()->getBody();
	Body* body1 = getHolePrim()->getBody();

	CoordinateFrame coord0 = body0->getCoordinateFrame() * jointCoord0;
	CoordinateFrame coord1 = body1->getCoordinateFrame() * jointCoord1;

	Vector3 axis0 = coord0.rotation.getColumn(2);
	Vector3 axis1 = coord1.rotation.getColumn(2);

	for (int i = 0; i < 2; ++i) {
		float offset = i == 0 ? -1.0f : 1.0f;
		Vector3 pos0 = coord0.translation + axis0 * offset;
		Vector3 pos1 = coord1.translation + axis1 * offset;

		Point* point0 = getKernel()->newPoint(body0, pos0);
		Point* point1 = getKernel()->newPoint(body1, pos1);

		Connector* connector = new PointToPointBreakConnector(point0, point1, getJointK(), Math::inf());
		addToMultiJoint(point0, point1, connector);
	}

	if (getJointType() == ROTATE_P_JOINT || getJointType() == ROTATE_V_JOINT) {
		Vector3 center = coord0.translation;
		Vector3 axis = coord0.rotation.getColumn(0);
		Vector3 pos = center + axis * 10.0f;

		Point* point0 = getKernel()->newPoint(body0, pos);
		Point* point1 = getKernel()->newPoint(body1, pos);

		rotateConnector =
			new RotateConnector(getPoint(0), getPoint(2), point0, point1, getJointK(), getTorqueArmLength());

		addToMultiJoint(point0, point1, rotateConnector);
	}
}

} // namespace RBX
