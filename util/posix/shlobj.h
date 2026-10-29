#ifndef POSIX_SHLOBJ_H
#define POSIX_SHLOBJ_H

#include <string>
#include <windows.h>

#define CSIDL_APPDATA 0x001a
#define CSIDL_COMMON_APPDATA 0x0023
#define CSIDL_FLAG_CREATE 0x8000

inline HRESULT SHGetFolderPathAndSubDir(HWND, int folder, HANDLE, DWORD, LPCSTR subDirectory, LPSTR path)
{
	const char* base = getenv("XDG_DATA_HOME");
	std::string root;
	if (base && *base) {
		root = base;
	}
	else {
		const char* home = getenv("HOME");
		root = std::string(home ? home : "/tmp") + "/.local/share";
	}
	std::string result = root;
	if (subDirectory) {
		result += "/";
		for (const char* p = subDirectory; *p; ++p) {
			result += *p == '\\' ? '/' : *p;
		}
	}
	if (folder & CSIDL_FLAG_CREATE) {
		std::string partial;
		for (size_t i = 0; i < result.size(); ++i) {
			partial += result[i];
			if (result[i] == '/' && i > 0) {
				CreateDirectory(partial.c_str(), NULL);
			}
		}
		CreateDirectory(result.c_str(), NULL);
	}
	snprintf(path, MAX_PATH, "%s", result.c_str());
	return S_OK;
}

#endif // POSIX_SHLOBJ_H
