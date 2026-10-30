#include "v8datamodel/JointsService.h"

#include "util/object.h"
#include "v8datamodel/Workspace.h"
#include "v8world/Joint.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x100dd9a0
// FUNCTION: ROBLOX 0x005f55d0
void JointsService::onDescendentAdded(Instance* instance)
{
	Instance::onDescendentAdded(instance);

	IRenderable* renderable = dynamic_cast<IRenderable*>(instance);
	if (renderable) {
		onAdded(renderable);
	}
}

// FUNCTION: WEBSERVICE 0x100dd9e0
// FUNCTION: ROBLOX 0x005f5610
void JointsService::onEvent(const World* source, AutoDestroy event)
{
	IJointOwner* owner = event.joint->getJointOwner();
	if (owner) {
		static_cast<JointInstance*>(owner)->setParent(NULL);
	}
}

// FUNCTION: WEBSERVICE 0x100dda00
// FUNCTION: ROBLOX 0x005f5630
void JointsService::onDescendentRemoving(const boost::shared_ptr<Instance>& instance)
{
	IRenderable* renderable = dynamic_cast<IRenderable*>(instance.get());
	if (renderable) {
		onRemoving(renderable);
	}

	Instance::onDescendentRemoving(instance);
}

// FUNCTION: WEBSERVICE 0x100ddd40
// FUNCTION: ROBLOX 0x005f5970
void JointsService::onEvent(const World* source, AutoJoin event)
{
	boost::shared_ptr<JointInstance> ji;

	switch (event.joint->getJointType()) {
	case Joint::SNAP_JOINT:
		ji = Creatable<Instance>::create<Snap>(event.joint);
		break;
	case Joint::WELD_JOINT:
		ji = Creatable<Instance>::create<Weld>(event.joint);
		break;
	case Joint::GLUE_JOINT:
		ji = Creatable<Instance>::create<Glue>(event.joint);
		break;
	case Joint::ROTATE_JOINT:
		ji = Creatable<Instance>::create<Rotate>(event.joint);
		break;
	case Joint::ROTATE_P_JOINT:
		ji = Creatable<Instance>::create<RotateP>(event.joint);
		break;
	case Joint::ROTATE_V_JOINT:
		ji = Creatable<Instance>::create<RotateV>(event.joint);
		break;
	}

	ji->setParent(this);
}

// FUNCTION: WEBSERVICE 0x100de040
// STUB: ROBLOX 0x005f5b90
void JointsService::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	stopListening<World, AutoJoin>(world, this);
	stopListening<World, AutoDestroy>(world, this);
	world = NULL;

	Instance::onServiceProvider(oldProvider, newProvider);

	Workspace* workspace = newProvider ? newProvider->find<Workspace>() : NULL;
	if (workspace) {
		world = workspace->getWorld();
	}

	startListening<World, AutoJoin>(world, this);
	startListening<World, AutoDestroy>(world, this);
}

} // namespace RBX
