#ifndef MARSHALWINDOW_H
#define MARSHALWINDOW_H

#include "decomp.h"

#include <atlbase.h>
#include <atlsync.h>
#include <atlwin.h>
#include <map>

namespace RBX {

template <class Source, class Event, class Source2, class Event2>
class MarshaledListener;

// SIZE 0x4
class EventDataBase
{
public:
	virtual void VTable0x00() = 0; // vtable+0x00
	virtual ~EventDataBase() {}    // vtable+0x04
};

// SIZE 0x44
class CMarshalWindow : public CWindowImpl<CMarshalWindow>
{
public:
	CMarshalWindow(DWORD threadId);
	~CMarshalWindow() {}

	// SYNTHETIC: ROBLOX 0x00436c80
	// RBX::CMarshalWindow::`scalar deleting destructor'

	// clang-format off
	BEGIN_MSG_MAP(CMarshalWindow)
		MESSAGE_HANDLER(WM_DESTROY, FUN_00436b20)
		MESSAGE_HANDLER(WM_USER + 101, OnEventData)
	END_MSG_MAP()
	// clang-format on

	// SYNTHETIC: ROBLOX 0x00436c00
	// ?ProcessWindowMessage@CMarshalWindow@RBX@@UAEHPAUHWND__@@IIJAAJK@Z

	void FUN_00436b00(EventDataBase* data);

	static void FUN_00436cc0(CMarshalWindow* window);
	static CMarshalWindow* FUN_00436e70();

private:
	template <class Source, class Event, class Source2, class Event2>
	friend class MarshaledListener;

	LRESULT FUN_00436b20(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnEventData(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		EventDataBase* data = reinterpret_cast<EventDataBase*>(lParam);
		data->VTable0x00();
		delete data;
		return 0;
	}

	int m_unk0x24;                   // 0x24
	DWORD m_unk0x28;                 // 0x28
	ATL::CCriticalSection m_unk0x2c; // 0x2c

	static std::map<DWORD, CMarshalWindow*> g_unk0x00953938;
	static ATL::CCriticalSection g_unk0x00953944;
};

DECOMP_SIZE_ASSERT(CMarshalWindow, 0x44)

} // namespace RBX

#endif // MARSHALWINDOW_H
