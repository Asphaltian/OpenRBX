#include "v8datamodel/JointsService.h"

#include "util/object.h"
#include "v8datamodel/Workspace.h"
#include "v8world/Joint.h"

namespace RBX {

void JointsService::onDescendentAdded(Instance* instance)
{
	Instance::onDescendentAdded(instance);

	IRenderable* renderable = dynamic_cast<IRenderable*>(instance);
	if (renderable) {
		onAdded(renderable);
	}
}

void JointsService::onEvent(const World* source, AutoDestroy event)
{
	IJointOwner* owner = event.joint->getJointOwner();
	if (owner) {
		static_cast<JointInstance*>(owner)->setParent(NULL);
	}
}

void JointsService::onDescendentRemoving(const boost::shared_ptr<Instance>& instance)
{
	IRenderable* renderable = dynamic_cast<IRenderable*>(instance.get());
	if (renderable) {
		onRemoving(renderable);
	}

	Instance::onDescendentRemoving(instance);
}

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
