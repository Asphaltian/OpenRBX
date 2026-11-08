#include "Content.h"

#include "InstanceExplorer.h"
#include "LogManager.h"
#include "util/Http.h"

#include <string>

// FUNCTION: ROBLOX 0x004135e0
STDMETHODIMP CContent::Upload(BSTR url)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	if (HRESULT hr = FUN_0041e630(url)) {
		CString message;
		message.Format("Untrusted Url: %s", CString(url));
		return LogManager::ReportCOMError(CLSID_Content, message, hr);
	}

	try {
		std::string response;
		RBX::Http(CString(url)).post(m_unk0x0c, true, response);
	}
	catch (const std::exception& e) {
		return LogManager::ReportExceptionAsCOMError(CLSID_Content, e);
	}

	return S_OK;
}
