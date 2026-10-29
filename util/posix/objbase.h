#ifndef POSIX_OBJBASE_H
#define POSIX_OBJBASE_H

#include <windows.h>

inline HRESULT CoCreateGuid(GUID* guid)
{
	FILE* f = fopen("/dev/urandom", "rb");
	if (!f || fread(guid, sizeof(GUID), 1, f) != 1) {
		if (f) {
			fclose(f);
		}
		return E_FAIL;
	}
	fclose(f);
	guid->Data3 = (uint16_t) ((guid->Data3 & 0x0FFF) | 0x4000);
	guid->Data4[0] = (uint8_t) ((guid->Data4[0] & 0x3F) | 0x80);
	return S_OK;
}

#endif // POSIX_OBJBASE_H
