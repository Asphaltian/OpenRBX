#ifndef V8KERNEL_CONNECTOR_H
#define V8KERNEL_CONNECTOR_H

#include "placeholder.h"
#include "util/IndexArray.h"
#include "util/NormalId.h"
#include "v8kernel/KernelIndex.h"
#include "v8kernel/KernelInput.h"
#include "v8kernel/Pair.h"
#include "v8kernel/Point.h"

#include <G3D/Vector3.h>

namespace RBX {

class Kernel;
class KernelData;

using G3D::Vector3;

class Connector : public KernelIndex
{
public:
	Connector() {}

	virtual ~Connector() {}

	virtual void computeForce(float dt, bool throttling) = 0;
	virtual bool canThrottle() const;
	virtual bool getBroken();
	virtual float potentialEnergy();

private:
	friend class Kernel;
	friend class KernelData;

	int& getKernelIndex() { return kernelIndex; }
};

class RotateConnector : public Connector
{
public:
	RotateConnector(Point* base0, Point* ray0, Point* ref0, Point* ref1, float kValue, float armLength);

	KernelInput& getKernelInput() { return kernelInput; }

	virtual void computeForce(float dt, bool throttling);

private:
	void computeParams(Vector3& normal, float& rotation, float& rotVel);

	float k;
	Point* base0;
	Point* ray0;
	Point* ref0;
	Point* ref1;
	float lastRotation;
	int windings;
	KernelInput kernelInput;
};

class PointToPointBreakConnector : public Connector
{
public:
	PointToPointBreakConnector(Point* point0, Point* point1, float k, float breakForce)
		: point0(point0), point1(point1), k(k), breakForce(breakForce), broken(false)
	{
	}

	virtual void computeForce(float dt, bool throttling);
	virtual bool getBroken();
	virtual float potentialEnergy();

	void setBroken();

protected:
	void forceToPoints(const Vector3& force)
	{
		point0->accumulateForce(-force);
		point1->accumulateForce(force);
	}

	Point* point0;
	Point* point1;
	float k;
	float breakForce;
	bool broken;
};

class NormalBreakConnector : public PointToPointBreakConnector
{
public:
	NormalBreakConnector(Point* point0, Point* point1, float k, float breakForce, NormalId normalIdBody0)
		: PointToPointBreakConnector(point0, point1, k, breakForce), normalIdBody0(normalIdBody0)
	{
	}

	virtual void computeForce(float dt, bool throttling);

private:
	NormalId normalIdBody0;
};

class ContactConnector : public Connector
{
public:
	ContactConnector(float _k, float _kNeg, float _kFriction)
	{
		k = _k;
		kNeg = _kNeg;
		kFriction = _kFriction;

		reset();
	}

	void reset()
	{
		frictionOffset = Vector3::zero();

		firstApproach = 0;
		threshold = 0;
		forceMagLast = 0;
	}

	void setBallBall(Body* body0, Body* body1, float radius0, float radiusSum)
	{
		geoPair.setBallBall(body0, body1, radius0, radiusSum);
	}

	void setBallBlock(
		Body* body0, Body* body1, float radius0, const Vector3* offset1, NormalId normalID1, GeoPairType geoPairType
	)
	{
		geoPair.setBallBlock(body0, body1, radius0, offset1, normalID1, geoPairType);
	}

	void setPointPlane(
		Body* body0, Body* body1, const Vector3* offset0, const Vector3* offset1, int point0ID, NormalId planeID
	)
	{
		geoPair.setPointPlane(body0, body1, offset0, offset1, point0ID, planeID);
	}

	void setEdgeEdgePlane(
		Body* body0,
		Body* body1,
		const Vector3* offset0,
		const Vector3* offset1,
		NormalId normalID0,
		NormalId normalID1,
		NormalId planeID,
		float edgeLength0
	)
	{
		geoPair.setEdgeEdgePlane(body0, body1, offset0, offset1, normalID0, normalID1, planeID, edgeLength0);
	}

	void setEdgeEdge(
		Body* body0, Body* body1, const Vector3* offset0, const Vector3* offset1, NormalId normalID0, NormalId normalID1
	)
	{
		geoPair.setEdgeEdge(body0, body1, offset0, offset1, normalID0, normalID1);
	}

	bool match(Body* body0, Body* body1, GeoPairType geoPairType, int point0ID, int point1ID)
	{
		return geoPair.match(body0, body1, geoPairType, point0ID, point1ID);
	}

	virtual void computeForce(float dt, bool throttling);
	virtual bool canThrottle() const;

protected:
	GeoPair geoPair;
	float k;
	float kFriction;
	float kNeg;
	float firstApproach;
	float threshold;
	float forceMagLast;
	Vector3 frictionOffset;
};

} // namespace RBX

#endif // V8KERNEL_CONNECTOR_H
