#ifndef IOBJECTSAFETYROBLOXIMPL_H
#define IOBJECTSAFETYROBLOXIMPL_H

#include "decomp.h"

#include <atlbase.h>
#include <atlcom.h>
#include <objsafe.h>

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
			*pdwSupportedOptions = dwSupportedSafety;
			*pdwEnabledOptions = FUN_004085e0() == S_OK ? m_unk0x04 : 0;
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
		if (hr == S_OK) {
			m_unk0x04 = (m_unk0x04 & ~dwOptionSetMask) | (dwOptionSetMask & dwEnabledOptions);
		}

		return hr;
	}

	HRESULT FUN_004085e0()
	{
		STUB(0x4085e0);
		return S_OK;
	}

	DWORD m_unk0x04; // 0x04
};

#endif // IOBJECTSAFETYROBLOXIMPL_H
