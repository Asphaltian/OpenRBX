#include <windows.h>

namespace RBX {

// FUNCTION: WEBSERVICE 0x10099040
double getRealTime()
{
	LARGE_INTEGER counter;
	LARGE_INTEGER frequency;

	QueryPerformanceCounter(&counter);
	QueryPerformanceFrequency(&frequency);

	return (double) counter.QuadPart / (double) frequency.QuadPart;
}

} // namespace RBX
