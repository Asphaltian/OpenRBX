#include "Content.h"

#include "InstanceExplorer.h"
#include "LogManager.h"
#include "util/ContentProvider.h"
#include "util/Http.h"

#include <atlsafe.h>
#include <string>

void FUN_004132b0(const RBX::Reflection::Value& value, VARIANT* result)
{
	if (value.isType<void>()) {
		V_VT(result) = VT_NULL;
		return;
	}

	if (value.isType<int>()) {
		CComVariant variant(value.cast<int>());
		variant.Detach(result);
		return;
	}

	if (value.isType<bool>()) {
		CComVariant variant(value.cast<bool>());
		variant.Detach(result);
		return;
	}

	if (value.isType<float>()) {
		CComVariant variant(value.cast<float>());
		variant.Detach(result);
		return;
	}

	if (value.isType<double>()) {
		CComVariant variant(value.cast<double>());
		variant.Detach(result);
		return;
	}

	if (value.isType<std::string>()) {
		CComVariant variant(value.cast<std::string>().c_str());
		variant.Detach(result);
		return;
	}

	if (value.isType<RBX::ContentId>()) {
		CComVariant variant(value.cast<RBX::ContentId>().c_str());
		variant.Detach(result);
		return;
	}

	if (value.isType<std::vector<RBX::Reflection::Value> >()) {
		CComVariant variant(FUN_00413500(value.cast<const std::vector<RBX::Reflection::Value>&>()));
		variant.Detach(result);
		return;
	}

	V_VT(result) = VT_EMPTY;
}

SAFEARRAY* FUN_00413500(const std::vector<RBX::Reflection::Value>& values)
{
	CComSafeArray<VARIANT> array;
	array.Create(values.size());

	for (int i = 0; i < values.size(); i++) {
		const RBX::Reflection::Value& value = values[i];
		FUN_004132b0(value, &array[i]);
	}

	return array.Detach();
}

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
