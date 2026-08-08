#include "util/Profiling.h"

#include "decomp.h"

namespace RBX {
namespace Profiling {

// GLOBAL: WEBSERVICE 0x102fcf6c
DWORD Mark::markTlsIndex = 0;

// STUB: WEBSERVICE 0x10084600
void CodeProfiler::log(__int64 kern, __int64 user, bool frameTick)
{
	STUB(0x10084600);
}

// STUB: WEBSERVICE 0x100846d0
double Bucket::getActualFPS() const
{
	STUB(0x100846d0);

	return 0.0;
}

// STUB: WEBSERVICE 0x100846f0
double Bucket::getFrameTime() const
{
	STUB(0x100846f0);

	return 0.0;
}

// STUB: WEBSERVICE 0x10084720
double Bucket::getTotalTime() const
{
	STUB(0x10084720);

	return 0.0;
}

// STUB: WEBSERVICE 0x10084750
Profiler::Profiler(const char* name) : bucketTimeSpan(0.0), name(name)
{
	STUB(0x10084750);
}

// STUB: WEBSERVICE 0x100847b0
CodeProfiler::CodeProfiler(const char* name) : Profiler(name)
{
	STUB(0x100847b0);
}

// STUB: WEBSERVICE 0x100847f0
Bucket Profiler::getData(double seconds) const
{
	STUB(0x100847f0);

	return Bucket();
}

// STUB: WEBSERVICE 0x10084a20
double Bucket::getNominalFPS() const
{
	STUB(0x10084a20);

	return 0.0;
}

} // namespace Profiling
} // namespace RBX
