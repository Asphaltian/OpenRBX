#ifndef VERSIONINFO_H
#define VERSIONINFO_H

#include <atlstr.h>
#include <windows.h>

// PJ Naughter's CVersionInfo, www.naughter.com
class CVersionInfo
{
public:
	struct TRANSLATION
	{
		WORD m_wLangID;
		WORD m_wCodePage;
	};

	CVersionInfo();
	~CVersionInfo();

	int Load(HINSTANCE hInst);
	int Load(const CString& fileName);
	VS_FIXEDFILEINFO* GetFixedFileInfo();
	DWORD GetFileFlagsMask();
	DWORD GetFileFlags();
	DWORD GetOS();
	DWORD GetFileType();
	DWORD GetFileSubType();
	FILETIME GetCreationTime();
	unsigned __int64 GetFileVersion();
	unsigned __int64 GetProductVersion();
	CString GetValue(const CString& sKeyName);
	CString GetComments();
	CString GetCompanyName();
	CString GetFileDescription();
	CString GetFileVersionAsString();
	CString GetInternalName();
	CString GetLegalCopyright();
	CString GetLegalTrademarks();
	CString GetOriginalFilename();
	CString GetPrivateBuild();
	CString GetProductName();
	CString GetProductVersionAsString();
	CString GetSpecialBuild();
	int GetNumberOfTranslations();
	TRANSLATION* GetTranslation(int nIndex);
	void SetTranslation(int nIndex);

protected:
	void Unload();

	WORD m_wLangID;
	WORD m_wCharset;
	void* m_pVerData;
	TRANSLATION* m_pTranslations;
	int m_nTranslations;
	VS_FIXEDFILEINFO* m_pffi;
};

#endif // VERSIONINFO_H
