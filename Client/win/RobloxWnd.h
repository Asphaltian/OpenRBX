#ifndef ROBLOXWND_H
#define ROBLOXWND_H

#include "DxUserInput.h"
#include "decomp.h"
#include "util/IMetric.h"
#include "util/RunStateOwner.h"

#include <GLG3D/GWindow.h>
#include <GLG3D/RenderDevice.h>
#include <memory>

// SIZE 0xb8
class CRobloxWnd : public CWnd, public RBX::IRunView, public RBX::IMetric
{
public:
	CRobloxWnd(bool p_unk0x80);
	virtual ~CRobloxWnd();

	void FUN_0045fc50(bool active);
	void FUN_00460160(G3D::GWindow* window);
	void FUN_00460430();
	void FUN_004610a0(int width, int height);
	void FUN_00461420(int width, int height);

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	virtual void InvalidateRunView();
	virtual void AdjustThrottle(double frameRate);

	virtual std::string getMetric(const std::string& metric) const;

protected:
	afx_msg LRESULT FUN_0045f4d0(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

	// SYNTHETIC: ROBLOX 0x00464210
	// ?GetMessageMap@CRobloxWnd@@MBEPBUAFX_MSGMAP@@XZ

private:
	int m_unk0x5c;                              // 0x5c
	undefined m_unk0x60[0x7c - 0x60];           // 0x60
	DxUserInput* m_unk0x7c;                     // 0x7c
	bool m_unk0x80;                             // 0x80
	undefined m_unk0x81[0x88 - 0x81];           // 0x81
	double m_unk0x88;                           // 0x88
	undefined m_unk0x90[0x98 - 0x90];           // 0x90
	DWORD m_unk0x98;                            // 0x98
	G3D::GWindow* m_unk0x9c;                    // 0x9c
	bool m_unk0xa0;                             // 0xa0
	undefined m_unk0xa1[0xa4 - 0xa1];           // 0xa1
	std::auto_ptr<G3D::RenderDevice> m_unk0xa4; // 0xa4
	undefined m_unk0xa8[0xb8 - 0xa8];           // 0xa8
};

DECOMP_SIZE_ASSERT(CRobloxWnd, 0xb8)

#endif // ROBLOXWND_H
