#include "ScriptDoc.h"

#include "script/Script.h"
#include "util/Events.h"
#include "v8datamodel/DataModel.h"
#include "v8tree/Service.h"

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

// FUNCTION: ROBLOX 0x00469b30
void CScriptEditor::FUN_00469b30(boost::shared_ptr<RBX::Script> script)
{
	if (m_unk0x134) {
		boost::recursive_mutex::scoped_lock lock(*m_unk0x128);
		RBX::stopListening<RBX::Instance, RBX::AncestorChanged>(m_unk0x134, this);
		RBX::stopListening<RBX::Instance, RBX::PropertyChanged>(m_unk0x134, this);
		RBX::stopListening<RBX::RunService, RBX::RunState>(m_unk0x13c, this);
		m_unk0x134.reset();
		m_unk0x13c.reset();
		m_unk0x128.reset();
	}

	if (script) {
		m_unk0x128 = RBX::DataModel::getMutex(script.get());
		m_unk0x134 = script;

		boost::recursive_mutex::scoped_lock lock(*m_unk0x128);
		m_unk0x13c = RBX::shared_from(RBX::ServiceProvider::find<RBX::RunService>(script.get()));
		RBX::startListening<RBX::RunService, RBX::RunState>(m_unk0x13c, this);
		RBX::startListening<RBX::Instance, RBX::PropertyChanged>(m_unk0x134, this);
		RBX::startListening<RBX::Instance, RBX::AncestorChanged>(m_unk0x134, this);
	}

	if (m_hWnd) {
		::PostMessage(m_hWnd, 0x465, 0, 0);
	}
}

// STUB: ROBLOX 0x00469e80
void CScriptEditor::onEvent(const RBX::Instance* source, RBX::AncestorChanged event)
{
	STUB(0x469e80);
}

// FUNCTION: ROBLOX 0x00469f10
CScriptEditor::~CScriptEditor()
{
	FUN_00469b30(boost::shared_ptr<RBX::Script>());
}
