#ifndef POSIX_ATLBASE_H
#define POSIX_ATLBASE_H

#include <windows.h>

class CRegKey
{
public:
	LONG Open(HKEY, LPCSTR, REGSAM = KEY_READ) { return ERROR_FILE_NOT_FOUND; }
	LONG QueryDWORDValue(LPCSTR, DWORD&) { return ERROR_FILE_NOT_FOUND; }
	LONG QueryStringValue(LPCSTR, LPSTR, ULONG*) { return ERROR_FILE_NOT_FOUND; }
	LONG Close() { return ERROR_SUCCESS; }
};

#endif // POSIX_ATLBASE_H
