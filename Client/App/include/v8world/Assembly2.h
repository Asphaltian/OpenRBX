#ifndef V8WORLD_ASSEMBLY2_H
#define V8WORLD_ASSEMBLY2_H

#include "placeholder.h"
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

class SleepInfo
{
public:
	SleepInfo();
	~SleepInfo();

private:
	friend class Assembly;
	friend class SleepStage;

	static float sleepTolerance() { return 0.02f; }

	Sim::AssemblyState state;
	int sleepCount;
	RunningAverageState runningAverageState;
};

class Assembly : public IPipelined, public boost::noncopyable
{
	typedef RBX::PrimIterator PrimIterator;
	typedef RBX::EdgeIterator EdgeIterator;

private:
	SleepInfo* sleepInfo;
	Primitive* rootPrimitive;
	Assembly* parent;
	std::vector<Assembly*> children;
	Mechanism* mechanism;

	void removeChild(Assembly*);
	void addChild(Assembly* child);

	ComputeProp<float, Assembly> maxRadius;
	ComputeProp<bool, Assembly> canSleep;

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
	virtual ~Assembly();

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

	virtual void putInKernel(Kernel* kernel);
	virtual void removeFromKernel();

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

class Clump : public Assembly
{
public:
	Clump(Primitive* root) : Assembly(root) {}
};

bool lessAssembly(const Assembly* a, const Assembly* b);

} // namespace RBX

#endif // V8WORLD_ASSEMBLY2_H
