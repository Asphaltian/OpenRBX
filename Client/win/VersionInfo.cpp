#include "VersionInfo.h"

// FUNCTION: ROBLOX 0x0046e640
CVersionInfo::CVersionInfo()
{
	m_pVerData = NULL;
	m_pffi = NULL;
	m_wLangID = 0;
	m_wCharset = 1252;
	m_pTranslations = NULL;
	m_nTranslations = 0;
}

// FUNCTION: WEBSERVICE 0x1000c890
// FUNCTION: ROBLOX 0x0046e900
CVersionInfo::~CVersionInfo()
{
	Unload();
}

void CVersionInfo::Unload()
{
	m_pffi = NULL;

	if (m_pVerData) {
		delete[] m_pVerData;
		m_pVerData = NULL;
	}

	m_wLangID = 0;
	m_wCharset = 1252;
	m_pTranslations = NULL;
	m_nTranslations = 0;
}

// FUNCTION: WEBSERVICE 0x1000c8c0
// FUNCTION: ROBLOX 0x0046e930
int CVersionInfo::Load(HINSTANCE module)
{
	char name[500];
	GetModuleFileName(module, name, sizeof(name));

	CString strFileName(name);
	return Load(strFileName);
}

// FUNCTION: WEBSERVICE 0x1000c990
// FUNCTION: ROBLOX 0x0046e660
int CVersionInfo::Load(const CString& fileName)
{
	Unload();

	CString sFileName(fileName);
	DWORD dwHandle = 0;
	int bSuccess = FALSE;
	DWORD dwSize = GetFileVersionInfoSize(sFileName.GetBuffer(), &dwHandle);

	if (dwSize) {
		m_pVerData = new BYTE[dwSize];

		if (GetFileVersionInfo(sFileName.GetBuffer(), dwHandle, dwSize, m_pVerData)) {
			UINT nLen = 0;

			if (VerQueryValue(m_pVerData, "\\", (LPVOID*) &m_pffi, &nLen)) {
				if (VerQueryValue(m_pVerData, "\\VarFileInfo\\Translation", (LPVOID*) &m_pTranslations, &nLen) &&
					nLen >= sizeof(TRANSLATION)) {
					m_nTranslations = nLen / sizeof(TRANSLATION);
					m_wLangID = m_pTranslations[0].m_wLangID;
					m_wCharset = m_pTranslations[0].m_wCodePage;
				}

				bSuccess = TRUE;
			}
		}
	}

	if (!bSuccess) {
		if (m_pVerData) {
			delete[] m_pVerData;
			m_pVerData = NULL;
		}
	}

	return bSuccess;
}

// FUNCTION: WEBSERVICE 0x1000cb20
// FUNCTION: ROBLOX 0x0046e7b0
CString CVersionInfo::GetValue(const CString& sKey)
{
	CString strResult;

	CString sQueryValue;
	sQueryValue.Format("\\StringFileInfo\\%04x%04x\\%s", m_wLangID, m_wCharset, sKey);

	LPCTSTR pVal = NULL;
	UINT nLen = 0;

	if (VerQueryValue(m_pVerData, sQueryValue.GetBuffer(), (LPVOID*) &pVal, &nLen)) {
		strResult = pVal;
	}

	return strResult;
}

// FUNCTION: WEBSERVICE 0x1000cc70
// FUNCTION: ROBLOX 0x0046e890
CString CVersionInfo::GetFileVersionAsString()
{
	return GetValue("FileVersion");
}
