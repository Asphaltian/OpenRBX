#ifndef ROBLOXWND_H
#define ROBLOXWND_H

#include "DxUserInput.h"
#include "ViewBase.h"
#include "decomp.h"
#include "rbxgraphics/Adorn.h"
#include "util/IMetric.h"
#include "util/Profiling.h"
#include "util/RunStateOwner.h"
#include "v8datamodel/Stats.h"

#include <GLG3D/GWindow.h>
#include <GLG3D/RenderDevice.h>
#include <atlbase.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>

class CWorkspace;
class RenderStats;
struct IWorkspace;

namespace Ogre {
class VisualEngine;
}

namespace RBX {
class DataModel;
}

// SIZE 0xb8
class CRobloxWnd : public CWnd, public RBX::IRunView, public RBX::IMetric
{
public:
	CRobloxWnd(bool p_unk0x80);
	virtual ~CRobloxWnd();

	void FUN_0045f9f0();
	void FUN_0045f670(UINT nType, int cx, int cy);
	void FUN_0045faf0();
	void FUN_0045fc50(bool active);
	void FUN_00460160(G3D::GWindow* window);
	void FUN_00460360();
	void FUN_00460430();
	void FUN_00460d30();
	void FUN_004610a0(int width, int height);
	void FUN_00461420(int width, int height);
	void FUN_004617b0(UINT nType, int cx, int cy);
	void FUN_004619e0();
	void FUN_004622d0(bool p_unk0x04);
	void FUN_00462450(bool p_unk0x04);
	void FUN_00462630(bool p_unk0x04);
	bool FUN_00462710();
	bool FUN_00462890();
	void FUN_00463c30(CWorkspace* workspace);

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

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
	// VTABLE: ROBLOX 0x0080daec
	// SIZE 0x140
	class RenderStatsItem : public RBX::Stats::Item
	{
	public:
		RenderStatsItem(RenderStats* stats);

		virtual void update(); // vtable+0x40

	private:
		RBX::Stats::Item* m_unk0x120; // 0x120
		RBX::Stats::Item* m_unk0x124; // 0x124
		RBX::Stats::Item* m_unk0x128; // 0x128
		RBX::Stats::Item* m_unk0x12c; // 0x12c
		RBX::Stats::Item* m_unk0x130; // 0x130
		RBX::Stats::Item* m_unk0x134; // 0x134
		RenderStats* m_unk0x138;      // 0x138
	};

	int m_unk0x5c;                                         // 0x5c
	int m_unk0x60;                                         // 0x60
	boost::shared_ptr<RenderStatsItem> m_unk0x64;          // 0x64
	CComPtr<IWorkspace> m_unk0x6c;                         // 0x6c
	boost::shared_ptr<RBX::DataModel> m_unk0x70;           // 0x70
	bool m_unk0x78;                                        // 0x78
	boost::scoped_ptr<DxUserInput> m_unk0x7c;              // 0x7c
	bool m_unk0x80;                                        // 0x80
	bool m_unk0x81;                                        // 0x81
	bool m_unk0x82;                                        // 0x82
	bool m_unk0x83;                                        // 0x83
	double m_unk0x88;                                      // 0x88
	std::auto_ptr<RBX::Profiling::CodeProfiler> m_unk0x90; // 0x90
	bool m_unk0x94;                                        // 0x94
	DWORD m_unk0x98;                                       // 0x98
	G3D::GWindow* m_unk0x9c;                               // 0x9c
	bool m_unk0xa0;                                        // 0xa0
	std::auto_ptr<G3D::RenderDevice> m_unk0xa4;            // 0xa4
	std::auto_ptr<RBX::ViewBase> m_unk0xa8;                // 0xa8
	std::auto_ptr<RBX::Adorn> m_unk0xac;                   // 0xac
	Ogre::VisualEngine* m_unk0xb0;                         // 0xb0
	undefined m_unk0xb4[0xb8 - 0xb4];                      // 0xb4
};

DECOMP_SIZE_ASSERT(CRobloxWnd, 0xb8)

#endif // ROBLOXWND_H
