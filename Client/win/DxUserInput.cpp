#include "DxUserInput.h"

#include "ActiveDocView.h"
#include "MainFrm.h"
#include "UserInputUtil.h"
#include "rbxgraphics/TextureProxyBase.h"
#include "util/FrameInterpolator.h"
#include "util/Rect.h"
#include "util/StandardOut.h"
#include "v8datamodel/DataModel.h"

#include <windowsx.h>

bool DxUserInput::FUN_0046b7f0(const G3D::Vector2& position)
{
	G3D::Vector2 extent = m_unk0x58.center();
	if (position.x < -extent.x) {
		return false;
	}

	if (position.x > extent.x) {
		return false;
	}

	if (position.y < -extent.y) {
		return false;
	}

	if (position.y > extent.y) {
		return false;
	}

	return true;
}

bool DxUserInput::FUN_0046b880()
{
	CRect rect;
	m_unk0x180->GetClientRect(&rect);

	CPoint clientOrigin = rect.TopLeft();
	CPoint screenOrigin = clientOrigin;
	m_unk0x180->ClientToScreen(&screenOrigin);

	if (screenOrigin == clientOrigin) {
		return true;
	}
	return false;
}

G3D::Vector2int16 DxUserInput::FUN_0046b8f0()
{
	return G3D::Vector2int16((G3D::int16) m_unk0x58.width(), (G3D::int16) m_unk0x58.height());
}

bool DxUserInput::FUN_0046b920()
{
	return keyDown(SDLK_w) || keyDown(SDLK_s) || keyDown(SDLK_a) || keyDown(SDLK_d) || keyDown(SDLK_UP) ||
		   keyDown(SDLK_DOWN) || keyDown(SDLK_LEFT) || keyDown(SDLK_RIGHT);
}

void DxUserInput::setKeyState(SDLKey key, bool down)
{
	m_unk0x17c = down ? key : SDLK_UNKNOWN;
}

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

void DxUserInput::FUN_0046bb10()
{
	CRect rect;
	m_unk0x180->GetClientRect(&rect);
	m_unk0x58 = G3D::Rect2D(G3D::Vector2(rect.Width(), rect.Height()));
}

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

void DxUserInput::FUN_0046be50(bool p_unk0x78)
{
	ATL::CCritSecLock lock(m_unk0x34);
	m_unk0x78 = p_unk0x78;
	FUN_0046bd60();
}

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
				::PostMessage(m_unk0x4c->m_hWnd, WM_COMMAND, MAKEWPARAM(0x80fc, 1), 0);
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
						::PostMessage(m_unk0x4c->m_hWnd, WM_COMMAND, MAKEWPARAM(m_unk0x50[j].cmd, 1), 0);
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

void DxUserInput::FUN_0046c3c0(RBX::UIEvent::EventType eventType, const G3D::Vector2& position)
{
	G3D::Vector2int16 windowSize(m_unk0x58.width(), m_unk0x58.height());
	G3D::Vector2int16 mousePosition(position);
	RBX::UIEvent event(this, eventType, mousePosition, windowSize);

	RBX::DataModel::Lock lock(m_unk0x194);
	m_unk0x194->processGui(event);
}

G3D::ReferenceCountedPointer<RBX::TextureProxyBase> DxUserInput::getGameCursor(RBX::Adorn* adorn)
{
	if (m_unk0x68) {
		return UserInputBase::getGameCursor(adorn);
	}

	return NULL;
}

G3D::Vector2 DxUserInput::FUN_0046c4f0()
{
	ATL::CCritSecLock lock(m_unk0x34);
	G3D::Rect2D rect = m_unk0x58;
	return rect.center() + m_unk0x70;
}

G3D::Vector2 DxUserInput::FUN_0046c590()
{
	ATL::CCritSecLock lock(m_unk0x34);

	CPoint point;
	GetCursorPos(&point);
	m_unk0x180->ScreenToClient(&point);

	return G3D::Vector2(point.x, point.y);
}

G3D::Vector2 DxUserInput::getCursorPosition()
{
	ATL::CCritSecLock lock(m_unk0x34);
	if (m_unk0x68 && RBX::FrameInterpolator::isRendering()) {
		return getPendingCursorPosition();
	}

	return m_unk0x68 ? FUN_0046c4f0() : FUN_0046c590();
}

G3D::Vector2 DxUserInput::getPendingCursorPosition()
{
	G3D::Vector2 position = m_unk0x70;

	DIDEVICEOBJECTDATA data[0x800];
	DWORD count = 0x800;
	if (SUCCEEDED(m_unk0x18c->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), data, &count, DIGDD_PEEK))) {
		G3D::Vector2 wrapMouseDelta(0.0f, 0.0f);
		for (DWORD i = 0; i < count; i++) {
			if (data[i].dwOfs == DIMOFS_X || data[i].dwOfs == DIMOFS_Y) {
				FUN_0046c950(FUN_0046d6c0(data[i]), wrapMouseDelta, position);
			}

			if (!FUN_0046b7f0(position)) {
				break;
			}
		}
	}

	G3D::Rect2D rect = m_unk0x58;
	return rect.center() + position;
}

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

bool DxUserInput::FUN_0046c780()
{
	if (static_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd)->m_unk0xec) {
		if (ActiveDocView::FUN_00434840()) {
			RBX::DataModel::Lock lock(ActiveDocView::FUN_00434840());
			RBX::Verb* verb = ActiveDocView::FUN_00434840()->getVerb("ToggleToolbox");
			if (verb) {
				return !verb->isChecked();
			}
		}
	}

	return false;
}

void DxUserInput::FUN_0046c950(const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position)
{
	switch (m_unk0x6c) {
	case WRAP_AUTO:
		if (FUN_0046b920()) {
			FUN_0046e530(mouseDelta, wrapMouseDelta, position, G3D::Vector2(FUN_0046b8f0()));
		}
		else if (!FUN_0046c780()) {
			FUN_0046d670(mouseDelta, wrapMouseDelta, position);
		}
		else if (FUN_0046b880()) {
			FUN_0046e560(mouseDelta, wrapMouseDelta, position, FUN_0046b8f0());
		}
		else {
			FUN_0046e5f0(mouseDelta, wrapMouseDelta, position, FUN_0046b8f0());
		}
		break;
	case WRAP_CENTER:
		FUN_0046d6a0(mouseDelta, wrapMouseDelta, position);
		break;
	}
}

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
			m_unk0x180->PostMessage(WM_USER + 0xbb);
			FUN_0046c3c0(
				data[i].dwData == 0x80 ? RBX::UIEvent::MOUSE_LEFT_BUTTON_DOWN : RBX::UIEvent::MOUSE_LEFT_BUTTON_UP,
				getCursorPosition()
			);
			break;
		case DIMOFS_BUTTON1:
			m_unk0x180->PostMessage(WM_USER + 0xbb);
			FUN_0046c3c0(
				data[i].dwData == 0x80 ? RBX::UIEvent::MOUSE_RIGHT_BUTTON_DOWN : RBX::UIEvent::MOUSE_RIGHT_BUTTON_UP,
				getCursorPosition()
			);
			break;
		case DIMOFS_X:
		case DIMOFS_Y:
			moved = true;
			FUN_0046c950(FUN_0046d6c0(data[i]), wrapMouseDelta, m_unk0x70);
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

		if (!m_unk0x68 || !FUN_0046b7f0(m_unk0x70)) {
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

void DxUserInput::onEvent(const RBX::RunService* source, RBX::RunTransition event)
{
	FUN_0046bd60();
}

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

void DxUserInput::setWrapMode(WrapMode wrapMode)
{
	m_unk0x6c = wrapMode;
}

void DxUserInput::centerCursor()
{
	m_unk0x70 = G3D::Vector2::zero();
}

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
