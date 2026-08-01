#ifndef V8WORLD_ASSEMBLY2_H
#define V8WORLD_ASSEMBLY2_H

#include "decomp.h"
#include "util/ComputeProp.h"
#include "util/Quaternion.h"
#include "v8world/IPipelined.h"

#include <G3D/Vector3.h>
#include <boost/noncopyable.hpp>
#include <vector>

namespace RBX {

using G3D::Vector3;

class Body;
class EdgeIterator;
class PrimIterator;
class Edge;
class Joint;
class Mechanism;
class MotorJoint;
class RigidJoint;
class Primitive;

namespace Sim {

enum AssemblyState
{
	ANCHORED = 0,
	RECURSIVE_WAKE_PENDING = 1,
	WAKE_PENDING = 2,
	AWAKE = 3,
	SLEEPING_CHECKING = 4,
	SLEEPING_DEEPLY = 5,
};

} // namespace Sim

// SIZE 0x1c
class RunningAverageState
{
public:
	RunningAverageState() : position(0, 0, 0)
	{
		angles.x = 0;
		angles.y = 0;
		angles.z = 0;
		angles.w = 1;
	}

private:
	Vector3 position;  // 0x00
	Quaternion angles; // 0x0c
};

DECOMP_SIZE_ASSERT(RunningAverageState, 0x1c)

// SIZE 0x24
class SleepInfo
{
public:
	SleepInfo();

	Sim::AssemblyState state; // 0x00

private:
	int sleepCount;                          // 0x04
	RunningAverageState runningAverageState; // 0x08
};

DECOMP_SIZE_ASSERT(SleepInfo, 0x24)

// VTABLE: WEBSERVICE 0x10246c4c
// SIZE 0x50
class Assembly : public IPipelined, public boost::noncopyable
{
public:
	Assembly(Primitive* rootPrimitive);
	~Assembly();

	// SYNTHETIC: WEBSERVICE 0x1011b5c0
	// RBX::Assembly::`scalar deleting destructor'

	PrimIterator assemblyPrimBegin();
	PrimIterator assemblyPrimEnd();

	EdgeIterator externalEdgeBegin();
	EdgeIterator externalEdgeEnd();

	Assembly* getRootAssembly();
	Primitive* getAssemblyPrimitive();

	static Joint* getJointToParent(Primitive* primitive);

	Assembly* otherAssembly(Edge* edge);

	const Primitive* getRootPrimitive() const { return rootPrimitive; }

	Mechanism* getMechanism();

	void onPrimitivesChanged();
	void onPrimitiveCanSleepChanged(Primitive* primitive);

	unsigned int numMotors();
	MotorJoint* getMotor(unsigned int index);

	void stepUi(int frameCount);

	void setParent(Assembly* value);

	static void addRigidChild(Primitive* parent, RigidJoint* joint, Primitive* child);
	static void addMotorChild(Primitive* parent, MotorJoint* joint, Primitive* child);
	static void addGroundChild(Primitive* child);
	Sim::AssemblyState getSleepStatus();
	bool getAnchored();
	float computeMaxRadius() const;
	bool computeCanSleep() const;

	void notifyMoved();

	virtual void putInKernel(Kernel* kernel); // vtable+0x04
	virtual void removeFromKernel();          // vtable+0x08

private:
	MotorJoint* getMotorImp(unsigned int& index);

	void addChild(Assembly* child);

	SleepInfo* sleepInfo;                   // 0x08
	Primitive* rootPrimitive;               // 0x0c
	Assembly* parent;                       // 0x10
	std::vector<Assembly*> children;        // 0x14
	Mechanism* mechanism;                   // 0x24
	ComputeProp<float, Assembly> maxRadius; // 0x28
	ComputeProp<bool, Assembly> canSleep;   // 0x40
};

DECOMP_SIZE_ASSERT(Assembly, 0x50)

// VTABLE: WEBSERVICE 0x10247e3c
// SIZE 0x50
class Clump : public Assembly
{
};

DECOMP_SIZE_ASSERT(Clump, 0x50)

bool lessAssembly(const Assembly* a, const Assembly* b);

} // namespace RBX

#endif // V8WORLD_ASSEMBLY2_H
