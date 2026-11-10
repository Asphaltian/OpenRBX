#include "DxUserInput.h"

#include "ActiveDocView.h"
#include "MainFrm.h"
#include "rbxgraphics/TextureProxyBase.h"
#include "util/Rect.h"
#include "util/StandardOut.h"
#include "v8datamodel/DataModel.h"

#include <windowsx.h>

SDLKey FUN_0046d6f0(DWORD scanCode);
int FUN_0046db70(SDLKey key);
int FUN_0046dff0(SDLKey key);
RBX::UIEvent::EventType FUN_0046e290(UINT message);
SDLMod FUN_0046e2e0(const BYTE* keys);
void FUN_0046d670(const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position);
void FUN_0046d6a0(const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position);
G3D::Vector2 FUN_0046d6c0(const DIDEVICEOBJECTDATA& data);
void FUN_0046e530(
	const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position, const G3D::Vector2& size
);
void FUN_0046e560(
	const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position, const G3D::Vector2& size
);
void FUN_0046e5f0(
	const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position, const G3D::Vector2& size
);

// FUNCTION: ROBLOX 0x0046b7f0
bool DxUserInput::FUN_0046b7f0()
{
	G3D::Vector2 extent = m_unk0x58.center();
	if (m_unk0x70.x < -extent.x) {
		return false;
	}

	if (m_unk0x70.x > extent.x) {
		return false;
	}

	if (m_unk0x70.y < -extent.y) {
		return false;
	}

	if (m_unk0x70.y > extent.y) {
		return false;
	}

	return true;
}

// STUB: ROBLOX 0x0046b880
bool DxUserInput::FUN_0046b880()
{
	CRect rect;
	m_unk0x180->GetClientRect(&rect);

	CPoint clientOrigin = rect.TopLeft();
	CPoint screenOrigin = clientOrigin;
	m_unk0x180->ClientToScreen(&screenOrigin);

	return screenOrigin == clientOrigin;
}

// FUNCTION: ROBLOX 0x0046b8f0
G3D::Vector2int16 DxUserInput::FUN_0046b8f0()
{
	return G3D::Vector2int16((G3D::int16) m_unk0x58.width(), (G3D::int16) m_unk0x58.height());
}

// FUNCTION: ROBLOX 0x0046b920
bool DxUserInput::FUN_0046b920()
{
	return keyDown(SDLK_w) || keyDown(SDLK_s) || keyDown(SDLK_a) || keyDown(SDLK_d) || keyDown(SDLK_UP) ||
		   keyDown(SDLK_DOWN) || keyDown(SDLK_LEFT) || keyDown(SDLK_RIGHT);
}

// FUNCTION: ROBLOX 0x0046b9b0
void DxUserInput::setKeyState(SDLKey key, bool down)
{
	m_unk0x17c = down ? key : SDLK_UNKNOWN;
}

// FUNCTION: ROBLOX 0x0046b9d0
void DxUserInput::FUN_0046b9d0()
{
	HRESULT hr = m_unk0x188->CreateDevice(GUID_SysMouse, &m_unk0x18c, NULL);
	if (FAILED(hr)) {
		AtlThrow(hr);
	}

	hr = m_unk0x18c->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hr)) {
		AtlThrow(hr);
	}

	hr = m_unk0x18c->SetCooperativeLevel(m_unk0x184, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)) {
		AtlThrow(hr);
	}

	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = 0x800;

	hr = m_unk0x18c->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	if (FAILED(hr)) {
		AtlThrow(hr);
	}
}

// FUNCTION: ROBLOX 0x0046ba80
void DxUserInput::FUN_0046ba80()
{
	HRESULT hr = m_unk0x188->CreateDevice(GUID_SysKeyboard, &m_unk0x190, NULL);
	if (FAILED(hr)) {
		AtlThrow(hr);
	}

	hr = m_unk0x190->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) {
		AtlThrow(hr);
	}

	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = 0x800;

	hr = m_unk0x190->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	if (FAILED(hr)) {
		AtlThrow(hr);
	}
}

// FUNCTION: ROBLOX 0x0046bb10
void DxUserInput::FUN_0046bb10()
{
	CRect rect;
	m_unk0x180->GetClientRect(&rect);
	m_unk0x58 = G3D::Rect2D(G3D::Vector2(rect.Width(), rect.Height()));
}

// FUNCTION: ROBLOX 0x0046bc20
void DxUserInput::FUN_0046bc20(const G3D::Vector2& position)
{
	ATL::CCritSecLock lock(m_unk0x34);

	if (m_unk0x68) {
		if (FAILED(m_unk0x18c->Acquire())) {
			m_unk0x68 = false;
		}

		if (m_unk0x68) {
			return;
		}
	}

	if (m_unk0x180->IsTopParentActive()) {
		m_unk0x18c->Unacquire();
		m_unk0x68 = SUCCEEDED(m_unk0x18c->Acquire());

		if (m_unk0x68) {
			G3D::Rect2D rect = m_unk0x58;
			m_unk0x70 = position - rect.center();
		}
	}
}

// STUB: ROBLOX 0x0046bd60
void DxUserInput::FUN_0046bd60()
{
	ATL::CCritSecLock lock(m_unk0x34);

	if (m_unk0x190) {
		if (m_unk0x79 && FAILED(m_unk0x190->Acquire())) {
			m_unk0x79 = false;
		}

		if (m_unk0x78 != m_unk0x79 || (m_unk0x79 && m_unk0x7a != true)) {
			m_unk0x190->Unacquire();

			if (m_unk0x78) {
				m_unk0x7a = true;

				if (m_unk0x190->SetCooperativeLevel(m_unk0x184, DISCL_EXCLUSIVE | DISCL_FOREGROUND) == DI_OK) {
					m_unk0x79 = SUCCEEDED(m_unk0x190->Acquire());
				}
			}
			else {
				m_unk0x79 = false;
			}
		}
	}
}

// FUNCTION: ROBLOX 0x0046be50
void DxUserInput::FUN_0046be50(bool p_unk0x78)
{
	ATL::CCritSecLock lock(m_unk0x34);
	m_unk0x78 = p_unk0x78;
	FUN_0046bd60();
}

// FUNCTION: ROBLOX 0x0046bed0
bool DxUserInput::FUN_0046bed0(DIDEVICEOBJECTDATA* data, DWORD* count, IDirectInputDevice8* device)
{
	ATL::CCritSecLock lock(m_unk0x34);

	switch (device->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), data, count, 0)) {
	case DI_OK:
		return true;
	case DI_BUFFEROVERFLOW: {
		std::string name = device == m_unk0x190 ? "Keyboard " : "Mouse ";
		std::string message = name + "DI_BUFFEROVERFLOW!\n";
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_ERROR, message.c_str());
		return true;
	}
	default: {
		std::string name = device == m_unk0x190 ? "Keyboard " : "Mouse ";
		return false;
	}
	}
}

// STUB: ROBLOX 0x0046c090
void DxUserInput::FUN_0046c090()
{
	DIDEVICEOBJECTDATA data[0x800];
	DWORD count = 0x800;
	{
		ATL::CCritSecLock lock(m_unk0x34);
		if (!FUN_0046bed0(data, &count, m_unk0x190)) {
			m_unk0x79 = false;
			return;
		}
	}

	RBX::DataModel::Lock dataModelLock(m_unk0x194);
	for (DWORD i = 0; i < count; i++) {
		bool down = (data[i].dwData & 0x80) != 0;
		DWORD scanCode = data[i].dwOfs;
		SDLMod mod = FUN_0046e2e0(m_unk0x7b);
		SDLKey key = FUN_0046d6f0(scanCode);
		bool handled = false;
		if (m_unk0x7a && down && m_unk0x4c) {
			static HKL layout = GetKeyboardLayout(0);
			UINT virtualKey = MapVirtualKeyEx(scanCode, 1, layout);
			if (virtualKey == 0) {
				virtualKey = FUN_0046dff0(key);
			}

			if (virtualKey == VK_SNAPSHOT) {
				::SendMessage(m_unk0x4c->m_hWnd, WM_COMMAND, MAKEWPARAM(0x80fc, 1), 0);
			}
			else if (
				virtualKey == VK_F1 || virtualKey == VK_F4 ||
				!static_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd)->m_unk0xec
			) {
				UINT character = MapVirtualKeyEx(virtualKey, 2, layout);
				BYTE flags =
					(mod & KMOD_ALT ? FALT : 0) | (mod & KMOD_CTRL ? FCONTROL : 0) | (mod & KMOD_SHIFT ? FSHIFT : 0);
				for (int j = 0; j < m_unk0x54; j++) {
					const ACCEL& accel = m_unk0x50[j];
					bool matches =
						accel.fVirt & FVIRTKEY ? accel.key == virtualKey : accel.key == static_cast<char>(character);
					if (matches && (accel.fVirt & (FSHIFT | FCONTROL | FALT)) == flags) {
						::SendMessage(m_unk0x4c->m_hWnd, WM_COMMAND, MAKEWPARAM(m_unk0x50[j].cmd, 1), 0);
						handled = true;
						break;
					}
				}
			}
		}

		if (!handled) {
			m_unk0x194->processGui(RBX::UIEvent(this, down ? RBX::UIEvent::KEY_DOWN : RBX::UIEvent::KEY_UP, key, mod));
		}
	}
}

// FUNCTION: ROBLOX 0x0046c3c0
void DxUserInput::FUN_0046c3c0(RBX::UIEvent::EventType eventType, const G3D::Vector2& position)
{
	G3D::Vector2int16 windowSize(m_unk0x58.width(), m_unk0x58.height());
	G3D::Vector2int16 mousePosition(position);
	RBX::UIEvent event(this, eventType, mousePosition, windowSize);

	RBX::DataModel::Lock lock(m_unk0x194);
	m_unk0x194->processGui(event);
}

// FUNCTION: ROBLOX 0x0046c4b0
G3D::ReferenceCountedPointer<RBX::TextureProxyBase> DxUserInput::getGameCursor(RBX::Adorn* adorn)
{
	if (m_unk0x68) {
		return UserInputBase::getGameCursor(adorn);
	}

	return NULL;
}

// FUNCTION: ROBLOX 0x0046c4f0
G3D::Vector2 DxUserInput::FUN_0046c4f0()
{
	ATL::CCritSecLock lock(m_unk0x34);
	G3D::Rect2D rect = m_unk0x58;
	return rect.center() + m_unk0x70;
}

// FUNCTION: ROBLOX 0x0046c590
G3D::Vector2 DxUserInput::FUN_0046c590()
{
	ATL::CCritSecLock lock(m_unk0x34);

	CPoint point;
	GetCursorPos(&point);
	m_unk0x180->ScreenToClient(&point);

	return G3D::Vector2(point.x, point.y);
}

// FUNCTION: ROBLOX 0x0046c600
G3D::Vector2 DxUserInput::getCursorPosition()
{
	ATL::CCritSecLock lock(m_unk0x34);
	return m_unk0x68 ? FUN_0046c4f0() : FUN_0046c590();
}

// STUB: ROBLOX 0x0046c6a0
bool DxUserInput::keyDown(SDLKey key) const
{
	ATL::CCritSecLock lock(m_unk0x34);

	if (m_unk0x17c != SDLK_UNKNOWN && key == m_unk0x17c) {
		return true;
	}

	if (m_unk0x7b[FUN_0046db70(key)] & 0x80) {
		return true;
	}

	return false;
}

// STUB: ROBLOX 0x0046c780
bool DxUserInput::FUN_0046c780()
{
	if (static_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd)->m_unk0xec && ActiveDocView::FUN_00434840()) {
		RBX::DataModel::Lock lock(ActiveDocView::FUN_00434840());
		RBX::Verb* verb = ActiveDocView::FUN_00434840()->getVerb("ToggleToolbox");
		if (verb) {
			return !verb->isChecked();
		}
	}

	return false;
}

// STUB: ROBLOX 0x0046c950
void DxUserInput::FUN_0046c950(const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta)
{
	switch (m_unk0x6c) {
	case WRAP_AUTO:
		if (FUN_0046b920()) {
			FUN_0046e530(mouseDelta, wrapMouseDelta, m_unk0x70, G3D::Vector2(FUN_0046b8f0()));
		}
		else if (!FUN_0046c780()) {
			FUN_0046d670(mouseDelta, wrapMouseDelta, m_unk0x70);
		}
		else if (FUN_0046b880()) {
			FUN_0046e560(mouseDelta, wrapMouseDelta, m_unk0x70, G3D::Vector2(FUN_0046b8f0()));
		}
		else {
			FUN_0046e5f0(mouseDelta, wrapMouseDelta, m_unk0x70, G3D::Vector2(FUN_0046b8f0()));
		}
		break;
	case WRAP_CENTER:
		FUN_0046d6a0(mouseDelta, wrapMouseDelta, m_unk0x70);
		break;
	}
}

// FUNCTION: ROBLOX 0x0046ca90
void DxUserInput::FUN_0046ca90()
{
	ATL::CCritSecLock lock(m_unk0x34);

	if (m_unk0x18c && m_unk0x68) {
		m_unk0x18c->Unacquire();

		G3D::Vector2 position = FUN_0046c4f0();
		CPoint point(position.x, position.y);
		m_unk0x180->ClientToScreen(&point);
		SetCursorPos(point.x, point.y);

		static HCURSOR cursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		SetCursor(cursor);

		m_unk0x180->InvalidateRect(NULL);
		m_unk0x68 = false;
	}
}

// FUNCTION: ROBLOX 0x0046cbc0
void DxUserInput::FUN_0046cbc0(UINT message, int wParam, LPARAM lParam)
{
	switch (message) {
	case WM_KEYDOWN:
		FUN_0046bd60();
		break;
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP: {
		G3D::Vector2 position(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		FUN_0046c3c0(FUN_0046e290(message), position);

		CPoint point(position.x, position.y);
		CClientDC dc(m_unk0x180);

		if (dc.PtVisible(point)) {
			FUN_0046bc20(position);
		}
		break;
	}
	case WM_MOUSEWHEEL: {
		G3D::Vector2 position(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		FUN_0046c3c0(wParam > 0.0 ? RBX::UIEvent::MOUSE_WHEEL_FORWARD : RBX::UIEvent::MOUSE_WHEEL_BACKWARD, position);
		break;
	}
	case WM_ENTERMENULOOP:
	case WM_ENTERSIZEMOVE:
		FUN_0046ca90();
		break;
	}
}

// STUB: ROBLOX 0x0046cd90
void DxUserInput::FUN_0046cd90()
{
	DIDEVICEOBJECTDATA data[0x800];
	DWORD count = 0x800;
	{
		ATL::CCritSecLock lock(m_unk0x34);
		if (!FUN_0046bed0(data, &count, m_unk0x18c)) {
			m_unk0x68 = false;
			return;
		}
	}

	G3D::Vector2 wrapMouseDelta(0.0f, 0.0f);
	bool moved = false;
	for (DWORD i = 0; i < count; i++) {
		switch (data[i].dwOfs) {
		case DIMOFS_BUTTON0:
			::PostMessage(m_unk0x180->m_hWnd, WM_USER + 0xbb, 0, 0);
			FUN_0046c3c0(
				data[i].dwData == 0x80 ? RBX::UIEvent::MOUSE_LEFT_BUTTON_DOWN : RBX::UIEvent::MOUSE_LEFT_BUTTON_UP,
				getCursorPosition()
			);
			break;
		case DIMOFS_BUTTON1:
			::PostMessage(m_unk0x180->m_hWnd, WM_USER + 0xbb, 0, 0);
			FUN_0046c3c0(
				data[i].dwData == 0x80 ? RBX::UIEvent::MOUSE_RIGHT_BUTTON_DOWN : RBX::UIEvent::MOUSE_RIGHT_BUTTON_UP,
				getCursorPosition()
			);
			break;
		case DIMOFS_X:
		case DIMOFS_Y:
			moved = true;
			FUN_0046c950(FUN_0046d6c0(data[i]), wrapMouseDelta);
			break;
		case DIMOFS_Z: {
			float wheel = (int) data[i].dwData / 120.0;
			FUN_0046c3c0(
				wheel > 0.0f ? RBX::UIEvent::MOUSE_WHEEL_FORWARD : RBX::UIEvent::MOUSE_WHEEL_BACKWARD,
				getCursorPosition()
			);
			break;
		}
		}

		if (!m_unk0x68 || !FUN_0046b7f0()) {
			break;
		}
	}

	if (wrapMouseDelta != G3D::Vector2::zero()) {
		RBX::DataModel::Lock lock(m_unk0x194);
		m_unk0x194->getWorkspace()->onWrapMouse(wrapMouseDelta);
		FUN_0046c3c0(RBX::UIEvent::MOUSE_DELTA, wrapMouseDelta);
	}

	if (moved) {
		FUN_0046c3c0(RBX::UIEvent::MOUSE_MOVE, getCursorPosition());
	}
}

// FUNCTION: ROBLOX 0x0046d0a0
void DxUserInput::FUN_0046d0a0()
{
	FUN_0046bb10();

	ATL::CCritSecLock lock(m_unk0x34);

	if (m_unk0x68) {
		G3D::Vector2 position = getCursorPosition();
		CPoint point(position.x, position.y);
		CClientDC dc(m_unk0x180);

		if (!dc.PtVisible(point)) {
			FUN_0046ca90();
		}
	}
}

// FUNCTION: ROBLOX 0x0046d180
void DxUserInput::onEvent(const RBX::RunService* source, RBX::Heartbeat event)
{
	ATL::CCritSecLock lock(m_unk0x34);

	if (m_unk0x68) {
		FUN_0046cd90();
	}

	if (m_unk0x190) {
		m_unk0x190->GetDeviceState(sizeof(m_unk0x7b), m_unk0x7b);
	}

	if (m_unk0x79) {
		FUN_0046c090();
	}
}

// FUNCTION: ROBLOX 0x0046d220
void DxUserInput::onEvent(const RBX::RunService* source, RBX::RunTransition event)
{
	FUN_0046bd60();
}

// FUNCTION: ROBLOX 0x0046d230
DxUserInput::~DxUserInput()
{
	if (m_unk0x194) {
		RBX::stopListening(
			m_unk0x194->find<RBX::RunService>(), static_cast<RBX::Listener<RBX::RunService, RBX::Heartbeat>*>(this)
		);
	}

	if (m_unk0x194) {
		RBX::stopListening(
			m_unk0x194->find<RBX::RunService>(), static_cast<RBX::Listener<RBX::RunService, RBX::RunTransition>*>(this)
		);
	}

	if (m_unk0x18c) {
		m_unk0x18c->Unacquire();
		m_unk0x18c = NULL;
	}

	if (m_unk0x190) {
		m_unk0x190->Unacquire();
		m_unk0x190 = NULL;
	}

	if (m_unk0x188) {
		m_unk0x188 = NULL;
	}

	delete[] m_unk0x50;
}

// FUNCTION: ROBLOX 0x0046d3f0
void DxUserInput::setWrapMode(WrapMode wrapMode)
{
	m_unk0x6c = wrapMode;
}

// FUNCTION: ROBLOX 0x0046d400
void DxUserInput::centerCursor()
{
	m_unk0x70 = G3D::Vector2::zero();
}

// STUB: ROBLOX 0x0046d440
DxUserInput::DxUserInput(CWnd* wnd, RBX::DataModel* dataModel)
	: m_unk0x50(NULL), m_unk0x54(0), m_unk0x68(false), m_unk0x6c(WRAP_AUTO), m_unk0x78(false), m_unk0x79(false),
	  m_unk0x7a(false), m_unk0x17c(SDLK_UNKNOWN), m_unk0x180(wnd), m_unk0x184(wnd->GetTopLevelFrame()->GetSafeHwnd()),
	  m_unk0x194(RBX::shared_from(dataModel))
{
	FUN_0046bb10();

	for (m_unk0x4c = m_unk0x180->GetParentFrame(); m_unk0x4c != NULL; m_unk0x4c = m_unk0x4c->GetParentFrame()) {
		m_unk0x54 = CopyAcceleratorTable(m_unk0x4c->GetDefaultAccelerator(), NULL, 0);

		if (m_unk0x54) {
			m_unk0x50 = new ACCEL[m_unk0x54];
			CopyAcceleratorTable(m_unk0x4c->GetDefaultAccelerator(), m_unk0x50, m_unk0x54);
			break;
		}
	}

	memset(m_unk0x7b, 0, sizeof(m_unk0x7b));

	HRESULT hr =
		DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**) &m_unk0x188, NULL);
	if (FAILED(hr)) {
		AtlThrow(hr);
	}

	FUN_0046b9d0();
	FUN_0046ba80();

	if (dataModel) {
		RBX::startListening(
			dataModel->create<RBX::RunService>(), static_cast<RBX::Listener<RBX::RunService, RBX::Heartbeat>*>(this)
		);
	}

	if (dataModel) {
		RBX::startListening(
			dataModel->create<RBX::RunService>(), static_cast<RBX::Listener<RBX::RunService, RBX::RunTransition>*>(this)
		);
		dataModel->create<RBX::RunService>();
	}

	FUN_0046bd60();
}

static SDLKey g_unk0x009544f0[0x100];
static bool g_unk0x009548f0;

// FUNCTION: ROBLOX 0x0046d670
void FUN_0046d670(const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position)
{
	wrapMouseDelta = G3D::Vector2::zero();
	position += mouseDelta;
}

// FUNCTION: ROBLOX 0x0046d6a0
void FUN_0046d6a0(const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position)
{
	wrapMouseDelta += mouseDelta;
}

// FUNCTION: ROBLOX 0x0046d6c0
G3D::Vector2 FUN_0046d6c0(const DIDEVICEOBJECTDATA& data)
{
	G3D::Vector2 delta(0.0f, 0.0f);
	float value = (int) data.dwData;
	if (data.dwOfs == DIMOFS_X) {
		delta.x = value;
	}
	else {
		delta.y = value;
	}
	return delta;
}

// FUNCTION: ROBLOX 0x0046d6f0
SDLKey FUN_0046d6f0(DWORD scanCode)
{
	if (!g_unk0x009548f0) {
		for (int i = 0; i < 0x100; i++) {
			g_unk0x009544f0[i] = SDLK_UNKNOWN;
		}

		g_unk0x009544f0[DIK_ESCAPE] = SDLK_ESCAPE;
		g_unk0x009544f0[DIK_1] = SDLK_1;
		g_unk0x009544f0[DIK_2] = SDLK_2;
		g_unk0x009544f0[DIK_3] = SDLK_3;
		g_unk0x009544f0[DIK_4] = SDLK_4;
		g_unk0x009544f0[DIK_5] = SDLK_5;
		g_unk0x009544f0[DIK_6] = SDLK_6;
		g_unk0x009544f0[DIK_7] = SDLK_7;
		g_unk0x009544f0[DIK_8] = SDLK_8;
		g_unk0x009544f0[DIK_9] = SDLK_9;
		g_unk0x009544f0[DIK_0] = SDLK_0;
		g_unk0x009544f0[DIK_MINUS] = SDLK_MINUS;
		g_unk0x009544f0[DIK_EQUALS] = SDLK_EQUALS;
		g_unk0x009544f0[DIK_BACK] = SDLK_BACKSPACE;
		g_unk0x009544f0[DIK_TAB] = SDLK_TAB;
		g_unk0x009544f0[DIK_Q] = SDLK_q;
		g_unk0x009544f0[DIK_W] = SDLK_w;
		g_unk0x009544f0[DIK_E] = SDLK_e;
		g_unk0x009544f0[DIK_R] = SDLK_r;
		g_unk0x009544f0[DIK_T] = SDLK_t;
		g_unk0x009544f0[DIK_Y] = SDLK_y;
		g_unk0x009544f0[DIK_U] = SDLK_u;
		g_unk0x009544f0[DIK_I] = SDLK_i;
		g_unk0x009544f0[DIK_O] = SDLK_o;
		g_unk0x009544f0[DIK_P] = SDLK_p;
		g_unk0x009544f0[DIK_LBRACKET] = SDLK_LEFTBRACKET;
		g_unk0x009544f0[DIK_RBRACKET] = SDLK_RIGHTBRACKET;
		g_unk0x009544f0[DIK_RETURN] = SDLK_RETURN;
		g_unk0x009544f0[DIK_LCONTROL] = SDLK_LCTRL;
		g_unk0x009544f0[DIK_A] = SDLK_a;
		g_unk0x009544f0[DIK_S] = SDLK_s;
		g_unk0x009544f0[DIK_D] = SDLK_d;
		g_unk0x009544f0[DIK_F] = SDLK_f;
		g_unk0x009544f0[DIK_G] = SDLK_g;
		g_unk0x009544f0[DIK_H] = SDLK_h;
		g_unk0x009544f0[DIK_J] = SDLK_j;
		g_unk0x009544f0[DIK_K] = SDLK_k;
		g_unk0x009544f0[DIK_L] = SDLK_l;
		g_unk0x009544f0[DIK_SEMICOLON] = SDLK_SEMICOLON;
		g_unk0x009544f0[DIK_APOSTROPHE] = SDLK_QUOTE;
		g_unk0x009544f0[DIK_GRAVE] = SDLK_BACKQUOTE;
		g_unk0x009544f0[DIK_LSHIFT] = SDLK_LSHIFT;
		g_unk0x009544f0[DIK_BACKSLASH] = SDLK_BACKSLASH;
		g_unk0x009544f0[DIK_OEM_102] = SDLK_BACKSLASH;
		g_unk0x009544f0[DIK_Z] = SDLK_z;
		g_unk0x009544f0[DIK_X] = SDLK_x;
		g_unk0x009544f0[DIK_C] = SDLK_c;
		g_unk0x009544f0[DIK_V] = SDLK_v;
		g_unk0x009544f0[DIK_B] = SDLK_b;
		g_unk0x009544f0[DIK_N] = SDLK_n;
		g_unk0x009544f0[DIK_M] = SDLK_m;
		g_unk0x009544f0[DIK_COMMA] = SDLK_COMMA;
		g_unk0x009544f0[DIK_PERIOD] = SDLK_PERIOD;
		g_unk0x009544f0[DIK_SLASH] = SDLK_SLASH;
		g_unk0x009544f0[DIK_RSHIFT] = SDLK_RSHIFT;
		g_unk0x009544f0[DIK_MULTIPLY] = SDLK_KP_MULTIPLY;
		g_unk0x009544f0[DIK_LMENU] = SDLK_LALT;
		g_unk0x009544f0[DIK_SPACE] = SDLK_SPACE;
		g_unk0x009544f0[DIK_CAPITAL] = SDLK_CAPSLOCK;
		g_unk0x009544f0[DIK_F1] = SDLK_F1;
		g_unk0x009544f0[DIK_F2] = SDLK_F2;
		g_unk0x009544f0[DIK_F3] = SDLK_F3;
		g_unk0x009544f0[DIK_F4] = SDLK_F4;
		g_unk0x009544f0[DIK_F5] = SDLK_F5;
		g_unk0x009544f0[DIK_F6] = SDLK_F6;
		g_unk0x009544f0[DIK_F7] = SDLK_F7;
		g_unk0x009544f0[DIK_F8] = SDLK_F8;
		g_unk0x009544f0[DIK_F9] = SDLK_F9;
		g_unk0x009544f0[DIK_F10] = SDLK_F10;
		g_unk0x009544f0[DIK_NUMLOCK] = SDLK_NUMLOCK;
		g_unk0x009544f0[DIK_SCROLL] = SDLK_SCROLLOCK;
		g_unk0x009544f0[DIK_NUMPAD7] = SDLK_KP7;
		g_unk0x009544f0[DIK_NUMPAD8] = SDLK_KP8;
		g_unk0x009544f0[DIK_NUMPAD9] = SDLK_KP9;
		g_unk0x009544f0[DIK_SUBTRACT] = SDLK_KP_MINUS;
		g_unk0x009544f0[DIK_NUMPAD4] = SDLK_KP4;
		g_unk0x009544f0[DIK_NUMPAD5] = SDLK_KP5;
		g_unk0x009544f0[DIK_NUMPAD6] = SDLK_KP6;
		g_unk0x009544f0[DIK_ADD] = SDLK_KP_PLUS;
		g_unk0x009544f0[DIK_NUMPAD1] = SDLK_KP1;
		g_unk0x009544f0[DIK_NUMPAD2] = SDLK_KP2;
		g_unk0x009544f0[DIK_NUMPAD3] = SDLK_KP3;
		g_unk0x009544f0[DIK_NUMPAD0] = SDLK_KP0;
		g_unk0x009544f0[DIK_DECIMAL] = SDLK_KP_PERIOD;
		g_unk0x009544f0[DIK_F11] = SDLK_F11;
		g_unk0x009544f0[DIK_F12] = SDLK_F12;
		g_unk0x009544f0[DIK_F13] = SDLK_F13;
		g_unk0x009544f0[DIK_F14] = SDLK_F14;
		g_unk0x009544f0[DIK_F15] = SDLK_F15;
		g_unk0x009544f0[DIK_NUMPADEQUALS] = SDLK_KP_EQUALS;
		g_unk0x009544f0[DIK_NUMPADENTER] = SDLK_KP_ENTER;
		g_unk0x009544f0[DIK_RCONTROL] = SDLK_RCTRL;
		g_unk0x009544f0[DIK_DIVIDE] = SDLK_KP_DIVIDE;
		g_unk0x009544f0[DIK_SYSRQ] = SDLK_SYSREQ;
		g_unk0x009544f0[DIK_RMENU] = SDLK_RALT;
		g_unk0x009544f0[DIK_PAUSE] = SDLK_PAUSE;
		g_unk0x009544f0[DIK_HOME] = SDLK_HOME;
		g_unk0x009544f0[DIK_UP] = SDLK_UP;
		g_unk0x009544f0[DIK_PRIOR] = SDLK_PAGEUP;
		g_unk0x009544f0[DIK_LEFT] = SDLK_LEFT;
		g_unk0x009544f0[DIK_RIGHT] = SDLK_RIGHT;
		g_unk0x009544f0[DIK_END] = SDLK_END;
		g_unk0x009544f0[DIK_DOWN] = SDLK_DOWN;
		g_unk0x009544f0[DIK_NEXT] = SDLK_PAGEDOWN;
		g_unk0x009544f0[DIK_INSERT] = SDLK_INSERT;
		g_unk0x009544f0[DIK_DELETE] = SDLK_DELETE;
		g_unk0x009544f0[DIK_LWIN] = SDLK_LMETA;
		g_unk0x009544f0[DIK_RWIN] = SDLK_RMETA;
		g_unk0x009544f0[DIK_APPS] = SDLK_MENU;
		g_unk0x009548f0 = true;
	}

	return g_unk0x009544f0[scanCode];
}

static int g_unk0x009548f8[SDLK_LAST];
static bool g_unk0x00954e04;

// FUNCTION: ROBLOX 0x0046db70
int FUN_0046db70(SDLKey key)
{
	if (!g_unk0x00954e04) {
		for (int i = 0; i < SDLK_LAST; i++) {
			g_unk0x009548f8[i] = 0;
		}

		g_unk0x009548f8[SDLK_ESCAPE] = DIK_ESCAPE;
		g_unk0x009548f8[SDLK_1] = DIK_1;
		g_unk0x009548f8[SDLK_2] = DIK_2;
		g_unk0x009548f8[SDLK_3] = DIK_3;
		g_unk0x009548f8[SDLK_4] = DIK_4;
		g_unk0x009548f8[SDLK_5] = DIK_5;
		g_unk0x009548f8[SDLK_6] = DIK_6;
		g_unk0x009548f8[SDLK_7] = DIK_7;
		g_unk0x009548f8[SDLK_8] = DIK_8;
		g_unk0x009548f8[SDLK_9] = DIK_9;
		g_unk0x009548f8[SDLK_0] = DIK_0;
		g_unk0x009548f8[SDLK_MINUS] = DIK_MINUS;
		g_unk0x009548f8[SDLK_EQUALS] = DIK_EQUALS;
		g_unk0x009548f8[SDLK_BACKSPACE] = DIK_BACK;
		g_unk0x009548f8[SDLK_TAB] = DIK_TAB;
		g_unk0x009548f8[SDLK_q] = DIK_Q;
		g_unk0x009548f8[SDLK_w] = DIK_W;
		g_unk0x009548f8[SDLK_e] = DIK_E;
		g_unk0x009548f8[SDLK_r] = DIK_R;
		g_unk0x009548f8[SDLK_t] = DIK_T;
		g_unk0x009548f8[SDLK_y] = DIK_Y;
		g_unk0x009548f8[SDLK_u] = DIK_U;
		g_unk0x009548f8[SDLK_i] = DIK_I;
		g_unk0x009548f8[SDLK_o] = DIK_O;
		g_unk0x009548f8[SDLK_p] = DIK_P;
		g_unk0x009548f8[SDLK_LEFTBRACKET] = DIK_LBRACKET;
		g_unk0x009548f8[SDLK_RIGHTBRACKET] = DIK_RBRACKET;
		g_unk0x009548f8[SDLK_RETURN] = DIK_RETURN;
		g_unk0x009548f8[SDLK_LCTRL] = DIK_LCONTROL;
		g_unk0x009548f8[SDLK_a] = DIK_A;
		g_unk0x009548f8[SDLK_s] = DIK_S;
		g_unk0x009548f8[SDLK_d] = DIK_D;
		g_unk0x009548f8[SDLK_f] = DIK_F;
		g_unk0x009548f8[SDLK_g] = DIK_G;
		g_unk0x009548f8[SDLK_h] = DIK_H;
		g_unk0x009548f8[SDLK_j] = DIK_J;
		g_unk0x009548f8[SDLK_k] = DIK_K;
		g_unk0x009548f8[SDLK_l] = DIK_L;
		g_unk0x009548f8[SDLK_SEMICOLON] = DIK_SEMICOLON;
		g_unk0x009548f8[SDLK_QUOTE] = DIK_APOSTROPHE;
		g_unk0x009548f8[SDLK_BACKQUOTE] = DIK_GRAVE;
		g_unk0x009548f8[SDLK_LSHIFT] = DIK_LSHIFT;
		g_unk0x009548f8[SDLK_BACKSLASH] = DIK_OEM_102;
		g_unk0x009548f8[SDLK_z] = DIK_Z;
		g_unk0x009548f8[SDLK_x] = DIK_X;
		g_unk0x009548f8[SDLK_c] = DIK_C;
		g_unk0x009548f8[SDLK_v] = DIK_V;
		g_unk0x009548f8[SDLK_b] = DIK_B;
		g_unk0x009548f8[SDLK_n] = DIK_N;
		g_unk0x009548f8[SDLK_m] = DIK_M;
		g_unk0x009548f8[SDLK_COMMA] = DIK_COMMA;
		g_unk0x009548f8[SDLK_PERIOD] = DIK_PERIOD;
		g_unk0x009548f8[SDLK_SLASH] = DIK_SLASH;
		g_unk0x009548f8[SDLK_RSHIFT] = DIK_RSHIFT;
		g_unk0x009548f8[SDLK_KP_MULTIPLY] = DIK_MULTIPLY;
		g_unk0x009548f8[SDLK_LALT] = DIK_LMENU;
		g_unk0x009548f8[SDLK_SPACE] = DIK_SPACE;
		g_unk0x009548f8[SDLK_CAPSLOCK] = DIK_CAPITAL;
		g_unk0x009548f8[SDLK_F1] = DIK_F1;
		g_unk0x009548f8[SDLK_F2] = DIK_F2;
		g_unk0x009548f8[SDLK_F3] = DIK_F3;
		g_unk0x009548f8[SDLK_F4] = DIK_F4;
		g_unk0x009548f8[SDLK_F5] = DIK_F5;
		g_unk0x009548f8[SDLK_F6] = DIK_F6;
		g_unk0x009548f8[SDLK_F7] = DIK_F7;
		g_unk0x009548f8[SDLK_F8] = DIK_F8;
		g_unk0x009548f8[SDLK_F9] = DIK_F9;
		g_unk0x009548f8[SDLK_F10] = DIK_F10;
		g_unk0x009548f8[SDLK_NUMLOCK] = DIK_NUMLOCK;
		g_unk0x009548f8[SDLK_SCROLLOCK] = DIK_SCROLL;
		g_unk0x009548f8[SDLK_KP7] = DIK_NUMPAD7;
		g_unk0x009548f8[SDLK_KP8] = DIK_NUMPAD8;
		g_unk0x009548f8[SDLK_KP9] = DIK_NUMPAD9;
		g_unk0x009548f8[SDLK_KP_MINUS] = DIK_SUBTRACT;
		g_unk0x009548f8[SDLK_KP4] = DIK_NUMPAD4;
		g_unk0x009548f8[SDLK_KP5] = DIK_NUMPAD5;
		g_unk0x009548f8[SDLK_KP6] = DIK_NUMPAD6;
		g_unk0x009548f8[SDLK_KP_PLUS] = DIK_ADD;
		g_unk0x009548f8[SDLK_KP1] = DIK_NUMPAD1;
		g_unk0x009548f8[SDLK_KP2] = DIK_NUMPAD2;
		g_unk0x009548f8[SDLK_KP3] = DIK_NUMPAD3;
		g_unk0x009548f8[SDLK_KP0] = DIK_NUMPAD0;
		g_unk0x009548f8[SDLK_KP_PERIOD] = DIK_DECIMAL;
		g_unk0x009548f8[SDLK_F11] = DIK_F11;
		g_unk0x009548f8[SDLK_F12] = DIK_F12;
		g_unk0x009548f8[SDLK_F13] = DIK_F13;
		g_unk0x009548f8[SDLK_F14] = DIK_F14;
		g_unk0x009548f8[SDLK_F15] = DIK_F15;
		g_unk0x009548f8[SDLK_KP_EQUALS] = DIK_NUMPADEQUALS;
		g_unk0x009548f8[SDLK_KP_ENTER] = DIK_NUMPADENTER;
		g_unk0x009548f8[SDLK_RCTRL] = DIK_RCONTROL;
		g_unk0x009548f8[SDLK_KP_DIVIDE] = DIK_DIVIDE;
		g_unk0x009548f8[SDLK_SYSREQ] = DIK_SYSRQ;
		g_unk0x009548f8[SDLK_RALT] = DIK_RMENU;
		g_unk0x009548f8[SDLK_PAUSE] = DIK_PAUSE;
		g_unk0x009548f8[SDLK_HOME] = DIK_HOME;
		g_unk0x009548f8[SDLK_UP] = DIK_UP;
		g_unk0x009548f8[SDLK_PAGEUP] = DIK_PRIOR;
		g_unk0x009548f8[SDLK_LEFT] = DIK_LEFT;
		g_unk0x009548f8[SDLK_RIGHT] = DIK_RIGHT;
		g_unk0x009548f8[SDLK_END] = DIK_END;
		g_unk0x009548f8[SDLK_DOWN] = DIK_DOWN;
		g_unk0x009548f8[SDLK_PAGEDOWN] = DIK_NEXT;
		g_unk0x009548f8[SDLK_INSERT] = DIK_INSERT;
		g_unk0x009548f8[SDLK_DELETE] = DIK_DELETE;
		g_unk0x009548f8[SDLK_LMETA] = DIK_LWIN;
		g_unk0x009548f8[SDLK_RMETA] = DIK_RWIN;
		g_unk0x009548f8[SDLK_MENU] = DIK_APPS;

		g_unk0x00954e04 = true;
	}

	return g_unk0x009548f8[key];
}

static int g_unk0x00954e08[SDLK_LAST];
static bool g_unk0x00955314;

// FUNCTION: ROBLOX 0x0046dff0
int FUN_0046dff0(SDLKey key)
{
	if (!g_unk0x00955314) {
		for (int i = 0; i < SDLK_LAST; i++) {
			g_unk0x00954e08[i] = 0;
		}

		g_unk0x00954e08[SDLK_RETURN] = VK_RETURN;
		g_unk0x00954e08[SDLK_KP_ENTER] = VK_RETURN;
		g_unk0x00954e08[SDLK_PRINT] = VK_PRINT;
		g_unk0x00954e08[SDLK_SYSREQ] = VK_SNAPSHOT;
		g_unk0x00954e08[SDLK_ESCAPE] = VK_ESCAPE;
		g_unk0x00954e08[SDLK_BACKSPACE] = VK_BACK;
		g_unk0x00954e08[SDLK_TAB] = VK_TAB;
		g_unk0x00954e08[SDLK_LCTRL] = VK_LCONTROL;
		g_unk0x00954e08[SDLK_LSHIFT] = VK_LSHIFT;
		g_unk0x00954e08[SDLK_BACKSLASH] = VK_OEM_102;
		g_unk0x00954e08[SDLK_RSHIFT] = VK_RSHIFT;
		g_unk0x00954e08[SDLK_KP_MULTIPLY] = VK_MULTIPLY;
		g_unk0x00954e08[SDLK_LALT] = VK_LMENU;
		g_unk0x00954e08[SDLK_SPACE] = VK_SPACE;
		g_unk0x00954e08[SDLK_CAPSLOCK] = VK_CAPITAL;
		g_unk0x00954e08[SDLK_F1] = VK_F1;
		g_unk0x00954e08[SDLK_F2] = VK_F2;
		g_unk0x00954e08[SDLK_F3] = VK_F3;
		g_unk0x00954e08[SDLK_F4] = VK_F4;
		g_unk0x00954e08[SDLK_F5] = VK_F5;
		g_unk0x00954e08[SDLK_F6] = VK_F6;
		g_unk0x00954e08[SDLK_F7] = VK_F7;
		g_unk0x00954e08[SDLK_F8] = VK_F8;
		g_unk0x00954e08[SDLK_F9] = VK_F9;
		g_unk0x00954e08[SDLK_F10] = VK_F10;
		g_unk0x00954e08[SDLK_NUMLOCK] = VK_NUMLOCK;
		g_unk0x00954e08[SDLK_SCROLLOCK] = VK_SCROLL;
		g_unk0x00954e08[SDLK_KP7] = VK_NUMPAD7;
		g_unk0x00954e08[SDLK_KP8] = VK_NUMPAD8;
		g_unk0x00954e08[SDLK_KP9] = VK_NUMPAD9;
		g_unk0x00954e08[SDLK_KP_MINUS] = VK_SUBTRACT;
		g_unk0x00954e08[SDLK_KP4] = VK_NUMPAD4;
		g_unk0x00954e08[SDLK_KP5] = VK_NUMPAD5;
		g_unk0x00954e08[SDLK_KP6] = VK_NUMPAD6;
		g_unk0x00954e08[SDLK_KP_PLUS] = VK_ADD;
		g_unk0x00954e08[SDLK_KP1] = VK_NUMPAD1;
		g_unk0x00954e08[SDLK_KP2] = VK_NUMPAD2;
		g_unk0x00954e08[SDLK_KP3] = VK_NUMPAD3;
		g_unk0x00954e08[SDLK_KP0] = VK_NUMPAD0;
		g_unk0x00954e08[SDLK_KP_PERIOD] = VK_DECIMAL;
		g_unk0x00954e08[SDLK_F11] = VK_F11;
		g_unk0x00954e08[SDLK_F12] = VK_F12;
		g_unk0x00954e08[SDLK_F13] = VK_F13;
		g_unk0x00954e08[SDLK_F14] = VK_F14;
		g_unk0x00954e08[SDLK_F15] = VK_F15;
		g_unk0x00954e08[SDLK_RCTRL] = VK_RCONTROL;
		g_unk0x00954e08[SDLK_KP_DIVIDE] = VK_DIVIDE;
		g_unk0x00954e08[SDLK_RALT] = VK_RMENU;
		g_unk0x00954e08[SDLK_HOME] = VK_HOME;
		g_unk0x00954e08[SDLK_UP] = VK_UP;
		g_unk0x00954e08[SDLK_PAGEUP] = VK_PRIOR;
		g_unk0x00954e08[SDLK_LEFT] = VK_LEFT;
		g_unk0x00954e08[SDLK_RIGHT] = VK_RIGHT;
		g_unk0x00954e08[SDLK_END] = VK_END;
		g_unk0x00954e08[SDLK_DOWN] = VK_DOWN;
		g_unk0x00954e08[SDLK_PAGEDOWN] = VK_NEXT;
		g_unk0x00954e08[SDLK_INSERT] = VK_INSERT;
		g_unk0x00954e08[SDLK_DELETE] = VK_DELETE;
		g_unk0x00954e08[SDLK_LMETA] = VK_LWIN;
		g_unk0x00954e08[SDLK_RMETA] = VK_RWIN;
		g_unk0x00954e08[SDLK_MENU] = VK_APPS;
		g_unk0x00955314 = true;
	}

	return g_unk0x00954e08[key];
}

// FUNCTION: ROBLOX 0x0046e290
RBX::UIEvent::EventType FUN_0046e290(UINT message)
{
	switch (message) {
	case WM_MOUSEMOVE:
		return RBX::UIEvent::MOUSE_MOVE;
	case WM_LBUTTONDOWN:
		return RBX::UIEvent::MOUSE_LEFT_BUTTON_DOWN;
	case WM_RBUTTONDOWN:
		return RBX::UIEvent::MOUSE_RIGHT_BUTTON_DOWN;
	case WM_LBUTTONUP:
		return RBX::UIEvent::MOUSE_LEFT_BUTTON_UP;
	case WM_RBUTTONUP:
		return RBX::UIEvent::MOUSE_RIGHT_BUTTON_UP;
	default:
		return RBX::UIEvent::NO_EVENT;
	}
}

// FUNCTION: ROBLOX 0x0046e2e0
SDLMod FUN_0046e2e0(const BYTE* keys)
{
	int mod = KMOD_NONE;
	if (keys[DIK_LSHIFT] & 0x80) {
		mod = KMOD_LSHIFT;
	}
	if (keys[DIK_RSHIFT] & 0x80) {
		mod |= KMOD_RSHIFT;
	}
	if (keys[DIK_LCONTROL] & 0x80) {
		mod |= KMOD_LCTRL;
	}
	if (keys[DIK_RCONTROL] & 0x80) {
		mod |= KMOD_RCTRL;
	}
	if (keys[DIK_LMENU] & 0x80) {
		mod |= KMOD_LALT;
	}
	if (keys[DIK_LMENU] & 0x80) {
		mod |= KMOD_RALT;
	}
	return static_cast<SDLMod>(mod);
}

// STUB: ROBLOX 0x0046e3e0
void FUN_0046e3e0(
	const G3D::Vector2& mouseDelta,
	G3D::Vector2& wrapMouseDelta,
	G3D::Vector2& position,
	const G3D::Vector2& size,
	int border,
	float stickiness
)
{
	G3D::Vector2 half(size.x * 0.5, size.y * 0.5);
	float inset = border;
	RBX::Rect bounds(inset - half.x, inset - half.y, half.x - inset, half.y - inset);

	G3D::Vector2 oldPosition = position;
	bounds.unionWith(RBX::Rect(oldPosition, oldPosition));

	G3D::Vector2 newPosition = mouseDelta + oldPosition;
	G3D::Vector2 clamped = newPosition.clamp(bounds.low, bounds.high);
	G3D::Vector2 overflow = newPosition - clamped;

	position = clamped + overflow * stickiness;
	wrapMouseDelta += overflow;
}

// FUNCTION: ROBLOX 0x0046e530
void FUN_0046e530(
	const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position, const G3D::Vector2& size
)
{
	FUN_0046e3e0(mouseDelta, wrapMouseDelta, position, size, 6, 0.0f);
}

// FUNCTION: ROBLOX 0x0046e560
void FUN_0046e560(
	const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position, const G3D::Vector2& size
)
{
	float y = mouseDelta.y + position.y;
	FUN_0046e3e0(mouseDelta, wrapMouseDelta, position, size, 6, 0.0f);
	wrapMouseDelta.y = 0.0f;
	position.y = y;

	float inset = size.y * 0.5 - 2.0;
	float top = -inset;
	position.y = std::max(top, position.y);
}

// FUNCTION: ROBLOX 0x0046e5f0
void FUN_0046e5f0(
	const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position, const G3D::Vector2& size
)
{
	float y = mouseDelta.y + position.y;
	FUN_0046e3e0(mouseDelta, wrapMouseDelta, position, size, 20, 0.05f);
	wrapMouseDelta.y = 0.0f;
	position.y = y;
}
