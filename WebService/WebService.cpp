#define _WIN32_WINNT 0x0400
#define _WIN32_DCOM

#include "WebService.h"

#include "util/Name.h"
#include "util/Sound.h"

namespace RBX {
class DataModel;
}

#include "WebServiceMaps.h"
#include "decomp.h"
#include "util/standardout.h"
#include "v8datamodel/Workspace.h"

#include <G3D/System.h>
#include <G3D/format.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlisapi.h>
#include <atlsoap.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/once.hpp>
#include <map>
#include <queue>
#include <stdexcept>
#include <string>

template const RBX::Name& RBX::Name::doDeclare<RBX::sWorkspace>();

boost::once_flag flagInitRoblox = BOOST_ONCE_INIT;
static bool initRobloxFailed;
static std::string dotVersion;

extern "C" BOOL WINAPI TerminateExtension(DWORD flags);

// VTABLE: WEBSERVICE 0x10261998
// SIZE 0x28
class StandardOutLog : public RBX::Listener<RBX::StandardOut, RBX::StandardOutMessage>
{
private:
	boost::shared_ptr<RBX::StandardOut> standardOut; // 0x04

public:
	StandardOutLog();
	virtual ~StandardOutLog();

	boost::mutex messagesMutex;                   // 0x0c
	std::queue<RBX::StandardOutMessage> messages; // 0x14

protected:
	virtual void onEvent(const RBX::StandardOut* source, RBX::StandardOutMessage event);
};

DECOMP_SIZE_ASSERT(StandardOutLog, 0x28)

static boost::scoped_ptr<StandardOutLog> standardOutLog;

// STUB: WEBSERVICE 0x1000d010
StandardOutLog::StandardOutLog() : standardOut(RBX::StandardOut::singleton())
{
	boost::mutex::scoped_lock lock(RBX::StandardOut::singleton()->sync);
	standardOut->addListener(this);
}

// STUB: WEBSERVICE 0x1000d110
StandardOutLog::~StandardOutLog()
{
	boost::mutex::scoped_lock lock(RBX::StandardOut::singleton()->sync);
	standardOut->removeListener(this);
}

// FUNCTION: WEBSERVICE 0x1000d230
void StandardOutLog::onEvent(const RBX::StandardOut* source, RBX::StandardOutMessage event)
{
	boost::mutex::scoped_lock lock(messagesMutex);

	messages.push(event);

	if (messages.size() > 1000) {
		messages.pop();
	}
}

// STUB: WEBSERVICE 0x1000d380
static void initRoblox()
{
	STUB(0x1000d380);
}

// SIZE 0x2c
class CDllMainOverride : public ATL::CAtlDllModuleT<CDllMainOverride>
{
};

CDllMainOverride _AtlModule;

namespace Roblox {

// SIZE 0x180
class CWebService : public IWebService, public ATL::CSoapHandler<CWebService>
{
public:
	// SIZE 0x10
	struct Job
	{
		boost::shared_ptr<RBX::DataModel> dataModel; // 0x00
		double expiration;                           // 0x08
	};

	HRESULT __stdcall HelloWorld(wchar_t** result);
	HRESULT __stdcall GetAllJobs(Strings* result);
	HRESULT __stdcall TouchJob(wchar_t* jobID, double expiration);
	HRESULT __stdcall OpenJob(wchar_t* jobID, ScriptExecution script, double expiration, LuaArguments* result);
	HRESULT __stdcall CloseJob(wchar_t* jobID);
	HRESULT __stdcall CloseTimedoutJobs(int* result);
	HRESULT __stdcall CloseOrphanedJobs(Strings jobs, int* result);
	HRESULT __stdcall CloseAllJobs();
	HRESULT __stdcall GetTimeout(wchar_t* jobID, double* timeout);
	HRESULT __stdcall Execute(wchar_t* jobID, ScriptExecution script, LuaArguments* result);
	HRESULT __stdcall BatchJob(wchar_t* jobID, ScriptExecution script, double expiration, LuaArguments* result);
	HRESULT __stdcall GetVersion(wchar_t** result);
	HRESULT __stdcall GetStatus(Status* result);
	HRESULT __stdcall Update(wchar_t* url);
	HRESULT __stdcall GetStandardOutMessages(int lastId, StandardOutMessages* result);

	// FUNCTION: WEBSERVICE 0x1000dcc0
	const _soapmap** GetFunctionMap() { return ___Roblox_CWebService_funcs; }

	// FUNCTION: WEBSERVICE 0x1000dcd0
	const _soapmap** GetHeaderMap() { return ___Roblox_CWebService_headers; }

	// FUNCTION: WEBSERVICE 0x1000dce0
	void* GetHeaderValue() { return this; }

	// FUNCTION: WEBSERVICE 0x1000dcf0
	HRESULT CallFunction(void* pvParam, const wchar_t* wszLocalName, int cchLocalName, size_t nItem)
	{
		switch (nItem) {
		case 0:
			return HelloWorld((wchar_t**) pvParam);
		case 1:
			return GetVersion((wchar_t**) pvParam);
		case 2:
			return GetStatus((Status*) pvParam);
		case 3:
			return Update(*(wchar_t**) pvParam);
		case 4: {
			OpenJobParams* p = (OpenJobParams*) pvParam;
			return OpenJob(p->jobID, p->script, p->expiration, &p->result);
		}
		case 5: {
			TouchJobParams* p = (TouchJobParams*) pvParam;
			return TouchJob(p->jobID, p->expiration);
		}
		case 6: {
			ExecuteParams* p = (ExecuteParams*) pvParam;
			return Execute(p->jobID, p->script, &p->result);
		}
		case 7:
			return CloseJob(*(wchar_t**) pvParam);
		case 8: {
			OpenJobParams* p = (OpenJobParams*) pvParam;
			return BatchJob(p->jobID, p->script, p->expiration, &p->result);
		}
		case 9: {
			GetTimeoutParams* p = (GetTimeoutParams*) pvParam;
			return GetTimeout(p->jobID, &p->timeout);
		}
		case 10: {
			CloseOrphanedJobsParams* p = (CloseOrphanedJobsParams*) pvParam;
			return CloseOrphanedJobs(p->jobs, &p->result);
		}
		case 11:
			return CloseAllJobs();
		case 12:
			return CloseTimedoutJobs((int*) pvParam);
		case 13:
			return GetAllJobs((Strings*) pvParam);
		case 14: {
			GetStandardOutMessagesParams* p = (GetStandardOutMessagesParams*) pvParam;
			return GetStandardOutMessages(p->lastId, &p->result);
		}
		}

		return E_FAIL;
	}

	// FUNCTION: WEBSERVICE 0x1000df50
	const wchar_t* GetNamespaceUri() { return L"urn:Roblox"; }

	// FUNCTION: WEBSERVICE 0x1000df60
	const char* GetNamespaceUriA() { return "urn:Roblox"; }

	// FUNCTION: WEBSERVICE 0x1000df70
	const char* GetServiceName() { return "Service"; }

private:
	friend BOOL WINAPI ::TerminateExtension(DWORD flags);

	std::map<std::string, Job>::iterator getJob(wchar_t* jobID);

	static std::map<std::string, Job> jobs;
	static boost::mutex sync;

	ATL::CComBSTR errorMessage; // 0x17c
};

DECOMP_SIZE_ASSERT(CWebService, 0x180)

} // namespace Roblox

DECLARE_REQUEST_HANDLER("Default", CWebService, Roblox::CWebService)

namespace Roblox {

std::map<std::string, CWebService::Job> CWebService::jobs;
boost::mutex CWebService::sync;

// FUNCTION: WEBSERVICE 0x1000dfb0
HRESULT __stdcall CWebService::HelloWorld(wchar_t** bstrOutput)
{
	ATL::CComBSTR answer(L"Hello World!");
	*bstrOutput = answer.Detach();

	return S_OK;
}

} // namespace Roblox

// FUNCTION: WEBSERVICE 0x1000e070
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

// FUNCTION: WEBSERVICE 0x1000e090
STDAPI DllRegisterServer()
{
	return _AtlModule.DllRegisterServer(FALSE);
}

// FUNCTION: WEBSERVICE 0x1000e0d0
STDAPI DllUnregisterServer()
{
	return _AtlModule.DllUnregisterServer(FALSE);
}

// FUNCTION: WEBSERVICE 0x1000e110
STDAPI DllCanUnloadNow()
{
	return _AtlModule.DllCanUnloadNow();
}

// FUNCTION: WEBSERVICE 0x1000e130
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	return _AtlModule.DllMain(dwReason, lpReserved);
}

// SIZE 0x328
class CRbxIsapiExtension : public ATL::CIsapiExtension<>
{
private:
	// FUNCTION: WEBSERVICE 0x1000e170
	const char* GetExtensionDesc() { return "ROBLOX Web Service"; }
};

CRbxIsapiExtension theExtension;

// FUNCTION: WEBSERVICE 0x1000e340
extern "C" DWORD WINAPI HttpExtensionProc(EXTENSION_CONTROL_BLOCK* lpECB)
{
	boost::call_once(initRoblox, flagInitRoblox);

	if (initRobloxFailed) {
		return HSE_STATUS_ERROR;
	}

	return theExtension.HttpExtensionProc(lpECB);
}

// FUNCTION: WEBSERVICE 0x1000e370
extern "C" BOOL WINAPI GetExtensionVersion(HSE_VERSION_INFO* pVer)
{
	return theExtension.GetExtensionVersion(pVer);
}

// FUNCTION: WEBSERVICE 0x1000e380
extern "C" BOOL WINAPI TerminateExtension(DWORD dwFlags)
{
	if (dwFlags & HSE_TERM_ADVISORY_UNLOAD && Roblox::CWebService::jobs.size() > 0) {
		return FALSE;
	}

	return theExtension.TerminateExtension(dwFlags);
}

namespace Roblox {

// STUB: WEBSERVICE 0x1000e4d0
HRESULT __stdcall CWebService::GetAllJobs(Strings* result)
{
	boost::mutex::scoped_lock lock(sync);

	wchar_t** items = NULL;
	result->count = jobs.size();

	if (result->count != 0) {
		size_t bytes = result->count * sizeof(wchar_t*);
		items = (wchar_t**) GetMemMgr()->Allocate(bytes);
		memset(items, 0, bytes);
	}

	result->items = items;

	int index = 0;
	for (std::map<std::string, Job>::const_iterator it = jobs.begin(); it != jobs.end(); ++it) {
		ATL::CComBSTR jobID(ATL::CStringA(it->first.c_str()));
		result->items[index] = jobID.Detach();
		index++;
	}

	return S_OK;
}

// STUB: WEBSERVICE 0x1000e650
HRESULT __stdcall CWebService::TouchJob(wchar_t* jobID, double timeout)
{
	boost::mutex::scoped_lock lock(sync);

	std::map<std::string, Job>::iterator job = getJob(jobID);
	job->second.expiration = G3D::System::getLocalTime() + timeout;

	return S_OK;
}

// STUB: WEBSERVICE 0x1000e730
HRESULT __stdcall CWebService::OpenJob(wchar_t* jobID, ScriptExecution script, double timeout, LuaArguments* result)
{
	STUB(0x1000e730);
	return E_NOTIMPL;
}

// STUB: WEBSERVICE 0x1000e980
HRESULT __stdcall CWebService::CloseJob(wchar_t* jobID)
{
	STUB(0x1000e980);
	return E_NOTIMPL;
}

// STUB: WEBSERVICE 0x1000ebd0
HRESULT __stdcall CWebService::CloseTimedoutJobs(int* result)
{
	STUB(0x1000ebd0);
	return E_NOTIMPL;
}

// STUB: WEBSERVICE 0x1000ee00
HRESULT __stdcall CWebService::CloseOrphanedJobs(Strings activeJobs, int* result)
{
	STUB(0x1000ee00);
	return E_NOTIMPL;
}

// STUB: WEBSERVICE 0x1000f0e0
HRESULT __stdcall CWebService::CloseAllJobs()
{
	STUB(0x1000f0e0);
	return E_NOTIMPL;
}

// STUB: WEBSERVICE 0x1000f2e0
HRESULT __stdcall CWebService::GetTimeout(wchar_t* jobID, double* timeout)
{
	boost::mutex::scoped_lock lock(sync);

	std::map<std::string, Job>::iterator job = getJob(jobID);
	*timeout = job->second.expiration - G3D::System::getLocalTime();

	return S_OK;
}

// STUB: WEBSERVICE 0x1000f3c0
std::map<std::string, CWebService::Job>::iterator CWebService::getJob(wchar_t* jobID)
{
	std::map<std::string, Job>::iterator job = jobs.find(std::string(ATL::CStringA(jobID)));

	if (job == jobs.end()) {
		throw std::runtime_error(G3D::format("Job %s not found", ATL::CStringA(jobID)));
	}

	return job;
}

// STUB: WEBSERVICE 0x1000f4e0
HRESULT __stdcall CWebService::Execute(wchar_t* jobID, ScriptExecution script, LuaArguments* result)
{
	STUB(0x1000f4e0);
	return E_NOTIMPL;
}

// FUNCTION: WEBSERVICE 0x1000f990
HRESULT __stdcall CWebService::BatchJob(wchar_t* jobID, ScriptExecution script, double timeout, LuaArguments* result)
{
	HRESULT hr = OpenJob(jobID, script, timeout, result);

	if (hr == S_OK) {
		hr = CloseJob(jobID);
	}

	return hr;
}

// FUNCTION: WEBSERVICE 0x1000f9f0
HRESULT __stdcall CWebService::GetVersion(wchar_t** result)
{
	ATL::CComBSTR version(ATL::CStringA(dotVersion.c_str()));
	*result = version.Detach();

	return S_OK;
}

// FUNCTION: WEBSERVICE 0x1000fab0
HRESULT __stdcall CWebService::GetStatus(Status* result)
{
	HRESULT hr = GetVersion(&result->version);

	if (hr != S_OK) {
		return hr;
	}

	boost::mutex::scoped_lock lock(sync);
	result->environmentCount = jobs.size();

	return S_OK;
}

// STUB: WEBSERVICE 0x1000faf0
HRESULT __stdcall CWebService::Update(wchar_t* url)
{
	throw std::runtime_error("Update not implemented");
}

// STUB: WEBSERVICE 0x1000fb80
HRESULT __stdcall CWebService::GetStandardOutMessages(int lastId, StandardOutMessages* result)
{
	boost::mutex::scoped_lock lock(standardOutLog->messagesMutex);

	while (standardOutLog->messages.size() > lastId) {
		standardOutLog->messages.pop();
	}

	StandardOutMessage* items = NULL;
	result->count = standardOutLog->messages.size();

	if (result->count != 0) {
		size_t bytes = result->count * sizeof(StandardOutMessage);
		items = (StandardOutMessage*) GetMemMgr()->Allocate(bytes);
		memset(items, 0, bytes);
	}

	result->items = items;

	int i = 0;
	while (!standardOutLog->messages.empty()) {
		const RBX::StandardOutMessage& message = standardOutLog->messages.front();

		result->items[i].type = (MessageType) message.type;
		result->items[i].time = message.time;

		ATL::CComBSTR text(ATL::CStringA(message.message.c_str()));
		result->items[i].text = text.Detach();

		standardOutLog->messages.pop();
		i++;
	}

	return S_OK;
}

} // namespace Roblox

template void RBX::Name::callDoDeclare<RBX::Soundscape::sSoundService>();
template const RBX::Name& RBX::Name::doDeclare<RBX::Soundscape::sSoundService>();
template void RBX::Name::callDoDeclare<RBX::Soundscape::sSoundChannel>();
template const RBX::Name& RBX::Name::doDeclare<RBX::Soundscape::sSoundChannel>();
