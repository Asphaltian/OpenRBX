#ifndef OUTPUTVIEW_H
#define OUTPUTVIEW_H

#include "SecureHtmlView.h"
#include "placeholder.h"
#include "util/StandardOut.h"

#include <atlbase.h>
#include <deque>
#include <mshtml.h>

class __declspec(novtable) COutputView : public CSecureHtmlView
{
	DECLARE_DYNAMIC(COutputView)

protected:
	COutputView() {}

public:
	virtual ~COutputView() {}

	virtual void OnInitialUpdate();
	virtual void OnDocumentComplete(LPCTSTR lpszURL);

protected:
	void FUN_0043b300(IHTMLElement* element);
	void FUN_0043b490(RBX::MessageType type, const char* text, __time64_t time);

private:
	std::deque<CComPtr<IHTMLDOMNode> > m_unk0xf4;
	CComQIPtr<IHTMLDocument2> m_unk0x108;
	CComQIPtr<IHTMLDOMNode> m_unk0x10c;
};

#endif // OUTPUTVIEW_H
