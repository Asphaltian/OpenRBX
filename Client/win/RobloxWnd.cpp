// clang-format off
#include <OgreAnimation.h>
#include <OgreCamera.h>
#include <OgreRenderSystem.h>
#include <OgreRenderTarget.h>
#include <OgreSceneManager.h>
#include <OgreTimer.h>
// clang-format on

#include "RobloxWnd.h"

#include "LogManager.h"
#include "RenderSettings.h"
#include "View.h"
#include "Workspace.h"
#include "appdraw/AdornG3D.h"
#include "appdraw/AdornRbxGfx.h"
#include "appdraw/VisualEngine.h"
#include "network/Players.h"
#include "rbxview/View.h"
#include "renderlib/Mesh.h"
#include "renderlib/Profiler.h"
#include "renderlib/RenderStats.h"
#include "renderlibnew/Chunk.h"
#include "util/RunStateOwner.h"
#include "util/scopedassign.h"
#include "util/standardout.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/GlobalSettings.h"
#include "v8datamodel/Stats.h"
#include "v8datamodel/UserController.h"

#include <G3D/Log.h>
#include <G3D/stringutils.h>
#include <GLG3D/DXCaps.h>
#include <GLG3D/GLCaps.h>
#include <GLG3D/Texture.h>
#include <GLG3D/VARArea.h>
#include <GLG3D/Win32Window.h>
#include <stdexcept>

static ATL::CCriticalSection g_unk0x0095448c;

// GLOBAL: ROBLOX 0x00954484
static bool g_unk0x00954484;

DECOMP_SIZE_ASSERT(CRobloxWnd::RenderStatsItem, 0x140)

// FUNCTION: ROBLOX 0x0045f4d0
LRESULT CRobloxWnd::FUN_0045f4d0(WPARAM wParam, LPARAM lParam)
{
	SetFocus();
	return 0;
}

// SIZE 0x4
class Unknown0045f640
{
public:
	Unknown0045f640(G3D::Stopwatch& stopwatch) : m_unk0x00(&stopwatch) { stopwatch.tick(); }

	// FUNCTION: ROBLOX 0x0045f640
	~Unknown0045f640() { m_unk0x00->tock(); }

private:
	G3D::Stopwatch* m_unk0x00; // 0x00
};

DECOMP_SIZE_ASSERT(Unknown0045f640, 0x4)

static void runViewValid(RBX::DataModel* dataModel, RBX::IRunView* runView)
{
	if (dataModel) {
		RBX::RunService* runService = dataModel->create<RBX::RunService>();
		if (runService) {
			runService->runViewValid(runView);
		}
	}
}

static void removeRunView(RBX::DataModel* dataModel, RBX::IRunView* runView)
{
	if (dataModel) {
		RBX::RunService* runService = dataModel->create<RBX::RunService>();
		if (runService) {
			runService->removeRunView(runView);
		}
	}
}

// FUNCTION: ROBLOX 0x0045f650
BOOL CRobloxWnd::OnEraseBkgnd(CDC* pDC)
{
	if (m_unk0xa0) {
		return CWnd::OnEraseBkgnd(pDC);
	}
	return TRUE;
}

// FUNCTION: ROBLOX 0x0045f670
void CRobloxWnd::FUN_0045f670(UINT nType, int cx, int cy)
{
	if (m_unk0xb0) {
		CRect rect;
		GetClientRect(&rect);

		Ogre::RenderWindow* window = m_unk0xb0->m_unk0x0c;
		Ogre::Camera* camera = m_unk0xb0->m_unk0x08;
		window->windowMovedOrResized();

		if (rect.Height() != 0 && camera) {
			camera->setAspectRatio(Ogre::Real(window->getWidth()) / Ogre::Real(window->getHeight()));
		}

		camera->yaw(Ogre::Radian(0.0f));
	}
}

// FUNCTION: ROBLOX 0x0045f9f0
void CRobloxWnd::FUN_0045f9f0()
{
	m_unk0xa8.reset(new RBX::ViewNew::View(m_unk0x70));
	m_unk0xac.reset(new RBX::AdornRbxGfx(m_unk0xb0));
}

// FUNCTION: ROBLOX 0x0045faf0
void CRobloxWnd::FUN_0045faf0()
{
	m_unk0x9c->makeCurrent();
	m_unk0x70->buildGui(m_unk0xac.get());
}

// STUB: ROBLOX 0x0045fb30
BOOL CRobloxWnd::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	bool handled;
	switch (message) {
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
		handled = false;
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		SetFocus();
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
		handled = true;
		*pResult = 0;
		break;
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
		handled = false;
		*pResult = 0;
		break;
	default:
		return CWnd::OnWndMsg(message, wParam, lParam, pResult);
	}

	if (m_unk0x70) {
		m_unk0x9c->makeCurrent();
		if (m_unk0x7c) {
			m_unk0x7c->FUN_0046cbc0(message, wParam, lParam);
		}
		InvalidateRect(NULL);
	}

	if (handled) {
		return TRUE;
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
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

// FUNCTION: ROBLOX 0x00460360
void CRobloxWnd::FUN_00460360()
{
	if (m_unk0x9c) {
		m_unk0x9c->makeCurrent();
	}

	if (m_unk0xa4.get()) {
		m_unk0xa4->cleanup();
		m_unk0xa4.reset();
	}

	if (m_unk0x9c) {
		delete m_unk0x9c;
		m_unk0x9c = NULL;
	}
}

// FUNCTION: ROBLOX 0x004603e0
void CRobloxWnd::FUN_004603e0()
{
	if (m_unk0x5c == 2) {
		FUN_00460360();
	}
	else if (m_unk0xb0) {
		m_unk0xac.reset();
		delete m_unk0xb0;
	}
}

// FUNCTION: ROBLOX 0x00460430
void CRobloxWnd::FUN_00460430()
{
	if (m_unk0x82) {
		Unknown0045f640 timer(m_unk0xa8->getRenderStats().swapBuffer);
		m_unk0x9c->swapGLBuffers();
		m_unk0x82 = false;
	}
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

// FUNCTION: ROBLOX 0x00460530
void CRobloxWnd::OnKillFocus(CWnd* pNewWnd)
{
	FUN_00460430();
	CWnd::OnKillFocus(pNewWnd);
	if (m_unk0x7c) {
		m_unk0x7c->FUN_0046be50(false);
	}
}

// FUNCTION: ROBLOX 0x00460560
void CRobloxWnd::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanging(lpwndpos);
	FUN_00460430();
}

// FUNCTION: ROBLOX 0x00460580
void CRobloxWnd::OnClose()
{
	FUN_00460430();
	CWnd::OnClose();
}

// FUNCTION: ROBLOX 0x00460bd0
CRobloxWnd::CRobloxWnd(bool p_unk0x80)
	: m_unk0x78(true), m_unk0x80(p_unk0x80), m_unk0x81(false), m_unk0x82(false), m_unk0x83(true), m_unk0x88(666.0),
	  m_unk0x90(new RBX::Profiling::CodeProfiler("Render")), m_unk0x94(false), m_unk0x98(0), m_unk0x9c(NULL),
	  m_unk0xa0(false), m_unk0xb0(NULL)
{
	m_unk0x60 = 0;
	m_unk0x5c = CRenderSettings::singleton().FUN_0044d430();
	if (m_unk0x5c == 1) {
		m_unk0x5c = 3;
	}
}

// FUNCTION: ROBLOX 0x00460d10
void CRobloxWnd::InvalidateRunView()
{
	InvalidateRgn(NULL, FALSE);
}

// FUNCTION: ROBLOX 0x00460d30
void CRobloxWnd::FUN_00460d30()
{
	double frameRate;
	{
		ATL::CCritSecLock lock(g_unk0x0095448c);
		if (m_unk0x88 == 666.0) {
			return;
		}
		frameRate = m_unk0x88;
		m_unk0x88 = 666.0;
	}

	if (m_unk0xa8.get() == NULL || m_unk0x70 == NULL) {
		return;
	}

	RBX::DataModel::Lock lock(m_unk0x70);

	float shadingQuality = m_unk0xa8->getShadingQuality();
	float meshDetail = m_unk0xa8->getMeshDetail();
	if (frameRate < -0.01) {
		shadingQuality *= 0.8f;
		meshDetail *= 0.8f;
	}
	else if (frameRate > 0.002) {
		shadingQuality = shadingQuality * 1.05f + 1.0f;
		meshDetail = meshDetail * 1.05f + 1.0f;
	}

	shadingQuality = std::min(shadingQuality, CRenderSettings::singleton().FUN_004482a0());
	shadingQuality = std::max(shadingQuality, CRenderSettings::singleton().FUN_00448290());
	meshDetail = std::min(meshDetail, CRenderSettings::singleton().FUN_004482c0());
	meshDetail = std::max(meshDetail, CRenderSettings::singleton().FUN_004482b0());

	if (m_unk0x70->getWorkspace() && m_unk0x70->getWorkspace()->getCamera()) {
		const G3D::CoordinateFrame& cameraFrame = m_unk0x70->getWorkspace()->getCamera()->getCameraCoordinateFrame();
		float distance =
			(cameraFrame.translation - m_unk0x70->getWorkspace()->getCamera()->getCameraFocus().translation).length();
		if (m_unk0xb0 != NULL &&
			!Ogre::g_unk0x0096134c->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_VERTEX_PROGRAM)) {
			RBX::ViewBase::getShadowsEnabled() = false;
		}
		m_unk0xa8->updateSettings(shadingQuality, meshDetail, RBX::ViewBase::getShadowsEnabled(), distance);
	}
}

// FUNCTION: ROBLOX 0x00461050
void CRobloxWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
	case 0:
		FUN_00460430();
		FUN_00460d30();
		break;
	case 1:
		if (m_unk0x70 && m_unk0x7c) {
			m_unk0x7c->FUN_0046d0a0();
		}
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

// STUB: ROBLOX 0x004610a0
void CRobloxWnd::FUN_004610a0(int width, int height)
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

	int videoMemory = Ogre::FUN_00671cf0();
	m_unk0xb0 = new Ogre::VisualEngine();
	if (RBX::ViewBase::getShadowsEnabled()) {
		m_unk0xb0->m_unk0x7f8 = 0;
	}

	int antialiasing;
	switch (CRenderSettings::singleton().m_unk0x100) {
	case CRenderSettings::c_antialiasingModeUnk0x0:
		antialiasing = videoMemory < 100000000 ? 2 : 0;
		break;
	case CRenderSettings::c_antialiasingModeUnk0x1:
		antialiasing = 0;
		break;
	default:
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_INFO, "Unknown antialiasing mode");
	case CRenderSettings::c_antialiasingModeUnk0x2:
		antialiasing = 2;
		break;
	}

	m_unk0xb0->FUN_00673860(
		m_unk0x5c != 3,
		(unsigned long) m_hWnd,
		rect.Width(),
		rect.Height(),
		antialiasing,
		LogManager::getMainLogManger()->MakeLogFileName("gfx")
	);

	Ogre::g_unk0x00949dd0 = CRenderSettings::singleton().m_unk0x108 != CRenderSettings::c_frameRateManagerModeUnk0x2;

	bool bevels;
	switch (CRenderSettings::singleton().m_unk0xfc) {
	case CRenderSettings::c_bevelModeUnk0x0:
		bevels = videoMemory >= 100000000 && !m_unk0xb0->m_unk0x804;
		break;
	case CRenderSettings::c_bevelModeUnk0x1:
		bevels = true;
		break;
	default:
		bevels = false;
		break;
	}
	RBX::ViewNew::FUN_0050b2a0(bevels);

	Ogre::Animation::setDefaultInterpolationMode(Ogre::Animation::IM_LINEAR);
	Ogre::Animation::setDefaultRotationInterpolationMode(Ogre::Animation::RIM_LINEAR);
	m_unk0xb0->m_unk0x04->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	m_unk0xb0->m_unk0x08->setPosition(0.0f, 50.0f, 200.0f);
	m_unk0xb0->m_unk0x08->lookAt(0.0f, 50.0f, 0.0f);
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

// FUNCTION: ROBLOX 0x004617b0
void CRobloxWnd::FUN_004617b0(UINT nType, int cx, int cy)
{
	if (m_unk0x9c && m_unk0xa4.get()) {
		std::string renderer = G3D::GLCaps::renderer();
		if (G3D::beginsWith(renderer, "MOBILITY RADEON 7500")) {
			FUN_004603e0();
			try {
				if (m_unk0x5c == 2) {
					FUN_00461420(cx, cy);
				}
				else {
					FUN_004610a0(cx, cy);
				}
				m_unk0x9c->makeCurrent();
			}
			catch (std::exception&) {
			}
		}
		else {
			m_unk0xa4->notifyResize(cx, cy);
			G3D::Rect2D viewport = G3D::Rect2D::xywh(0, 0, m_unk0xa4->width(), m_unk0xa4->height());
			m_unk0xa4->setViewport(viewport);
		}
		m_unk0x83 = true;
	}
}

// FUNCTION: ROBLOX 0x00461950
void CRobloxWnd::OnSize(UINT nType, int cx, int cy)
{
	if (cx > 0 && cy > 0 && cx <= 0x2000 && cy <= 0x2000) {
		CWnd::OnSize(nType, cx, cy);

		if (m_unk0x5c == 2) {
			FUN_004617b0(nType, cx, cy);
		}
		else {
			FUN_0045f670(nType, cx, cy);
		}
	}
}

// FUNCTION: ROBLOX 0x004619e0
void CRobloxWnd::FUN_004619e0()
{
	m_unk0xa8.reset(new RBX::View::View(m_unk0x70));
	m_unk0xac.reset(new RBX::AdornG3D(m_unk0xa4.get()));
}

// FUNCTION: ROBLOX 0x004622d0
void CRobloxWnd::FUN_004622d0(bool p_unk0x04)
{
	m_unk0xa4->pushState();
	try {
		RBX::DataModel::Lock lock(m_unk0x70);
		m_unk0xa8->render(m_unk0xa4.get());
		Unknown0045f640 timer(m_unk0xa8->getRenderStats().render2D);
		m_unk0x70->renderPass3dAdorn(m_unk0xac.get());
		if (p_unk0x04) {
			m_unk0xa4->push2D();
			m_unk0x70->renderPass2d(m_unk0xac.get(), this);
			m_unk0xa4->pop2D();
		}
	}
	catch (std::exception& e) {
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_ERROR, e);
	}
	runViewValid(m_unk0x70.get(), this);
	m_unk0xa4->popState();
}

// FUNCTION: ROBLOX 0x00462450
void CRobloxWnd::FUN_00462450(bool p_unk0x04)
{
	if (RBX::Network::Players::findLocalCharacter(m_unk0x70.get()) == NULL) {
		Ogre::g_unk0x00949dd0 = false;
	}
	else {
		Ogre::g_unk0x00949dd0 =
			CRenderSettings::singleton().m_unk0x108 != CRenderSettings::c_frameRateManagerModeUnk0x2;
	}

	Ogre::Timer timer;
	float milliseconds;
	{
		Unknown0045f640 cpuTimer(m_unk0xa8->getRenderStats().cpuRenderTotal);
		timer.reset();
		RBX::DataModel::Lock lock(m_unk0x70);
		m_unk0x70->renderPass2d(m_unk0xac.get(), this);
		m_unk0x70->renderPass3dAdorn(m_unk0xac.get());
		m_unk0xac->prepareRenderPass();
		m_unk0xa8->render(m_unk0xb0);
		m_unk0xac->finishRenderPass();
		milliseconds = timer.getMilliseconds();
	}

	if (Ogre::g_unk0x00949dd0) {
		m_unk0xb0->m_unk0x18.FUN_00671ef0(0).m_unk0x00 = milliseconds;
	}

	Unknown0045f640 render2DTimer(m_unk0xa8->getRenderStats().render2D);
	runViewValid(m_unk0x70.get(), this);
}

// FUNCTION: ROBLOX 0x00462630
void CRobloxWnd::FUN_00462630(bool p_unk0x04)
{
	if (!m_unk0x81) {
		RBX::ScopedAssign<bool> rendering(m_unk0x81, true);
		if (!m_unk0x70) {
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else if (!m_unk0xa8.get()) {
			runViewValid(m_unk0x70.get(), this);
		}
		else if (m_unk0x5c == 2) {
			FUN_004622d0(p_unk0x04);
		}
		else {
			FUN_00462450(p_unk0x04);
		}
	}
}

// FUNCTION: ROBLOX 0x00462710
bool CRobloxWnd::FUN_00462710()
{
	if (!g_unk0x00954484) {
		bool resized = false;
		{
			CPaintDC dc(this);
			RECT paint = dc.m_ps.rcPaint;
			if (paint.right != 0 || paint.left != 0 || paint.top != 0 || paint.bottom != 0) {
				if (paint.right - paint.left + 1 < (int) m_unk0xb0->m_unk0x0c->getWidth() ||
					paint.bottom - paint.top + 1 < (int) m_unk0xb0->m_unk0x0c->getHeight()) {
					int width = m_unk0xb0->m_unk0x0c->getWidth();
					int height = m_unk0xb0->m_unk0x0c->getHeight();
					resized = true;
				}
			}
		}
		if (resized) {
			CWnd* frame = this;
			while (frame->GetParent() != NULL) {
				frame = frame->GetParent();
			}
			frame->InvalidateRgn(NULL, FALSE);
			g_unk0x00954484 = true;
			return false;
		}
	}
	else {
		g_unk0x00954484 = false;
	}
	FUN_00462630(true);
	return true;
}

// FUNCTION: ROBLOX 0x00462890
bool CRobloxWnd::FUN_00462890()
{
	bool painted = false;
	CPaintDC dc(this);
	if (m_unk0x9c && dc.m_hDC && m_unk0x70 && !IsRectEmpty(&dc.m_ps.rcPaint)) {
		m_unk0x9c->makeCurrent();
		if (!m_unk0xa4.get()) {
			CRect rect;
			GetClientRect(&rect);
			dc.DrawText("Roblox can't run with your graphics driver", -1, &rect, DT_END_ELLIPSIS);
		}
		else {
			RBX::Profiling::Mark mark(*m_unk0x90, true);
			FUN_00460430();
			m_unk0xa4->beginFrame();
			FUN_00462630(true);
			painted = true;
			m_unk0xa4->endFrame();
			m_unk0x82 = true;
			if (m_unk0x83 || !g_unk0x00953c68.getValue(&CRenderSettings::singleton()) ||
				!m_unk0x70->canDelaySwapBuffer()) {
				FUN_00460430();
				m_unk0x83 = false;
			}
			if (m_unk0x78) {
				m_unk0x78 = false;
				RBX::Render::Mesh::makeVAR();
				RBX::Render::Mesh::varDirty = true;
			}
		}
	}
	return painted;
}

// FUNCTION: ROBLOX 0x00462a50
void CRobloxWnd::OnPaint()
{
	bool painted;
	if (m_unk0x5c == 2) {
		painted = FUN_00462890();
	}
	else {
		painted = FUN_00462710();
	}
	if (!painted && m_unk0x70) {
		runViewValid(m_unk0x70.get(), this);
	}
}

// STUB: ROBLOX 0x00462be0
std::string CRobloxWnd::getMetric(const std::string& metric) const
{
	STUB(0x462be0);
	return std::string();
}

// STUB: ROBLOX 0x00463710
CRobloxWnd::RenderStatsItem::RenderStatsItem(RenderStats* stats)
{
	m_unk0x138 = stats;
	createBoundChildItem("2D CPU Render", stats->render2D);

	RBX::Stats::Item* total = createBoundChildItem("3D CPU Total", stats->cpuRenderTotal);
	total->createBoundChildItem("CPU Shadow", stats->cpuShadow);
	total->createBoundChildItem("computeProxyArrays", stats->computeProxyArrays);
	total->createBoundChildItem("Sort", stats->sort);
	total->createBoundChildItem("Proxy count", stats->diffuseProxyCount);

	RBX::Stats::Item* triangles = total->createBoundChildItem("Total Triangles", stats->totalTriangles);
	triangles->createBoundChildItem("Ambient + Unshadowed", stats->unshadowedTriangles);
	triangles->createBoundChildItem("Mark Shadows", stats->markShadowsTriangles);
	triangles->createBoundChildItem("Shadowed Light", stats->shadowedLightTriangles);

	createBoundChildItem("SwapBuffer", stats->swapBuffer);

	m_unk0x120 = createChildItem("Aggregation");
	m_unk0x120->createBoundChildItem("Total Chunks", RenderStats::chunkCount);
	m_unk0x120->createBoundChildItem("Aggregated Chunks", RenderStats::aggregatedChunkCount);
	m_unk0x120->createBoundChildItem("AggregateChunks", RenderStats::aggregateChunkCount);

	RBX::Stats::Item* memory = createChildItem("Memory");
	m_unk0x124 = memory->createChildItem("Tex");
	m_unk0x128 = memory->createChildItem("VAR");
	m_unk0x12c = memory->createChildItem("Video");

	RBX::Stats::Item* stateChanges = createChildItem("State Changes");
	m_unk0x130 = stateChanges->createChildItem("Major");
	m_unk0x134 = stateChanges->createChildItem("Minor");
}

// FUNCTION: ROBLOX 0x00463950
void CRobloxWnd::RenderStatsItem::update()
{
	int chunkCount = RenderStats::chunkCount;
	if (chunkCount == 0) {
		m_unk0x120->setValue(0.0, "n/a");
	}
	else if (RenderStats::aggregateChunkCount == 0) {
		m_unk0x120->formatValue(0.0, "%.0f%%, %.2g", 0.0, 1.0);
	}
	else {
		float total = chunkCount;
		float rendered = RenderStats::aggregateChunkCount - RenderStats::aggregatedChunkCount + chunkCount;
		float ratio = (total - rendered) / (total - 1.0);
		m_unk0x120->formatValue(ratio, "%.0f%%, %.2g", ratio * 100.0, total / rendered);
	}

	m_unk0x130->formatValue(
		m_unk0x138->majorGLStateChanges, "%d/%d", m_unk0x138->majorGLStateChanges, m_unk0x138->majorStateChanges
	);
	m_unk0x134->formatValue(
		m_unk0x138->minorGLStateChanges, "%d/%d", m_unk0x138->minorGLStateChanges, m_unk0x138->minorStateChanges
	);

	m_unk0x124->formatMem(G3D::Texture::sizeOfAllTexturesInMemory());
	m_unk0x128->formatMem(G3D::VARArea::sizeOfAllVARAreasInMemory());
	m_unk0x12c->formatMem(G3D::DXCaps::videoMemorySize());
}

// FUNCTION: ROBLOX 0x00463c30
void CRobloxWnd::FUN_00463c30(CWorkspace* workspace)
{
	if (workspace != m_unk0x6c) {
		if (m_unk0x70) {
			RBX::DataModel::Lock lock(m_unk0x70);
			if (m_unk0x64) {
				m_unk0x64->setParent(NULL);
				m_unk0x64.reset();
			}
			if (RBX::ControllerService* controllers = m_unk0x70 ? m_unk0x70->find<RBX::ControllerService>() : NULL) {
				controllers->setHardwareDevice(NULL);
			}
			m_unk0x9c->makeCurrent();
			m_unk0xa8.reset();
			removeRunView(m_unk0x70.get(), this);
			m_unk0x7c.reset();
		}

		m_unk0x6c = workspace;
		if (workspace) {
			m_unk0x70 = workspace->FUN_0046edc0();
		}
		else {
			m_unk0x70.reset();
		}

		if (m_unk0x70) {
			if (!m_unk0x94) {
				m_unk0x7c.reset(new DxUserInput(this, m_unk0x70.get()));
			}

			RBX::DataModel::Lock lock(m_unk0x70);
			CRenderSettings::singleton().FUN_0044d1b0(false);
			if (m_unk0x5c == 2) {
				FUN_004619e0();
			}
			else {
				FUN_0045f9f0();
			}
			m_unk0x7c->init(m_unk0xac.get());
			AdjustThrottle(0.0);
			FUN_00460d30();

			(m_unk0x70 ? m_unk0x70->create<RBX::ControllerService>() : NULL)->setHardwareDevice(m_unk0x7c.get());
			RBX::Stats::StatsService* stats = m_unk0x70 ? m_unk0x70->create<RBX::Stats::StatsService>() : NULL;

			m_unk0x64 = RBX::Creatable<RBX::Instance>::create<RenderStatsItem>(&m_unk0xa8->getRenderStats());
			m_unk0x64->setName("Render");
			m_unk0x64->setParent(stats);
			m_unk0x64->createBoundChildItem(*m_unk0x90);

			if (m_unk0x70) {
				RBX::RunService* runService = m_unk0x70->create<RBX::RunService>();
				if (runService) {
					runService->addRunView(this);
				}
			}
		}
	}
	if (m_hWnd != NULL) {
		Invalidate();
	}
}

// STUB: ROBLOX 0x00464000
CRobloxWnd::~CRobloxWnd()
{
	FUN_00463c30(NULL);
}

// FUNCTION: ROBLOX 0x00464180
void CRobloxWnd::OnDestroy()
{
	FUN_00460430();
	FUN_00463c30(NULL);

	if (m_unk0x5c == 2) {
		FUN_00460360();
		CWnd::OnDestroy();
		return;
	}

	if (m_unk0xb0) {
		m_unk0xac.reset();
		delete m_unk0xb0;
	}

	CWnd::OnDestroy();
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
