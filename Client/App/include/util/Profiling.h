#ifndef UTIL_PROFILING_H
#define UTIL_PROFILING_H

#include "placeholder.h"

#include <boost/noncopyable.hpp>
#include <cstddef>
#include <string>
#include <windows.h>

namespace RBX {
namespace Profiling {

void init(bool enabled);

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

	double sampleTimeSpan;
	__int64 kernTimeSpan;
	__int64 userTimeSpan;
	int frames;
};

class Profiler : public boost::noncopyable
{
public:
	Profiler(const char* name);

	Bucket getData(double seconds) const;

protected:
	static const unsigned int bucketCount = 4096;

	const double bucketTimeSpan;
	int currentBucket;
	Bucket buckets[bucketCount];
	double lastSampleTime;

public:
	const std::string name;
};

class CodeProfiler : public Profiler
{
public:
	CodeProfiler(const char* name);

	CodeProfiler* parent;

private:
	void log(__int64 kern, __int64 user, bool frameTick);

	friend class Mark;
};

class ThreadProfiler : public Profiler
{
public:
	ThreadProfiler(const char* name);

	void sample(void* thread);

private:
	bool initialized;
};

class Mark
{
public:
	Mark(CodeProfiler& section, bool frameTick);
	~Mark();

	static DWORD markTlsIndex;

private:
	CodeProfiler& section;
	CodeProfiler* enclosingSection;
	FILETIME creationTime;
	FILETIME exitTime;
	FILETIME kernelTime;
	FILETIME userTime;
	bool frameTick;
};

inline Mark::Mark(CodeProfiler& section, bool frameTick) : section(section), frameTick(frameTick)
{
	if (markTlsIndex != 0) {
		enclosingSection = (CodeProfiler*) TlsGetValue(markTlsIndex);

		GetThreadTimes(GetCurrentThread(), &creationTime, &exitTime, &kernelTime, &userTime);

		TlsSetValue(markTlsIndex, &section);
	}
}

inline Mark::~Mark()
{
	if (markTlsIndex != 0) {
		TlsSetValue(markTlsIndex, enclosingSection);

		FILETIME endKernelTime;
		FILETIME endUserTime;
		GetThreadTimes(GetCurrentThread(), &creationTime, &exitTime, &endKernelTime, &endUserTime);

		ULARGE_INTEGER kernelStart = {kernelTime.dwLowDateTime, kernelTime.dwHighDateTime};
		ULARGE_INTEGER kernelEnd = {endKernelTime.dwLowDateTime, endKernelTime.dwHighDateTime};
		ULARGE_INTEGER userStart = {userTime.dwLowDateTime, userTime.dwHighDateTime};
		ULARGE_INTEGER userEnd = {endUserTime.dwLowDateTime, endUserTime.dwHighDateTime};

		__int64 kernel = kernelEnd.QuadPart - kernelStart.QuadPart;
		__int64 user = userEnd.QuadPart - userStart.QuadPart;

		section.log(kernel, user, frameTick);

		if (enclosingSection != NULL && enclosingSection != &section && enclosingSection != section.parent) {
			enclosingSection->log(-kernel, -user, false);
		}
	}
}

} // namespace Profiling
} // namespace RBX

#endif // UTIL_PROFILING_H
