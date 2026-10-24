#include "util/Face.h"

#include "decomp.h"
#include "util/Math.h"

#include <G3D/Plane.h>
#include <algorithm>

namespace RBX {

// FUNCTION: WEBSERVICE 0x1010ae60
Vector3& Face::operator[](int i)
{
	return (&c0)[i];
}

// FUNCTION: WEBSERVICE 0x1010ae70
void Face::snapToGrid(float grid)
{
	for (int i = 0; i < 4; ++i) {
		(*this)[i] = Math::toGrid((*this)[i], grid);
	}
}

// FUNCTION: WEBSERVICE 0x1010aec0
bool Face::fuzzyContainsInExtrusion(const Vector3& point, float tolerance) const
{
	for (int i = 0; i < 4; ++i) {
		const Vector3& corner = (*this)[i];
		const Vector3& next = (*this)[(i + 1) % 4];

		if ((next - corner).dot(point - corner) < -tolerance) {
			return false;
		}
	}

	return true;
}

// FUNCTION: WEBSERVICE 0x1010af40
bool Face::cornersAligned(const Face& f0, const Face& f1, float tolerance)
{
	for (int i = 0; i < 4; ++i) {
		bool found = false;

		for (int j = 0; j < 4; ++j) {
			if ((f0[i] - f1[j]).squaredLength() < tolerance * tolerance) {
				found = true;
				break;
			}
		}

		if (!found) {
			return false;
		}
	}

	return true;
}

// FUNCTION: WEBSERVICE 0x1010afb0
Face Face::fromExtentsSide(const Extents& e, NormalId faceId)
{
	Face answer;
	e.getFaceCorners(faceId, answer.c0, answer.c1, answer.c2, answer.c3);
	return answer;
}

// FUNCTION: WEBSERVICE 0x1010b000
Face Face::toWorldSpace(const CoordinateFrame& objectCoord) const
{
	Face answer;
	for (int i = 0; i < 4; ++i) {
		answer[i] = objectCoord.pointToWorldSpace((*this)[i]);
	}
	return answer;
}

// FUNCTION: WEBSERVICE 0x1010b180
Face Face::toObjectSpace(const CoordinateFrame& objectCoord) const
{
	Face answer;
	for (int i = 0; i < 4; ++i) {
		answer[i] = objectCoord.pointToObjectSpace((*this)[i]);
	}
	return answer;
}

// FUNCTION: WEBSERVICE 0x1010b3c0
void Face::minMax(const Vector3& point, const Vector3& normal, float& min, float& max) const
{
	min = 1e10f;
	max = -1e10f;

	for (int i = 0; i < 4; ++i) {
		float thisDistance = normal.dot((*this)[i] - point);
		min = std::min(min, thisDistance);
		max = std::max(max, thisDistance);
	}
}

// FUNCTION: WEBSERVICE 0x1010b520
bool Face::hasOverlap(const Face& f0, const Face& f1, float byAtLeast)
{
	for (int i = 0; i < 2; ++i) {
		Vector3 axis = f0.getAxis(i);

		float f0min, f0max, f1min, f1max;
		f0.minMax(f0.c0, axis, f0min, f0max);
		f1.minMax(f0.c0, axis, f1min, f1max);

		if (f1max - byAtLeast < f0min || f0max < f1min + byAtLeast) {
			return false;
		}
	}

	return true;
}

// FUNCTION: WEBSERVICE 0x1010b5b0
Face Face::projectOverlapOnMe(const Face& other) const
{
	float f0min, f0max, f1min, f1max;
	float minL[2];
	float maxL[2];

	Vector3 normal[2] = {getAxis(0), getAxis(1)};

	for (int i = 0; i < 2; ++i) {
		minMax(c0, normal[i], f0min, f0max);
		other.minMax(c0, normal[i], f1min, f1max);

		minL[i] = std::max(f0min, f1min);
		maxL[i] = std::min(f0max, f1max);
	}

	return Face(
		normal[0] * minL[0] + normal[1] * minL[1] + c0,
		normal[0] * minL[0] + normal[1] * maxL[1] + c0,
		normal[0] * maxL[0] + normal[1] * maxL[1] + c0,
		normal[0] * maxL[0] + normal[1] * minL[1] + c0
	);
}

// STUB: WEBSERVICE 0x1010b890
bool Face::overlapWithinPlanes(const Face& f0, const Face& f1, float tolerance)
{
	for (int i = 0; i < 2; ++i) {
		const Face* a;
		const Face* b;

		if (i == 0) {
			a = &f0;
			b = &f1;
		}
		else {
			a = &f1;
			b = &f0;
		}

		G3D::Plane p0(a->c0, a->c1, a->c3);
		Vector3 normal;
		float distance;
		p0.getEquation(normal, distance);

		Face overlapOn1 = b->projectOverlapOnMe(*a);

		for (int j = 0; j < 4; ++j) {
			if (fabsf(overlapOn1[j].dot(normal) + distance) > tolerance) {
				return false;
			}
		}
	}

	return true;
}

} // namespace RBX
