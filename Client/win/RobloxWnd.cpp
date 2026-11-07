// clang-format off
#include <OgreAnimation.h>
#include <OgreCamera.h>
#include <OgreRenderTarget.h>
#include <OgreSceneManager.h>
#include <OgreTimer.h>
// clang-format on

#include "RobloxWnd.h"

#include "LogManager.h"
#include "RenderSettings.h"
#include "View.h"
#include "Workspace.h"
#include "appdraw/AdornRbxGfx.h"
#include "appdraw/VisualEngine.h"
#include "network/Players.h"
#include "renderlib/Profiler.h"
#include "renderlib/RenderStats.h"
#include "renderlibnew/Chunk.h"
#include "util/RunStateOwner.h"
#include "util/scopedassign.h"
#include "util/standardout.h"
#include "v8datamodel/GlobalSettings.h"
#include "v8datamodel/Stats.h"
#include "v8datamodel/UserController.h"

#include <G3D/Log.h>
#include <GLG3D/GLCaps.h>
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

// FUNCTION: ROBLOX 0x0045f650
BOOL CRobloxWnd::OnEraseBkgnd(CDC* pDC)
{
	if (m_unk0xa0) {
		return CWnd::OnEraseBkgnd(pDC);
	}
	return TRUE;
}

// FUNCTION: ROBLOX 0x0045f9f0
void CRobloxWnd::FUN_0045f9f0()
{
	m_unk0xa8.reset(new RBX::ViewNew::View(m_unk0x70));
	m_unk0xac.reset(new RBX::AdornRbxGfx(m_unk0xb0));
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
	if (m_unk0x82) {
		RenderStats& stats = m_unk0xa8->getRenderStats();
		stats.swapBuffer.tick();
		m_unk0x9c->swapGLBuffers();
		m_unk0x82 = false;
		stats.swapBuffer.tock();
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

// STUB: ROBLOX 0x00460d30
void CRobloxWnd::FUN_00460d30()
{
	STUB(0x460d30);
}

// STUB: ROBLOX 0x00461050
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

// STUB: ROBLOX 0x00461950
void CRobloxWnd::OnSize(UINT nType, int cx, int cy)
{
	STUB(0x461950);
}

// STUB: ROBLOX 0x004619e0
void CRobloxWnd::FUN_004619e0()
{
	STUB(0x4619e0);
}

// STUB: ROBLOX 0x004622d0
void CRobloxWnd::FUN_004622d0(bool p_unk0x04)
{
	STUB(0x4622d0);
}

// STUB: ROBLOX 0x00462450
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
	RenderStats& stats = m_unk0xa8->getRenderStats();
	stats.cpuRenderTotal.tick();
	timer.reset();
	float milliseconds;
	{
		RBX::DataModel::Lock lock(m_unk0x70);
		m_unk0x70->renderPass2d(m_unk0xac.get(), this);
		m_unk0x70->renderPass3dAdorn(m_unk0xac.get());
		m_unk0xac->prepareRenderPass();
		m_unk0xa8->render(m_unk0xb0);
		m_unk0xac->finishRenderPass();
		milliseconds = timer.getMilliseconds();
	}
	stats.cpuRenderTotal.tock();

	if (Ogre::g_unk0x00949dd0) {
		m_unk0xb0->m_unk0x18.FUN_00671ef0(0).m_unk0x00 = milliseconds;
	}

	RenderStats& renderStats = m_unk0xa8->getRenderStats();
	renderStats.render2D.tick();
	if (m_unk0x70) {
		RBX::RunService* runService = m_unk0x70->create<RBX::RunService>();
		if (runService) {
			runService->runViewValid(this);
		}
	}
	renderStats.render2D.tock();
}

// STUB: ROBLOX 0x00462630
void CRobloxWnd::FUN_00462630(bool p_unk0x04)
{
	if (!m_unk0x81) {
		RBX::ScopedAssign<bool> rendering(m_unk0x81, true);
		if (!m_unk0x70) {
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else if (!m_unk0xa8.get()) {
			RBX::RunService* runService = m_unk0x70->create<RBX::RunService>();
			if (runService) {
				runService->runViewValid(this);
			}
		}
		else if (m_unk0x5c == 2) {
			FUN_004622d0(p_unk0x04);
		}
		else {
			FUN_00462450(p_unk0x04);
		}
	}
}

// STUB: ROBLOX 0x00462710
bool CRobloxWnd::FUN_00462710()
{
	if (g_unk0x00954484) {
		g_unk0x00954484 = false;
	}
	else {
		bool resized = false;
		{
			CPaintDC dc(this);
			const RECT& paint = dc.m_ps.rcPaint;
			if (paint.right != 0 || paint.left != 0 || paint.top != 0 || paint.bottom != 0) {
				Ogre::RenderTarget* target = m_unk0xb0->m_unk0x0c;
				if (paint.right - paint.left + 1 < (int) target->getWidth() ||
					paint.bottom - paint.top + 1 < (int) target->getHeight()) {
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
	FUN_00462630(true);
	return true;
}

// STUB: ROBLOX 0x00462890
bool CRobloxWnd::FUN_00462890()
{
	STUB(0x462890);
	return false;
}

// STUB: ROBLOX 0x00462a50
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
		RBX::RunService* runService = m_unk0x70->create<RBX::RunService>();
		if (runService) {
			runService->runViewValid(this);
		}
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

// STUB: ROBLOX 0x00463950
void CRobloxWnd::RenderStatsItem::update()
{
	STUB(0x463950);
}

// STUB: ROBLOX 0x00463c30
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
			if (m_unk0x70) {
				RBX::RunService* runService = m_unk0x70->create<RBX::RunService>();
				if (runService) {
					runService->removeRunView(this);
				}
			}
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
