#include "util/Log.h"

#include "decomp.h"

namespace RBX {

DECOMP_SIZE_ASSERT(Log, 0xac)
DECOMP_SIZE_ASSERT(ILogProvider, 0x04)

Log::Severity Log::aggregateWorstSeverity = Log::Information;
ILogProvider* Log::provider;

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

// STUB: WEBSERVICE 0x1003e020
void Log::writeEntry(Severity severity, const char* entry)
{
	STUB(0x1003e020);
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
