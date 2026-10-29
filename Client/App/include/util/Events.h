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
	// FUNCTION: ROBLOX 0x00420000
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
	Listener& operator=(const Listener&);
	virtual ~Listener() {} // vtable+0x04
};

// SIZE 0x18
template <class Source, class Event>
class __declspec(novtable) Notifier
{
private:
	mutable std::vector<Listener<Source, Event>*> listeners; // 0x04
	mutable RaiseRange* raiseRange;                          // 0x14

protected:
	Notifier() : raiseRange(NULL) {}
	Notifier(const Notifier&);
	Notifier& operator=(const Notifier&);
	virtual ~Notifier() {} // vtable+0x00

public:
	void addListener(Listener<Source, Event>* listener) const;
	void removeListener(Listener<Source, Event>* listener) const;

protected:
	bool hasListeners() const { return !listeners.empty(); }

	void raise(Event event, Listener<Source, Event>* listener) const;
	void raise(Event event) const throw();
	void raise() const;

	virtual void onAddListener(Listener<Source, Event>* listener) const {}    // vtable+0x04
	virtual void onRemoveListener(Listener<Source, Event>* listener) const {} // vtable+0x08
};

// FUNCTION: WEBSERVICE 0x1001acb0
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

// clang-format off
// FUNCTION: WEBSERVICE 0x10032240
// FUNCTION: ROBLOX 0x00583060
// RBX::Notifier<RBX::StandardOut,RBX::StandardOutMessage>::raise
// FUNCTION: WEBSERVICE 0x100cf390
// FUNCTION: ROBLOX 0x005ee4d0
// RBX::Notifier<RBX::World,RBX::AutoJoin>::raise
// FUNCTION: WEBSERVICE 0x100cf450
// FUNCTION: ROBLOX 0x005ee590
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
// FUNCTION: ROBLOX 0x005ee830
// RBX::Notifier<RBX::World,RBX::AutoJoin>::raise
// FUNCTION: WEBSERVICE 0x100cf790
// FUNCTION: ROBLOX 0x005ee8d0
// RBX::Notifier<RBX::World,RBX::AutoDestroy>::raise
// TEMPLATE: WEBSERVICE 0x100485a0
// TEMPLATE: ROBLOX 0x005768b0
// RBX::Notifier<RBX::Instance,RBX::ChildAdded>::raise
// TEMPLATE: WEBSERVICE 0x100486c0
// TEMPLATE: ROBLOX 0x005769d0
// RBX::Notifier<RBX::Instance,RBX::ChildRemoved>::raise
// TEMPLATE: WEBSERVICE 0x10049220
// TEMPLATE: ROBLOX 0x00577360
// RBX::Notifier<RBX::Instance,RBX::ChildAdded>::raise
// TEMPLATE: WEBSERVICE 0x10049300
// TEMPLATE: ROBLOX 0x00577440
// RBX::Notifier<RBX::Instance,RBX::ChildRemoved>::raise
// TEMPLATE: WEBSERVICE 0x1006b500
// TEMPLATE: ROBLOX 0x0058fd60
// RBX::Notifier<RBX::Workspace,RBX::DrawChanged>::raise
// TEMPLATE: WEBSERVICE 0x1006bc40
// TEMPLATE: ROBLOX 0x00590420
// RBX::Notifier<RBX::Workspace,RBX::DrawChanged>::raise
// TEMPLATE: WEBSERVICE 0x1018c910
// TEMPLATE: ROBLOX 0x004a6180
// RBX::Notifier<RBX::Network::Player,RBX::Network::CharacterAdded>::raise
// TEMPLATE: WEBSERVICE 0x1018ca50
// TEMPLATE: ROBLOX 0x004a62c0
// RBX::Notifier<RBX::Network::Player,RBX::Network::CharacterRemoving>::raise
// TEMPLATE: WEBSERVICE 0x1018cfc0
// TEMPLATE: ROBLOX 0x004a6b30
// RBX::Notifier<RBX::Network::Player,RBX::Network::CharacterAdded>::raise
// TEMPLATE: WEBSERVICE 0x1018d0a0
// TEMPLATE: ROBLOX 0x004a6c10
// RBX::Notifier<RBX::Network::Player,RBX::Network::CharacterRemoving>::raise
// TEMPLATE: WEBSERVICE 0x10190ee0
// TEMPLATE: ROBLOX 0x004b0050
// RBX::Notifier<RBX::Network::Players,RBX::Network::ChatMessage>::raise
// TEMPLATE: WEBSERVICE 0x10191020
// TEMPLATE: ROBLOX 0x004b0190
// RBX::Notifier<RBX::Network::Players,RBX::SuperSafeChanged>::raise
// TEMPLATE: WEBSERVICE 0x10191460
// TEMPLATE: ROBLOX 0x004b0690
// RBX::Notifier<RBX::Network::Players,RBX::Network::ChatMessage>::raise
// TEMPLATE: WEBSERVICE 0x10191550
// TEMPLATE: ROBLOX 0x004b0780
// RBX::Notifier<RBX::Network::Players,RBX::SuperSafeChanged>::raise
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
