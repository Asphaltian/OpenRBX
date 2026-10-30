#include "AboutRobloxDialog.h"

#include "VersionInfo.h"

#include <G3D/System.h>
#include <GLG3D/GLCaps.h>
#include <string>

IMPLEMENT_DYNCREATE(CAboutRobloxDialog, CRobloxDHtmlDialog)

// FUNCTION: ROBLOX 0x004010b0
CAboutRobloxDialog::~CAboutRobloxDialog()
{
}

// FUNCTION: ROBLOX 0x004010d0
void CAboutRobloxDialog::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CDHtmlDialog::OnDocumentComplete(pDisp, szUrl);
	UpdateData(FALSE);
}

// FUNCTION: ROBLOX 0x00401100
BOOL CAboutRobloxDialog::OnInitDialog()
{
	CRobloxDHtmlDialog::OnInitDialog();
	return TRUE;
}

// FUNCTION: ROBLOX 0x00401110
HRESULT CAboutRobloxDialog::OnButtonOK(IHTMLElement* pElement)
{
	OnOK();
	return S_OK;
}

// FUNCTION: ROBLOX 0x00401260
CAboutRobloxDialog::CAboutRobloxDialog(CWnd* pParentWnd) : CRobloxDHtmlDialog(117, 118, pParentWnd)
{
	SetHostFlags(m_dwHostFlags | 0x1844007);
}

BEGIN_MESSAGE_MAP(CAboutRobloxDialog, CRobloxDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CAboutRobloxDialog)
DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
END_DHTML_EVENT_MAP()

// STUB: ROBLOX 0x004013f0
void CAboutRobloxDialog::DoDataExchange(CDataExchange* pDX)
{
	if (!pDX->m_bSaveAndValidate) {
		CVersionInfo versionInfo;
		CString fileName;
		AfxGetModuleShortFileName(AfxGetApp()->m_hInstance, fileName);
		versionInfo.Load(fileName);
		CString version = versionInfo.GetFileVersionAsString();
		DDX_DHtml_ElementText(pDX, _T("Version"), DISPID_IHTMLELEMENT_INNERHTML, version);

		std::string systemInfo;
		G3D::System::describeSystem(systemInfo);
		DDX_DHtml_ElementText(pDX, _T("systemInfo"), DISPID_IHTMLELEMENT_INNERHTML, CString(systemInfo.c_str()));

		if (glGetString(GL_VENDOR) == NULL) {
			CComPtr<IHTMLElement> element;
			if (GetElement(_T("glInfo"), &element) == S_OK) {
				CComPtr<IHTMLStyle> style;
				element->get_style(&style);
				style->put_display(CComBSTR("none"));
			}
		}
		else {
			CString glVersion(G3D::GLCaps::glVersion().c_str());
			DDX_DHtml_ElementText(pDX, _T("glVersion"), DISPID_IHTMLELEMENT_INNERHTML, glVersion);
			CString driverVersion(G3D::GLCaps::driverVersion().c_str());
			DDX_DHtml_ElementText(pDX, _T("driverVersion"), DISPID_IHTMLELEMENT_INNERHTML, driverVersion);
			CString vendor(G3D::GLCaps::vendor().c_str());
			DDX_DHtml_ElementText(pDX, _T("vendor"), DISPID_IHTMLELEMENT_INNERHTML, vendor);
			CString renderer(G3D::GLCaps::renderer().c_str());
			DDX_DHtml_ElementText(pDX, _T("renderer"), DISPID_IHTMLELEMENT_INNERHTML, renderer);
		}
	}
}
