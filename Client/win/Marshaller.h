#ifndef MARSHALLER_H
#define MARSHALLER_H

#include "placeholder.h"

#include <atlbase.h>
#include <atlwin.h>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <deque>
#include <map>

class Marshaller : public CWindowImpl<Marshaller>
{
public:
	Marshaller(DWORD threadId);

	~Marshaller() {}

	// clang-format off
	BEGIN_MSG_MAP(Marshaller)
		MESSAGE_HANDLER(WM_USER + 101, FUN_00418660)
	END_MSG_MAP()
	// clang-format on

	virtual void OnFinalMessage(HWND hWnd) { delete this; }

	void FUN_00418fd0(boost::function0<void> function)
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

	int m_unk0x24;
	DWORD m_unk0x28;
	std::deque<boost::function0<void> > m_unk0x2c;
	boost::mutex m_unk0x40;

	static std::map<DWORD, Marshaller*> g_unk0x00953144;
	static boost::recursive_mutex g_unk0x00953150;
};

extern Marshaller* g_unk0x00952f1c;

#endif // MARSHALLER_H
