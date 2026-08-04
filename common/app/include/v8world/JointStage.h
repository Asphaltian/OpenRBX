#ifndef V8WORLD_JOINTSTAGE_H
#define V8WORLD_JOINTSTAGE_H

#include "decomp.h"
#include "v8world/IWorldStage.h"

#include <map>
#include <set>

namespace RBX {

class ClumpStage;
class Joint;
class Primitive;

// VTABLE: WEBSERVICE 0x10247dcc
// SIZE 0x28
class JointStage : public IWorldStage
{
public:
	typedef std::multimap<Primitive*, Joint*> JointMap;

	JointStage(IStage* upstream, World* world);

	// STUB: WEBSERVICE 0x10118810
	virtual ~JointStage() {} // vtable+0x00

	// SYNTHETIC: WEBSERVICE 0x101188d0
	// RBX::JointStage::`scalar deleting destructor'

	// FUNCTION: WEBSERVICE 0x10059d20 FOLDED
	virtual StageType getStageType() { return JOINT_STAGE; } // vtable+0x04

	virtual void onEdgeAdded(Edge* edge);    // vtable+0x10
	virtual void onEdgeRemoving(Edge* edge); // vtable+0x14

	void onPrimitiveAdded(Primitive* p);
	void onPrimitiveRemoving(Primitive* p);

	void onJointPrimitiveNulling(Joint* j, Primitive* nulling);
	void onJointPrimitiveSet(Joint* j, Primitive* p);

private:
	ClumpStage* getClumpStage();

	void moveEdgeToDownstream(Edge* e);
	void removeEdgeFromDownstream(Edge* e);

	void moveJointToDownstream(Joint* j);
	void removeJointFromDownstream(Joint* j);

	bool jointInList(Joint* j);
	void removeFromList(Joint* j);
	void insertToList(Joint* j);

	bool pairInMap(Joint* j, Primitive* p);
	void insertToMap(Joint* j, Primitive* p);
	void removeFromMap(Joint* j, Primitive* p);

	bool edgeHasPrimitivesDownstream(Edge* e);

	std::set<Joint*> incompleteJoints; // 0x10
	JointMap jointMap;                 // 0x1c
};

DECOMP_SIZE_ASSERT(JointStage, 0x28)

} // namespace RBX

#endif // V8WORLD_JOINTSTAGE_H
