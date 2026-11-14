#ifndef FILTEREDLISTENER_H
#define FILTEREDLISTENER_H

#include "MarshaledListener.h"
#include "placeholder.h"

namespace RBX {

template <class Source, class Event, class Source2, class Event2>
class FilteredListener : public MarshaledListener<Source, Event, Source2, Event2>
{
protected:
	typedef MarshaledListener<Source, Event, Source2, Event2> Base;

	virtual void VTable0x0c(const Source* source, Event event) throw()
	{
		for (typename std::vector<typename Base::EventData*>::iterator it = this->m_unk0x04.begin();
			 it != this->m_unk0x04.end();
			 ++it) {
			if (VTable0x10(source, event, (*it)->m_unk0x04.get(), (*it)->m_unk0x10)) {
				if ((*it)->m_unk0x04.get() != source) {
					(*it)->FUN_0045b7d0(NULL);
				}
				return;
			}
		}
		Base::VTable0x0c(source, event);
	}

	virtual bool VTable0x10(
		const Source* source, const Event& event, const Source* oldSource, const Event& oldEvent
	) = 0;
};

} // namespace RBX

#endif // FILTEREDLISTENER_H
