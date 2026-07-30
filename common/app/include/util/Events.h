#ifndef UTIL_EVENTS_H
#define UTIL_EVENTS_H

#include <vector>

namespace RBX {

template <class Source, class Event>
class Listener
{
public:
	virtual ~Listener() {}
};

template <class Source, class Event>
class Notifier
{
public:
	virtual ~Notifier() {}

private:
	std::vector<Listener<Source, Event>*> listeners; // 0x04
	int raiseRange;                                  // 0x14
};

} // namespace RBX

#endif // UTIL_EVENTS_H
