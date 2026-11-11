#ifndef DECLARATIONVIEW_H
#define DECLARATIONVIEW_H

#include "decomp.h"

#include <afxhtml.h>

namespace RBX {
namespace Reflection {
class Descriptor;

namespace Metadata {
class Item;
}
} // namespace Reflection
} // namespace RBX

// SIZE 0x110
class CDeclarationView : public CHtmlView
{
	DECLARE_DYNCREATE(CDeclarationView)

	// SYNTHETIC: ROBLOX 0x00437550
	// ?GetThisClass@CDeclarationView@@SGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x00438c60
	// ?CreateObject@CDeclarationView@@SGPAVCObject@@XZ

protected:
	CDeclarationView();

public:
	virtual ~CDeclarationView();

	// SYNTHETIC: ROBLOX 0x00438c40
	// CDeclarationView::`scalar deleting destructor'

	virtual void OnInitialUpdate();
	virtual void OnDocumentComplete(LPCTSTR lpszURL);

	void FUN_00438530(RBX::Reflection::Metadata::Item* metadata, const char* declaration, const char* owner);
	void FUN_00439c30(const RBX::Reflection::Descriptor* descriptor);

	DECLARE_MESSAGE_MAP()

	// SYNTHETIC: ROBLOX 0x00437b20
	// ?GetMessageMap@CDeclarationView@@MBEPBUAFX_MSGMAP@@XZ

private:
	CComQIPtr<IHTMLDocument2> m_unk0xf0; // 0xf0
	CComQIPtr<IHTMLElement> m_unk0xf4;   // 0xf4
	CComQIPtr<IHTMLElement> m_unk0xf8;   // 0xf8
	CComQIPtr<IHTMLElement> m_unk0xfc;   // 0xfc
	CComQIPtr<IHTMLElement> m_unk0x100;  // 0x100
	CComQIPtr<IHTMLElement> m_unk0x104;  // 0x104
	CComQIPtr<IHTMLElement> m_unk0x108;  // 0x108
	CComQIPtr<IHTMLElement> m_unk0x10c;  // 0x10c
};

DECOMP_SIZE_ASSERT(CDeclarationView, 0x110)

#endif // DECLARATIONVIEW_H
