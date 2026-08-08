#ifndef UTIL_PROFILING_H
#define UTIL_PROFILING_H

#include "decomp.h"

#include <boost/noncopyable.hpp>
#include <cstddef>
#include <string>
#include <windows.h>

namespace RBX {
namespace Profiling {

void init(bool enabled);

// SIZE 0x20
struct Bucket
{
public:
	Bucket() : sampleTimeSpan(0.0), kernTimeSpan(0), userTimeSpan(0), frames(0) {}

	double getActualFPS() const;
	double getNominalFPS() const;
	double getFrameTime() const;
	double getTotalTime() const;

	Bucket& operator+=(const Bucket& other)
	{
		sampleTimeSpan += other.sampleTimeSpan;
		kernTimeSpan += other.kernTimeSpan;
		userTimeSpan += other.userTimeSpan;
		frames += other.frames;

		return *this;
	}

	double sampleTimeSpan; // 0x00
	__int64 kernTimeSpan;  // 0x08
	__int64 userTimeSpan;  // 0x10
	int frames;            // 0x18
};

DECOMP_SIZE_ASSERT(Bucket, 0x20)

// SIZE 0x20038
class Profiler : public boost::noncopyable
{
public:
	Profiler(const char* name);

	Bucket getData(double seconds) const;

protected:
	static const unsigned int bucketCount = 4096;

	const double bucketTimeSpan; // 0x00000
	int currentBucket;           // 0x00008
	Bucket buckets[bucketCount]; // 0x00010
	double lastSampleTime;       // 0x20010

public:
	const std::string name; // 0x20018
};

DECOMP_SIZE_ASSERT(Profiler, 0x20038)

// SIZE 0x20040
class CodeProfiler : public Profiler
{
public:
	CodeProfiler(const char* name);

	CodeProfiler* parent; // 0x20038

private:
	void log(__int64 kern, __int64 user, bool frameTick);

	friend class Mark;
};

DECOMP_SIZE_ASSERT(CodeProfiler, 0x20040)

// SIZE 0x20040
class ThreadProfiler : public Profiler
{
public:
	ThreadProfiler(const char* name);

	void sample(void* thread);

private:
	bool initialized; // 0x20038
};

DECOMP_SIZE_ASSERT(ThreadProfiler, 0x20040)

// SIZE 0x2c
class Mark
{
public:
	Mark(CodeProfiler& section, bool frameTick);
	~Mark();

	static DWORD markTlsIndex;

private:
	CodeProfiler& section;          // 0x00
	CodeProfiler* enclosingSection; // 0x04
	FILETIME creationTime;          // 0x08
	FILETIME exitTime;              // 0x10
	FILETIME kernelTime;            // 0x18
	FILETIME userTime;              // 0x20
	bool frameTick;                 // 0x28
};

DECOMP_SIZE_ASSERT(Mark, 0x2c)

// FUNCTION: WEBSERVICE 0x10071500
inline Mark::Mark(CodeProfiler& section, bool frameTick) : section(section), frameTick(frameTick)
{
	if (markTlsIndex != 0) {
		enclosingSection = (CodeProfiler*) TlsGetValue(markTlsIndex);

		GetThreadTimes(GetCurrentThread(), &creationTime, &exitTime, &kernelTime, &userTime);

		TlsSetValue(markTlsIndex, &section);
	}
}

// STUB: WEBSERVICE 0x10071560
inline Mark::~Mark()
{
	if (markTlsIndex != 0) {
		TlsSetValue(markTlsIndex, enclosingSection);

		__int64 endKernelTime;
		__int64 endUserTime;

		GetThreadTimes(
			GetCurrentThread(),
			&creationTime,
			&exitTime,
			(FILETIME*) &endKernelTime,
			(FILETIME*) &endUserTime
		);

		__int64 kern = *(const __int64*) &kernelTime;
		__int64 user = *(const __int64*) &userTime;

		__int64 kernelSpan = endKernelTime - kern;
		__int64 userSpan = endUserTime - user;

		section.log(kernelSpan, userSpan, frameTick);

		if (enclosingSection != NULL && enclosingSection != &section && enclosingSection != section.parent) {
			enclosingSection->log(-kernelSpan, -userSpan, false);
		}
	}
}

} // namespace Profiling
} // namespace RBX

#endif // UTIL_PROFILING_H
