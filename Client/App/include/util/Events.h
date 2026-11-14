#ifndef UTIL_EVENTS_H
#define UTIL_EVENTS_H

#include <algorithm>
#include <vector>

namespace RBX {

class StandardOut;

template <class Source, class Event>
class Notifier;

struct RaiseRange
{
	unsigned int index;
	unsigned int upper;
	RaiseRange* previous;

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

template <class Source, class Event>
class __declspec(novtable) Listener
{
protected:
	friend class Notifier<Source, Event>;

	virtual void onEvent(const Source* source, Event event) = 0;
	Listener& operator=(const Listener&);
	virtual ~Listener() {}
};

template <class Source, class Event>
class __declspec(novtable) Notifier
{
private:
	mutable std::vector<Listener<Source, Event>*> listeners;
	mutable RaiseRange* raiseRange;

protected:
	Notifier() : raiseRange(NULL) {}
	Notifier(const Notifier&);
	Notifier& operator=(const Notifier&);
	virtual ~Notifier() {}

public:
	void addListener(Listener<Source, Event>* listener) const;
	void removeListener(Listener<Source, Event>* listener) const;

protected:
	bool hasListeners() const { return !listeners.empty(); }

	void raise(Event event, Listener<Source, Event>* listener) const;
	void raise(Event event) const throw();
	void raise() const;

	virtual void onAddListener(Listener<Source, Event>* listener) const {}
	virtual void onRemoveListener(Listener<Source, Event>* listener) const {}
};

template <class Source, class Event>
void Notifier<Source, Event>::addListener(Listener<Source, Event>* listener) const
{
	if (std::find(listeners.begin(), listeners.end(), listener) == listeners.end()) {
		listeners.push_back(listener);
		onAddListener(listener);
	}
}

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

template <class Source, class Event, class SourcePtr>
void startListening(const SourcePtr& source, Listener<Source, Event>* listener)
{
	if (source) {
		source->Notifier<Source, Event>::addListener(listener);
	}
}

template <class Source, class Event, class SourcePtr>
void stopListening(const SourcePtr& source, Listener<Source, Event>* listener)
{
	if (source) {
		source->Notifier<Source, Event>::removeListener(listener);
	}
}

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
