#ifndef MARSHALEDLISTENER_H
#define MARSHALEDLISTENER_H

#include "MarshalWindow.h"
#include "decomp.h"
#include "util/Events.h"
#include "util/object.h"

#include <boost/shared_ptr.hpp>
#include <vector>

namespace RBX {

// clang-format off
// TEMPLATE: ROBLOX 0x004209b0
// ??0?$MarshaledListener@VInstance@RBX@@UDescendentAdded@2@V12@U32@@RBX@@QAE@XZ
// TEMPLATE: ROBLOX 0x00420a50
// ??0?$MarshaledListener@VInstance@RBX@@UDescendentRemoving@2@V12@U32@@RBX@@QAE@XZ
// TEMPLATE: ROBLOX 0x00420ac0
// ?FUN_00420ac0@?$MarshaledListener@VInstance@RBX@@UDescendentRemoving@2@V12@U32@@RBX@@QAEXXZ
// TEMPLATE: ROBLOX 0x00420b80
// ??1?$MarshaledListener@VInstance@RBX@@UDescendentAdded@2@V12@U32@@RBX@@UAE@XZ
// TEMPLATE: ROBLOX 0x00420c00
// ??1?$MarshaledListener@VInstance@RBX@@UDescendentRemoving@2@V12@U32@@RBX@@UAE@XZ
// SYNTHETIC: ROBLOX 0x00420c80
// RBX::MarshaledListener<RBX::Instance,RBX::DescendentAdded,RBX::Instance,RBX::DescendentAdded>::`scalar deleting destructor'
// SYNTHETIC: ROBLOX 0x00420ca0
// RBX::MarshaledListener<RBX::Instance,RBX::DescendentRemoving,RBX::Instance,RBX::DescendentRemoving>::`scalar deleting destructor'
// TEMPLATE: ROBLOX 0x00420cc0
// ??0EventData@?$MarshaledListener@VInstance@RBX@@UDescendentAdded@2@V12@U32@@RBX@@QAE@PAV12@PBVInstance@2@UDescendentAdded@2@@Z
// TEMPLATE: ROBLOX 0x00420d80
// ??1EventData@?$MarshaledListener@VInstance@RBX@@UDescendentAdded@2@V12@U32@@RBX@@UAE@XZ
// TEMPLATE: ROBLOX 0x00420e20
// ??0EventData@?$MarshaledListener@VInstance@RBX@@UDescendentRemoving@2@V12@U32@@RBX@@QAE@PAV12@PBVInstance@2@UDescendentRemoving@2@@Z
// TEMPLATE: ROBLOX 0x00420ee0
// ??1EventData@?$MarshaledListener@VInstance@RBX@@UDescendentRemoving@2@V12@U32@@RBX@@UAE@XZ
// TEMPLATE: ROBLOX 0x00420f80
// ?VTable0x00@EventData@?$MarshaledListener@VInstance@RBX@@UDescendentAdded@2@V12@U32@@RBX@@UAEXXZ
// SYNTHETIC: ROBLOX 0x00421040
// RBX::MarshaledListener<RBX::Instance,RBX::DescendentAdded,RBX::Instance,RBX::DescendentAdded>::EventData::`scalar deleting destructor'
// SYNTHETIC: ROBLOX 0x00421060
// RBX::MarshaledListener<RBX::Instance,RBX::DescendentRemoving,RBX::Instance,RBX::DescendentRemoving>::EventData::`scalar deleting destructor'
// TEMPLATE: ROBLOX 0x00421190
// ?VTable0x0c@?$MarshaledListener@VInstance@RBX@@UDescendentAdded@2@V12@U32@@RBX@@MAEXPBVInstance@2@UDescendentAdded@2@@Z
// TEMPLATE: ROBLOX 0x00421230
// ?VTable0x0c@?$MarshaledListener@VInstance@RBX@@UDescendentRemoving@2@V12@U32@@RBX@@MAEXPBVInstance@2@UDescendentRemoving@2@@Z
// TEMPLATE: ROBLOX 0x0045b660
// ??0?$MarshaledListener@VInstance@RBX@@UChildAdded@2@V12@U32@@RBX@@QAE@XZ
// TEMPLATE: ROBLOX 0x0045b6d0
// ??0?$MarshaledListener@VInstance@RBX@@UChildRemoved@2@V12@U32@@RBX@@QAE@XZ
// TEMPLATE: ROBLOX 0x0045b740
// ?onEvent@?$MarshaledListener@VInstance@RBX@@UChildAdded@2@V12@U32@@RBX@@MAEXPBVInstance@2@UChildAdded@2@@Z
// TEMPLATE: ROBLOX 0x0045b7d0
// ?FUN_0045b7d0@EventData@?$MarshaledListener@VInstance@RBX@@UDescendentAdded@2@V12@U32@@RBX@@QAEXPBVInstance@3@@Z
// TEMPLATE: ROBLOX 0x0045b870
// ??1?$MarshaledListener@VInstance@RBX@@UChildAdded@2@V12@U32@@RBX@@UAE@XZ
// TEMPLATE: ROBLOX 0x0045b8f0
// ??1?$MarshaledListener@VInstance@RBX@@UChildRemoved@2@V12@U32@@RBX@@UAE@XZ
// SYNTHETIC: ROBLOX 0x0045b970
// RBX::MarshaledListener<RBX::Instance,RBX::ChildAdded,RBX::Instance,RBX::ChildAdded>::`scalar deleting destructor'
// SYNTHETIC: ROBLOX 0x0045b990
// RBX::MarshaledListener<RBX::Instance,RBX::ChildRemoved,RBX::Instance,RBX::ChildRemoved>::`scalar deleting destructor'
// TEMPLATE: ROBLOX 0x0045b9b0
// ??0EventData@?$MarshaledListener@VInstance@RBX@@UChildAdded@2@V12@U32@@RBX@@QAE@PAV12@PBVInstance@2@UChildAdded@2@@Z
// TEMPLATE: ROBLOX 0x0045ba70
// ??1EventData@?$MarshaledListener@VInstance@RBX@@UChildAdded@2@V12@U32@@RBX@@UAE@XZ
// TEMPLATE: ROBLOX 0x0045bb40
// ?VTable0x00@EventData@?$MarshaledListener@VInstance@RBX@@UChildAdded@2@V12@U32@@RBX@@UAEXXZ
// SYNTHETIC: ROBLOX 0x0045bbd0
// RBX::MarshaledListener<RBX::Instance,RBX::ChildAdded,RBX::Instance,RBX::ChildAdded>::EventData::`scalar deleting destructor'
// TEMPLATE: ROBLOX 0x0045bbf0
// ??0EventData@?$MarshaledListener@VInstance@RBX@@UChildRemoved@2@V12@U32@@RBX@@QAE@PAV12@PBVInstance@2@UChildRemoved@2@@Z
// TEMPLATE: ROBLOX 0x0045bcb0
// ?FUN_0045bcb0@?$MarshaledListener@VInstance@RBX@@UDescendentRemoving@2@V12@U32@@RBX@@QAEXPAVEventData@12@@Z
// TEMPLATE: ROBLOX 0x0045bd60
// ??1EventData@?$MarshaledListener@VInstance@RBX@@UChildRemoved@2@V12@U32@@RBX@@UAE@XZ
// SYNTHETIC: ROBLOX 0x0045be30
// RBX::MarshaledListener<RBX::Instance,RBX::ChildRemoved,RBX::Instance,RBX::ChildRemoved>::EventData::`scalar deleting destructor'
// TEMPLATE: ROBLOX 0x0045bf10
// ?VTable0x0c@?$MarshaledListener@VInstance@RBX@@UChildAdded@2@V12@U32@@RBX@@MAEXPBVInstance@2@UChildAdded@2@@Z
// TEMPLATE: ROBLOX 0x0045bfa0
// ?VTable0x0c@?$MarshaledListener@VInstance@RBX@@UChildRemoved@2@V12@U32@@RBX@@MAEXPBVInstance@2@UChildRemoved@2@@Z
// TEMPLATE: ROBLOX 0x0046a5e0
// ??0?$MarshaledListener@VSelection@RBX@@VSelectionChanged@2@V12@V32@@RBX@@QAE@XZ
// TEMPLATE: ROBLOX 0x0046a650
// ?onEvent@?$MarshaledListener@VInstance@RBX@@UDescendentAdded@2@V12@U32@@RBX@@MAEXPBVInstance@2@UDescendentAdded@2@@Z
// TEMPLATE: ROBLOX 0x0046a6f0
// ??0EventData@?$MarshaledListener@VSelection@RBX@@VSelectionChanged@2@V12@V32@@RBX@@QAE@PAV12@PBVSelection@2@VSelectionChanged@2@@Z
// TEMPLATE: ROBLOX 0x0046a7b0
// ??1EventData@?$MarshaledListener@VSelection@RBX@@VSelectionChanged@2@V12@V32@@RBX@@UAE@XZ
// SYNTHETIC: ROBLOX 0x0046a850
// RBX::MarshaledListener<RBX::Selection,RBX::SelectionChanged,RBX::Selection,RBX::SelectionChanged>::EventData::`scalar deleting destructor'
// TEMPLATE: ROBLOX 0x0046a870
// ??1?$MarshaledListener@VSelection@RBX@@VSelectionChanged@2@V12@V32@@RBX@@UAE@XZ
// TEMPLATE: ROBLOX 0x0046ac00
// ?VTable0x0c@?$MarshaledListener@VSelection@RBX@@VSelectionChanged@2@V12@V32@@RBX@@MAEXPBVSelection@2@VSelectionChanged@2@@Z
// TEMPLATE: ROBLOX 0x00468f00
// ??0?$MarshaledListener@VInstance@RBX@@VPropertyChanged@2@V12@V32@@RBX@@QAE@XZ
// TEMPLATE: ROBLOX 0x00468f70
// ?onEvent@?$MarshaledListener@VInstance@RBX@@VPropertyChanged@2@V12@V32@@RBX@@MAEXPBVInstance@2@VPropertyChanged@2@@Z
// TEMPLATE: ROBLOX 0x00468fd0
// ??0?$MarshaledListener@VRunService@RBX@@W4RunState@2@V12@W432@@RBX@@QAE@XZ
// TEMPLATE: ROBLOX 0x00469040
// ?onEvent@?$MarshaledListener@VRunService@RBX@@W4RunState@2@V12@W432@@RBX@@MAEXPBVRunService@2@W4RunState@2@@Z
// TEMPLATE: ROBLOX 0x00469090
// ??1?$MarshaledListener@VInstance@RBX@@VPropertyChanged@2@V12@V32@@RBX@@UAE@XZ
// TEMPLATE: ROBLOX 0x00469110
// ??1?$MarshaledListener@VRunService@RBX@@W4RunState@2@V12@W432@@RBX@@UAE@XZ
// TEMPLATE: ROBLOX 0x00469970
// ?VTable0x0c@?$MarshaledListener@VInstance@RBX@@VPropertyChanged@2@V12@V32@@RBX@@MAEXPBVInstance@2@VPropertyChanged@2@@Z
// TEMPLATE: ROBLOX 0x004699d0
// ?VTable0x0c@?$MarshaledListener@VRunService@RBX@@W4RunState@2@V12@W432@@RBX@@MAEXPBVRunService@2@W4RunState@2@@Z
// TEMPLATE: ROBLOX 0x004691d0
// ??0EventData@?$MarshaledListener@VInstance@RBX@@VPropertyChanged@2@V12@V32@@RBX@@QAE@PAV12@PBVInstance@2@VPropertyChanged@2@@Z
// TEMPLATE: ROBLOX 0x00469380
// ??0EventData@?$MarshaledListener@VRunService@RBX@@W4RunState@2@V12@W432@@RBX@@QAE@PAV12@PBVRunService@2@W4RunState@2@@Z
// SYNTHETIC: ROBLOX 0x00469190
// RBX::MarshaledListener<RBX::Instance,RBX::PropertyChanged,RBX::Instance,RBX::PropertyChanged>::`scalar deleting destructor'
// SYNTHETIC: ROBLOX 0x004691b0
// RBX::MarshaledListener<RBX::RunService,enum RBX::RunState,RBX::RunService,enum RBX::RunState>::`scalar deleting destructor'
// clang-format on

// SIZE 0x1c
template <class Source, class Event, class Source2, class Event2>
class MarshaledListener : public Listener<Source, Event>
{
public:
	// SIZE 0x20
	class EventData : public EventDataBase
	{
	public:
		EventData(MarshaledListener* listener, const Source* source, Event event)
			: m_unk0x0c(listener), m_unk0x10(event)
		{
			FUN_0045b7d0(source);
		}

		virtual ~EventData()
		{
			if (m_unk0x0c) {
				m_unk0x0c->FUN_0045bcb0(this);
			}
		}

		virtual void VTable0x00()
		{
			if (m_unk0x0c) {
				try {
					m_unk0x0c->FUN_0045bcb0(this);
					m_unk0x0c->VTable0x08(m_unk0x04.get(), m_unk0x10);
				}
				catch (...) {
				}
				m_unk0x0c = NULL;
			}
		}

		void FUN_0045b7d0(const Source* source) { m_unk0x04 = shared_from(source); }

		boost::shared_ptr<const Source> m_unk0x04; // 0x04
		MarshaledListener* m_unk0x0c;              // 0x0c
		Event m_unk0x10;                           // 0x10
	};

	MarshaledListener() : m_unk0x14(GetCurrentThreadId()), m_unk0x18(CMarshalWindow::FUN_00436e70()) {}

	virtual ~MarshaledListener()
	{
		FUN_00420ac0();
		CMarshalWindow::FUN_00436cc0(m_unk0x18);
	}

	void FUN_00420ac0()
	{
		CCritSecLock lock(m_unk0x18->m_unk0x2c);
		for (typename std::vector<EventData*>::iterator it = m_unk0x04.begin(); it != m_unk0x04.end(); ++it) {
			(*it)->m_unk0x0c = NULL;
		}
		m_unk0x04.clear();
	}

	void FUN_0045bcb0(EventData* data)
	{
		CCritSecLock lock(m_unk0x18->m_unk0x2c);
		for (typename std::vector<EventData*>::iterator it = m_unk0x04.begin(); it != m_unk0x04.end(); ++it) {
			if (*it == data) {
				m_unk0x04.erase(it);
				break;
			}
		}
	}

protected:
	virtual void onEvent(const Source* source, Event event) throw()
	{
		CCritSecLock lock(m_unk0x18->m_unk0x2c);
		VTable0x0c(source, event);
	}

	virtual void VTable0x08(const Source2* source, Event2 event) = 0; // vtable+0x08

	virtual void VTable0x0c(const Source* source, Event event) throw() // vtable+0x0c
	{
		EventData* data = new EventData(this, source, event);
		m_unk0x04.push_back(data);
		m_unk0x18->FUN_00436b00(data);
	}

protected:
	std::vector<EventData*> m_unk0x04; // 0x04

private:
	DWORD m_unk0x14;           // 0x14
	CMarshalWindow* m_unk0x18; // 0x18
};

} // namespace RBX

#endif // MARSHALEDLISTENER_H
