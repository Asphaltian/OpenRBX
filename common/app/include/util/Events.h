#ifndef UTIL_EVENTS_H
#define UTIL_EVENTS_H

#include <vector>

namespace RBX {

// SIZE 0x0c
struct RaiseRange
{
	unsigned int index;   // 0x00
	unsigned int upper;   // 0x04
	RaiseRange* previous; // 0x08

	void removeIndex(unsigned int removed);
};

// SIZE 0x04
template <class Source, class Event>
class __declspec(novtable) Listener
{
protected:
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
	virtual ~Notifier() {}                                              // vtable+0x00
	virtual void onAddListener(Listener<Source, Event>* listener) {}    // vtable+0x04
	virtual void onRemoveListener(Listener<Source, Event>* listener) {} // vtable+0x08

	bool hasListeners() const { return !listeners.empty(); }

	void raise(Event event) const
	{
		for (size_t index = 0; index < listeners.size(); index++) {
			listeners[index]->onEvent(static_cast<const Source*>(this), event);
		}
	}

private:
	std::vector<Listener<Source, Event>*> listeners; // 0x04
	RaiseRange* raiseRange;                          // 0x14
};

template <class Source, class Event>
void Notifier<Source, Event>::addListener(Listener<Source, Event>* listener) const
{
	const_cast<Notifier*>(this)->listeners.push_back(listener);
	const_cast<Notifier*>(this)->onAddListener(listener);
}

template <class Source, class Event>
void Notifier<Source, Event>::removeListener(Listener<Source, Event>* listener) const
{
	std::vector<Listener<Source, Event>*>& all = const_cast<Notifier*>(this)->listeners;

	for (size_t index = 0; index < all.size(); index++) {
		if (all[index] == listener) {
			all.erase(all.begin() + index);
			break;
		}
	}

	const_cast<Notifier*>(this)->onRemoveListener(listener);
}

} // namespace RBX

#endif // UTIL_EVENTS_H
