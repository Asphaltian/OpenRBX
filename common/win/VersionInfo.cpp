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
int CVersionInfo::Load(HINSTANCE module)
{
	char name[500];
	GetModuleFileName(module, name, sizeof(name));

	CString strFileName(name);
	return Load(strFileName);
}

// STUB: WEBSERVICE 0x1000c990
int CVersionInfo::Load(const CString& fileName)
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

	CString sFileName(fileName);
	DWORD dwHandle = 0;
	DWORD dwSize = GetFileVersionInfoSize(sFileName.GetBuffer(), &dwHandle);

	if (dwSize) {
		m_pVerData = new BYTE[dwSize];

		if (GetFileVersionInfo(sFileName.GetBuffer(), dwHandle, dwSize, m_pVerData)) {
			UINT nLen = 0;

			if (VerQueryValue(m_pVerData, "\\", (LPVOID*) &m_pffi, &nLen)) {
				if (VerQueryValue(m_pVerData, "\\VarFileInfo\\Translation", (LPVOID*) &m_pTranslations, &nLen) &&
					nLen > 3) {
					m_nTranslations = nLen / 4;
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

// STUB: WEBSERVICE 0x1000cc70
CString CVersionInfo::GetFileVersionAsString()
{
	return GetValue(CString("FileVersion"));
}
