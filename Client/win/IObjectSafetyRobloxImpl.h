#ifndef IOBJECTSAFETYROBLOXIMPL_H
#define IOBJECTSAFETYROBLOXIMPL_H

#include "LogManager.h"
#include "decomp.h"

#include <atlbase.h>
#include <atlcom.h>
#include <exdisp.h>
#include <objsafe.h>
#include <shlguid.h>
#include <urlmon.h>

HRESULT FUN_0041e6b0(BSTR url, DWORD zone);

template <class T, DWORD dwSupportedSafety>
class IObjectSafetyRobloxImpl : public IObjectSafety
{
public:
	IObjectSafetyRobloxImpl() : m_unk0x04(0) {}

	STDMETHOD(GetInterfaceSafetyOptions)(REFIID riid, DWORD* pdwSupportedOptions, DWORD* pdwEnabledOptions)
	{
		T* pT = static_cast<T*>(this);

		if (pdwSupportedOptions == NULL || pdwEnabledOptions == NULL) {
			return E_POINTER;
		}

		IUnknown* pUnk;
		HRESULT hr = pT->GetUnknown()->QueryInterface(riid, (void**) &pUnk);

		if (SUCCEEDED(hr)) {
			pUnk->Release();

			if (FUN_004085e0() == S_OK) {
				*pdwSupportedOptions = dwSupportedSafety;
				*pdwEnabledOptions = m_unk0x04;
			}
			else {
				*pdwSupportedOptions = dwSupportedSafety;
				*pdwEnabledOptions = 0;
			}
		}
		else {
			*pdwSupportedOptions = 0;
			*pdwEnabledOptions = 0;
		}

		return hr;
	}

	STDMETHOD(SetInterfaceSafetyOptions)(REFIID riid, DWORD dwOptionSetMask, DWORD dwEnabledOptions)
	{
		T* pT = static_cast<T*>(this);

		IUnknown* pUnk;
		if (FAILED(pT->GetUnknown()->QueryInterface(riid, (void**) &pUnk))) {
			return E_NOINTERFACE;
		}

		pUnk->Release();

		if (dwOptionSetMask & ~dwSupportedSafety) {
			return E_FAIL;
		}

		HRESULT hr = FUN_004085e0();
		if (hr != S_OK) {
			return hr;
		}

		m_unk0x04 = (m_unk0x04 & ~dwOptionSetMask) | (dwOptionSetMask & dwEnabledOptions);
		return S_OK;
	}

	HRESULT FUN_00408260(CComBSTR& url, DWORD& zone)
	{
		CComPtr<IServiceProvider> provider;
		T* pT = static_cast<T*>(this);

		HRESULT hr = pT->GetSite(__uuidof(IServiceProvider), (void**) &provider);
		if (FAILED(hr)) {
			return hr;
		}

		CComPtr<IWebBrowser2> browser;
		hr = provider->QueryService(SID_SWebBrowserApp, IID_IWebBrowser2, (void**) &browser);
		if (FAILED(hr)) {
			return hr;
		}

		if (FAILED(browser->get_LocationURL(&url))) {
			return hr;
		}

		CComPtr<IInternetSecurityManager> manager;
		hr = provider->QueryService(SID_SInternetSecurityManager, IID_IInternetSecurityManager, (void**) &manager);
		if (FAILED(hr)) {
			return hr;
		}

		hr = manager->MapUrlToZone(url, &zone, 0);
		if (FAILED(hr)) {
			return hr;
		}

		return S_OK;
	}

	HRESULT FUN_004085e0()
	{
		T* pT = static_cast<T*>(this);

		CComPtr<IUnknown> site;
		if (pT->GetSite(IID_IUnknown, (void**) &site) == S_OK && site) {
			CComBSTR url;
			DWORD zone;
			HRESULT hr = pT->FUN_00408260(url, zone);
			if (hr != S_OK) {
				return LogManager::ReportCOMError(T::GetObjectCLSID(), "Failed to get SiteUrl", hr);
			}

			hr = FUN_0041e6b0(url, zone);
			if (hr != S_OK) {
				CString message;
				message.Format("Untrusted Site: %s", CString(url));
				return LogManager::ReportCOMError(T::GetObjectCLSID(), message, hr);
			}
		}

		return S_OK;
	}

	DWORD m_unk0x04; // 0x04
};

#endif // IOBJECTSAFETYROBLOXIMPL_H
