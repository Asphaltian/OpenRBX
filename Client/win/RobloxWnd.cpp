#include "RobloxWnd.h"

#include "RenderSettings.h"
#include "renderlib/Profiler.h"
#include "util/standardout.h"
#include "v8datamodel/GlobalSettings.h"

#include <G3D/Log.h>
#include <GLG3D/GLCaps.h>
#include <GLG3D/Win32Window.h>
#include <stdexcept>

static ATL::CCriticalSection g_unk0x0095448c;

// FUNCTION: ROBLOX 0x0045f4d0
LRESULT CRobloxWnd::FUN_0045f4d0(WPARAM wParam, LPARAM lParam)
{
	SetFocus();
	return 0;
}

// FUNCTION: ROBLOX 0x0045f650
BOOL CRobloxWnd::OnEraseBkgnd(CDC* pDC)
{
	if (m_unk0xa0) {
		return CWnd::OnEraseBkgnd(pDC);
	}
	return TRUE;
}

// FUNCTION: ROBLOX 0x0045fc50
void CRobloxWnd::FUN_0045fc50(bool active)
{
	if (!active && m_unk0x7c) {
		m_unk0x7c->FUN_0046ca90();
	}
}

// FUNCTION: ROBLOX 0x0045fc70
void CRobloxWnd::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
	if (m_unk0x7c) {
		m_unk0x7c->FUN_0046be50(true);
	}
}

// FUNCTION: ROBLOX 0x00460120
void CRobloxWnd::AdjustThrottle(double frameRate)
{
	ATL::CCritSecLock lock(g_unk0x0095448c);
	m_unk0x88 = frameRate;
}

// FUNCTION: ROBLOX 0x00460160
void CRobloxWnd::FUN_00460160(G3D::GWindow* window)
{
	m_unk0x9c = window;
	window->makeCurrent();

	if (G3D::GLCaps::renderer() == "GDI Generic") {
		throw std::runtime_error(
			"This computer does not have correctly installed graphics drivers and is using the default Microsoft "
			"OpenGL implementation. Most graphics capabilities are disabled. To correct this problem, download and "
			"install the latest drivers for the graphics card."
		);
	}

	m_unk0xa4.reset(new G3D::RenderDevice());
	if (!m_unk0xa4->init(m_unk0x9c, G3D::Log::common())) {
		m_unk0xa4.reset();
		throw std::runtime_error("renderDevice->init failed");
	}

	RBX::Render::SpecData::glLoaded = true;
	m_unk0xa4->setSwapBuffersAutomatically(false);
	if (m_unk0x80) {
		m_unk0xa4->enableAlphaWrite();
	}

	if (glGetError() != GL_NO_ERROR) {
		throw std::runtime_error("GL error creating RenderDevice");
	}
}

// STUB: ROBLOX 0x00460430
void CRobloxWnd::FUN_00460430()
{
	STUB(0x460430);
}

// FUNCTION: ROBLOX 0x004604c0
BOOL CRobloxWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass =
		AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC, LoadCursor(NULL, IDC_ARROW), NULL, NULL);
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	cs.cx = std::max<int>(cs.cx, 100);
	cs.cy = std::max<int>(cs.cy, 100);
	return CWnd::PreCreateWindow(cs);
}

// STUB: ROBLOX 0x00460530
void CRobloxWnd::OnKillFocus(CWnd* pNewWnd)
{
	FUN_00460430();
	CWnd::OnKillFocus(pNewWnd);
	if (m_unk0x7c) {
		m_unk0x7c->FUN_0046be50(false);
	}
}

// STUB: ROBLOX 0x00460560
void CRobloxWnd::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanging(lpwndpos);
	FUN_00460430();
}

// STUB: ROBLOX 0x00460580
void CRobloxWnd::OnClose()
{
	FUN_00460430();
	CWnd::OnClose();
}

// STUB: ROBLOX 0x00460bd0
CRobloxWnd::CRobloxWnd(bool p_unk0x80)
{
	STUB(0x460bd0);
}

// FUNCTION: ROBLOX 0x00460d10
void CRobloxWnd::InvalidateRunView()
{
	InvalidateRgn(NULL, FALSE);
}

// STUB: ROBLOX 0x00461050
void CRobloxWnd::OnTimer(UINT_PTR nIDEvent)
{
	STUB(0x461050);
}

// STUB: ROBLOX 0x004610a0
void CRobloxWnd::FUN_004610a0(int width, int height)
{
	STUB(0x4610a0);
}

// STUB: ROBLOX 0x00461420
void CRobloxWnd::FUN_00461420(int width, int height)
{
	G3D::GWindow::Settings settings;
	settings.width = width;
	settings.height = height;
	settings.stencilBits = 8;
	settings.rgbBits = 8;
	settings.depthBits = 24;
	settings.alphaBits = m_unk0x80 ? 8 : 0;
	settings.fsaaSamples = g_unk0x0091a2e8;

	CClientDC dc(this);
	CRect rect;
	GetClientRect(&rect);

	G3D::GWindow* window;
	try {
		window = G3D::Win32Window::create(settings, dc.m_hDC);
	}
	catch (std::exception&) {
		if (CRenderSettings::singleton().FUN_0044d430() == 1) {
			CRenderSettings::singleton().FUN_0044d0b0(CRenderSettings::c_graphicsModeUnk0x3);
			RBX::GlobalSettings::singleton()->saveState();
			AfxMessageBox(
				"There has been an error in graphics initialization. ROBLOX has made changes to your setting which "
				"will take effect after you restart. Please restart ROBLOX and try again.",
				MB_ICONEXCLAMATION
			);
			AfxPostQuitMessage(0);
			throw;
		}
		throw std::runtime_error("Graphics failed to initialize");
	}

	try {
		FUN_00460160(window);
	}
	catch (std::exception& e) {
		if (m_unk0x60 == 0 && CRenderSettings::singleton().FUN_0044d430() == 1) {
			m_unk0x60++;
			RBX::StandardOut::singleton()->print(
				RBX::MESSAGE_WARNING, "OpenGL-G3D failed: \"%s\". Trying RbxGfx.", e.what()
			);
			m_unk0x5c = 3;
			FUN_004610a0(width, height);
		}
		else {
			throw std::runtime_error("Graphics failed to initialize");
		}
	}
}

// STUB: ROBLOX 0x00461690
int CRobloxWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	m_unk0x98 = GetCurrentThreadId();

	CRect rect;
	GetClientRect(&rect);

	try {
		if (m_unk0x5c == 2) {
			FUN_00461420(rect.Width(), rect.Height());
		}
		else {
			FUN_004610a0(rect.Width(), rect.Height());
		}
	}
	catch (std::exception& e) {
		CString message;
		message.Format("Unable to run Roblox.\n\n%s", e.what());
		AfxMessageBox(message);
		return -1;
	}

	SetTimer(0, 200, NULL);
	SetTimer(1, 100, NULL);
	return 0;
}

// STUB: ROBLOX 0x00461950
void CRobloxWnd::OnSize(UINT nType, int cx, int cy)
{
	STUB(0x461950);
}

// STUB: ROBLOX 0x00462a50
void CRobloxWnd::OnPaint()
{
	STUB(0x462a50);
}

// STUB: ROBLOX 0x00462be0
std::string CRobloxWnd::getMetric(const std::string& metric) const
{
	STUB(0x462be0);
	return std::string();
}

// STUB: ROBLOX 0x00464000
CRobloxWnd::~CRobloxWnd()
{
	STUB(0x464000);
}

// STUB: ROBLOX 0x00464180
void CRobloxWnd::OnDestroy()
{
	STUB(0x464180);
}

// clang-format off
BEGIN_MESSAGE_MAP(CRobloxWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_MESSAGE(WM_USER + 0xbb, FUN_0045f4d0)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_CLOSE()
END_MESSAGE_MAP()
// clang-format on
