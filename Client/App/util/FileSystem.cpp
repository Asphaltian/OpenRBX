#include "util/FileSystem.h"

#include <atlpath.h>
#include <atlstr.h>
#include <shlobj.h>
#include <windows.h>

namespace RBX {

// SIZE 0x8
class VistaAPIs
{
	typedef HRESULT(WINAPI* GetKnownFolderPathPtr)(const GUID& rfid, DWORD dwFlags, HANDLE hToken, PWSTR* ppszPath);

	GetKnownFolderPathPtr getKnownFolderPath; // 0x00
	HINSTANCE gShell32DLLInst;                // 0x04

public:
	VistaAPIs() : getKnownFolderPath(NULL)
	{
		gShell32DLLInst = LoadLibrary("Shell32.dll");
		if (gShell32DLLInst) {
			getKnownFolderPath = (GetKnownFolderPathPtr) GetProcAddress(gShell32DLLInst, "SHGetKnownFolderPath");
		}
	}

	~VistaAPIs();

	HRESULT SHGetKnownFolderPath(const GUID& rfid, DWORD dwFlags, HANDLE hToken, PWSTR* ppszPath)
	{
		if (getKnownFolderPath) {
			return getKnownFolderPath(rfid, dwFlags, hToken, ppszPath);
		}
		return E_NOTIMPL;
	}

	typedef struct _TOKEN_MANDATORY_LABEL
	{
		SID_AND_ATTRIBUTES Label;
	} TOKEN_MANDATORY_LABEL, *PTOKEN_MANDATORY_LABEL;

	HRESULT STDMETHODCALLTYPE GetIntegrity(BYTE* pVal);
};

// FUNCTION: WEBSERVICE 0x1003e160
// FUNCTION: ROBLOX 0x005b99f0
VistaAPIs::~VistaAPIs()
{
	if (gShell32DLLInst) {
		FreeLibrary(gShell32DLLInst);
	}
}

// FUNCTION: WEBSERVICE 0x1003e170
// FUNCTION: ROBLOX 0x005b9a00
HRESULT STDMETHODCALLTYPE VistaAPIs::GetIntegrity(BYTE* pVal)
{
	*pVal = 0;

	OSVERSIONINFO osvi = {0};
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	if (osvi.dwMajorVersion < 6) {
		return S_OK;
	}

	HANDLE hToken;
	HANDLE hProcess = GetCurrentProcess();
	if (OpenProcessToken(hProcess, TOKEN_QUERY | TOKEN_QUERY_SOURCE, &hToken)) {
		DWORD dwLengthNeeded;
		if (!GetTokenInformation(hToken, (TOKEN_INFORMATION_CLASS) 25, NULL, 0, &dwLengthNeeded)) {
			DWORD dwError = GetLastError();
			if (dwError == ERROR_INSUFFICIENT_BUFFER) {
				PTOKEN_MANDATORY_LABEL pTIL = (PTOKEN_MANDATORY_LABEL) LocalAlloc(0, dwLengthNeeded);
				if (pTIL != NULL) {
					if (GetTokenInformation(
							hToken, (TOKEN_INFORMATION_CLASS) 25, pTIL, dwLengthNeeded, &dwLengthNeeded
						)) {
						DWORD dwIntegrityLevel = *GetSidSubAuthority(
							pTIL->Label.Sid, (DWORD) (UCHAR) (*GetSidSubAuthorityCount(pTIL->Label.Sid) - 1)
						);

						if (dwIntegrityLevel < 0x2000) {
							*pVal = 1;
						}
						else if (dwIntegrityLevel < 0x3000) {
							*pVal = 2;
						}
						else {
							*pVal = 3;
						}
					}
					LocalFree(pTIL);
				}
			}
		}
		CloseHandle(hToken);
	}

	return S_OK;
}

// FUNCTION: WEBSERVICE 0x1003e280
// FUNCTION: ROBLOX 0x005b9c50
std::string FileSystem::getUserAppDataDirectory(bool create, const char* subDirectory)
{
	VistaAPIs apis;

	BYTE integrity;
	apis.GetIntegrity(&integrity);
	if (integrity > 0) {
		GUID folderID;
		if (integrity == 1) {
			CLSIDFromString(L"{A520A1A4-1780-4FF6-BD18-167343C5AF16}", &folderID);
		}
		else {
			CLSIDFromString(L"{F1B32785-6FBA-4FCF-9D55-7B8E7F157091}", &folderID);
		}

		LPWSTR lppwstrPath = NULL;
		if (apis.SHGetKnownFolderPath(folderID, create ? 0x8000 : 0, NULL, &lppwstrPath) != E_NOTIMPL) {
			CString s(lppwstrPath);
			CoTaskMemFree(lppwstrPath);

			s += "\\Roblox\\";
			if (create) {
				if (!CreateDirectory(s, NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
					return "";
				}
			}

			if (subDirectory) {
				s += subDirectory;
				if (create) {
					if (!CreateDirectory(s, NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
						return "";
					}
				}
			}

			CPath path(s);
			path.AddBackslash();
			return (LPCSTR) path;
		}
	}

	int flags = create ? CSIDL_FLAG_CREATE : 0;
	std::string s = "Roblox\\";
	if (subDirectory) {
		s += subDirectory;
	}

	CPath path;
	HRESULT hr =
		SHGetFolderPathAndSubDir(NULL, flags | CSIDL_APPDATA, NULL, 0, s.c_str(), path.m_strPath.GetBuffer(MAX_PATH));
	if (hr != S_OK) {
		hr = SHGetFolderPathAndSubDir(
			NULL, flags | CSIDL_COMMON_APPDATA, NULL, 0, s.c_str(), path.m_strPath.GetBuffer(MAX_PATH)
		);
	}
	if (hr == S_OK) {
		path.m_strPath.ReleaseBuffer();
		path.AddBackslash();
		return (LPCSTR) path;
	}
	else {
		return "";
	}
}

// FUNCTION: WEBSERVICE 0x1003e670
// FUNCTION: ROBLOX 0x005ba040
std::string FileSystem::getCacheDirectory(bool create)
{
	VistaAPIs apis;

	BYTE integrity;
	apis.GetIntegrity(&integrity);
	if (integrity == 1) {
		GUID folderID;
		CLSIDFromString(L"{352481E8-33BE-4251-BA85-6007CAEDCF9D", &folderID);

		LPWSTR lppwstrPath = NULL;
		if (apis.SHGetKnownFolderPath(folderID, 0x8000, NULL, &lppwstrPath) != E_NOTIMPL) {
			CString s(lppwstrPath);
			CoTaskMemFree(lppwstrPath);

			s += "\\Roblox\\";
			if (create) {
				if (!CreateDirectory(s, NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
					return "";
				}
			}

			CPath path(s);
			path.AddBackslash();
			return (LPCSTR) path;
		}
	}

	char path[MAX_PATH];
	if (SHGetFolderPathAndSubDir(
			NULL, (create ? CSIDL_FLAG_CREATE : 0) | CSIDL_COMMON_APPDATA, NULL, 0, "Roblox\\Cache", path
		) == S_OK) {
		return path;
	}
	else {
		return "";
	}
}

} // namespace RBX
