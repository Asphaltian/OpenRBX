#ifndef BROWSERVIEW_H
#define BROWSERVIEW_H

#include "NullDoc.h"
#include "SecureHtmlView.h"
#include "decomp.h"

// SIZE 0x2bc
class CBrowserView : public CXTPCommandBarsSiteBase<CSecureHtmlView>
{
	DECLARE_DYNCREATE(CBrowserView)

protected:
	CBrowserView();

public:
	virtual ~CBrowserView();

private:
	undefined m_unk0xf8[0x2bc - 0xf8]; // 0xf8
};

DECOMP_SIZE_ASSERT(CBrowserView, 0x2bc)

// SIZE 0x2bc
class CIDEBrowserView : public CBrowserView
{
	DECLARE_DYNCREATE(CIDEBrowserView)

	// SYNTHETIC: ROBLOX 0x0040a160
	// ?GetThisClass@CIDEBrowserView@@SGPAUCRuntimeClass@@XZ

protected:
	CIDEBrowserView() {}
};

DECOMP_SIZE_ASSERT(CIDEBrowserView, 0x2bc)

// SIZE 0x2bc
class CPlayBrowserView : public CBrowserView
{
	DECLARE_DYNCREATE(CPlayBrowserView)

protected:
	CPlayBrowserView() {}
};

DECOMP_SIZE_ASSERT(CPlayBrowserView, 0x2bc)

// SIZE 0x58
class CBrowserDoc : public CNullDoc
{
	DECLARE_DYNCREATE(CBrowserDoc)

	// SYNTHETIC: ROBLOX 0x0040a1a0
	// ?GetThisClass@CBrowserDoc@@SGPAUCRuntimeClass@@XZ

protected:
	CBrowserDoc() {}

private:
	undefined4 m_unk0x54; // 0x54
};

DECOMP_SIZE_ASSERT(CBrowserDoc, 0x58)

#endif // BROWSERVIEW_H
