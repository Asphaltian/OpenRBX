#define _WIN32_WINNT 0x0400
#define _WIN32_DCOM

#include "WebService.h"

#include "util/Name.h"
#include "util/Sound.h"

namespace RBX {
class DataModel;
}

#include "ErrorUploader.h"
#include "LogManager.h"
#include "ThumbnailGenerator.h"
#include "VersionInfo.h"
#include "ViewBase.h"
#include "decomp.h"
#include "network/API.h"
#include "reflection/type.h"
#include "script/ScriptContext.h"
#include "util/ContentProvider.h"
#include "util/Profiling.h"
#include "util/standardout.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Workspace.h"
#include "v8datamodel/factoryregistration.h"
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
#include <boost/tokenizer.hpp>
#include <map>
#include <queue>
#include <shlobj.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

template const RBX::Name& RBX::Name::doDeclare<RBX::sWorkspace>();

boost::once_flag flagInitRoblox = BOOST_ONCE_INIT;
static bool initRobloxFailed;
static std::string dotVersion;

extern "C" BOOL WINAPI TerminateExtension(DWORD flags);

static boost::scoped_ptr<MainLogManager> mainLogManager;

// VTABLE: WEBSERVICE 0x10261998
// SIZE 0x28
class StandardOutLog : public RBX::Listener<RBX::StandardOut, RBX::StandardOutMessage>
{
private:
	boost::shared_ptr<RBX::StandardOut> standardOut; // 0x04

public:
	boost::mutex messagesMutex;                   // 0x0c
	std::queue<RBX::StandardOutMessage> messages; // 0x14

	// FUNCTION: WEBSERVICE 0x1000d010
	StandardOutLog() : standardOut(RBX::StandardOut::singleton())
	{
		boost::mutex::scoped_lock lock(RBX::StandardOut::singleton()->sync);
		RBX::startListening(standardOut, this);
	}

	// STUB: WEBSERVICE 0x1000d110
	virtual ~StandardOutLog()
	{
		boost::mutex::scoped_lock lock(RBX::StandardOut::singleton()->sync);
		RBX::stopListening(standardOut, this);
	}

protected:
	// FUNCTION: WEBSERVICE 0x1000d230
	virtual void onEvent(const RBX::StandardOut* source, RBX::StandardOutMessage event)
	{
		boost::mutex::scoped_lock lock(messagesMutex);

		messages.push(event);

		if (messages.size() > 1000) {
			messages.pop();
		}
	}
};

DECOMP_SIZE_ASSERT(StandardOutLog, 0x28)

static boost::scoped_ptr<StandardOutLog> standardOutLog;

// STUB: WEBSERVICE 0x1000d380
static void initRoblox()
{
	static bool firstTime = true;
	if (!firstTime) {
		return;
	}

	firstTime = false;
	initRobloxFailed = false;

	try {
		RBX::Profiling::init(true);

		mainLogManager.reset(new MainLogManager("Roblox Web Service"));

		RobloxCrashReporter::silent = true;
		mainLogManager->WriteCrashDump();

		standardOutLog.reset(new StandardOutLog());
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_INFO, "Intializing Roblox Web Service");

		RBX::ContentProvider::singleton();

		char szPath[MAX_PATH] = "";
		if (SHGetFolderPathAndSubDir(NULL, CSIDL_COMMON_APPDATA, NULL, 0, "Roblox\\content\\", szPath) != S_OK) {
			RBX::StandardOut::singleton()->print(
				RBX::MESSAGE_ERROR, "Unable to find CSIDL_COMMON_APPDATA\\Roblox\\content"
			);
		}
		else {
			RBX::ContentProvider::singleton().setAssetFolder(szPath);
		}

		static RBX::FactoryRegistrator registerFactoryObjects;
		ThumbnailGenerator::classDescriptor();

		RBX::Soundscape::SoundService::soundDisabled = true;
		RBX::ViewBase::getShadowsEnabled() = false;

		CVersionInfo vi;
		vi.Load(ATL::_AtlBaseModule.GetModuleInstance());

		std::string v = (LPCSTR) vi.GetFileVersionAsString();

		RBX::Network::API::init(v.c_str());

		boost::tokenizer<boost::char_separator<char> > tokens(v, boost::char_separator<char>(" ,."));
		for (boost::tokenizer<boost::char_separator<char> >::iterator tok_iter = tokens.begin();
			 tok_iter != tokens.end();
			 ++tok_iter) {
			if (tok_iter != tokens.begin()) {
				dotVersion += ".";
			}
			dotVersion += *tok_iter;
		}

		static ErrorUploader errorUploader;
		errorUploader.Upload("http://www.roblox.com/Error/Grid.ashx", true);
	}
	catch (...) {
		initRobloxFailed = true;
	}
}

extern "C" const __declspec(selectany) GUID __LIBID_ = {
	0x7424937a, 0xe183, 0x3486, {0xa9, 0x8f, 0x2c, 0x3b, 0x62, 0xad, 0x01, 0xf3}
};
struct __declspec(uuid("7424937a-e183-3486-a98f-2c3b62ad01f3")) RobloxWebService;

// SIZE 0x2c
class CDllMainOverride : public ATL::CAtlDllModuleT<CDllMainOverride>
{
public:
	DECLARE_LIBID(__uuidof(RobloxWebService))

	BOOL WINAPI DllMain(DWORD dwReason, LPVOID lpReserved) { return __super::DllMain(dwReason, lpReserved); }
};

extern CDllMainOverride _AtlModule;

#ifndef DLLMAIN_HINSTANCE
#ifndef _WIN32_WCE
#define DLLMAIN_HINSTANCE HINSTANCE
#else
#define DLLMAIN_HINSTANCE HANDLE
#endif
#endif

extern "C" STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv);
extern "C" STDAPI DllRegisterServer(void);
extern "C" STDAPI DllUnregisterServer(void);
extern "C" STDAPI DllCanUnloadNow(void);
BOOL WINAPI DllMain(DLLMAIN_HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);

CDllMainOverride _AtlModule;

// FUNCTION: WEBSERVICE 0x1000e070
extern "C" STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
	return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

// FUNCTION: WEBSERVICE 0x1000e090
extern "C" STDAPI DllRegisterServer(void)
{
	return _AtlModule.DllRegisterServer();
}

// FUNCTION: WEBSERVICE 0x1000e0d0
extern "C" STDAPI DllUnregisterServer(void)
{
	return _AtlModule.DllUnregisterServer();
}

// FUNCTION: WEBSERVICE 0x1000e110
extern "C" STDAPI DllCanUnloadNow(void)
{
	return _AtlModule.DllCanUnloadNow();
}

#pragma managed(push, off)
// FUNCTION: WEBSERVICE 0x1000e130
BOOL WINAPI DllMain(DLLMAIN_HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved);
}
#pragma managed(pop)

#if defined(_M_IX86)
#pragma comment(linker, "/EXPORT:DllMain=_DllMain@12,PRIVATE")
#pragma comment(linker, "/EXPORT:DllRegisterServer=_DllRegisterServer@0,PRIVATE")
#pragma comment(linker, "/EXPORT:DllUnregisterServer=_DllUnregisterServer@0,PRIVATE")
#pragma comment(linker, "/EXPORT:DllGetClassObject=_DllGetClassObject@12,PRIVATE")
#pragma comment(linker, "/EXPORT:DllCanUnloadNow=_DllCanUnloadNow@0,PRIVATE")
#elif defined(_M_IA64) || defined(_M_AMD64)
#pragma comment(linker, "/EXPORT:DllMain,PRIVATE")
#pragma comment(linker, "/EXPORT:DllRegisterServer,PRIVATE")
#pragma comment(linker, "/EXPORT:DllUnregisterServer,PRIVATE")
#pragma comment(linker, "/EXPORT:DllGetClassObject,PRIVATE")
#pragma comment(linker, "/EXPORT:DllCanUnloadNow,PRIVATE")
#endif

namespace Roblox {

CWebService::Jobs CWebService::jobs;
boost::mutex CWebService::sync;

} // namespace Roblox

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
	if (dwFlags & HSE_TERM_ADVISORY_UNLOAD && Roblox::CWebService::hasOpenEnvironments()) {
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
	result->items = allocate(result->items, result->count);

	int i = 0;
	Jobs::iterator iter = jobs.begin();
	Jobs::iterator end = jobs.end();
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

// FUNCTION: WEBSERVICE 0x1000e730
HRESULT __stdcall CWebService::OpenJob(wchar_t* jobID, ScriptExecution script, double timeout, LuaArguments* result)
{
	try {
		std::string id(ATL::CStringA(jobID).GetString());
		boost::shared_ptr<RBX::DataModel> dataModel = RBX::DataModel::createDataModel();
		{
			boost::mutex::scoped_lock lock(sync);
			Job job;
			job.dataModel = dataModel;
			touch(job, timeout);
			jobs[id] = job;
		}
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_INFO, "Opened Job %s", id.c_str());
	}
	catch (std::exception& e) {
		return ReportException(e);
	}

	return Execute(jobID, script, result);
}

// FUNCTION: WEBSERVICE 0x1000e980
HRESULT __stdcall CWebService::CloseJob(wchar_t* jobID)
{
	try {
		boost::shared_ptr<RBX::DataModel> dataModel;
		{
			boost::mutex::scoped_lock lock(sync);
			Jobs::iterator iter = jobs.find(ATL::CStringA(jobID).GetString());
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
			Jobs::iterator end = jobs.end();
			for (Jobs::iterator iter = jobs.begin(); iter != end; ++iter) {
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
			Jobs::iterator end = jobs.end();
			for (Jobs::iterator iter = jobs.begin(); iter != end; ++iter) {
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

		Jobs::iterator end = jobs.end();
		for (Jobs::iterator iter = jobs.begin(); iter != end; ++iter) {
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
CWebService::Jobs::iterator CWebService::getJob(wchar_t* jobID)
{
	Jobs::iterator job = jobs.find(ATL::CStringA(jobID).GetString());

	if (job == jobs.end()) {
		throw std::runtime_error(G3D::format("Job %s not found", ATL::CStringA(jobID).GetString()));
	}

	return job;
}

template <class T>
static void convert(const T& source, RBX::Reflection::Value& dest)
{
	switch (source.type) {
	case LUA_TNUMBER:
		dest = std::string(ATL::CStringA(source.value));
		dest.convert<double>();
		break;

	case LUA_TBOOLEAN:
		dest = std::string(ATL::CStringA(source.value));
		dest.convert<bool>();
		break;

	case LUA_TSTRING:
		dest = std::string(ATL::CStringA(source.value));
		break;

	case LUA_TTABLE: {
		dest = std::vector<RBX::Reflection::Value>(source.tableLength);

		std::vector<RBX::Reflection::Value>& table = dest.convert<std::vector<RBX::Reflection::Value> >();
		for (int i = 0; i < source.tableLength; ++i) {
			convert(source.table[i], table[i]);
		}
		break;
	}
	}
}

template <class T>
void CWebService::convertToLua(const RBX::Reflection::Value& source, T& dest)
{
	if (source.isType<void>()) {
		dest.type = (LuaType) LUA_TNIL;
		return;
	}

	if (source.isType<int>()) {
		dest.type = (LuaType) LUA_TNUMBER;
		ATL::CComBSTR str(source.get<std::string>().c_str());
		dest.value = str.Detach();
		return;
	}

	if (source.isType<float>()) {
		dest.type = (LuaType) LUA_TNUMBER;
		ATL::CComBSTR str(source.get<std::string>().c_str());
		dest.value = str.Detach();
		return;
	}

	if (source.isType<double>()) {
		dest.type = (LuaType) LUA_TNUMBER;
		ATL::CComBSTR str(source.get<std::string>().c_str());
		dest.value = str.Detach();
		return;
	}

	if (source.isType<bool>()) {
		dest.type = (LuaType) LUA_TBOOLEAN;
		ATL::CComBSTR str(source.get<std::string>().c_str());
		dest.value = str.Detach();
		return;
	}

	if (source.isType<std::string>()) {
		dest.type = (LuaType) LUA_TSTRING;
		ATL::CComBSTR str(source.get<std::string>().c_str());
		dest.value = str.Detach();
		return;
	}

	if (source.isType<RBX::ContentId>()) {
		dest.type = (LuaType) LUA_TSTRING;
		ATL::CComBSTR str(source.get<std::string>().c_str());
		dest.value = str.Detach();
		return;
	}

	if (source.isType<std::vector<RBX::Reflection::Value> >()) {
		const std::vector<RBX::Reflection::Value>& table = source.cast<const std::vector<RBX::Reflection::Value>&>();

		dest.type = (LuaType) LUA_TTABLE;
		dest.tableLength = table.size();
		dest.table = allocate(dest.table, table.size());
		for (size_t i = 0; i < table.size(); ++i) {
			convertToLua(table[i], dest.table[i]);
		}
		return;
	}
	dest.type = (LuaType) LUA_TNIL;
}

template <>
void convert<LuaValue10>(const LuaValue10& source, RBX::Reflection::Value& dest)
{
	switch (source.type) {
	case LUA_TNUMBER:
		dest = std::string(ATL::CStringA(source.value));
		dest.convert<double>();
		break;

	case LUA_TBOOLEAN:
		dest = std::string(ATL::CStringA(source.value));
		dest.convert<bool>();
		break;

	case LUA_TSTRING:
		dest = std::string(ATL::CStringA(source.value));
		break;
	}
}

template <>
void CWebService::convertToLua<LuaValue10>(const RBX::Reflection::Value& source, LuaValue10& dest)
{
	if (source.isType<void>()) {
		dest.type = (LuaType) LUA_TNIL;
		return;
	}

	if (source.isType<int>()) {
		dest.type = (LuaType) LUA_TNUMBER;
		ATL::CComBSTR str(source.get<std::string>().c_str());
		dest.value = str.Detach();
		return;
	}

	if (source.isType<float>()) {
		dest.type = (LuaType) LUA_TNUMBER;
		ATL::CComBSTR str(source.get<std::string>().c_str());
		dest.value = str.Detach();
		return;
	}

	if (source.isType<double>()) {
		dest.type = (LuaType) LUA_TNUMBER;
		ATL::CComBSTR str(source.get<std::string>().c_str());
		dest.value = str.Detach();
		return;
	}

	if (source.isType<bool>()) {
		dest.type = (LuaType) LUA_TBOOLEAN;
		ATL::CComBSTR str(source.get<std::string>().c_str());
		dest.value = str.Detach();
		return;
	}

	if (source.isType<std::string>()) {
		dest.type = (LuaType) LUA_TSTRING;
		ATL::CComBSTR str(source.get<std::string>().c_str());
		dest.value = str.Detach();
		return;
	}

	if (source.isType<RBX::ContentId>()) {
		dest.type = (LuaType) LUA_TSTRING;
		ATL::CComBSTR str(source.get<std::string>().c_str());
		dest.value = str.Detach();
		return;
	}
	dest.type = (LuaType) LUA_TNIL;
}

// FUNCTION: WEBSERVICE 0x1000f4e0
HRESULT __stdcall CWebService::Execute(wchar_t* jobID, ScriptExecution script, LuaArguments* result)
{
	try {
		std::string code;
		code = ATL::CStringA(script.script);
		if (RBX::ContentProvider::isHttpUrl(code)) {
			std::auto_ptr<std::istream> stream = RBX::ContentProvider::singleton().getContent(RBX::ContentId(code));
			std::stringstream out;
			out << stream->rdbuf();
			code = out.str();
		}

		boost::shared_ptr<RBX::DataModel> dataModel;
		{
			boost::mutex::scoped_lock lock(sync);
			Jobs::iterator iter = getJob(jobID);
			dataModel = iter->second.dataModel;
		}

		std::auto_ptr<std::vector<RBX::Reflection::Value> > tuple;
		{
			std::vector<RBX::Reflection::Value> args(script.arguments.count);
			for (int i = 0; i < script.arguments.count; ++i) {
				convert(script.arguments.items[i], args[i]);
			}

			RBX::DataModel::Lock lock(dataModel);
			RBX::ScriptContext* context = dataModel ? dataModel->create<RBX::ScriptContext>() : NULL;
			tuple = context->executeInNewThread(
				RBX::Security::TrustedWebService, code.c_str(), ATL::CStringA(script.name), args
			);
		}

		result->count = tuple->size();
		result->items = allocate(result->items, tuple->size());
		for (size_t i = 0; i < tuple->size(); ++i) {
			convertToLua((*tuple)[i], result->items[i]);
		}
	}
	catch (std::exception& e) {
		return ReportException(e);
	}

	return S_OK;
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

// FUNCTION: WEBSERVICE 0x1000faf0
HRESULT __stdcall CWebService::Update(wchar_t* url)
{
	try {
		throw std::runtime_error("Update not implemented");
	}
	catch (std::exception& e) {
		return ReportException(e);
	}

	return S_OK;
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
		result->items = allocate(result->items, result->count);

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

template const RBX::FactoryProduct<RBX::Soundscape::SoundService, RBX::Instance, RBX::Soundscape::sSoundService>::
	Creator RBX::FactoryProduct<RBX::Soundscape::SoundService, RBX::Instance, RBX::Soundscape::sSoundService>::creator;
template const RBX::FactoryProduct<RBX::Soundscape::SoundChannel, RBX::Instance, RBX::Soundscape::sSoundChannel>::
	Creator RBX::FactoryProduct<RBX::Soundscape::SoundChannel, RBX::Instance, RBX::Soundscape::sSoundChannel>::creator;

typedef std::vector<RBX::Reflection::Value> ValueVector;
template ValueVector::const_iterator ValueVector::begin() const;
template ValueVector::const_iterator ValueVector::end() const;
