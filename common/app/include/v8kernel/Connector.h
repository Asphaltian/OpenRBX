#ifndef V8KERNEL_CONNECTOR_H
#define V8KERNEL_CONNECTOR_H

#include "decomp.h"
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

// VTABLE: WEBSERVICE 0x10236efc
// SIZE 0x08
class Connector : public KernelIndex
{
public:
	virtual ~Connector();                                     // vtable+0x00
	virtual void computeForce(float dt, bool throttling) = 0; // vtable+0x04
	virtual bool canThrottle();                               // vtable+0x08
	virtual bool getBroken();                                 // vtable+0x0c
	virtual float potentialEnergy();                          // vtable+0x10

private:
	friend class Kernel;
	friend class KernelData;

	int& getKernelIndex() { return kernelIndex; }
};

DECOMP_SIZE_ASSERT(Connector, 0x08)

// VTABLE: WEBSERVICE 0x10247f5c
// SIZE 0x34
class RotateConnector : public Connector
{
public:
	RotateConnector(Point* base0, Point* ray0, Point* ref0, Point* ref1, float kValue, float armLength);

	KernelInput& getKernelInput() { return kernelInput; }

	virtual void computeForce(float dt, bool throttling); // vtable+0x04

	// SYNTHETIC: WEBSERVICE 0x100a0720
	// RBX::RotateConnector::`scalar deleting destructor'

private:
	void computeParams(Vector3& normal, float& rotation, float& rotVel);

	float k;                 // 0x08
	Point* base0;            // 0x0c
	Point* ray0;             // 0x10
	Point* ref0;             // 0x14
	Point* ref1;             // 0x18
	float lastRotation;      // 0x1c
	int windings;            // 0x20
	KernelInput kernelInput; // 0x24
};

DECOMP_SIZE_ASSERT(RotateConnector, 0x34)

// SIZE 0x1c
class PointToPointBreakConnector : public Connector
{
public:
	PointToPointBreakConnector(Point* point0, Point* point1, float k, float breakForce)
		: point0(point0), point1(point1), k(k), breakForce(breakForce), broken(false)
	{
	}

	virtual void computeForce(float dt, bool throttling); // vtable+0x04
	virtual bool getBroken();                             // vtable+0x0c
	virtual float potentialEnergy();                      // vtable+0x10

	void setBroken();

protected:
	void forceToPoints(const Vector3& force)
	{
		point0->accumulateForce(-force);
		point1->accumulateForce(force);
	}

	Point* point0;    // 0x08
	Point* point1;    // 0x0c
	float k;          // 0x10
	float breakForce; // 0x14
	bool broken;      // 0x18
};

DECOMP_SIZE_ASSERT(PointToPointBreakConnector, 0x1c)

// SIZE 0x20
class NormalBreakConnector : public PointToPointBreakConnector
{
public:
	NormalBreakConnector(Point* point0, Point* point1, float k, float breakForce, NormalId normalIdBody0)
		: PointToPointBreakConnector(point0, point1, k, breakForce), normalIdBody0(normalIdBody0)
	{
	}

	virtual void computeForce(float dt, bool throttling); // vtable+0x04

private:
	NormalId normalIdBody0; // 0x1c
};

DECOMP_SIZE_ASSERT(NormalBreakConnector, 0x20)

// VTABLE: WEBSERVICE 0x1023c7f4
// SIZE 0x50
class ContactConnector : public Connector
{
public:
	void reset();

	virtual void computeForce(float dt, bool throttling); // vtable+0x04
	virtual bool canThrottle();                           // vtable+0x08

protected:
	GeoPair geoPair;        // 0x08
	float k;                // 0x2c
	float kFriction;        // 0x30
	float kNeg;             // 0x34
	float firstApproach;    // 0x38
	float threshold;        // 0x3c
	float forceMagLast;     // 0x40
	Vector3 frictionOffset; // 0x44
};

DECOMP_SIZE_ASSERT(ContactConnector, 0x50)

} // namespace RBX

#endif // V8KERNEL_CONNECTOR_H
