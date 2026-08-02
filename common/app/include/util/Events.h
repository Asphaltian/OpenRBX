#ifndef UTIL_EVENTS_H
#define UTIL_EVENTS_H

#include <algorithm>
#include <vector>

namespace RBX {

template <class Source, class Event>
class Notifier;

// SIZE 0x0c
struct RaiseRange
{
	unsigned int index;   // 0x00
	unsigned int upper;   // 0x04
	RaiseRange* previous; // 0x08

	// STUB: WEBSERVICE 0x1000cd10
	void removeIndex(unsigned int removed)
	{
		RaiseRange* range = this;

		do {
			if (range->upper > removed) {
				range->upper--;
			}

			if (range->index >= removed) {
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

	void raise(Event event, Listener<Source, Event>* listener) const
	{
		listener->onEvent(static_cast<const Source*>(this), event);
	}

	void raise(Event event) const;

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

// STUB: WEBSERVICE 0x1001ad50
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

template <class Source, class Event>
void Notifier<Source, Event>::raise(Event event) const
{
	RaiseRange range;
	range.index = 0;
	range.upper = listeners.size();
	range.previous = raiseRange;
	raiseRange = &range;

	while (range.index < range.upper) {
		raise(event, listeners[range.index]);
		range.index++;
	}

	raiseRange = range.previous;
}

} // namespace RBX

#endif // UTIL_EVENTS_H
