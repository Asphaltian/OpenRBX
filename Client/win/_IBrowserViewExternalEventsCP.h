#ifndef IBROWSERVIEWEXTERNALEVENTSCP_H
#define IBROWSERVIEWEXTERNALEVENTSCP_H

#include "Roblox_h.h"

#include <atlbase.h>
#include <atlcom.h>

template <class T>
class CProxy_IBrowserViewExternalEvents : public IConnectionPointImpl<T, &__uuidof(_IBrowserViewExternalEvents)>
{
};

#endif // IBROWSERVIEWEXTERNALEVENTSCP_H
