#include "util/Log.h"

#include "decomp.h"

#include <cstdio>
#include <windows.h>

namespace RBX {

DECOMP_SIZE_ASSERT(Log, 0xac)
DECOMP_SIZE_ASSERT(ILogProvider, 0x04)

Log::Severity Log::aggregateWorstSeverity = Log::Information;
ILogProvider* Log::provider;

static const char* information = "          ";
static const char* warning = " Warning: ";
static const char* error = " Error:   ";

// FUNCTION: WEBSERVICE 0x1003d880
void Log::setLogProvider(ILogProvider* logProvider)
{
	provider = logProvider;
}

// STUB: WEBSERVICE 0x1003d890
std::string Log::formatMem(unsigned int bytes)
{
	STUB(0x1003d890);
	return std::string();
}

// STUB: WEBSERVICE 0x1003d930
std::string Log::formatTime(double seconds)
{
	STUB(0x1003d930);
	return std::string();
}

// STUB: WEBSERVICE 0x1003def0
Log::Log(const char* directory, const char* name)
{
	STUB(0x1003def0);
}

// STUB: WEBSERVICE 0x1003dfa0
Log::~Log()
{
	STUB(0x1003dfa0);
}

// FUNCTION: WEBSERVICE 0x1003e020
void Log::writeEntry(Severity severity, const char* entry)
{
	std::ofstream* stream = currentStream();

	SYSTEMTIME now;
	GetLocalTime(&now);

	char stamp[260];
	sprintf(stamp, "%02u:%02u.%03u ", now.wHour, now.wMinute, now.wMilliseconds);

	*stream << stamp;
	stream->flush();

	switch (severity) {
	case Error:
		*currentStream() << error;
	case Warning:
		*currentStream() << warning;
	case Information:
		*currentStream() << information;
	}

	*currentStream() << entry << '\n';
	currentStream()->flush();
}

std::string Log::timeStamp()
{
	return std::string();
}

std::ofstream* Log::currentStream()
{
	Log* log = current();

	return log != NULL ? &log->stream : NULL;
}

} // namespace RBX
