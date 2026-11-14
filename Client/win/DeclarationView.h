#ifndef DECLARATIONVIEW_H
#define DECLARATIONVIEW_H

#include "placeholder.h"

#include <afxhtml.h>

namespace RBX {
namespace Reflection {
class Descriptor;

namespace Metadata {
class Item;
}
} // namespace Reflection
} // namespace RBX

class CDeclarationView : public CHtmlView
{
	DECLARE_DYNCREATE(CDeclarationView)

protected:
	CDeclarationView();

public:
	virtual ~CDeclarationView();

	virtual void OnInitialUpdate();
	virtual void OnDocumentComplete(LPCTSTR lpszURL);

	void FUN_00438530(RBX::Reflection::Metadata::Item* metadata, const char* declaration, const char* owner);
	void FUN_00439c30(const RBX::Reflection::Descriptor* descriptor);

	DECLARE_MESSAGE_MAP()

private:
	CComQIPtr<IHTMLDocument2> m_unk0xf0;
	CComQIPtr<IHTMLElement> m_unk0xf4;
	CComQIPtr<IHTMLElement> m_unk0xf8;
	CComQIPtr<IHTMLElement> m_unk0xfc;
	CComQIPtr<IHTMLElement> m_unk0x100;
	CComQIPtr<IHTMLElement> m_unk0x104;
	CComQIPtr<IHTMLElement> m_unk0x108;
	CComQIPtr<IHTMLElement> m_unk0x10c;
};

#endif // DECLARATIONVIEW_H
