#ifndef V8WORLD_JOINTSTAGE_H
#define V8WORLD_JOINTSTAGE_H

#include "placeholder.h"
#include "v8world/IWorldStage.h"

#include <map>
#include <set>

namespace RBX {

class ClumpStage;
class Joint;
class Primitive;

class JointStage : public IWorldStage
{
public:
	typedef std::multimap<Primitive*, Joint*> JointMap;

	JointStage(IStage* upstream, World* world);

	virtual ~JointStage() {}

	virtual StageType getStageType() { return JOINT_STAGE; }

	virtual void onEdgeAdded(Edge* edge);
	virtual void onEdgeRemoving(Edge* edge);

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

	std::set<Joint*> incompleteJoints;
	JointMap jointMap;
};

} // namespace RBX

#endif // V8WORLD_JOINTSTAGE_H
