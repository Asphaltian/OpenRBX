#ifndef UTIL_EVENTS_H
#define UTIL_EVENTS_H

#include <algorithm>
#include <vector>

namespace RBX {

class StandardOut;

template <class Source, class Event>
class Notifier;

// SIZE 0x0c
struct RaiseRange
{
	unsigned int index;   // 0x00
	unsigned int upper;   // 0x04
	RaiseRange* previous; // 0x08

	// FUNCTION: WEBSERVICE 0x1000cd10
	void removeIndex(unsigned int index)
	{
		RaiseRange* range = this;

		do {
			if (range->upper > index) {
				range->upper--;
			}

			if (range->index >= index) {
				range->index--;
			}

			range = range->previous;
		} while (range != NULL);
	}
};

// SIZE 0x04
template <class Source, class Event>
class __declspec(novtable) Listener
{
protected:
	friend class Notifier<Source, Event>;

	virtual void onEvent(const Source* source, Event event) = 0; // vtable+0x00
	virtual ~Listener() {}                                       // vtable+0x04
};

// SIZE 0x18
template <class Source, class Event>
class __declspec(novtable) Notifier
{
public:
	void addListener(Listener<Source, Event>* listener) const;
	void removeListener(Listener<Source, Event>* listener) const;

protected:
	Notifier() : raiseRange(NULL) {}

	virtual ~Notifier() {}                                                    // vtable+0x00
	virtual void onAddListener(Listener<Source, Event>* listener) const {}    // vtable+0x04
	virtual void onRemoveListener(Listener<Source, Event>* listener) const {} // vtable+0x08

	bool hasListeners() const { return !listeners.empty(); }

	void raise(Event event, Listener<Source, Event>* listener) const;

	void raise(Event event) const throw();

private:
	mutable std::vector<Listener<Source, Event>*> listeners; // 0x04
	mutable RaiseRange* raiseRange;                          // 0x14
};

// STUB: WEBSERVICE 0x1001acb0
// RBX::Notifier<RBX::StandardOut,RBX::StandardOutMessage>::addListener
template <class Source, class Event>
void Notifier<Source, Event>::addListener(Listener<Source, Event>* listener) const
{
	if (std::find(listeners.begin(), listeners.end(), listener) == listeners.end()) {
		listeners.push_back(listener);
		onAddListener(listener);
	}
}

// FUNCTION: WEBSERVICE 0x1001ad50
// RBX::Notifier<RBX::StandardOut,RBX::StandardOutMessage>::removeListener
template <class Source, class Event>
void Notifier<Source, Event>::removeListener(Listener<Source, Event>* listener) const
{
	typename std::vector<Listener<Source, Event>*>::iterator found =
		std::find(listeners.begin(), listeners.end(), listener);

	if (found != listeners.end()) {
		onRemoveListener(listener);

		if (raiseRange != NULL) {
			raiseRange->removeIndex(found - listeners.begin());
		}

		listeners.erase(found);
	}
}

// clang-format off
// FUNCTION: WEBSERVICE 0x10032240
// RBX::Notifier<RBX::StandardOut,RBX::StandardOutMessage>::raise
// FUNCTION: WEBSERVICE 0x100cf390
// RBX::Notifier<RBX::World,RBX::AutoJoin>::raise
// FUNCTION: WEBSERVICE 0x100cf450
// RBX::Notifier<RBX::World,RBX::AutoDestroy>::raise
// clang-format on
template <class Source, class Event>
void Notifier<Source, Event>::raise(Event event, Listener<Source, Event>* listener) const
{
	try {
		listener->onEvent(static_cast<const Source*>(this), event);
	}
	catch (std::exception& error) {
		std::string what(error.what());

		StandardOut::singleton()->print(MESSAGE_WARNING, "Exception caught in onEvent. %s", what.c_str());
	}
}

// clang-format off
// FUNCTION: WEBSERVICE 0x100cf6f0
// RBX::Notifier<RBX::World,RBX::AutoJoin>::raise
// FUNCTION: WEBSERVICE 0x100cf790
// RBX::Notifier<RBX::World,RBX::AutoDestroy>::raise
// TEMPLATE: WEBSERVICE 0x100485a0
// RBX::Notifier<RBX::Instance,RBX::ChildAdded>::raise
// TEMPLATE: WEBSERVICE 0x100486c0
// RBX::Notifier<RBX::Instance,RBX::ChildRemoved>::raise
// TEMPLATE: WEBSERVICE 0x10049220
// RBX::Notifier<RBX::Instance,RBX::ChildAdded>::raise
// TEMPLATE: WEBSERVICE 0x10049300
// RBX::Notifier<RBX::Instance,RBX::ChildRemoved>::raise
// TEMPLATE: WEBSERVICE 0x1006b500
// RBX::Notifier<RBX::Workspace,RBX::DrawChanged>::raise
// TEMPLATE: WEBSERVICE 0x1006bc40
// RBX::Notifier<RBX::Workspace,RBX::DrawChanged>::raise
// clang-format on
template <class Source, class Event>
void Notifier<Source, Event>::raise(Event event) const throw()
{
	RaiseRange range;

	range.index = 0;
	range.upper = listeners.size();
	range.previous = raiseRange;
	raiseRange = &range;

	while (range.index < range.upper) {
		raise(event, listeners[range.index++]);
	}

	raiseRange = range.previous;
}

} // namespace RBX

#endif // UTIL_EVENTS_H
