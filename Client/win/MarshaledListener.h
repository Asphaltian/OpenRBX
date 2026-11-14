#ifndef MARSHALEDLISTENER_H
#define MARSHALEDLISTENER_H

#include "MarshalWindow.h"
#include "placeholder.h"
#include "util/Events.h"
#include "util/object.h"

#include <boost/shared_ptr.hpp>
#include <vector>

namespace RBX {

template <class Source, class Event, class Source2, class Event2>
class MarshaledListener : public Listener<Source, Event>
{
public:
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

		boost::shared_ptr<const Source> m_unk0x04;
		MarshaledListener* m_unk0x0c;
		Event m_unk0x10;
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

	virtual void VTable0x08(const Source2* source, Event2 event) = 0;

	virtual void VTable0x0c(const Source* source, Event event) throw()
	{
		EventData* data = new EventData(this, source, event);
		m_unk0x04.push_back(data);
		m_unk0x18->FUN_00436b00(data);
	}

protected:
	std::vector<EventData*> m_unk0x04;

private:
	DWORD m_unk0x14;
	CMarshalWindow* m_unk0x18;
};

} // namespace RBX

#endif // MARSHALEDLISTENER_H
