#ifndef V8WORLD_ASSEMBLY2_H
#define V8WORLD_ASSEMBLY2_H

#include "decomp.h"
#include "util/ComputeProp.h"
#include "util/RunningAverage.h"
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

// SIZE 0x24
class SleepInfo
{
public:
	SleepInfo();
	~SleepInfo();

private:
	friend class Assembly;
	friend class SleepStage;

	static float sleepTolerance() { return 0.02f; }

	Sim::AssemblyState state;                // 0x00
	int sleepCount;                          // 0x04
	RunningAverageState runningAverageState; // 0x08
};

DECOMP_SIZE_ASSERT(SleepInfo, 0x24)

// VTABLE: WEBSERVICE 0x10246c4c
// VTABLE: ROBLOX 0x00831efc
// SIZE 0x50
class Assembly : public IPipelined, public boost::noncopyable
{
	typedef RBX::PrimIterator PrimIterator;
	typedef RBX::EdgeIterator EdgeIterator;

private:
	SleepInfo* sleepInfo;            // 0x08
	Primitive* rootPrimitive;        // 0x0c
	Assembly* parent;                // 0x10
	std::vector<Assembly*> children; // 0x14
	Mechanism* mechanism;            // 0x24

	void removeChild(Assembly*);
	void addChild(Assembly* child);

	ComputeProp<float, Assembly> maxRadius; // 0x28
	ComputeProp<bool, Assembly> canSleep;   // 0x40

	float computeMaxRadius() const;
	bool computeCanSleep() const;
	void onPrimitivesChanged();
	void onPrimitiveRemoved(Primitive*);
	void onPrimitiveAdded(Primitive*);
	Body* getRootBody();
	const Body* getRootBody() const;
	const MotorJoint* getMotorImp(unsigned int& index) const;
	static Joint* getJointToParent(Primitive* primitive);

public:
	Assembly(Primitive* root);
	virtual ~Assembly(); // vtable+0x00

	// SYNTHETIC: WEBSERVICE 0x1011b5c0
	// RBX::Assembly::`scalar deleting destructor'

	void setParent(Assembly* value);

	static void addRigidChild(Primitive* parent, RigidJoint* joint, Primitive* child);
	static void addMotorChild(Primitive* parent, MotorJoint* joint, Primitive* child);
	static void addGroundChild(Primitive* child);

	Assembly* getParent();

	Primitive* getClumpPrimitive() { return rootPrimitive; }
	const Primitive* getClumpPrimitive() const { return rootPrimitive; }
	Primitive* getAssemblyPrimitive();
	const Primitive* getAssemblyPrimitiveConst() const;
	Assembly* getRootAssembly();
	const Assembly* getRootAssemblyConst() const;
	Assembly* getRootClump();

	bool getAnchored() const;

	void notifyMoved();

	int getDof() const;

	virtual void putInKernel(Kernel* kernel); // vtable+0x04
	virtual void removeFromKernel();          // vtable+0x08

	void setMechanism(Mechanism* value) { mechanism = value; }
	Mechanism* getMechanism();

	void stepUi(int frameCount);

	void setSleepInfo(SleepInfo* value) { sleepInfo = value; }
	SleepInfo* getSleepInfo() { return sleepInfo; }
	Sim::AssemblyState getSleepStatus() const;
	bool moving() const;
	bool getCanSleep() { return canSleep; }
	float getMaxRadius() { return maxRadius; }

	void onPrimitiveCanSleepChanged(Primitive* primitive);

	PrimIterator assemblyPrimBegin() const;
	PrimIterator assemblyPrimEnd() const;

	EdgeIterator externalEdgeBegin() const;
	EdgeIterator externalEdgeEnd() const;

	Assembly* otherAssembly(Edge* edge) const;

	unsigned int numMotors() const;
	MotorJoint* getMotor(unsigned int index);
	const MotorJoint* getMotorConst(unsigned int index) const;
};

DECOMP_SIZE_ASSERT(Assembly, 0x50)

// VTABLE: WEBSERVICE 0x10247e3c
// VTABLE: ROBLOX 0x0083ade4
// SIZE 0x50
class Clump : public Assembly
{
public:
	// FUNCTION: WEBSERVICE 0x1011b310
	// FUNCTION: ROBLOX 0x00644e00
	Clump(Primitive* root) : Assembly(root) {}
};

DECOMP_SIZE_ASSERT(Clump, 0x50)

bool lessAssembly(const Assembly* a, const Assembly* b);

} // namespace RBX

#endif // V8WORLD_ASSEMBLY2_H
