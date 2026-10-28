#define _WIN32_WINNT 0x0400
#define _WIN32_DCOM

#include "WebService.h"

#include "util/Name.h"
#include "util/Sound.h"

namespace RBX {
class DataModel;
}

#include "ErrorUploader.h"
#include "WebServiceMaps.h"
#include "decomp.h"
#include "reflection/type.h"
#include "util/standardout.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Workspace.h"
#include "v8xml/XmlSerializer.h"

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
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

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
	static ErrorUploader errorUploader;
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

	virtual ~CWebService() {}

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
	HRESULT __stdcall GetStandardOutMessages(int maxCount, StandardOutMessages* result);

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

protected:
	// FUNCTION: WEBSERVICE 0x1000df80
	virtual HRESULT GenerateAppError(ATL::IWriteStream* pStream, HRESULT hr)
	{
		if (errorMessage.GetLength() > 0) {
			return SoapFault(SOAP_E_SERVER, errorMessage, -1);
		}

		return ATL::CSoapHandler<CWebService>::GenerateAppError(pStream, hr);
	}

private:
	friend BOOL WINAPI ::TerminateExtension(DWORD flags);

	HRESULT ReportException(const std::exception& e);

	std::map<std::string, Job>::iterator getJob(wchar_t* jobID);
	void touch(Job& job, double timeout) { job.expiration = G3D::System::getLocalTime() + timeout; }
	double secondsToTimeout(const Job& job) { return job.expiration - G3D::System::getLocalTime(); }

	template <class T>
	T* allocate(int count)
	{
		if (count == 0) {
			return NULL;
		}

		size_t bytes = count * sizeof(T);
		T* items = (T*) GetMemMgr()->Allocate(bytes);
		memset(items, 0, bytes);
		return items;
	}

	static std::map<std::string, Job> jobs;
	static boost::mutex sync;

	CStringW errorMessage; // 0x17c
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

// FUNCTION: WEBSERVICE 0x1000e3b0
HRESULT CWebService::ReportException(const std::exception& e)
{
	std::stringstream ss;
	TextXmlWriter::encodedWrite(ss, e.what());
	errorMessage = ATL::CStringW(ss.str().c_str());
	return E_FAIL;
}

// FUNCTION: WEBSERVICE 0x1000e4d0
HRESULT __stdcall CWebService::GetAllJobs(Strings* result)
{
	boost::mutex::scoped_lock lock(sync);

	result->count = jobs.size();
	result->items = allocate<wchar_t*>(result->count);

	int i = 0;
	std::map<std::string, Job>::iterator iter = jobs.begin();
	std::map<std::string, Job>::iterator end = jobs.end();
	while (iter != end) {
		ATL::CComBSTR bstr(ATL::CStringA(iter->first.c_str()));
		result->items[i] = bstr.Detach();
		++iter;
		++i;
	}

	return S_OK;
}

// FUNCTION: WEBSERVICE 0x1000e650
HRESULT __stdcall CWebService::TouchJob(wchar_t* jobID, double timeout)
{
	try {
		boost::mutex::scoped_lock lock(sync);
		touch(getJob(jobID)->second, timeout);
	}
	catch (std::exception& e) {
		return ReportException(e);
	}

	return S_OK;
}

// STUB: WEBSERVICE 0x1000e730
HRESULT __stdcall CWebService::OpenJob(wchar_t* jobID, ScriptExecution script, double timeout, LuaArguments* result)
{
	STUB(0x1000e730);
	return E_NOTIMPL;
}

// FUNCTION: WEBSERVICE 0x1000e980
HRESULT __stdcall CWebService::CloseJob(wchar_t* jobID)
{
	try {
		boost::shared_ptr<RBX::DataModel> dataModel;
		{
			boost::mutex::scoped_lock lock(sync);
			std::map<std::string, Job>::iterator iter = jobs.find(ATL::CStringA(jobID).GetString());
			if (iter != jobs.end()) {
				dataModel = iter->second.dataModel;
				jobs.erase(iter);
			}
		}

		if (dataModel) {
			RBX::DataModel::closeDataModel(dataModel, true);
			RBX::StandardOut::singleton()->print(RBX::MESSAGE_INFO, "Closed Job %s", ATL::CStringA(jobID).GetString());
		}
	}
	catch (std::exception& e) {
		return ReportException(e);
	}

	return S_OK;
}

// FUNCTION: WEBSERVICE 0x1000ebd0
HRESULT __stdcall CWebService::CloseTimedoutJobs(int* result)
{
	*result = 0;

	try {
		std::vector<boost::shared_ptr<RBX::DataModel> > dataModels;
		{
			boost::mutex::scoped_lock lock(sync);

			std::map<std::string, Job> keep;
			std::map<std::string, Job>::iterator end = jobs.end();
			for (std::map<std::string, Job>::iterator iter = jobs.begin(); iter != end; ++iter) {
				if (secondsToTimeout(iter->second) >= 0) {
					keep.insert(*iter);
				}
				else {
					dataModels.push_back(iter->second.dataModel);
				}
			}

			jobs = keep;
		}

		std::vector<boost::shared_ptr<RBX::DataModel> >::iterator iter = dataModels.begin();
		std::vector<boost::shared_ptr<RBX::DataModel> >::iterator end = dataModels.end();
		while (iter != end) {
			RBX::DataModel::closeDataModel(*iter, true);
			(*result)++;
			++iter;
		}
	}
	catch (std::exception& e) {
		return ReportException(e);
	}

	return S_OK;
}

// FUNCTION: WEBSERVICE 0x1000ee00
HRESULT __stdcall CWebService::CloseOrphanedJobs(Strings activeJobs, int* result)
{
	*result = 0;

	try {
		std::vector<boost::shared_ptr<RBX::DataModel> > dataModels;
		{
			std::map<std::string, Job> keep;
			boost::mutex::scoped_lock lock(sync);
			std::map<std::string, Job>::iterator end = jobs.end();
			for (std::map<std::string, Job>::iterator iter = jobs.begin(); iter != end; ++iter) {
				bool found = false;
				for (int i = 0; i < activeJobs.count; ++i) {
					if (iter->first == ATL::CStringA(activeJobs.items[i]).GetString()) {
						keep.insert(*iter);
						found = true;
						break;
					}
				}
				if (!found) {
					dataModels.push_back(iter->second.dataModel);
				}
			}

			jobs = keep;
		}

		std::vector<boost::shared_ptr<RBX::DataModel> >::iterator iter = dataModels.begin();
		std::vector<boost::shared_ptr<RBX::DataModel> >::iterator end = dataModels.end();
		while (iter != end) {
			RBX::DataModel::closeDataModel(*iter, true);
			(*result)++;
			++iter;
		}
	}
	catch (std::exception& e) {
		return ReportException(e);
	}

	return S_OK;
}

// FUNCTION: WEBSERVICE 0x1000f0e0
HRESULT __stdcall CWebService::CloseAllJobs()
{
	try {
		boost::mutex::scoped_lock lock(sync);

		std::map<std::string, Job>::iterator end = jobs.end();
		for (std::map<std::string, Job>::iterator iter = jobs.begin(); iter != end; ++iter) {
			RBX::DataModel::closeDataModel(iter->second.dataModel, true);
			RBX::StandardOut::singleton()->print(RBX::MESSAGE_INFO, "Closed Job %s", iter->first.c_str());
		}

		jobs.clear();
	}
	catch (std::exception& e) {
		return ReportException(e);
	}

	return S_OK;
}

// FUNCTION: WEBSERVICE 0x1000f2e0
HRESULT __stdcall CWebService::GetTimeout(wchar_t* jobID, double* timeout)
{
	try {
		boost::mutex::scoped_lock lock(sync);
		*timeout = secondsToTimeout(getJob(jobID)->second);
	}
	catch (std::exception& e) {
		return ReportException(e);
	}

	return S_OK;
}

// FUNCTION: WEBSERVICE 0x1000f3c0
std::map<std::string, CWebService::Job>::iterator CWebService::getJob(wchar_t* jobID)
{
	std::map<std::string, Job>::iterator job = jobs.find(ATL::CStringA(jobID).GetString());

	if (job == jobs.end()) {
		throw std::runtime_error(G3D::format("Job %s not found", ATL::CStringA(jobID).GetString()));
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

// FUNCTION: WEBSERVICE 0x1000fb80
HRESULT __stdcall CWebService::GetStandardOutMessages(int maxCount, StandardOutMessages* result)
{
	try {
		boost::mutex::scoped_lock lock(standardOutLog->messagesMutex);

		while (standardOutLog->messages.size() > maxCount) {
			standardOutLog->messages.pop();
		}

		result->count = standardOutLog->messages.size();
		result->items = allocate<StandardOutMessage>(result->count);

		int i = 0;
		while (standardOutLog->messages.size() > 0) {
			const RBX::StandardOutMessage& message = standardOutLog->messages.front();
			result->items[i].type = (MessageType) message.type;
			result->items[i].time = message.time;
			ATL::CComBSTR bstr(ATL::CStringA(message.message.c_str()));
			result->items[i].text = bstr.Detach();
			i++;
			standardOutLog->messages.pop();
		}
	}
	catch (std::exception& e) {
		return ReportException(e);
	}

	return S_OK;
}

} // namespace Roblox

template void RBX::Name::callDoDeclare<RBX::Soundscape::sSoundService>();
template const RBX::Name& RBX::Name::doDeclare<RBX::Soundscape::sSoundService>();
template void RBX::Name::callDoDeclare<RBX::Soundscape::sSoundChannel>();
template const RBX::Name& RBX::Name::doDeclare<RBX::Soundscape::sSoundChannel>();

typedef std::vector<RBX::Reflection::Value> ValueVector;
template ValueVector::const_iterator ValueVector::begin() const;
template ValueVector::const_iterator ValueVector::end() const;
