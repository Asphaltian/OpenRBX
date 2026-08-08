#include "network/CrashReporter.h"

#include "decomp.h"

CrashReporter* CrashReporter::singleton;

// STUB: WEBSERVICE 0x1018b340
LONG CrashReporter::ProcessException(_EXCEPTION_POINTERS* exceptionInfo)
{
	STUB(0x1018b340);

	return EXCEPTION_CONTINUE_SEARCH;
}

// STUB: WEBSERVICE 0x1018b550
static LONG WINAPI CrashExceptionFilter(_EXCEPTION_POINTERS* exceptionInfo)
{
	STUB(0x1018b550);

	return EXCEPTION_CONTINUE_SEARCH;
}

static LPTOP_LEVEL_EXCEPTION_FILTER WINAPI
DummySetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
{
	return NULL;
}

// STUB: WEBSERVICE 0x1018b560
static void PreventSetUnhandledExceptionFilter()
{
	HMODULE kernel32 = LoadLibraryA("kernel32.dll");

	if (kernel32 == NULL) {
		return;
	}

	void* entry = GetProcAddress(kernel32, "SetUnhandledExceptionFilter");

	if (entry == NULL) {
		return;
	}

	unsigned char jump[5];
	SIZE_T written;

	jump[0] = 0xe9;
	*reinterpret_cast<DWORD*>(jump + 1) =
		reinterpret_cast<DWORD>(DummySetUnhandledExceptionFilter) - reinterpret_cast<DWORD>(entry) - 5;

	WriteProcessMemory(GetCurrentProcess(), entry, jump, 5, &written);
}

// FUNCTION: WEBSERVICE 0x1018b5e0
void CrashReporter::Start()
{
	SetUnhandledExceptionFilter(CrashExceptionFilter);

	PreventSetUnhandledExceptionFilter();
}
