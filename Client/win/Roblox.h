#ifndef ROBLOX_H
#define ROBLOX_H

#include "ErrorUploader.h"
#include "LogManager.h"
#include "decomp.h"

#include <G3D/Color3uint8.h>
#include <afxdhtml.h>
#include <boost/scoped_ptr.hpp>
#include <string>

// SIZE 0x110
class CRobloxApp : public CWinApp
{
public:
	CRobloxApp();
	~CRobloxApp();

	virtual BOOL InitInstance(); // vtable+0x58
	virtual int Run();           // vtable+0x5c
	virtual int ExitInstance();  // vtable+0x70

	// SYNTHETIC: ROBLOX 0x00451530
	// CRobloxApp::`scalar deleting destructor'

	BOOL FUN_0044dac0(bool p_unk0x04);
	void FUN_0044e250();

	afx_msg void OnAppAbout();
	afx_msg void FUN_0044dc90();
	afx_msg void FUN_0044e270();
	afx_msg void FUN_0044e290();
	afx_msg void FUN_004510b0(CCmdUI* p_unk0x04);
	afx_msg void FUN_00451100();
	afx_msg void FUN_00453eb0(CCmdUI* p_unk0x04);
	afx_msg void FUN_0044d450(WPARAM p_unk0x04, LPARAM p_unk0x08);

	DECLARE_MESSAGE_MAP()

private:
	MainLogManager m_unk0xa4;                    // 0xa4
	std::string m_unk0xf0;                       // 0xf0
	boost::scoped_ptr<ErrorUploader> m_unk0x10c; // 0x10c
};

DECOMP_SIZE_ASSERT(CRobloxApp, 0x110)

extern CRobloxApp theApp;

// SIZE 0x17c
class CRobloxControlColorSelector : public CXTPControl
{
	DECLARE_XTP_CONTROL(CRobloxControlColorSelector)

	// SYNTHETIC: ROBLOX 0x004529f0
	// ?GetThisClass@CRobloxControlColorSelector@@SGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x004539d0
	// ?CreateObject@CRobloxControlColorSelector@@SGPAVCObject@@XZ

	// SYNTHETIC: ROBLOX 0x00453a40
	// ?Clone@CRobloxControlColorSelector@@UAEPAVCXTPControl@@H@Z

public:
	struct CColorInfo
	{
		CColorInfo(COLORREF _clrButton, UINT _toolTipID)
		{
			clrButton = _clrButton;
			toolTipID = _toolTipID;
		}

		COLORREF clrButton; // 0x00
		UINT toolTipID;     // 0x04
		CString strTip;     // 0x08
	};

	CRobloxControlColorSelector();

	COLORREF GetColor() const;
	void SetColor(COLORREF clr);

protected:
	CSize GetSize(CDC* pDC);                                        // vtable+0x9c
	void Draw(CDC* pDC);                                            // vtable+0xa0
	void OnMouseMove(CPoint point);                                 // vtable+0xfc
	BOOL OnSetSelected(int bSelected);                              // vtable+0x104
	void OnClick(BOOL bKeyboard = FALSE, CPoint pt = CPoint(0, 0)); // vtable+0xe8
	void OnExecute();                                               // vtable+0x98
	void OnLButtonUp(CPoint point);                                 // vtable+0xec

private:
	CRect GetRect(int nIndex) const;
	int HitTest(CPoint point) const;

	int m_unk0x16c;      // 0x16c
	int m_unk0x170;      // 0x170
	bool m_unk0x174;     // 0x174
	COLORREF m_unk0x178; // 0x178
};

DECOMP_SIZE_ASSERT(CRobloxControlColorSelector, 0x17c)

// SIZE 0x130
class CRobloxDHtmlDialog : public CDHtmlDialog
{
	DECLARE_DYNAMIC(CRobloxDHtmlDialog)

	// SYNTHETIC: ROBLOX 0x00453a80
	// ?GetRuntimeClass@CRobloxDHtmlDialog@@UBEPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x00453a90
	// ?GetEventSinkMap@CRobloxDHtmlDialog@@MBEPBUAFX_EVENTSINKMAP@@XZ

public:
	CRobloxDHtmlDialog(UINT nIDTemplate, const char* url, CWnd* pParentWnd);
	CRobloxDHtmlDialog(UINT nIDTemplate, UINT nHtmlResID, CWnd* pParentWnd);

	// SYNTHETIC: ROBLOX 0x00453cc0
	// ??_GCRobloxDHtmlDialog@@UAEPAXI@Z

	virtual BOOL OnInitDialog(); // vtable+0x150

protected:
	void OnBeforeNavigate2(
		LPDISPATCH pDisp,
		VARIANT* url,
		VARIANT* flags,
		VARIANT* targetFrameName,
		VARIANT* postData,
		VARIANT* headers,
		BOOL* cancel
	);
	void OnWindowClosing(BOOL isChildWindow, BOOL* cancel);

	DECLARE_EVENTSINK_MAP()

private:
	std::string m_unk0x114; // 0x114
};

DECOMP_SIZE_ASSERT(CRobloxDHtmlDialog, 0x130)

#endif // ROBLOX_H
