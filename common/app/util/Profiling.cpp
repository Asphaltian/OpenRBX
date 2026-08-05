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
float Bucket::getActualFPS()
{
	STUB(0x100846d0);

	return 0.0f;
}

// STUB: WEBSERVICE 0x100846f0
float Bucket::getFrameTime()
{
	STUB(0x100846f0);

	return 0.0f;
}

// STUB: WEBSERVICE 0x10084720
float Bucket::getTotalTime()
{
	STUB(0x10084720);

	return 0.0f;
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
void Profiler::getData(Bucket& answer, double seconds)
{
	STUB(0x100847f0);
}

// STUB: WEBSERVICE 0x10084a20
float Bucket::getNominalFPS()
{
	STUB(0x10084a20);

	return 0.0f;
}

} // namespace Profiling
} // namespace RBX
