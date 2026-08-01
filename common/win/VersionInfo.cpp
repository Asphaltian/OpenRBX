#include "VersionInfo.h"

CVersionInfo::CVersionInfo()
{
	m_pffi = NULL;
	m_pVerData = NULL;
	m_wLangID = 0;
	m_pTranslations = NULL;
	m_nTranslations = 0;
	m_wCharset = 1252;
}

// FUNCTION: WEBSERVICE 0x1000c890
CVersionInfo::~CVersionInfo()
{
	m_pffi = NULL;

	if (m_pVerData) {
		delete[] m_pVerData;
		m_pVerData = NULL;
	}

	m_wLangID = 0;
	m_pTranslations = NULL;
	m_nTranslations = 0;
	m_wCharset = 1252;
}

// STUB: WEBSERVICE 0x1000c8c0
int CVersionInfo::Load(HINSTANCE hInst)
{
	char szFileName[500];
	GetModuleFileName(hInst, szFileName, sizeof(szFileName));

	CString strFileName(szFileName);
	return Load(strFileName);
}

// STUB: WEBSERVICE 0x1000c990
int CVersionInfo::Load(const CString& strFileName)
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

	CString str(strFileName);
	DWORD dwHandle = 0;
	DWORD dwSize = GetFileVersionInfoSize(str.GetBuffer(), &dwHandle);

	if (dwSize) {
		m_pVerData = new BYTE[dwSize];

		if (GetFileVersionInfo(str.GetBuffer(), dwHandle, dwSize, m_pVerData)) {
			UINT uLen = 0;

			if (VerQueryValue(m_pVerData, "\\", (LPVOID*) &m_pffi, &uLen)) {
				if (VerQueryValue(m_pVerData, "\\VarFileInfo\\Translation", (LPVOID*) &m_pTranslations, &uLen) &&
					uLen > 3) {
					m_nTranslations = uLen / 4;
					m_wLangID = m_pTranslations[0].m_wLangID;
					m_wCharset = m_pTranslations[0].m_wCodePage;
				}

				return 1;
			}
		}

		delete[] m_pVerData;
		m_pVerData = NULL;
	}

	return 0;
}

// STUB: WEBSERVICE 0x1000cb20
CString CVersionInfo::GetValue(const CString& strName)
{
	CString strResult;

	CString strQuery;
	strQuery.Format("\\StringFileInfo\\%04x%04x\\%s", m_wLangID, m_wCharset, strName);

	LPCTSTR pValue = NULL;
	UINT uLen = 0;

	if (VerQueryValue(m_pVerData, strQuery.GetBuffer(), (LPVOID*) &pValue, &uLen)) {
		strResult = pValue;
	}

	return strResult;
}

// STUB: WEBSERVICE 0x1000cc70
CString CVersionInfo::GetFileVersionAsString()
{
	return GetValue(CString("FileVersion"));
}
