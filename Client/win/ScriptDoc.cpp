#include "ScriptDoc.h"

#include "script/Script.h"
#include "v8datamodel/DataModel.h"

IMPLEMENT_DYNCREATE(CScriptDoc, CNullDoc)
IMPLEMENT_DYNCREATE(CScriptEditor, Scintilla::CScintillaView)

CScriptDoc::CScriptDoc()
{
}

// STUB: ROBLOX 0x00467de0
void CScriptEditor::VTable0x08(const RBX::RunService* source, RBX::RunState event)
{
	STUB(0x467de0);
}

// FUNCTION: ROBLOX 0x00469550
CScriptEditor::CScriptEditor() : m_unk0x130(0), m_unk0x144(false), m_unk0x145(false), m_unk0x146(false)
{
}

// STUB: ROBLOX 0x00469a70
void CScriptEditor::VTable0x0c(const RBX::Instance* source, RBX::PropertyChanged event) throw()
{
	STUB(0x469a70);
}

// STUB: ROBLOX 0x00469aa0
void CScriptEditor::VTable0x08(const RBX::Instance* source, RBX::PropertyChanged event)
{
	STUB(0x469aa0);
}

// STUB: ROBLOX 0x00469b30
void CScriptEditor::FUN_00469b30(boost::shared_ptr<RBX::Script> script)
{
	STUB(0x469b30);
}

// STUB: ROBLOX 0x00469e80
void CScriptEditor::onEvent(const RBX::Instance* source, RBX::AncestorChanged event)
{
	STUB(0x469e80);
}

// STUB: ROBLOX 0x00469f10
CScriptEditor::~CScriptEditor()
{
	STUB(0x469f10);
	FUN_00469b30(boost::shared_ptr<RBX::Script>());
}
