#ifndef SCRIPTDOC_H
#define SCRIPTDOC_H

#include "MarshaledListener.h"
#include "NullDoc.h"
#include "ScintillaDocView.h"
#include "decomp.h"
#include "util/RunStateOwner.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace RBX {
class Script;
} // namespace RBX

// SIZE 0x5c
class CScriptDoc : public CNullDoc
{
	DECLARE_DYNCREATE(CScriptDoc)

	// SYNTHETIC: ROBLOX 0x00467db0
	// ?GetThisClass@CScriptDoc@@SGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x00468d30
	// ?CreateObject@CScriptDoc@@SGPAVCObject@@XZ

protected:
	CScriptDoc();

public:
	// SYNTHETIC: ROBLOX 0x00468be0
	// ??1CScriptDoc@@UAE@XZ

	// SYNTHETIC: ROBLOX 0x00468c70
	// CScriptDoc::`scalar deleting destructor'

	void FUN_0044eaf0(boost::shared_ptr<RBX::Script> script);

	friend class CScriptEditor;

private:
	boost::shared_ptr<RBX::Script> m_unk0x54; // 0x54
};

DECOMP_SIZE_ASSERT(CScriptDoc, 0x5c)

// SIZE 0x150
class CScriptEditor
	: public Scintilla::CScintillaView,
	  public RBX::Listener<RBX::Instance, RBX::AncestorChanged>,
	  public RBX::MarshaledListener<RBX::Instance, RBX::PropertyChanged, RBX::Instance, RBX::PropertyChanged>,
	  public RBX::MarshaledListener<RBX::RunService, RBX::RunState, RBX::RunService, RBX::RunState>
{
	DECLARE_DYNCREATE(CScriptEditor)

	// SYNTHETIC: ROBLOX 0x00467dd0
	// ?GetThisClass@CScriptEditor@@SGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x00469660
	// ?CreateObject@CScriptEditor@@SGPAVCObject@@XZ

protected:
	CScriptEditor();

public:
	virtual ~CScriptEditor();

	void FUN_00468aa0();
	void FUN_00468db0();
	void FUN_004696d0();
	void FUN_00469b30(boost::shared_ptr<RBX::Script> script);

protected:
	virtual void onEvent(const RBX::Instance* source, RBX::AncestorChanged event);
	virtual void VTable0x08(const RBX::RunService* source, RBX::RunState event);
	virtual void VTable0x08(const RBX::Instance* source, RBX::PropertyChanged event);
	virtual void VTable0x0c(const RBX::Instance* source, RBX::PropertyChanged event) throw();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnCharAdded(Scintilla::SCNotification* pSCNotification);
	virtual void OnDwellStart(Scintilla::SCNotification* pSCNotification);
	virtual void OnDwellEnd(Scintilla::SCNotification* pSCNotification);
	virtual void OnKillFocus();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnOptionsAddmarker();
	afx_msg void OnUpdateOptionsAddmarker(CCmdUI* pCmdUI);
	afx_msg void OnOptionsDeletemarker();
	afx_msg void OnUpdateOptionsDeletemarker(CCmdUI* pCmdUI);
	afx_msg void OnOptionsFindNextmarker();
	afx_msg void OnOptionsFindPrevmarker();
	afx_msg void OnOptionsFoldMargin();
	afx_msg void OnUpdateOptionsFoldMargin(CCmdUI* pCmdUI);
	afx_msg void OnOptionsSelectionMargin();
	afx_msg void OnUpdateOptionsSelectionMargin(CCmdUI* pCmdUI);
	afx_msg void OnOptionsViewLinenumbers();
	afx_msg void OnUpdateOptionsViewLinenumbers(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInsert(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStyle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFold(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLine(CCmdUI* pCmdUI);
	afx_msg LRESULT FUN_00469a20(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	// SYNTHETIC: ROBLOX 0x00469ad0
	// ?GetMessageMap@CScriptEditor@@MBEPBUAFX_MSGMAP@@XZ

	void SetAStyle(
		int style, COLORREF fore, COLORREF back = RGB(0xff, 0xff, 0xff), int size = -1, const char* face = NULL
	);
	void DefineMarker(int marker, int markerType, COLORREF fore, COLORREF back);

private:
	boost::shared_ptr<boost::recursive_mutex> m_unk0x128; // 0x128
	CWnd* m_unk0x130;                                     // 0x130
	boost::shared_ptr<RBX::Script> m_unk0x134;            // 0x134
	boost::shared_ptr<RBX::RunService> m_unk0x13c;        // 0x13c
	bool m_unk0x144;                                      // 0x144
	bool m_unk0x145;                                      // 0x145
	bool m_unk0x146;                                      // 0x146
	CFont m_unk0x148;                                     // 0x148
};

DECOMP_SIZE_ASSERT(CScriptEditor, 0x150)

#endif // SCRIPTDOC_H
