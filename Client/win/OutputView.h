#ifndef OUTPUTVIEW_H
#define OUTPUTVIEW_H

#include "SecureHtmlView.h"
#include "decomp.h"
#include "util/StandardOut.h"

#include <atlbase.h>
#include <deque>
#include <mshtml.h>

// SIZE 0x110
class __declspec(novtable) COutputView : public CSecureHtmlView
{
	DECLARE_DYNAMIC(COutputView)

protected:
	COutputView() {}

public:
	// FUNCTION: ROBLOX 0x0043ada0
	virtual ~COutputView() {}

	virtual void OnInitialUpdate();
	virtual void OnDocumentComplete(LPCTSTR lpszURL);

protected:
	void FUN_0043b300(IHTMLElement* element);
	void FUN_0043b490(RBX::MessageType type, const char* text, __time64_t time);

private:
	std::deque<CComPtr<IHTMLDOMNode> > m_unk0xf4; // 0xf4
	CComQIPtr<IHTMLDocument2> m_unk0x108;         // 0x108
	CComQIPtr<IHTMLDOMNode> m_unk0x10c;           // 0x10c
};

DECOMP_SIZE_ASSERT(COutputView, 0x110)

#endif // OUTPUTVIEW_H
