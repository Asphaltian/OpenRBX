#include "ScriptDoc.h"

#include "script/Script.h"
#include "util/ContentProvider.h"
#include "util/Events.h"
#include "v8datamodel/DataModel.h"
#include "v8tree/Service.h"

#include <boost/iostreams/copy.hpp>
#include <memory>
#include <sstream>

// GLOBAL: ROBLOX 0x009544b0
static HMODULE g_unk0x009544b0;

// GLOBAL: ROBLOX 0x009544b4
static int g_unk0x009544b4;

IMPLEMENT_DYNCREATE(CScriptDoc, CNullDoc)
IMPLEMENT_DYNCREATE(CScriptEditor, Scintilla::CScintillaView)

CScriptDoc::CScriptDoc()
{
}

// FUNCTION: ROBLOX 0x00467de0
void CScriptEditor::VTable0x08(const RBX::RunService* source, RBX::RunState event)
{
	m_unk0x145 = event != RBX::RS_NORMAL;
	GetCtrl().SetReadOnly(m_unk0x145 || !m_unk0x146);
}

// FUNCTION: ROBLOX 0x00467e20
BOOL CScriptEditor::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScintillaView::PreCreateWindow(cs) != FALSE;
}

// FUNCTION: ROBLOX 0x00467e90
void CScriptEditor::PostNcDestroy()
{
	if (--g_unk0x009544b4 == 0) {
		FreeLibrary(g_unk0x009544b0);
	}
	CScintillaView::PostNcDestroy();
}

// FUNCTION: ROBLOX 0x00468a30
void CScriptEditor::OnDwellEnd(Scintilla::SCNotification* pSCNotification)
{
	Scintilla::CScintillaCtrl& ctrl = GetCtrl();
	if (ctrl.CallTipActive()) {
		ctrl.CallTipCancel();
	}
}

// FUNCTION: ROBLOX 0x00468aa0
void CScriptEditor::FUN_00468aa0()
{
	if (GetParentFrame()) {
		if (!m_unk0x134) {
			GetParentFrame()->SetWindowText("");
			return;
		}
		GetParentFrame()->SetWindowText(m_unk0x134->getName().c_str());
	}
}

// FUNCTION: ROBLOX 0x00469550
CScriptEditor::CScriptEditor() : m_unk0x130(NULL), m_unk0x144(false), m_unk0x145(false), m_unk0x146(false)
{
}

// FUNCTION: ROBLOX 0x004696d0
void CScriptEditor::FUN_004696d0()
{
	if (m_hWnd) {
		m_unk0x144 = true;

		if (m_unk0x134) {
			try {
				boost::recursive_mutex::scoped_lock lock(*m_unk0x128);
				m_unk0x146 = m_unk0x134->getScriptId().isNull();

				if (m_unk0x146) {
					GetCtrl().SetText(m_unk0x134->getEmbeddedCode().c_str());
				}
				else {
					std::auto_ptr<std::istream> stream(
						RBX::ContentProvider::singleton().getContent(m_unk0x134->getScriptId())
					);
					std::ostringstream text;
					boost::iostreams::copy(*stream, text, 0x1000);
					GetCtrl().SetText(text.str().c_str());
				}

				GetCtrl().SetReadOnly(m_unk0x145 || !m_unk0x146);
			}
			catch (std::exception& e) {
				CString message;
				message.Format("An error occurred while getting the script\n\n%s", e.what());
				GetCtrl().SetText(message);
				GetCtrl().SetReadOnly(FALSE);
			}
		}
		else {
			GetCtrl().SetText("");
		}

		GetCtrl().SetSavePoint();
		m_unk0x144 = false;
	}
}

// FUNCTION: ROBLOX 0x00469a70
void CScriptEditor::VTable0x0c(const RBX::Instance* source, RBX::PropertyChanged event) throw()
{
	if (&event.getDescriptor() == &RBX::Script::prop_EmbeddedSourceCode ||
		&event.getDescriptor() == &RBX::Instance::desc_Name) {
		RBX::MarshaledListener<RBX::Instance, RBX::PropertyChanged, RBX::Instance, RBX::PropertyChanged>::VTable0x0c(
			source, event
		);
	}
}

// FUNCTION: ROBLOX 0x00469aa0
void CScriptEditor::VTable0x08(const RBX::Instance* source, RBX::PropertyChanged event)
{
	if (&event.getDescriptor() == &RBX::Script::prop_EmbeddedSourceCode) {
		FUN_004696d0();
	}
	else if (&event.getDescriptor() == &RBX::Instance::desc_Name) {
		FUN_00468aa0();
	}
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

// FUNCTION: ROBLOX 0x00469e80
void CScriptEditor::onEvent(const RBX::Instance* source, RBX::AncestorChanged event)
{
	if (!event.newParent) {
		FUN_00469b30(boost::shared_ptr<RBX::Script>());
		::PostMessage(m_unk0x130->m_hWnd, WM_CLOSE, 0, 0);
	}
}

// FUNCTION: ROBLOX 0x00469ed0
void CScriptEditor::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if (lHint == 1) {
		FUN_00469b30(static_cast<CScriptDoc*>(m_pDocument)->m_unk0x54);
	}
}

// FUNCTION: ROBLOX 0x00469f10
CScriptEditor::~CScriptEditor()
{
	FUN_00469b30(boost::shared_ptr<RBX::Script>());
}
