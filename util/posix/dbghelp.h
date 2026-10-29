#ifndef POSIX_DBGHELP_H
#define POSIX_DBGHELP_H

#include <windows.h>

typedef struct _EXCEPTION_POINTERS EXCEPTION_POINTERS, *PEXCEPTION_POINTERS;

typedef struct _MINIDUMP_EXCEPTION_INFORMATION
{
	DWORD ThreadId;
	PEXCEPTION_POINTERS ExceptionPointers;
	BOOL ClientPointers;
} MINIDUMP_EXCEPTION_INFORMATION;

typedef enum _MINIDUMP_TYPE
{
	MiniDumpNormal = 0,
	MiniDumpWithDataSegs = 1,
	MiniDumpWithFullMemory = 2,
	MiniDumpWithIndirectlyReferencedMemory = 0x40,
} MINIDUMP_TYPE;

inline BOOL MiniDumpWriteDump(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, MINIDUMP_EXCEPTION_INFORMATION*, void*, void*)
{
	return FALSE;
}

#endif // POSIX_DBGHELP_H
