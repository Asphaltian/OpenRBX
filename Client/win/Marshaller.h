#ifndef MARSHALLER_H
#define MARSHALLER_H

#include "decomp.h"

#include <atlbase.h>
#include <atlwin.h>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <deque>
#include <map>

// VTABLE: ROBLOX 0x008016c4
// SIZE 0x48
class Marshaller : public CWindowImpl<Marshaller>
{
public:
	Marshaller(DWORD threadId);

	// FUNCTION: ROBLOX 0x00418db0
	~Marshaller() {}

	// SYNTHETIC: ROBLOX 0x00419f90
	// Marshaller::`scalar deleting destructor'

	// clang-format off
	BEGIN_MSG_MAP(Marshaller)
		MESSAGE_HANDLER(WM_USER + 101, FUN_00418660)
	END_MSG_MAP()
	// clang-format on

	// SYNTHETIC: ROBLOX 0x00418f90
	// ?ProcessWindowMessage@Marshaller@@UAEHPAUHWND__@@IIJAAJK@Z

	// FUNCTION: ROBLOX 0x00418e30
	virtual void OnFinalMessage(HWND hWnd) { delete this; }

	// FUNCTION: ROBLOX 0x00418fd0
	void FUN_00418fd0(boost::function<void()> function)
	{
		{
			boost::mutex::scoped_lock lock(m_unk0x40);
			m_unk0x2c.push_back(function);
		}

		::PostMessage(m_hWnd, WM_USER + 101, 0, 0);
	}

	static Marshaller* FUN_0041a4a0();

private:
	LRESULT FUN_00418660(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	int m_unk0x24;                                  // 0x24
	DWORD m_unk0x28;                                // 0x28
	std::deque<boost::function<void()> > m_unk0x2c; // 0x2c
	boost::mutex m_unk0x40;                         // 0x40

	static std::map<DWORD, Marshaller*> g_unk0x00953144;
	static boost::recursive_mutex g_unk0x00953150;
};

DECOMP_SIZE_ASSERT(Marshaller, 0x48)

extern Marshaller* g_unk0x00952f1c;

#endif // MARSHALLER_H
