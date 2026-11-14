#ifndef ROBLOX_H
#define ROBLOX_H

#include "ErrorUploader.h"
#include "LogManager.h"
#include "placeholder.h"

#include <G3D/Color3uint8.h>
#include <afxdhtml.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

extern CString g_unk0x00954290;

inline CString FUN_00407190()
{
	return g_unk0x00954290;
}

void FUN_0044ebb0(const std::exception& e);
void FUN_00450800(CString p_unk0x04);

class CRobloxCommandLineInfo : public CCommandLineInfo
{
public:
	CRobloxCommandLineInfo();

	virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);

	bool m_unk0x24;
	bool m_unk0x25;
	bool m_unk0x26;
	bool m_unk0x27;
	std::vector<std::string> m_unk0x28;
};

namespace RBX {
class Script;
}

class CRobloxApp : public CWinApp
{
public:
	CRobloxApp();
	~CRobloxApp();

	virtual BOOL InitInstance();
	virtual int Run();
	virtual int ExitInstance();

	BOOL FUN_0044dac0(bool p_unk0x04);
	BOOL FUN_0044e180(std::string script);
	void FUN_0044e250();
	BOOL FUN_0044ea70(std::string script);
	void FUN_0044ef90(boost::shared_ptr<RBX::Script> script);
	CDocument* FUN_0044fbc0();
	void FUN_004509e0();
	BOOL FUN_00451550(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

	afx_msg void OnAppAbout();
	afx_msg void FUN_0044dc90();
	afx_msg void FUN_0044e270();
	afx_msg void FUN_0044e290();
	afx_msg void FUN_004510b0(CCmdUI* p_unk0x04);
	afx_msg void FUN_00451100();
	afx_msg void FUN_00453eb0(CCmdUI* p_unk0x04);
	afx_msg void FUN_0044d450(WPARAM p_unk0x04, LPARAM p_unk0x08);

	DECLARE_MESSAGE_MAP()

	friend class CApp;

private:
	MainLogManager m_unk0xa4;
	std::string m_unk0xf0;
	boost::scoped_ptr<ErrorUploader> m_unk0x10c;
};

extern CRobloxApp theApp;

class CRobloxControlColorSelector : public CXTPControl
{
	DECLARE_XTP_CONTROL(CRobloxControlColorSelector)

public:
	struct CColorInfo
	{
		CColorInfo(COLORREF _clrButton, UINT _toolTipID)
		{
			clrButton = _clrButton;
			toolTipID = _toolTipID;
		}

		COLORREF clrButton;
		UINT toolTipID;
		CString strTip;
	};

	CRobloxControlColorSelector();

	COLORREF GetColor() const;
	void SetColor(COLORREF clr);

protected:
	CSize GetSize(CDC* pDC);
	void Draw(CDC* pDC);
	void OnMouseMove(CPoint point);
	BOOL OnSetSelected(int bSelected);
	void OnClick(BOOL bKeyboard = FALSE, CPoint pt = CPoint(0, 0));
	void OnExecute();
	void OnLButtonUp(CPoint point);

private:
	friend class CPropertyGridItemBrickColor;

	CRect GetRect(int nIndex) const;
	int HitTest(CPoint point) const;

	int m_unk0x16c;
	int m_unk0x170;
	bool m_unk0x174;
	COLORREF m_unk0x178;
};

class CRobloxDHtmlDialog : public CDHtmlDialog
{
	DECLARE_DYNAMIC(CRobloxDHtmlDialog)

public:
	CRobloxDHtmlDialog(UINT nIDTemplate, const char* url, CWnd* pParentWnd);
	CRobloxDHtmlDialog(UINT nIDTemplate, UINT nHtmlResID, CWnd* pParentWnd);

	virtual BOOL OnInitDialog();

protected:
	virtual BOOL IsExternalDispatchSafe() { return TRUE; }

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
	std::string m_unk0x114;
};

#endif // ROBLOX_H
