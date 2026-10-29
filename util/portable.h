#ifndef PORTABLE_H
#define PORTABLE_H

#ifndef _MSC_VER
#define __declspec(x)
#define __thiscall
#define __forceinline inline
#define __int64 long long

#include "msvcrt.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#define stricmp strcasecmp
#define strnicmp strncasecmp
#define _strdup strdup
#define __time64_t long long

inline long long portableTime64(long long* out)
{
	long long now = (long long) time(NULL);
	if (out) {
		*out = now;
	}
	return now;
}

#define _time64 portableTime64
#define _localtime64 localtime
#define __try if (1)
#define __except(x) else if (0)
#define _gmtime64 gmtime

#ifdef __cplusplus
#include <string>
#endif
#endif

#endif // PORTABLE_H
