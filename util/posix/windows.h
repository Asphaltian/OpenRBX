#ifndef POSIX_WINDOWS_H
#define POSIX_WINDOWS_H

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>

typedef uint32_t DWORD;
typedef int BOOL;
typedef unsigned char BYTE;
typedef uint16_t WORD;
typedef int32_t LONG;
typedef uint32_t ULONG;
typedef unsigned int UINT;
typedef int INT;
typedef short SHORT;
typedef unsigned short USHORT;
typedef char CHAR;
typedef wchar_t WCHAR;
typedef long long LONGLONG;
typedef unsigned long long ULONGLONG;
typedef uintptr_t ULONG_PTR;
typedef uintptr_t DWORD_PTR;
typedef intptr_t INT_PTR;
typedef uintptr_t UINT_PTR;
typedef size_t SIZE_T;
typedef void* HANDLE;
typedef void* HMODULE;
typedef void* HINSTANCE;
typedef void* HWND;
typedef void* LPVOID;
typedef const void* LPCVOID;
typedef char* LPSTR;
typedef const char* LPCSTR;
typedef char* LPTSTR;
typedef const char* LPCTSTR;
typedef wchar_t* LPWSTR;
typedef const wchar_t* LPCWSTR;
typedef DWORD* LPDWORD;
typedef BYTE* LPBYTE;
typedef BYTE* PBYTE;
typedef BOOL* LPBOOL;
typedef long HRESULT;

#define WINAPI
#define CALLBACK
#define APIENTRY
#define __stdcall
#define __cdecl

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define MAX_PATH 260
#define INFINITE 0xFFFFFFFF
#define WAIT_OBJECT_0 0
#define WAIT_TIMEOUT 258
#define WAIT_FAILED 0xFFFFFFFF
#define INVALID_HANDLE_VALUE ((HANDLE) (intptr_t) -1)
#define TLS_OUT_OF_INDEXES 0xFFFFFFFF
#define S_OK 0
#define E_FAIL ((HRESULT) 0x80004005L)
#define SUCCEEDED(hr) (((HRESULT) (hr)) >= 0)
#define FAILED(hr) (((HRESULT) (hr)) < 0)

#define THREAD_PRIORITY_BELOW_NORMAL -1
#define THREAD_PRIORITY_NORMAL 0
#define THREAD_PRIORITY_HIGHEST 2

#define MB_OK 0x0
#define MB_YESNO 0x4
#define MB_ICONERROR 0x10
#define MB_ICONINFORMATION 0x40
#define IDOK 1
#define IDYES 6
#define IDNO 7

#define GENERIC_READ 0x80000000
#define GENERIC_WRITE 0x40000000
#define FILE_SHARE_READ 0x1
#define FILE_SHARE_WRITE 0x2
#define CREATE_NEW 1
#define CREATE_ALWAYS 2
#define OPEN_EXISTING 3
#define OPEN_ALWAYS 4
#define FILE_ATTRIBUTE_NORMAL 0x80
#define FILE_ATTRIBUTE_DIRECTORY 0x10
#define ERROR_ALREADY_EXISTS 183
#define ERROR_PATH_NOT_FOUND 3
#define VER_PLATFORM_WIN32_NT 2

typedef union _ULARGE_INTEGER {
	struct
	{
		DWORD LowPart;
		DWORD HighPart;
	} u;
	ULONGLONG QuadPart;
} ULARGE_INTEGER;

typedef union _LARGE_INTEGER {
	struct
	{
		DWORD LowPart;
		LONG HighPart;
	} u;
	LONGLONG QuadPart;
} LARGE_INTEGER;

typedef struct _FILETIME
{
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
} FILETIME, *LPFILETIME;

typedef struct _SYSTEMTIME
{
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
} SYSTEMTIME, *LPSYSTEMTIME;

typedef struct _OSVERSIONINFOA
{
	DWORD dwOSVersionInfoSize;
	DWORD dwMajorVersion;
	DWORD dwMinorVersion;
	DWORD dwBuildNumber;
	DWORD dwPlatformId;
	CHAR szCSDVersion[128];
} OSVERSIONINFO, OSVERSIONINFOA, *LPOSVERSIONINFO;

typedef struct _OSVERSIONINFOEXA
{
	DWORD dwOSVersionInfoSize;
	DWORD dwMajorVersion;
	DWORD dwMinorVersion;
	DWORD dwBuildNumber;
	DWORD dwPlatformId;
	CHAR szCSDVersion[128];
	WORD wServicePackMajor;
	WORD wServicePackMinor;
	WORD wSuiteMask;
	BYTE wProductType;
	BYTE wReserved;
} OSVERSIONINFOEX, *LPOSVERSIONINFOEX;

typedef struct _MEMORYSTATUS
{
	DWORD dwLength;
	DWORD dwMemoryLoad;
	SIZE_T dwTotalPhys;
	SIZE_T dwAvailPhys;
	SIZE_T dwTotalPageFile;
	SIZE_T dwAvailPageFile;
	SIZE_T dwTotalVirtual;
	SIZE_T dwAvailVirtual;
} MEMORYSTATUS, *LPMEMORYSTATUS;

typedef pthread_mutex_t CRITICAL_SECTION;
typedef CRITICAL_SECTION* LPCRITICAL_SECTION;

typedef struct _GUID
{
	uint32_t Data1;
	uint16_t Data2;
	uint16_t Data3;
	uint8_t Data4[8];
} GUID, UUID;

inline DWORD GetLastError()
{
	return (DWORD) errno;
}

inline void SetLastError(DWORD error)
{
	errno = (int) error;
}

inline BOOL QueryPerformanceFrequency(LARGE_INTEGER* frequency)
{
	frequency->QuadPart = 1000000000LL;
	return TRUE;
}

inline BOOL QueryPerformanceCounter(LARGE_INTEGER* counter)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	counter->QuadPart = (LONGLONG) ts.tv_sec * 1000000000LL + ts.tv_nsec;
	return TRUE;
}

inline DWORD GetTickCount()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (DWORD) (ts.tv_sec * 1000ULL + ts.tv_nsec / 1000000);
}

inline DWORD timeGetTime()
{
	return GetTickCount();
}

inline void Sleep(DWORD milliseconds)
{
	usleep((useconds_t) milliseconds * 1000);
}

inline HANDLE GetCurrentThread()
{
	return (HANDLE) (intptr_t) -2;
}

inline HANDLE GetCurrentProcess()
{
	return (HANDLE) (intptr_t) -1;
}

inline DWORD GetCurrentThreadId()
{
	return (DWORD) (uintptr_t) pthread_self();
}

inline DWORD GetCurrentProcessId()
{
	return (DWORD) getpid();
}

inline void timevalToFileTime(const struct timeval& tv, FILETIME* ft)
{
	ULONGLONG ticks = (ULONGLONG) tv.tv_sec * 10000000ULL + (ULONGLONG) tv.tv_usec * 10ULL;
	ft->dwLowDateTime = (DWORD) ticks;
	ft->dwHighDateTime = (DWORD) (ticks >> 32);
}

inline BOOL GetThreadTimes(HANDLE, LPFILETIME creation, LPFILETIME exitTime, LPFILETIME kernel, LPFILETIME user)
{
	struct rusage usage;
	if (getrusage(RUSAGE_THREAD, &usage) != 0) {
		return FALSE;
	}
	memset(creation, 0, sizeof(FILETIME));
	memset(exitTime, 0, sizeof(FILETIME));
	timevalToFileTime(usage.ru_stime, kernel);
	timevalToFileTime(usage.ru_utime, user);
	return TRUE;
}

inline DWORD TlsAlloc()
{
	pthread_key_t key;
	if (pthread_key_create(&key, NULL) != 0) {
		return TLS_OUT_OF_INDEXES;
	}
	return (DWORD) key;
}

inline LPVOID TlsGetValue(DWORD index)
{
	return pthread_getspecific((pthread_key_t) index);
}

inline BOOL TlsSetValue(DWORD index, LPVOID value)
{
	return pthread_setspecific((pthread_key_t) index, value) == 0;
}

inline BOOL TlsFree(DWORD index)
{
	return pthread_key_delete((pthread_key_t) index) == 0;
}

inline LONG InterlockedIncrement(volatile LONG* value)
{
	return __sync_add_and_fetch(value, 1);
}

inline LONG InterlockedDecrement(volatile LONG* value)
{
	return __sync_sub_and_fetch(value, 1);
}

inline LONG InterlockedExchange(volatile LONG* target, LONG value)
{
	return __sync_lock_test_and_set(target, value);
}

inline LONG InterlockedCompareExchange(volatile LONG* target, LONG exchange, LONG comparand)
{
	return __sync_val_compare_and_swap(target, comparand, exchange);
}

inline void InitializeCriticalSection(LPCRITICAL_SECTION section)
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(section, &attr);
	pthread_mutexattr_destroy(&attr);
}

inline void DeleteCriticalSection(LPCRITICAL_SECTION section)
{
	pthread_mutex_destroy(section);
}

inline void EnterCriticalSection(LPCRITICAL_SECTION section)
{
	pthread_mutex_lock(section);
}

inline void LeaveCriticalSection(LPCRITICAL_SECTION section)
{
	pthread_mutex_unlock(section);
}

struct PosixHandle
{
	enum Kind
	{
		EVENT,
		THREAD,
		FILE
	};

	Kind kind;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	bool signalled;
	bool manualReset;
	pthread_t thread;
	bool finished;
	int fd;
};

inline PosixHandle* newPosixHandle(PosixHandle::Kind kind)
{
	PosixHandle* h = new PosixHandle();
	h->kind = kind;
	pthread_mutex_init(&h->mutex, NULL);
	pthread_cond_init(&h->cond, NULL);
	h->signalled = false;
	h->manualReset = false;
	h->finished = false;
	h->fd = -1;
	return h;
}

inline HANDLE CreateEvent(void*, BOOL manualReset, BOOL initialState, LPCSTR)
{
	PosixHandle* h = newPosixHandle(PosixHandle::EVENT);
	h->manualReset = manualReset != FALSE;
	h->signalled = initialState != FALSE;
	return h;
}

inline BOOL SetEvent(HANDLE handle)
{
	PosixHandle* h = (PosixHandle*) handle;
	pthread_mutex_lock(&h->mutex);
	h->signalled = true;
	pthread_cond_broadcast(&h->cond);
	pthread_mutex_unlock(&h->mutex);
	return TRUE;
}

inline DWORD WaitForSingleObject(HANDLE handle, DWORD milliseconds)
{
	PosixHandle* h = (PosixHandle*) handle;
	if (h->kind == PosixHandle::THREAD) {
		if (milliseconds == INFINITE) {
			pthread_join(h->thread, NULL);
			h->finished = true;
			return WAIT_OBJECT_0;
		}
	}
	pthread_mutex_lock(&h->mutex);
	DWORD result = WAIT_OBJECT_0;
	bool* flag = h->kind == PosixHandle::THREAD ? &h->finished : &h->signalled;
	if (milliseconds == INFINITE) {
		while (!*flag) {
			pthread_cond_wait(&h->cond, &h->mutex);
		}
	}
	else {
		struct timespec deadline;
		clock_gettime(CLOCK_REALTIME, &deadline);
		deadline.tv_sec += milliseconds / 1000;
		deadline.tv_nsec += (long) (milliseconds % 1000) * 1000000L;
		if (deadline.tv_nsec >= 1000000000L) {
			deadline.tv_sec += 1;
			deadline.tv_nsec -= 1000000000L;
		}
		while (!*flag) {
			if (pthread_cond_timedwait(&h->cond, &h->mutex, &deadline) == ETIMEDOUT) {
				break;
			}
		}
		if (!*flag) {
			result = WAIT_TIMEOUT;
		}
	}
	if (result == WAIT_OBJECT_0 && h->kind == PosixHandle::EVENT && !h->manualReset) {
		h->signalled = false;
	}
	pthread_mutex_unlock(&h->mutex);
	return result;
}

inline BOOL CloseHandle(HANDLE handle)
{
	if (handle == NULL || handle == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	PosixHandle* h = (PosixHandle*) handle;
	if (h->kind == PosixHandle::THREAD && !h->finished) {
		pthread_detach(h->thread);
	}
	if (h->kind == PosixHandle::FILE && h->fd >= 0) {
		close(h->fd);
	}
	pthread_cond_destroy(&h->cond);
	pthread_mutex_destroy(&h->mutex);
	delete h;
	return TRUE;
}

inline BOOL SetThreadPriority(HANDLE, int)
{
	return TRUE;
}

inline void GetLocalTime(LPSYSTEMTIME st)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	struct tm local;
	localtime_r(&tv.tv_sec, &local);
	st->wYear = (WORD) (local.tm_year + 1900);
	st->wMonth = (WORD) (local.tm_mon + 1);
	st->wDayOfWeek = (WORD) local.tm_wday;
	st->wDay = (WORD) local.tm_mday;
	st->wHour = (WORD) local.tm_hour;
	st->wMinute = (WORD) local.tm_min;
	st->wSecond = (WORD) local.tm_sec;
	st->wMilliseconds = (WORD) (tv.tv_usec / 1000);
}

inline void GetSystemTime(LPSYSTEMTIME st)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	struct tm utc;
	gmtime_r(&tv.tv_sec, &utc);
	st->wYear = (WORD) (utc.tm_year + 1900);
	st->wMonth = (WORD) (utc.tm_mon + 1);
	st->wDayOfWeek = (WORD) utc.tm_wday;
	st->wDay = (WORD) utc.tm_mday;
	st->wHour = (WORD) utc.tm_hour;
	st->wMinute = (WORD) utc.tm_min;
	st->wSecond = (WORD) utc.tm_sec;
	st->wMilliseconds = (WORD) (tv.tv_usec / 1000);
}

inline BOOL GetVersionEx(LPOSVERSIONINFO info)
{
	struct utsname name;
	uname(&name);
	int major = 0;
	int minor = 0;
	sscanf(name.release, "%d.%d", &major, &minor);
	info->dwMajorVersion = (DWORD) major;
	info->dwMinorVersion = (DWORD) minor;
	info->dwBuildNumber = 0;
	info->dwPlatformId = VER_PLATFORM_WIN32_NT;
	snprintf(info->szCSDVersion, sizeof(info->szCSDVersion), "%s", name.sysname);
	return TRUE;
}

inline BOOL GetVersionEx(LPOSVERSIONINFOEX info)
{
	return GetVersionEx((LPOSVERSIONINFO) info);
}

inline void GlobalMemoryStatus(LPMEMORYSTATUS status)
{
	struct sysinfo info;
	sysinfo(&info);
	memset(status, 0, sizeof(MEMORYSTATUS));
	status->dwLength = sizeof(MEMORYSTATUS);
	status->dwTotalPhys = (SIZE_T) info.totalram * info.mem_unit;
	status->dwAvailPhys = (SIZE_T) info.freeram * info.mem_unit;
	status->dwTotalPageFile = (SIZE_T) info.totalswap * info.mem_unit;
	status->dwAvailPageFile = (SIZE_T) info.freeswap * info.mem_unit;
	if (info.totalram) {
		status->dwMemoryLoad = (DWORD) (100 - (info.freeram * 100) / info.totalram);
	}
}

inline int MessageBox(HWND, LPCSTR text, LPCSTR caption, UINT type)
{
	fprintf(stderr, "%s: %s\n", caption ? caption : "", text ? text : "");
	return (type & 0xF) == MB_YESNO ? IDYES : IDOK;
}

inline BOOL CreateDirectory(LPCSTR path, void*)
{
	return mkdir(path, 0755) == 0 || errno == EEXIST;
}

inline BOOL DeleteFile(LPCSTR path)
{
	return unlink(path) == 0;
}

inline HANDLE CreateFile(LPCSTR path, DWORD access, DWORD, void*, DWORD disposition, DWORD, HANDLE)
{
	int flags = 0;
	if ((access & GENERIC_READ) && (access & GENERIC_WRITE)) {
		flags = O_RDWR;
	}
	else if (access & GENERIC_WRITE) {
		flags = O_WRONLY;
	}
	else {
		flags = O_RDONLY;
	}
	if (disposition == CREATE_ALWAYS) {
		flags |= O_CREAT | O_TRUNC;
	}
	else if (disposition == CREATE_NEW) {
		flags |= O_CREAT | O_EXCL;
	}
	else if (disposition == OPEN_ALWAYS) {
		flags |= O_CREAT;
	}
	int fd = open(path, flags, 0644);
	if (fd < 0) {
		return INVALID_HANDLE_VALUE;
	}
	PosixHandle* h = newPosixHandle(PosixHandle::FILE);
	h->fd = fd;
	return h;
}

inline BOOL WriteFile(HANDLE handle, LPCVOID buffer, DWORD size, LPDWORD written, void*)
{
	PosixHandle* h = (PosixHandle*) handle;
	ssize_t n = write(h->fd, buffer, size);
	if (written) {
		*written = n < 0 ? 0 : (DWORD) n;
	}
	return n >= 0;
}

inline BOOL ReadFile(HANDLE handle, LPVOID buffer, DWORD size, LPDWORD read_, void*)
{
	PosixHandle* h = (PosixHandle*) handle;
	ssize_t n = read(h->fd, buffer, size);
	if (read_) {
		*read_ = n < 0 ? 0 : (DWORD) n;
	}
	return n >= 0;
}

inline DWORD GetModuleFileName(HMODULE, LPSTR buffer, DWORD size)
{
	ssize_t n = readlink("/proc/self/exe", buffer, size ? size - 1 : 0);
	if (n < 0) {
		n = 0;
	}
	if (size) {
		buffer[n] = 0;
	}
	return (DWORD) n;
}

typedef wchar_t* PWSTR;
#define E_NOTIMPL ((HRESULT) 0x80004001L)

inline HMODULE LoadLibrary(LPCSTR)
{
	return NULL;
}

inline void* GetProcAddress(HMODULE, LPCSTR)
{
	return NULL;
}

inline BOOL FreeLibrary(HMODULE)
{
	return TRUE;
}

inline void CoTaskMemFree(void* p)
{
	free(p);
}

inline long InterlockedIncrement(volatile long* value)
{
	return __sync_add_and_fetch(value, 1);
}

inline long InterlockedDecrement(volatile long* value)
{
	return __sync_sub_and_fetch(value, 1);
}

inline int GetThreadPriority(HANDLE)
{
	return THREAD_PRIORITY_NORMAL;
}

#define ZeroMemory(p, n) memset((p), 0, (n))

#define SM_CXSCREEN 0
#define SM_CYSCREEN 1

inline int GetSystemMetrics(int)
{
	return 0;
}

typedef struct _DISPLAY_DEVICEA
{
	DWORD cb;
	CHAR DeviceName[32];
	CHAR DeviceString[128];
	DWORD StateFlags;
	CHAR DeviceID[128];
	CHAR DeviceKey[128];
} DISPLAY_DEVICE, *PDISPLAY_DEVICE;

inline BOOL EnumDisplayDevices(LPCSTR, DWORD, PDISPLAY_DEVICE, DWORD)
{
	return FALSE;
}

struct _EXCEPTION_POINTERS;
typedef struct _EXCEPTION_POINTERS EXCEPTION_POINTERS;
typedef LONG(WINAPI* LPTOP_LEVEL_EXCEPTION_FILTER)(struct _EXCEPTION_POINTERS*);

#define EXCEPTION_EXECUTE_HANDLER 1
#define EXCEPTION_CONTINUE_SEARCH 0
#define EXCEPTION_CONTINUE_EXECUTION -1

inline LPTOP_LEVEL_EXCEPTION_FILTER SetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER)
{
	return NULL;
}

typedef void* HKEY;
typedef DWORD REGSAM;

#define HKEY_CLASSES_ROOT ((HKEY) (uintptr_t) 0x80000000)
#define HKEY_CURRENT_USER ((HKEY) (uintptr_t) 0x80000001)
#define HKEY_LOCAL_MACHINE ((HKEY) (uintptr_t) 0x80000002)
#define KEY_READ 0x20019
#define ERROR_SUCCESS 0L
#define ERROR_FILE_NOT_FOUND 2L

#define FORMAT_MESSAGE_ALLOCATE_BUFFER 0x00000100
#define FORMAT_MESSAGE_IGNORE_INSERTS 0x00000200
#define FORMAT_MESSAGE_FROM_SYSTEM 0x00001000
#define LANG_NEUTRAL 0x00
#define SUBLANG_DEFAULT 0x01
#define SUBLANG_SYS_DEFAULT 0x02
#define MAKELANGID(p, s) ((((WORD) (s)) << 10) | (WORD) (p))

inline DWORD FormatMessage(DWORD, LPCVOID, DWORD error, DWORD, LPSTR buffer, DWORD size, void*)
{
	if (!buffer || !size) {
		return 0;
	}
	int n = snprintf(buffer, size, "%s", strerror((int) error));
	return n < 0 ? 0 : (DWORD) n;
}

#define CP_ACP 0

inline int WideCharToMultiByte(UINT, DWORD, LPCWSTR in, int inLength, LPSTR out, int outSize, LPCSTR, LPBOOL)
{
	int n = 0;
	for (int i = 0; inLength < 0 ? in[i] != 0 : i < inLength; ++i) {
		if (out && n < outSize) {
			out[n] = (char) in[i];
		}
		++n;
	}
	if (inLength < 0) {
		if (out && n < outSize) {
			out[n] = 0;
		}
		++n;
	}
	return n;
}

inline int MultiByteToWideChar(UINT, DWORD, LPCSTR in, int inLength, LPWSTR out, int outSize)
{
	int n = 0;
	for (int i = 0; inLength < 0 ? in[i] != 0 : i < inLength; ++i) {
		if (out && n < outSize) {
			out[n] = (wchar_t) (unsigned char) in[i];
		}
		++n;
	}
	if (inLength < 0) {
		if (out && n < outSize) {
			out[n] = 0;
		}
		++n;
	}
	return n;
}

inline int StringFromGUID2(const GUID& guid, LPWSTR out, int size)
{
	char text[40];
	snprintf(
		text,
		sizeof(text),
		"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		guid.Data1,
		guid.Data2,
		guid.Data3,
		guid.Data4[0],
		guid.Data4[1],
		guid.Data4[2],
		guid.Data4[3],
		guid.Data4[4],
		guid.Data4[5],
		guid.Data4[6],
		guid.Data4[7]
	);
	int n = (int) strlen(text) + 1;
	if (size < n) {
		return 0;
	}
	for (int i = 0; i < n; ++i) {
		out[i] = (wchar_t) text[i];
	}
	return n;
}

inline BOOL WriteProcessMemory(HANDLE, void*, const void*, SIZE_T, SIZE_T*)
{
	return FALSE;
}

#endif // POSIX_WINDOWS_H
