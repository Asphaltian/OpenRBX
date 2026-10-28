#ifndef V8DATAMODEL_DATAMODEL_H
#define V8DATAMODEL_DATAMODEL_H

#include "decomp.h"
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

class ContentId;
class GuiRoot;
class GuiTarget;
class IMetric;
class LocalBackpack;
class RunService;
class StarterPackService;
class TimeState;
class Workspace;

extern const char sDataModel[];

// SIZE 0x1fc
class DataModel : public DescribedNonCreatable<DataModel, ServiceProvider, sDataModel>,
				  public VerbContainer,
				  public IDataState,
				  public Evaluator,
				  public Listener<RunService, RunTransition>
{
private:
	boost::shared_ptr<LocalBackpack> localBackpack;           // 0x190
	boost::shared_ptr<Workspace> workspace;                   // 0x198
	boost::shared_ptr<RunService> runService;                 // 0x1a0
	boost::shared_ptr<StarterPackService> starterPackService; // 0x1a8
	boost::shared_ptr<GuiRoot> guiRoot;                       // 0x1b0
	boost::shared_ptr<GuiRoot> guiHooks;                      // 0x1b8
	GuiTarget* guiTarget;                                     // 0x1c0
	bool forceArrowCursor;                                    // 0x1c4
	std::string uiMessage;                                    // 0x1c8
	int drawId;                                               // 0x1e4
	IMetric* tempMetric;                                      // 0x1e8
	bool dirty;                                               // 0x1ec
	bool isInitialized;                                       // 0x1ed
	boost::shared_ptr<boost::recursive_mutex> mutex;          // 0x1f0

	void initializeContents();

public:
	// SIZE 0xc
	class Lock
	{
	public:
		Lock(const DataModel* dataModel);

		// FUNCTION: WEBSERVICE 0x1000cd40
		Lock(boost::shared_ptr<const DataModel> dataModel) : lock(NULL) { doLock(dataModel.get()); }

		~Lock();

	private:
		void doLock(const DataModel* dataModel);

		boost::shared_ptr<boost::recursive_mutex> mutex; // 0x00
		boost::recursive_mutex::scoped_lock* lock;       // 0x08
	};

	boost::scoped_ptr<TimeState> timeState; // 0x1f8

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
	void clearContents();

	virtual void setDirty(bool dirty) { this->dirty = dirty; }
	virtual bool isDirty() const { return dirty; }

	virtual bool isStackTooBig() const { return getStackSize() > 10; }

	Workspace* getWorkspace() const { return workspace.get(); }
	float step(float distributedGameTime);
	float getSimTime() const;
	TimeState* getTimeState();

	virtual std::string evaluate(const std::string& valueName) const;

	// FUNCTION: WEBSERVICE 0x10071420
	void setUiMessage(std::string message) { uiMessage = message; }

	// FUNCTION: WEBSERVICE 0x10071470
	void clearUiMessage() { uiMessage = ""; }

	// FUNCTION: WEBSERVICE 0x10071490
	void setUiMessageBrickCount() { uiMessage = "[[[progress]]]"; }

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

// clang-format off
// TEMPLATE: WEBSERVICE 0x100719f0
// RBX::Reflection::BoundFuncDesc<RBX::DataModel,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,bool),2>::declareSignature
// TEMPLATE: WEBSERVICE 0x10071a50
// RBX::Reflection::BoundFuncDesc<RBX::DataModel,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> >,bool),3>::declareSignature
// TEMPLATE: WEBSERVICE 0x10072ce0
// ?raise@?$Notifier@VServiceProvider@RBX@@VClosing@2@@RBX@@IBEXVClosing@2@PAV?$Listener@VServiceProvider@RBX@@VClosing@2@@2@@Z
// TEMPLATE: WEBSERVICE 0x100735b0
// ?raise@?$Notifier@VServiceProvider@RBX@@VClosing@2@@RBX@@IBEXVClosing@2@@Z
// TEMPLATE: WEBSERVICE 0x10078920
// RBX::Reflection::BoundFuncDesc<RBX::DataModel,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> >,bool),3>::~BoundFuncDesc<RBX::DataModel,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> >,bool),3>
// TEMPLATE: WEBSERVICE 0x10078a10
// RBX::StateStack<RBX::XmlState>::clearStackAfter
// TEMPLATE: WEBSERVICE 0x10079df0
// RBX::StateStack<RBX::XmlState>::~StateStack<RBX::XmlState>
// TEMPLATE: WEBSERVICE 0x10079e50
// RBX::Reflection::BoundFuncDesc<RBX::DataModel,boost::shared_ptr<std::vector<boost::shared_ptr<RBX::Instance>,std::allocator<boost::shared_ptr<RBX::Instance> > > const > __cdecl(RBX::ContentId),1>::BoundFuncDesc<RBX::DataModel,boost::shared_ptr<std::vector<boost::shared_ptr<RBX::Instance>,std::allocator<boost::shared_ptr<RBX::Instance> > > const > __cdecl(RBX::ContentId),1>
// TEMPLATE: WEBSERVICE 0x10079ef0
// RBX::Reflection::BoundFuncDesc<RBX::DataModel,boost::shared_ptr<std::vector<boost::shared_ptr<RBX::Instance>,std::allocator<boost::shared_ptr<RBX::Instance> > > const > __cdecl(RBX::ContentId),1>::call<boost::shared_ptr<std::vector<boost::shared_ptr<RBX::Instance>,std::allocator<boost::shared_ptr<RBX::Instance> > > const > >
// TEMPLATE: WEBSERVICE 0x10079fb0
// RBX::Reflection::BoundFuncDesc<RBX::DataModel,boost::shared_ptr<std::vector<boost::shared_ptr<RBX::Instance>,std::allocator<boost::shared_ptr<RBX::Instance> > > const > __cdecl(RBX::ContentId),1>::execute
// TEMPLATE: WEBSERVICE 0x1007a080
// RBX::Reflection::BoundFuncDesc<RBX::DataModel,void __cdecl(RBX::ContentId),1>::BoundFuncDesc<RBX::DataModel,void __cdecl(RBX::ContentId),1>
// TEMPLATE: WEBSERVICE 0x1007a120
// RBX::Reflection::BoundFuncDesc<RBX::DataModel,void __cdecl(RBX::ContentId),1>::execute
// TEMPLATE: WEBSERVICE 0x1007a210
// RBX::Reflection::BoundFuncDesc<RBX::DataModel,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,bool),2>::BoundFuncDesc<RBX::DataModel,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,bool),2>
// TEMPLATE: WEBSERVICE 0x1007a2d0
// RBX::Reflection::BoundFuncDesc<RBX::DataModel,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,bool),2>::call<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >
// TEMPLATE: WEBSERVICE 0x1007a390
// RBX::Reflection::BoundFuncDesc<RBX::DataModel,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,bool),2>::execute
// TEMPLATE: WEBSERVICE 0x1007a4d0
// RBX::Reflection::BoundFuncDesc<RBX::DataModel,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> >,bool),3>::BoundFuncDesc<RBX::DataModel,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> >,bool),3>
// TEMPLATE: WEBSERVICE 0x1007a5a0
// RBX::Reflection::BoundFuncDesc<RBX::DataModel,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> >,bool),3>::call<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >
// TEMPLATE: WEBSERVICE 0x1007a680
// RBX::Reflection::BoundFuncDesc<RBX::DataModel,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> >,bool),3>::execute
// SYNTHETIC: WEBSERVICE 0x1007a7f0
// RBX::Reflection::BoundFuncDesc<RBX::DataModel,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> >,bool),3>::`scalar deleting destructor'
// TEMPLATE: WEBSERVICE 0x1007a810
// RBX::Reflection::BoundFuncDesc<RBX::DataModel,void __cdecl(void),0>::BoundFuncDesc<RBX::DataModel,void __cdecl(void),0>
// TEMPLATE: WEBSERVICE 0x1007a880
// RBX::Reflection::BoundFuncDesc<RBX::DataModel,void __cdecl(void),0>::execute
// TEMPLATE: WEBSERVICE 0x1007a8e0
// RBX::Reflection::BoundFuncDesc<RBX::DataModel,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >),1>::BoundFuncDesc<RBX::DataModel,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >),1>
// TEMPLATE: WEBSERVICE 0x1007a980
// RBX::Reflection::BoundFuncDesc<RBX::DataModel,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >),1>::execute
// SYNTHETIC: WEBSERVICE 0x1007aef0
// RBX::IDataState::~IDataState
// FUNCTION: WEBSERVICE 0x1007af10
// RBX::NonFactoryProduct<RBX::ServiceProvider,&RBX::sDataModel>::NonFactoryProduct<RBX::ServiceProvider,&RBX::sDataModel>
// FUNCTION: WEBSERVICE 0x1007b720
// RBX::DataModel::setDirty
// FUNCTION: WEBSERVICE 0x1007b730
// RBX::DataModel::isDirty
// FUNCTION: WEBSERVICE 0x1007b740
// RBX::DataModel::onChildChanged
// FUNCTION: WEBSERVICE 0x1007b770
// RBX::DataModel::onDescendentAdded
// FUNCTION: WEBSERVICE 0x1007b7a0
// RBX::DataModel::onDescendentRemoving
// FUNCTION: WEBSERVICE 0x1007b8a0
// RBX::DataModel::isStackTooBig
// FUNCTION: WEBSERVICE 0x1007b8e0
// RBX::DescribedNonCreatable<RBX::DataModel,RBX::ServiceProvider,&RBX::sDataModel>::DescribedNonCreatable<RBX::DataModel,RBX::ServiceProvider,&RBX::sDataModel>
// SYNTHETIC: WEBSERVICE 0x1007bf10
// RBX::DataModel::`scalar deleting destructor'
// TEMPLATE: WEBSERVICE 0x1007bf30
// RBX::Creatable<RBX::Instance>::create<RBX::DataModel>
// SYNTHETIC: WEBSERVICE 0x102190a0
// RBX::`dynamic initializer for 'getContentFunctionOld''
// SYNTHETIC: WEBSERVICE 0x102190d0
// RBX::`dynamic initializer for 'getContentFunction''
// SYNTHETIC: WEBSERVICE 0x10219100
// RBX::`dynamic initializer for 'loadFunction''
// SYNTHETIC: WEBSERVICE 0x10219130
// RBX::`dynamic initializer for 'saveFunction''
// SYNTHETIC: WEBSERVICE 0x10219160
// RBX::`dynamic initializer for 'httpGetFunction''
// SYNTHETIC: WEBSERVICE 0x102191a0
// RBX::`dynamic initializer for 'httpPostFunction''
// SYNTHETIC: WEBSERVICE 0x102191e0
// RBX::`dynamic initializer for 'sanitizeFunction''
// SYNTHETIC: WEBSERVICE 0x10219210
// RBX::`dynamic initializer for 'closeFunction''
// SYNTHETIC: WEBSERVICE 0x10219240
// RBX::`dynamic initializer for 'func_SetUiMessage''
// SYNTHETIC: WEBSERVICE 0x10219270
// RBX::`dynamic initializer for 'func_ClearUIMessage''
// SYNTHETIC: WEBSERVICE 0x102192a0
// RBX::`dynamic initializer for 'func_SetUIMessageBrickCount''
// SYNTHETIC: WEBSERVICE 0x102192d0
// RBX::`dynamic initializer for 'event_Closing''
// SYNTHETIC: WEBSERVICE 0x10221ec0
// RBX::`dynamic atexit destructor for 'saveFunction''
// SYNTHETIC: WEBSERVICE 0x10221ed0
// RBX::`dynamic atexit destructor for 'closeFunction''
// SYNTHETIC: WEBSERVICE 0x10221ee0
// RBX::`dynamic atexit destructor for 'func_ClearUIMessage''
// SYNTHETIC: WEBSERVICE 0x10221ef0
// RBX::`dynamic atexit destructor for 'func_SetUIMessageBrickCount''
// SYNTHETIC: WEBSERVICE 0x10222240
// RBX::`dynamic atexit destructor for 'getContentFunction''
// SYNTHETIC: WEBSERVICE 0x10222250
// RBX::`dynamic atexit destructor for 'loadFunction''
// SYNTHETIC: WEBSERVICE 0x10222260
// RBX::`dynamic atexit destructor for 'httpGetFunction''
// SYNTHETIC: WEBSERVICE 0x10222270
// RBX::`dynamic atexit destructor for 'httpPostFunction''
// SYNTHETIC: WEBSERVICE 0x10222280
// RBX::`dynamic atexit destructor for 'sanitizeFunction''
// SYNTHETIC: WEBSERVICE 0x10222290
// RBX::`dynamic atexit destructor for 'func_SetUiMessage''
// SYNTHETIC: WEBSERVICE 0x102222a0
// RBX::`dynamic atexit destructor for 'getContentFunctionOld''
// SYNTHETIC: WEBSERVICE 0x102222b0
// RBX::`dynamic atexit destructor for 'event_Closing''
// clang-format on

DECOMP_SIZE_ASSERT(DataModel::Lock, 0xc)
DECOMP_SIZE_ASSERT(DataModel, 0x1fc)

} // namespace RBX

#endif // V8DATAMODEL_DATAMODEL_H
