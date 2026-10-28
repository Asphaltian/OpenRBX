#ifndef V8DATAMODEL_JOINTSSERVICE_H
#define V8DATAMODEL_JOINTSSERVICE_H

#include "decomp.h"
#include "util/Events.h"
#include "util/IRenderable.h"
#include "v8datamodel/JointInstance.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"
#include "v8world/World.h"

#include <boost/shared_ptr.hpp>

namespace RBX {

// SIZE 0x11c
class JointsService : public NonFactoryProduct<Instance, static_cast<const char*>(0)>,
					  public Service,
					  public IRenderableBucket,
					  public Listener<World, AutoJoin>,
					  public Listener<World, AutoDestroy>
{
private:
	virtual void onDescendentAdded(Instance* instance);
	virtual void onDescendentRemoving(const boost::shared_ptr<Instance>& instance);
	// FUNCTION: WEBSERVICE 0x10074ec0
	// RBX::JointsService::JointsService

	// FUNCTION: WEBSERVICE 0x10074fc0
	virtual bool askAddChild(const Instance* instance) const
	{
		return dynamic_cast<const JointInstance*>(instance) != NULL;
	}
	virtual XmlElement* write() { return NULL; }
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
	virtual void onEvent(const World* source, AutoDestroy event);
	virtual void onEvent(const World* source, AutoJoin event);

public:
	World* world; // 0x118

	JointsService() : world(NULL) { setName("JointsService"); }

	// SYNTHETIC: WEBSERVICE 0x10075090
	// RBX::JointsService::~JointsService

	// SYNTHETIC: WEBSERVICE 0x100750f0
	// RBX::JointsService::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(JointsService, 0x11c)

} // namespace RBX

#endif // V8DATAMODEL_JOINTSSERVICE_H
