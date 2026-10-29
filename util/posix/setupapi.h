#ifndef POSIX_SETUPAPI_H
#define POSIX_SETUPAPI_H

#include <windows.h>

typedef void* HDEVINFO;

typedef struct _SP_DEVINFO_DATA
{
	DWORD cbSize;
	GUID ClassGuid;
	DWORD DevInst;
	ULONG_PTR Reserved;
} SP_DEVINFO_DATA, *PSP_DEVINFO_DATA;

typedef struct _SP_CLASSIMAGELIST_DATA
{
	DWORD cbSize;
	void* ImageList;
	ULONG_PTR Reserved;
} SP_CLASSIMAGELIST_DATA, *PSP_CLASSIMAGELIST_DATA;

#define DIGCF_PRESENT 0x2
#define DIGCF_ALLCLASSES 0x4
#define DIGCF_PROFILE 0x8
#define SPDRP_DEVICEDESC 0x0
#define SPDRP_CLASS 0x7
#define SPDRP_FRIENDLYNAME 0xC
#define SPDRP_DRIVER 0x9

inline HDEVINFO SetupDiGetClassDevs(const GUID*, LPCSTR, HWND, DWORD)
{
	return INVALID_HANDLE_VALUE;
}

inline BOOL SetupDiEnumDeviceInfo(HDEVINFO, DWORD, PSP_DEVINFO_DATA)
{
	return FALSE;
}

inline BOOL SetupDiGetDeviceRegistryProperty(HDEVINFO, PSP_DEVINFO_DATA, DWORD, DWORD*, BYTE*, DWORD, DWORD*)
{
	return FALSE;
}

inline BOOL SetupDiGetClassImageList(PSP_CLASSIMAGELIST_DATA)
{
	return FALSE;
}

inline BOOL SetupDiDestroyClassImageList(PSP_CLASSIMAGELIST_DATA)
{
	return TRUE;
}

inline BOOL SetupDiGetClassImageIndex(PSP_CLASSIMAGELIST_DATA, const GUID*, int*)
{
	return FALSE;
}

inline BOOL SetupDiGetClassDescription(const GUID*, LPSTR, DWORD, DWORD*)
{
	return FALSE;
}

#endif // POSIX_SETUPAPI_H
