#ifndef FILTEREDLISTENER_H
#define FILTEREDLISTENER_H

#include "MarshaledListener.h"
#include "decomp.h"

namespace RBX {

// clang-format off
// SYNTHETIC: ROBLOX 0x0046a900
// RBX::FilteredListener<RBX::Selection,RBX::SelectionChanged,RBX::Selection,RBX::SelectionChanged>::`scalar deleting destructor'
// TEMPLATE: ROBLOX 0x0046aca0
// ?VTable0x0c@?$FilteredListener@VSelection@RBX@@VSelectionChanged@2@V12@V32@@RBX@@MAEXPBVSelection@2@VSelectionChanged@2@@Z
// clang-format on

// SIZE 0x1c
template <class Source, class Event, class Source2, class Event2>
class FilteredListener : public MarshaledListener<Source, Event, Source2, Event2>
{
protected:
	typedef MarshaledListener<Source, Event, Source2, Event2> Base;

	virtual void VTable0x0c(const Source* source, Event event) throw() // vtable+0x0c
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
		const Source* source,
		const Event& event,
		const Source* oldSource,
		const Event& oldEvent
	) = 0; // vtable+0x10
};

} // namespace RBX

#endif // FILTEREDLISTENER_H
