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

// FUNCTION: WEBSERVICE 0x1003d890
std::string Log::formatMem(unsigned int bytes)
{
	char text[64];

	if (bytes < 1000) {
		sprintf(text, "%dB", bytes);
	}
	else if (bytes < 1000000) {
		sprintf(text, "%dKB", bytes / 1000);
	}
	else if (bytes < 1000000000) {
		sprintf(text, "%dMB", bytes / 1000000);
	}
	else {
		sprintf(text, "%dGB", bytes / 1000000000);
	}

	return std::string(text);
}

// STUB: WEBSERVICE 0x1003d930
std::string Log::formatTime(double seconds)
{
	char text[64];

	if (seconds == 0.0) {
		sprintf(text, "0s");
	}

	if (seconds < 0.0) {
		sprintf(text, "%.3gs", seconds);
	}
	else if (!(seconds < 0.1)) {
		sprintf(text, "%.3gs", seconds);
	}
	else {
		sprintf(text, "%.3gms", seconds * 1000.0);
	}

	return std::string(text);
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
	std::ofstream* out = currentStream();

	SYSTEMTIME now;
	GetLocalTime(&now);

	char stamp[256];
	sprintf(stamp, "%02u:%02u.%03u ", now.wHour, now.wMinute, now.wMilliseconds);

	*out << stamp;
	out->flush();

	switch (severity) {
	case Error:
		*currentStream() << error;
	case Warning:
		*currentStream() << warning;
	case Information:
		*currentStream() << information;
	}

	*currentStream() << entry;
	stream << '\n';
	currentStream()->flush();
}

std::string Log::timeStamp()
{
	return std::string();
}

std::ofstream* Log::currentStream()
{
	return &provider->provideLog()->stream;
}

} // namespace RBX
