#define NOMINMAX

#include "util/Profiling.h"

#include "decomp.h"

#include <G3D/System.h>
#include <algorithm>

namespace RBX {
namespace Profiling {

// GLOBAL: WEBSERVICE 0x102f7474
// GLOBAL: ROBLOX 0x0095d9ec
DWORD Mark::markTlsIndex = 0;

// FUNCTION: WEBSERVICE 0x100844e0
// FUNCTION: ROBLOX 0x005ce280
void init(bool enabled)
{
	if (enabled) {
		Mark::markTlsIndex = TlsAlloc();
	}
}

// FUNCTION: WEBSERVICE 0x10084500
// FUNCTION: ROBLOX 0x005ce2a0
void ThreadProfiler::sample(void* thread)
{
	double time = G3D::System::getTick();

	if (time >= lastSampleTime + bucketTimeSpan) {
		__int64 creationTime;
		__int64 exitTime;
		__int64 kernelTime;
		__int64 userTime;

		if (GetThreadTimes(
				thread, (FILETIME*) &creationTime, (FILETIME*) &exitTime, (FILETIME*) &kernelTime, (FILETIME*) &userTime
			)) {
			__int64 kern = kernelTime;
			__int64 user = userTime;

			if (initialized) {
				buckets[currentBucket].sampleTimeSpan = time - lastSampleTime;
				buckets[currentBucket].kernTimeSpan += kern;
				buckets[currentBucket].userTimeSpan += user;

				currentBucket = (currentBucket + 1) % bucketCount;
			}
			else {
				initialized = true;
			}

			lastSampleTime = time;

			buckets[currentBucket].kernTimeSpan = -kern;
			buckets[currentBucket].userTimeSpan = -user;
		}
	}
}

// FUNCTION: WEBSERVICE 0x10084600
// FUNCTION: ROBLOX 0x005ce3a0
void CodeProfiler::log(__int64 kern, __int64 user, bool frameTick)
{
	double now = G3D::System::getTick();

	if (now >= lastSampleTime + bucketTimeSpan) {
		buckets[currentBucket].sampleTimeSpan = now - lastSampleTime;

		currentBucket = (currentBucket + 1) % bucketCount;

		buckets[currentBucket].frames = frameTick ? 1 : 0;
		buckets[currentBucket].kernTimeSpan = kern;
		buckets[currentBucket].userTimeSpan = user;

		lastSampleTime = now;
	}
	else {
		if (frameTick) {
			buckets[currentBucket].frames++;
		}

		buckets[currentBucket].kernTimeSpan += kern;
		buckets[currentBucket].userTimeSpan += user;
	}
}

// FUNCTION: WEBSERVICE 0x100846d0
// FUNCTION: ROBLOX 0x005ce470
double Bucket::getActualFPS() const
{
	if (sampleTimeSpan > 0.0) {
		return frames / sampleTimeSpan;
	}

	return 0.0;
}

// FUNCTION: WEBSERVICE 0x100846f0
// FUNCTION: ROBLOX 0x005ce490
double Bucket::getFrameTime() const
{
	return getTotalTime() / frames;
}

// FUNCTION: WEBSERVICE 0x10084720
// FUNCTION: ROBLOX 0x005ce4c0
double Bucket::getTotalTime() const
{
	return (kernTimeSpan + userTimeSpan) * 1e-7;
}

// FUNCTION: WEBSERVICE 0x10084750
// FUNCTION: ROBLOX 0x005ce4f0
Profiler::Profiler(const char* name)
	: bucketTimeSpan(1.0), currentBucket(0), lastSampleTime(G3D::System::getTick()), name(name)
{
}

// FUNCTION: WEBSERVICE 0x100847b0
// FUNCTION: ROBLOX 0x005ce550
CodeProfiler::CodeProfiler(const char* name) : Profiler(name), parent(NULL)
{
}

// FUNCTION: WEBSERVICE 0x100847d0
// FUNCTION: ROBLOX 0x005ce570
ThreadProfiler::ThreadProfiler(const char* name) : Profiler(name), initialized(false)
{
}

// FUNCTION: WEBSERVICE 0x100847f0
// FUNCTION: ROBLOX 0x005ce590
Bucket Profiler::getData(double seconds) const
{
	Bucket result;

	if (Mark::markTlsIndex != 0) {
		double span = seconds - (G3D::System::getTick() - lastSampleTime);

		unsigned int firstBucket = currentBucket + bucketCount - 1;

		unsigned int count = std::min((unsigned int) (span / bucketTimeSpan), bucketCount - 2);

		for (unsigned int i = 0; i < count; i++) {
			if (result.sampleTimeSpan >= span) {
				break;
			}

			result += buckets[(firstBucket - i) % bucketCount];
		}
	}

	return result;
}

// FUNCTION: WEBSERVICE 0x10084a20
// FUNCTION: ROBLOX 0x005ce7c0
double Bucket::getNominalFPS() const
{
	return frames / getTotalTime();
}

} // namespace Profiling
} // namespace RBX
