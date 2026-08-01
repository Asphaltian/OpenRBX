#define _WIN32_WINNT 0x0400
#define _WIN32_DCOM

#include "decomp.h"

#include <atlbase.h>
#include <atlcom.h>
#include <atlisapi.h>
#include <boost/thread/once.hpp>

static boost::once_flag flagInitRoblox = BOOST_ONCE_INIT;
static bool initRobloxFailed;

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

// FUNCTION: WEBSERVICE 0x1000e070
STDAPI DllGetClassObject(REFCLSID clsid, REFIID iid, LPVOID* ppv)
{
	return _AtlModule.DllGetClassObject(clsid, iid, ppv);
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
extern "C" BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	return _AtlModule.DllMain(reason, reserved);
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
extern "C" DWORD WINAPI HttpExtensionProc(EXTENSION_CONTROL_BLOCK* ecb)
{
	boost::call_once(initRoblox, flagInitRoblox);

	if (initRobloxFailed) {
		return HSE_STATUS_ERROR;
	}

	return theExtension.HttpExtensionProc(ecb);
}

// FUNCTION: WEBSERVICE 0x1000e370
extern "C" BOOL WINAPI GetExtensionVersion(HSE_VERSION_INFO* version)
{
	return theExtension.GetExtensionVersion(version);
}

// FUNCTION: WEBSERVICE 0x1000e380
extern "C" BOOL WINAPI TerminateExtension(DWORD flags)
{
	return theExtension.TerminateExtension(flags);
}
