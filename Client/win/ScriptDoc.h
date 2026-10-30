#ifndef SCRIPTDOC_H
#define SCRIPTDOC_H

#include "NullDoc.h"
#include "ScintillaDocView.h"
#include "decomp.h"

// SIZE 0x5c
class CScriptDoc : public CNullDoc
{
	DECLARE_DYNCREATE(CScriptDoc)

	// SYNTHETIC: ROBLOX 0x00467db0
	// ?GetThisClass@CScriptDoc@@SGPAUCRuntimeClass@@XZ

protected:
	CScriptDoc();

public:
	virtual ~CScriptDoc();

private:
	undefined4 m_unk0x54; // 0x54
	undefined4 m_unk0x58; // 0x58
};

DECOMP_SIZE_ASSERT(CScriptDoc, 0x5c)

// SIZE 0x150
class CScriptEditor : public Scintilla::CScintillaView
{
	DECLARE_DYNCREATE(CScriptEditor)

	// SYNTHETIC: ROBLOX 0x00467dd0
	// ?GetThisClass@CScriptEditor@@SGPAUCRuntimeClass@@XZ

protected:
	CScriptEditor();

public:
	virtual ~CScriptEditor();

private:
	undefined m_unk0xec[0x150 - 0xec]; // 0xec
};

DECOMP_SIZE_ASSERT(CScriptEditor, 0x150)

#endif // SCRIPTDOC_H
