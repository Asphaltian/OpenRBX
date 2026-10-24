#ifndef V8DATAMODEL_ROOTINSTANCE_H
#define V8DATAMODEL_ROOTINSTANCE_H

#include "decomp.h"
#include "util/ComputeProp.h"
#include "util/Extents.h"
#include "util/InsertMode.h"
#include "v8datamodel/ICameraOwner.h"
#include "v8datamodel/ModelInstance.h"
#include "v8world/Controller.h"

#include <G3D/Rect2D.h>
#include <G3D/Vector3.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <memory>
#include <vector>

namespace RBX {

class HopperBin;
class MegaDragger;
class PartInstance;
class PVInstance;
class SpawnLocation;
class World;

extern char sRootInstance[];

// SIZE 0x2ac
class RootInstance : public Reflection::Described<RootInstance, sRootInstance, ModelInstance>, public ICameraOwner
{
private:
	ComputeProp<ControllerTypeArray, RootInstance> ControllersUsed; // 0x250

	ControllerTypeArray computeControllersUsed() const;

	mutable G3D::Vector3 insertPoint; // 0x270

	G3D::Vector3 computeIdeInsertPoint() const;
	G3D::Vector3 computeCharacterInsertPoint(const Extents& extents) const;

	enum MoveType
	{
		MOVE_DROP = 0,
		MOVE_NO_DROP = 1,
	};

	void moveSafe(std::vector<boost::weak_ptr<PartInstance> >& partArray, G3D::Vector3 move, MoveType moveType);
	void moveSafe(MegaDragger& megaDragger, G3D::Vector3 move, MoveType moveType);
	void moveToCharacterInsertPoint(std::vector<boost::weak_ptr<PartInstance> >& partArray);
	void moveToIdeInsertPoint(std::vector<boost::weak_ptr<PartInstance> >& partArray);
	void insertRaw(
		const std::vector<boost::shared_ptr<Instance> >& instances,
		Instance* requestedParent,
		std::vector<boost::weak_ptr<PartInstance> >& partArray
	);
	void insertToTree(const std::vector<boost::shared_ptr<Instance> >& instances, Instance* requestedParent);
	void insertCharacterView(
		const std::vector<boost::shared_ptr<Instance> >& instances,
		std::vector<boost::weak_ptr<PartInstance> >& partArray
	);
	void insertIdeView(
		const std::vector<boost::shared_ptr<Instance> >& instances,
		std::vector<boost::weak_ptr<PartInstance> >& partArray,
		PromptMode promptMode
	);
	void insert3dView(const std::vector<boost::shared_ptr<Instance> >& instances, PromptMode promptMode);
	void insertHopperBin(HopperBin* h);
	void insertSpawnLocation(SpawnLocation* s);

protected:
	virtual void onDescendentAdded(Instance* instance);
	virtual void onDescendentRemoving(const boost::shared_ptr<Instance>& instance);
	virtual void onLastChildRemoved() {}
	// FUNCTION: WEBSERVICE 0x1006ab60
	virtual void onChildControllerChanged() { ControllersUsed.setDirty(); }

	G3D::Rect2D viewPort;       // 0x27c
	std::auto_ptr<World> world; // 0x28c

	RootInstance();

	virtual ~RootInstance();

public:
	void insertInstances(
		const std::vector<boost::shared_ptr<Instance> >& instances,
		Instance* requestedParent,
		InsertMode insertMode,
		PromptMode promptMode
	);
	void setInsertPoint(const G3D::Vector3& topCenter);
	void moveToPoint(PVInstance* pv, G3D::Vector3 point);
	ControllerTypeArray getControllersUsed() const { return ControllersUsed; }
	World* getWorld() { return world.get(); }
	const G3D::Rect2D& getViewPort() { return viewPort; }
};

DECOMP_SIZE_ASSERT(RootInstance, 0x2ac)

} // namespace RBX

#endif // V8DATAMODEL_ROOTINSTANCE_H
