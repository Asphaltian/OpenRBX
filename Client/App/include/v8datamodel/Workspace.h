#ifndef V8DATAMODEL_WORKSPACE_H
#define V8DATAMODEL_WORKSPACE_H

#include "decomp.h"
#include "gui/GuiEvent.h"
#include "script/Script.h"
#include "util/IRenderable.h"
#include "util/Name.h"
#include "util/RunStateOwner.h"
#include "v8datamodel/RootInstance.h"
#include "v8tree/Service.h"
#include "v8tree/Verb.h"
#include "v8world/IMoving.h"

namespace RBX {

class IDataState;
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
				  public GuiTarget,
				  public VerbContainer,
				  public IMovingManager,
				  public IScriptOwner,
				  public Notifier<Workspace, DrawChanged>,
				  public Notifier<Workspace, ToolChanged>,
				  public Listener<RunService, Heartbeat>,
				  public Service,
				  public IRenderableBucket
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
	virtual IScriptOwner* scriptShouldRun(Script* script);
	virtual void runScript(Script* script, ScriptContext* context);
	virtual void releaseScript(Script* script);

	virtual Extents computeCameraOwnerExtents(); // ICameraOwner vtable+0x10
	virtual void cameraMoved();                  // ICameraOwner vtable+0x0c

	virtual void onEvent(const RunService* source, Heartbeat event);

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

private:
	undefined m_unk0x310[0x374 - 0x310]; // 0x310
};

DECOMP_SIZE_ASSERT(Workspace, 0x394)

// clang-format off
// TEMPLATE: WEBSERVICE 0x1006fab0
// RBX::DescribedNonCreatable<RBX::Workspace,RBX::RootInstance,&RBX::sWorkspace>::DescribedNonCreatable<RBX::Workspace,RBX::RootInstance,&RBX::sWorkspace>
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_WORKSPACE_H
