#include "MarshalWindow.h"

namespace RBX {

// GLOBAL: ROBLOX 0x00953938
std::map<DWORD, CMarshalWindow*> CMarshalWindow::g_unk0x00953938;

// GLOBAL: ROBLOX 0x00953944
ATL::CCriticalSection CMarshalWindow::g_unk0x00953944;

// FUNCTION: ROBLOX 0x00436b00
void CMarshalWindow::FUN_00436b00(EventDataBase* data)
{
	PostMessage(WM_USER + 101, 0, (LPARAM) data);
}

// FUNCTION: ROBLOX 0x00436b20
LRESULT CMarshalWindow::FUN_00436b20(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CCritSecLock lock(m_unk0x2c);
	MSG msg;
	while (PeekMessage(&msg, m_hWnd, WM_USER + 101, WM_USER + 101, PM_REMOVE)) {
		delete reinterpret_cast<EventDataBase*>(msg.lParam);
	}
	bHandled = FALSE;
	return 0;
}

// FUNCTION: ROBLOX 0x00436cc0
void CMarshalWindow::FUN_00436cc0(CMarshalWindow* window)
{
	CCritSecLock lock(g_unk0x00953944);
	if (--window->m_unk0x24 == 0) {
		g_unk0x00953938.erase(window->m_unk0x28);
		::DestroyWindow(window->m_hWnd);
	}
}

// FUNCTION: ROBLOX 0x00436dd0
CMarshalWindow::CMarshalWindow(DWORD threadId) : m_unk0x24(0)
{
	m_unk0x28 = threadId;
	Create(NULL, NULL, NULL, WS_POPUP);
}

// FUNCTION: ROBLOX 0x00436e70
CMarshalWindow* CMarshalWindow::FUN_00436e70()
{
	CCritSecLock lock(g_unk0x00953944);
	DWORD threadId = GetCurrentThreadId();
	std::map<DWORD, CMarshalWindow*>::iterator it = g_unk0x00953938.find(threadId);
	if (it != g_unk0x00953938.end()) {
		++it->second->m_unk0x24;
		return it->second;
	}

	CMarshalWindow* window = new CMarshalWindow(threadId);
	g_unk0x00953938[threadId] = window;
	++window->m_unk0x24;
	return window;
}

} // namespace RBX
