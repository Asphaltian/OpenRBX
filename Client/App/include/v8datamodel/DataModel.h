#ifndef V8DATAMODEL_DATAMODEL_H
#define V8DATAMODEL_DATAMODEL_H

#include "placeholder.h"
#include "util/Evaluator.h"
#include "util/Events.h"
#include "util/RunStateOwner.h"
#include "v8tree/Service.h"
#include "v8tree/Verb.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <string>
#include <vector>

namespace RBX {

class Adorn;
class ContentId;
class GuiEvent;
class GuiResponse;
class GuiRoot;
class GuiTarget;
class IMetric;
class LocalBackpack;
class RunService;
class StarterPackService;
class TimeState;
class UIEvent;
class Workspace;

extern const char sDataModel[];

class DataModel : public DescribedNonCreatable<DataModel, ServiceProvider, sDataModel>,
				  public VerbContainer,
				  public IDataState,
				  public Evaluator,
				  public Listener<RunService, RunTransition>
{
	GuiResponse processAccelerators(const GuiEvent& event);
	void processEvent(const UIEvent&);

private:
	boost::shared_ptr<LocalBackpack> localBackpack;
	boost::shared_ptr<Workspace> workspace;
	boost::shared_ptr<RunService> runService;
	boost::shared_ptr<StarterPackService> starterPackService;
	boost::shared_ptr<GuiRoot> guiRoot;
	boost::shared_ptr<GuiRoot> guiHooks;
	GuiTarget* guiTarget;
	bool forceArrowCursor;
	std::string uiMessage;
	int drawId;
	IMetric* tempMetric;
	bool dirty;
	bool isInitialized;
	boost::shared_ptr<boost::recursive_mutex> mutex;

	void initializeContents();

public:
	class Lock
	{
	public:
		Lock(const DataModel* dataModel) : lock(NULL) { doLock(dataModel); }

		Lock(boost::shared_ptr<const DataModel> dataModel) : lock(NULL) { doLock(dataModel.get()); }

		~Lock();

	private:
		void doLock(const DataModel* dataModel);

		boost::shared_ptr<boost::recursive_mutex> mutex;
		boost::recursive_mutex::scoped_lock* lock;
	};

	boost::scoped_ptr<TimeState> timeState;

	static boost::shared_ptr<DataModel> createDataModel();
	static void closeDataModel(boost::shared_ptr<DataModel> dataModel, bool joinRunThread);

	DataModel();
	virtual ~DataModel();

	void loadContent(ContentId contentId);
	void save(ContentId contentId);
	std::string httpGet(std::string url, bool synchronous);
	std::string httpPost(std::string url, std::string data, bool synchronous);
	void close();
	boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > get(ContentId contentId);
	void raiseClose();
	int numChatOptions();
	void buildGui(Adorn*);
	void processGui(const UIEvent&);
	GuiRoot* getGuiRoot() const { return guiRoot.get(); }
	GuiRoot* getGuiHooks() const { return guiHooks.get(); }
	void clearContents();
	boost::shared_ptr<boost::recursive_mutex> getMutex() const { return mutex; }

	static boost::shared_ptr<boost::recursive_mutex> getMutex(Instance* instance)
	{
		if (!instance) {
			return boost::shared_ptr<boost::recursive_mutex>();
		}

		DataModel* dataModel = dynamic_cast<DataModel*>(instance->getRootAncestor());
		if (!dataModel) {
			return boost::shared_ptr<boost::recursive_mutex>();
		}

		return dataModel->getMutex();
	}

	virtual void setDirty(bool dirty) { this->dirty = dirty; }
	virtual bool isDirty() const { return dirty; }

	virtual bool isStackTooBig() const { return getStackSize() > 10; }

	Workspace* getWorkspace() const { return workspace.get(); }
	float step(float timeInterval);
	float getSimTime() const;
	TimeState* getTimeState();

	virtual std::string evaluate(const std::string& valueName) const;
	void renderPass2d(Adorn*, IMetric*);
	void renderPass3dAdorn(Adorn*);

	void setUiMessage(std::string message) { uiMessage = message; }

	void clearUiMessage() { uiMessage = ""; }

	void setUiMessageBrickCount() { uiMessage = "[[[progress]]]"; }
	bool canDelaySwapBuffer() const;

protected:
	virtual void onChildAdded(Instance* child);

	virtual void onChildChanged(Instance* instance, const PropertyChanged& event)
	{
		ServiceProvider::onChildChanged(instance, event);
		setDirty(true);
	}

	virtual void onDescendentAdded(Instance* instance)
	{
		ServiceProvider::onDescendentAdded(instance);
		setDirty(true);
	}

	virtual void onDescendentRemoving(const boost::shared_ptr<Instance>& instance)
	{
		ServiceProvider::onDescendentRemoving(instance);
		setDirty(true);
	}

	virtual void onEvent(const RunService* source, RunTransition event);

private:
	static std::string doHttpGet(std::string url);
	static std::string doHttpPost(std::string url, std::string data);
};

} // namespace RBX

#endif // V8DATAMODEL_DATAMODEL_H
