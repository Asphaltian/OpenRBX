#ifndef V8DATAMODEL_WORKSPACE_H
#define V8DATAMODEL_WORKSPACE_H

#include "gui/GuiEvent.h"
#include "placeholder.h"
#include "script/Script.h"
#include "util/ContentProvider.h"
#include "util/IRenderable.h"
#include "util/InsertMode.h"
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

class DrawChanged
{
};

class ToolChanged
{
};

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
	static bool showHashGrid;

	static Workspace* findWorkspace(const Instance* context);
	static Workspace* getMyWorkspaceFast(const Instance* context);
	static World* getWorldIfInWorkspace(const Instance* context);
	static Workspace* getWorkspaceIfInWorkspace(const Instance* context);
	static bool contextInWorkspace(const Instance* context);
	static World* getMyWorldFast(const Instance* context);
	static Instance* findTopInstance(Instance*);

private:
	virtual bool askAddChild(const Instance* instance) const;
	virtual void onChildChanged(Instance* instance, const PropertyChanged& event);
	virtual void onDescendentAdded(Instance* instance);
	virtual void onDescendentRemoving(const boost::shared_ptr<Instance>& instance);

public:
	virtual void render2d(Adorn* adorn);
	virtual void render3dAdorn(Adorn* adorn);

	virtual void render3dSelect(Adorn* adorn, SelectState selectState) {}

private:
	virtual IScriptOwner* scriptShouldRun(Script* script);
	virtual void runScript(Script* script, ScriptContext* context);
	virtual void releaseScript(Script* script);

	virtual Extents computeCameraOwnerExtents(); // ICameraOwner vtable+0x10
	virtual void cameraMoved();                  // ICameraOwner vtable+0x0c

	virtual void onExtentsChanged() const
	{
		ModelInstance::onExtentsChanged();
		raiseDrawChanged();
	}

	virtual void onEvent(const RunService* source, Heartbeat event);
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

public:
	virtual ~Workspace();

	virtual const G3D::GCamera& getGCamera() const; // ICameraOwner vtable+0x08
	virtual Camera* getCamera() const;              // ICameraOwner vtable+0x04
	void setCamera(Camera* value);

	Workspace(IDataState* dataState);

	World* getWorld() const { return world.get(); }
	MouseCommand* getCurrentMouseCommand() { return currentCommand.get(); }
	void cancelMouseCommand();
	IDataState& getDataState() const { return *dataState; }

	bool getInMouselookMode() { return inMouselookMode; }
	void setInMouselookMode(bool);

	void setMouseCommand(MouseCommand* newMouseCommand);
	void setDefaultMouseCommand();
	void setNullMouseCommand();

	void setImageServerView(const G3D::Rect2D& viewPortRect);
	void zoomToExtents();
	void onWrapMouse(const G3D::Vector2&);

	virtual GuiResponse process(const GuiEvent& guiEvent);
	void selectAll();

	void start();
	void stop();
	void reset();
	void setThrottleEnabled(bool value);
	void joinAllHack();
	void insertItems(XmlElement*, std::vector<boost::shared_ptr<Instance> >&, InsertMode, PromptMode);

	float step(float timeInterval);

	void insertContent(
		ContentId contentId,
		std::vector<boost::shared_ptr<Instance> >& items,
		InsertMode insertMode,
		PromptMode promptMode
	);
	boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > insertContent(ContentId contentId);

	void makeJoints(boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > instances);
	void breakJoints(boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > instances);
	void setCursor(Adorn*);

private:
	void handleFallenParts(const G3D::Array<boost::shared_ptr<PartInstance> >& fallenParts);

public:
	void raiseDrawChanged() const { Notifier<Workspace, DrawChanged>::raise(DrawChanged()); }

private:
	boost::shared_ptr<Instance> statsItem;
	IDataState* dataState;
	bool arrowCameraControls;
	std::auto_ptr<MouseCommand> currentCommand;
	std::auto_ptr<MouseCommand> stickyCommand;
	UIEvent idleMouseEvent;
	boost::shared_ptr<Camera> currentCamera;
	int flySteps;
	bool isSimulating;
	std::set<Script*> pendingScripts;
	ScriptContext* scriptContext;
	bool inMouselookMode;

public:
	boost::scoped_ptr<Profiling::CodeProfiler> profileWorkspaceStep;
	boost::scoped_ptr<Profiling::CodeProfiler> profileRunServiceStepped;
	boost::scoped_ptr<Profiling::CodeProfiler> profileRunServiceHeartbeat;
	boost::scoped_ptr<Profiling::ThreadProfiler> profileRun;
	int imageServerViewHack;

	template <class _InIt>
	boost::shared_ptr<ModelInstance> FUN_005984c0(_InIt first, _InIt last)
	{
		boost::shared_ptr<ModelInstance> model = Creatable<Instance>::create<ModelInstance>();
		model->setParent(this);

		for (; first != last; ++first) {
			(*first)->setParent(model.get());
		}

		return model;
	}
};

} // namespace RBX

#endif // V8DATAMODEL_WORKSPACE_H
