#include "ScriptDoc.h"

#include "SciLexer.h"
#include "script/Script.h"
#include "util/ContentProvider.h"
#include "util/Events.h"
#include "v8datamodel/DataModel.h"
#include "v8tree/Service.h"

#include <algorithm>
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

// FUNCTION: ROBLOX 0x00467e40
int CScriptEditor::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (g_unk0x009544b4 == 0) {
		g_unk0x009544b0 = LoadLibrary("SciLexer.dll");
		if (!g_unk0x009544b0) {
			return -1;
		}
	}
	g_unk0x009544b4++;

	return CScintillaView::OnCreate(lpCreateStruct) == -1 ? -1 : 0;
}

// FUNCTION: ROBLOX 0x00467e90
void CScriptEditor::PostNcDestroy()
{
	if (--g_unk0x009544b4 == 0) {
		FreeLibrary(g_unk0x009544b0);
	}
	CScintillaView::PostNcDestroy();
}

void CScriptEditor::SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char* face)
{
	Scintilla::CScintillaCtrl& rCtrl = GetCtrl();

	rCtrl.StyleSetFore(style, fore);
	rCtrl.StyleSetBack(style, back);
	if (size >= 1) {
		rCtrl.StyleSetSize(style, size);
	}
	if (face) {
		rCtrl.StyleSetFont(style, face);
	}
}

void CScriptEditor::DefineMarker(int marker, int markerType, COLORREF fore, COLORREF back)
{
	Scintilla::CScintillaCtrl& rCtrl = GetCtrl();

	rCtrl.MarkerDefine(marker, markerType);
	rCtrl.MarkerSetFore(marker, fore);
	rCtrl.MarkerSetBack(marker, back);
}

// FUNCTION: ROBLOX 0x00467eb0
void CScriptEditor::OnInitialUpdate()
{
	CScintillaView::OnInitialUpdate();

	m_unk0x130 = EnsureParentFrame();

	Scintilla::CScintillaCtrl& rCtrl = GetCtrl();

	rCtrl.SetLexer(SCLEX_LUA);
	rCtrl.SetKeyWords(
		0, "and break do else elseif end false for function if in local nil not or repeat return then true until while"
	);

	SetAStyle(STYLE_DEFAULT, RGB(0, 0, 0), RGB(0xff, 0xff, 0xff), 9, "Verdana");
	rCtrl.StyleClearAll();
	SetAStyle(SCE_LUA_DEFAULT, RGB(0, 0, 0));
	SetAStyle(SCE_LUA_COMMENT, RGB(0, 0x80, 0));
	SetAStyle(SCE_LUA_COMMENTLINE, RGB(0, 0x80, 0));
	SetAStyle(SCE_LUA_COMMENTDOC, RGB(0, 0x80, 0));
	SetAStyle(SCE_LUA_NUMBER, RGB(0, 0x80, 0x80));
	SetAStyle(SCE_LUA_WORD, RGB(0, 0, 0x80));
	SetAStyle(SCE_LUA_WORD2, RGB(0, 0, 0x80));
	SetAStyle(SCE_LUA_WORD3, RGB(0, 0, 0x80));
	SetAStyle(SCE_LUA_WORD4, RGB(0, 0, 0x80));
	SetAStyle(SCE_LUA_WORD5, RGB(0, 0, 0x80));
	SetAStyle(SCE_LUA_WORD6, RGB(0, 0, 0x80));
	SetAStyle(SCE_LUA_WORD7, RGB(0, 0, 0x80));
	SetAStyle(SCE_LUA_WORD8, RGB(0, 0, 0x80));
	rCtrl.StyleSetBold(SCE_LUA_WORD, TRUE);
	SetAStyle(SCE_LUA_STRING, RGB(0x80, 0, 0x80));
	SetAStyle(SCE_LUA_IDENTIFIER, RGB(0, 0, 0));
	SetAStyle(SCE_LUA_PREPROCESSOR, RGB(0x80, 0, 0));
	SetAStyle(SCE_LUA_OPERATOR, RGB(0x80, 0x80, 0));

	rCtrl.SetMarginWidthN(2, 16);
	rCtrl.SetMarginSensitiveN(2, TRUE);
	rCtrl.SetMarginTypeN(2, SC_MARGIN_SYMBOL);
	rCtrl.SetMarginMaskN(2, SC_MASK_FOLDERS);
	rCtrl.SetProperty("fold", "1");

	DefineMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0xff));
	DefineMarker(SC_MARKNUM_FOLDER, SC_MARK_PLUS, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));

	rCtrl.AutoCSetSeparator(10);
	rCtrl.SetMouseDwellTime(1000);
	rCtrl.SetMarginWidthN(0, 32);
}

// FUNCTION: ROBLOX 0x004683e0
void CScriptEditor::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CScintillaView::OnActivate(nState, pWndOther, bMinimized);

	Scintilla::CScintillaCtrl& rCtrl = GetCtrl();
	if (nState == WA_INACTIVE && rCtrl.CallTipActive()) {
		rCtrl.CallTipCancel();
	}
}

// FUNCTION: ROBLOX 0x00468420
void CScriptEditor::OnOptionsAddmarker()
{
	Scintilla::CScintillaCtrl& rCtrl = GetCtrl();
	int nPos = rCtrl.GetCurrentPos();
	int nLine = rCtrl.LineFromPosition(nPos);
	rCtrl.MarkerAdd(nLine, 0);
}

// FUNCTION: ROBLOX 0x00468450
void CScriptEditor::OnUpdateOptionsAddmarker(CCmdUI* pCmdUI)
{
	Scintilla::CScintillaCtrl& rCtrl = GetCtrl();
	int nPos = rCtrl.GetCurrentPos();
	int nLine = rCtrl.LineFromPosition(nPos);
	int nBits = rCtrl.MarkerGet(nLine);
	pCmdUI->Enable((nBits & 0x1) == 0);
}

// FUNCTION: ROBLOX 0x00468490
void CScriptEditor::OnOptionsDeletemarker()
{
	Scintilla::CScintillaCtrl& rCtrl = GetCtrl();
	int nPos = rCtrl.GetCurrentPos();
	int nLine = rCtrl.LineFromPosition(nPos);
	rCtrl.MarkerDelete(nLine, 0);
}

// FUNCTION: ROBLOX 0x004684c0
void CScriptEditor::OnUpdateOptionsDeletemarker(CCmdUI* pCmdUI)
{
	Scintilla::CScintillaCtrl& rCtrl = GetCtrl();
	int nPos = rCtrl.GetCurrentPos();
	int nLine = rCtrl.LineFromPosition(nPos);
	int nBits = rCtrl.MarkerGet(nLine);
	pCmdUI->Enable(nBits & 0x1);
}

// FUNCTION: ROBLOX 0x00468500
void CScriptEditor::OnOptionsFindNextmarker()
{
	Scintilla::CScintillaCtrl& rCtrl = GetCtrl();
	int nPos = rCtrl.GetCurrentPos();
	int nLine = rCtrl.LineFromPosition(nPos);
	int nFoundLine = rCtrl.MarkerNext(nLine + 1, 0x1);
	if (nFoundLine >= 0) {
		rCtrl.GotoLine(nFoundLine);
	}
	else {
		MessageBeep(MB_ICONHAND);
	}
}

// FUNCTION: ROBLOX 0x00468550
void CScriptEditor::OnOptionsFindPrevmarker()
{
	Scintilla::CScintillaCtrl& rCtrl = GetCtrl();
	int nPos = rCtrl.GetCurrentPos();
	int nLine = rCtrl.LineFromPosition(nPos);
	int nFoundLine = rCtrl.MarkerPrevious(nLine - 1, 0x1);
	if (nFoundLine >= 0) {
		rCtrl.GotoLine(nFoundLine);
	}
	else {
		MessageBeep(MB_ICONHAND);
	}
}

// FUNCTION: ROBLOX 0x004685a0
void CScriptEditor::OnOptionsFoldMargin()
{
	Scintilla::CScintillaCtrl& rCtrl = GetCtrl();
	int nMarginWidth = rCtrl.GetMarginWidthN(2);
	if (nMarginWidth) {
		rCtrl.SetMarginWidthN(2, 0);
	}
	else {
		rCtrl.SetMarginWidthN(2, 16);
	}
}

// FUNCTION: ROBLOX 0x004685e0
void CScriptEditor::OnUpdateOptionsFoldMargin(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(2) != 0);
}

// FUNCTION: ROBLOX 0x00468610
void CScriptEditor::OnOptionsSelectionMargin()
{
	Scintilla::CScintillaCtrl& rCtrl = GetCtrl();
	int nMarginWidth = rCtrl.GetMarginWidthN(1);
	if (nMarginWidth) {
		rCtrl.SetMarginWidthN(1, 0);
	}
	else {
		rCtrl.SetMarginWidthN(1, 16);
	}
}

// FUNCTION: ROBLOX 0x00468650
void CScriptEditor::OnUpdateOptionsSelectionMargin(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(1) != 0);
}

// FUNCTION: ROBLOX 0x00468680
void CScriptEditor::OnOptionsViewLinenumbers()
{
	Scintilla::CScintillaCtrl& rCtrl = GetCtrl();
	int nMarginWidth = rCtrl.GetMarginWidthN(0);
	if (nMarginWidth) {
		rCtrl.SetMarginWidthN(0, 0);
	}
	else {
		rCtrl.SetMarginWidthN(0, 32);
	}
}

// FUNCTION: ROBLOX 0x004686c0
void CScriptEditor::OnUpdateOptionsViewLinenumbers(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(0) != 0);
}

// FUNCTION: ROBLOX 0x004686f0
void CScriptEditor::OnUpdateInsert(CCmdUI* pCmdUI)
{
	CString sText;
	if (GetCtrl().GetOvertype()) {
		sText.LoadString(ID_INDICATOR_OVR);
	}
	pCmdUI->SetText(sText);
}

// FUNCTION: ROBLOX 0x00468780
void CScriptEditor::OnUpdateStyle(CCmdUI* pCmdUI)
{
	Scintilla::CScintillaCtrl& rCtrl = GetCtrl();
	int nPos = rCtrl.GetCurrentPos();
	int nStyle = rCtrl.GetStyleAt(nPos);
	CString sLine;
	sLine.Format(200, nStyle);
	pCmdUI->SetText(sLine);
}

// FUNCTION: ROBLOX 0x00468820
void CScriptEditor::OnUpdateFold(CCmdUI* pCmdUI)
{
	Scintilla::CScintillaCtrl& rCtrl = GetCtrl();
	int nPos = rCtrl.GetCurrentPos();
	int nLine = rCtrl.LineFromPosition(nPos);
	int nLevel = rCtrl.GetFoldLevel(nLine) & SC_FOLDLEVELNUMBERMASK;
	nLevel -= 1024;

	CString sLine;
	sLine.Format(201, nLevel);
	pCmdUI->SetText(sLine);
}

// FUNCTION: ROBLOX 0x004688e0
void CScriptEditor::OnUpdateLine(CCmdUI* pCmdUI)
{
	Scintilla::CScintillaCtrl& rCtrl = GetCtrl();
	int nPos = rCtrl.GetCurrentPos();
	int nLine = rCtrl.LineFromPosition(nPos);
	int nColumn = rCtrl.GetColumn(nPos);

	CString sLine;
	sLine.Format(199, nLine, nColumn, nPos);
	pCmdUI->SetText(sLine);
}

// FUNCTION: ROBLOX 0x00468990
void CScriptEditor::OnCharAdded(Scintilla::SCNotification* pSCNotification)
{
	Scintilla::CScintillaCtrl& rCtrl = GetCtrl();

	int nStartSel = rCtrl.GetSelectionStart();
	int nEndSel = rCtrl.GetSelectionEnd();
	if ((nStartSel == nEndSel) && (nStartSel > 12)) {
		Scintilla::TextRange tr;
		tr.chrg.cpMin = nStartSel - 13;
		tr.chrg.cpMax = nEndSel - 1;
		char sText[14];
		tr.lpstrText = sText;
		rCtrl.GetTextRange(&tr);

		if (_stricmp(sText, "scintilla is") == 0) {
			rCtrl.AutoCShow(0, "very cool\neasy to use\nway cool!!");
		}
	}
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

// FUNCTION: ROBLOX 0x00468af0
void CScriptEditor::OnDwellStart(Scintilla::SCNotification* pSCNotification)
{
	Scintilla::CScintillaCtrl& rCtrl = GetCtrl();

	CWnd* pFocusWnd = GetFocus();
	if (pFocusWnd && (pFocusWnd->GetSafeHwnd() == rCtrl.GetSafeHwnd())) {
		Scintilla::TextRange tr;
		tr.chrg.cpMin = std::max(0, pSCNotification->position - 7);
		tr.chrg.cpMax = std::min(pSCNotification->position + 7, rCtrl.GetLength());
		char sText[15];
		tr.lpstrText = sText;
		rCtrl.GetTextRange(&tr);

		_strupr(sText);
		if (strstr(sText, "AUTHOR ")) {
			rCtrl.CallTipShow(pSCNotification->position, "PJ Naughter");
		}
	}
}

// FUNCTION: ROBLOX 0x00468db0
void CScriptEditor::FUN_00468db0()
{
	if (GetCtrl().GetModify() && !m_unk0x144) {
		if (m_unk0x134) {
			boost::recursive_mutex::scoped_lock lock(*m_unk0x128);
			int length = GetCtrl().GetTextLength();
			CString text;
			GetCtrl().GetText(length + 1, text.GetBuffer(length + 1));
			text.ReleaseBuffer();
			m_unk0x134->setEmbeddedCode(static_cast<LPCTSTR>(text));
		}

		GetCtrl().SetSavePoint();
	}
}

// FUNCTION: ROBLOX 0x00468ef0
void CScriptEditor::OnKillFocus()
{
	FUN_00468db0();
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

// FUNCTION: ROBLOX 0x00469a20
LRESULT CScriptEditor::FUN_00469a20(WPARAM wParam, LPARAM lParam)
{
	FUN_00468aa0();
	FUN_004696d0();
	return 0;
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

// clang-format off
BEGIN_MESSAGE_MAP(CScriptEditor, Scintilla::CScintillaView)
	ON_WM_CREATE()
	ON_COMMAND(0x80e5, OnOptionsAddmarker)
	ON_COMMAND(0x80e6, OnOptionsDeletemarker)
	ON_UPDATE_COMMAND_UI(0x80e6, OnUpdateOptionsDeletemarker)
	ON_COMMAND(0x80e7, OnOptionsFindNextmarker)
	ON_COMMAND(0x80e8, OnOptionsFindPrevmarker)
	ON_COMMAND(0x80e4, OnOptionsFoldMargin)
	ON_COMMAND(0x80e3, OnOptionsSelectionMargin)
	ON_UPDATE_COMMAND_UI(0x80e3, OnUpdateOptionsSelectionMargin)
	ON_COMMAND(0x80e2, OnOptionsViewLinenumbers)
	ON_UPDATE_COMMAND_UI(0x80e2, OnUpdateOptionsViewLinenumbers)
	ON_UPDATE_COMMAND_UI(0x80e5, OnUpdateOptionsAddmarker)
	ON_UPDATE_COMMAND_UI(0x80e4, OnUpdateOptionsFoldMargin)
	ON_UPDATE_COMMAND_UI(203, OnUpdateLine)
	ON_UPDATE_COMMAND_UI(204, OnUpdateStyle)
	ON_UPDATE_COMMAND_UI(205, OnUpdateFold)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_OVR, OnUpdateInsert)
	ON_WM_ACTIVATE()
	ON_MESSAGE(0x465, FUN_00469a20)
END_MESSAGE_MAP()
// clang-format on
