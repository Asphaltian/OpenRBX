#include "util/Timer.h"

#include <windows.h>

namespace RBX {

// FUNCTION: WEBSERVICE 0x10099040
double getRealTime()
{
	LARGE_INTEGER count;
	LARGE_INTEGER frequency;

	QueryPerformanceCounter(&count);
	QueryPerformanceFrequency(&frequency);

	return (double) count.QuadPart / (double) frequency.QuadPart;
}

} // namespace RBX
