#ifndef V8DATAMODEL_WORKSPACE_H
#define V8DATAMODEL_WORKSPACE_H

#include "decomp.h"
#include "gui/GuiEvent.h"
#include "script/Script.h"
#include "util/IRenderable.h"
#include "util/Name.h"
#include "util/Profiling.h"
#include "util/RunStateOwner.h"
#include "util/UIEvent.h"
#include "v8datamodel/RootInstance.h"
#include "v8tree/Service.h"
#include "v8tree/Verb.h"
#include "v8world/IMoving.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>
#include <set>

namespace RBX {

class Camera;
class IDataState;
class MouseCommand;
class World;

extern char sWorkspace[];

// SIZE 0x1
class DrawChanged
{
};

DECOMP_SIZE_ASSERT(DrawChanged, 0x1)

// SIZE 0x1
class ToolChanged
{
};

DECOMP_SIZE_ASSERT(ToolChanged, 0x1)

// SIZE 0x394
class Workspace : public DescribedNonCreatable<Workspace, RootInstance, sWorkspace>,
				  public Service,
				  public GuiTarget,
				  public VerbContainer,
				  public IRenderableBucket,
				  public IMovingManager,
				  public IScriptOwner,
				  public Notifier<Workspace, DrawChanged>,
				  public Notifier<Workspace, ToolChanged>,
				  public Listener<RunService, Heartbeat>
{
public:
	static bool showWorldCoord;

	static Workspace* findWorkspace(const Instance* context);
	static Workspace* getMyWorkspaceFast(const Instance* context);
	static World* getWorldIfInWorkspace(const Instance* context);
	static Workspace* getWorkspaceIfInWorkspace(const Instance* context);
	static bool contextInWorkspace(const Instance* context);
	static World* getMyWorldFast(const Instance* context);

private:
	virtual bool askAddChild(const Instance* instance) const;
	virtual void onChildChanged(Instance* instance, const PropertyChanged& event);
	virtual void onDescendentAdded(Instance* instance);
	virtual void onDescendentRemoving(const boost::shared_ptr<Instance>& instance);

public:
	virtual void render2d(Adorn* adorn);

private:
	virtual IScriptOwner* scriptShouldRun(Script* script);
	virtual void runScript(Script* script, ScriptContext* context);
	virtual void releaseScript(Script* script);

	virtual Extents computeCameraOwnerExtents(); // ICameraOwner vtable+0x10
	virtual void cameraMoved();                  // ICameraOwner vtable+0x0c

	// FUNCTION: WEBSERVICE 0x1006d250
	virtual void onExtentsChanged() const
	{
		ModelInstance::onExtentsChanged();
		raiseDrawChanged();
	}

	virtual void onEvent(const RunService* source, Heartbeat event);
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

public:
	virtual ~Workspace();

	// clang-format off
	// SYNTHETIC: WEBSERVICE 0x1006d4d0
	// RBX::Workspace::`scalar deleting destructor'
	// clang-format on

	virtual const G3D::GCamera& getGCamera() const; // ICameraOwner vtable+0x08
	virtual Camera* getCamera() const;              // ICameraOwner vtable+0x04

	Workspace(IDataState* dataState);

	World* getWorld() const { return world.get(); }

	bool getInMouselookMode() { return inMouselookMode; }

	void setMouseCommand(MouseCommand* command);
	void setDefaultMouseCommand();
	void setNullMouseCommand();

	void zoomToExtents();

	void stop();
	void reset();
	void setThrottleEnabled(bool value);
	void joinAllHack();

	float step(float timeInterval);

	void makeJoints(boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > instances);
	void breakJoints(boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > instances);

private:
	void handleFallenParts(const G3D::Array<boost::shared_ptr<PartInstance> >& fallenParts);

public:
	void raiseDrawChanged() const { Notifier<Workspace, DrawChanged>::raise(DrawChanged()); }

private:
	boost::shared_ptr<Instance> statsItem;      // 0x310
	IDataState* dataState;                      // 0x318
	bool arrowCameraControls;                   // 0x31c
	std::auto_ptr<MouseCommand> currentCommand; // 0x320
	std::auto_ptr<MouseCommand> stickyCommand;  // 0x324
	UIEvent idleMouseEvent;                     // 0x328
	boost::shared_ptr<Camera> currentCamera;    // 0x33c
	int flySteps;                               // 0x344
	bool isSimulating;                          // 0x348
	std::set<Script*> pendingScripts;           // 0x34c
	ScriptContext* scriptContext;               // 0x358
	bool inMouselookMode;                       // 0x35c

public:
	boost::scoped_ptr<Profiling::CodeProfiler> profileWorkspaceStep;       // 0x360
	boost::scoped_ptr<Profiling::CodeProfiler> profileRunServiceStepped;   // 0x364
	boost::scoped_ptr<Profiling::CodeProfiler> profileRunServiceHeartbeat; // 0x368
	boost::scoped_ptr<Profiling::ThreadProfiler> profileRun;               // 0x36c
	int imageServerViewHack;                                               // 0x370
};

DECOMP_SIZE_ASSERT(Workspace, 0x394)

// clang-format off
// TEMPLATE: WEBSERVICE 0x1006bf50
// RBX::wrapper<1>
// TEMPLATE: WEBSERVICE 0x1006bf90
// RBX::wrapper<0>
// TEMPLATE: WEBSERVICE 0x1006c130
// RBX::wrapper2<1>
// TEMPLATE: WEBSERVICE 0x1006c1d0
// RBX::wrapper2<0>
// TEMPLATE: WEBSERVICE 0x1006fab0
// RBX::DescribedNonCreatable<RBX::Workspace,RBX::RootInstance,&RBX::sWorkspace>::DescribedNonCreatable<RBX::Workspace,RBX::RootInstance,&RBX::sWorkspace>
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_WORKSPACE_H
