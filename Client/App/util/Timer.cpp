#include "util/Timer.h"

#include <windows.h>

namespace RBX {

double getRealTime()
{
	LARGE_INTEGER count;
	LARGE_INTEGER frequency;

	QueryPerformanceCounter(&count);
	QueryPerformanceFrequency(&frequency);

	return (double) count.QuadPart / (double) frequency.QuadPart;
}

} // namespace RBX
