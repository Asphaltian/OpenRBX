#include "util/Log.h"

#include "placeholder.h"

#include <cstdio>
#include <windows.h>

namespace RBX {

Log::Severity Log::aggregateWorstSeverity = Log::Information;
ILogProvider* Log::provider;

static const char* information = "          ";
static const char* warning = " Warning: ";
static const char* error = " Error:   ";

void Log::setLogProvider(ILogProvider* provider)
{
	Log::provider = provider;
}

std::string Log::formatMem(unsigned int bytes)
{
	char buffer[64];

	if (bytes < 1000) {
		sprintf(buffer, "%dB", bytes);
	}
	else if (bytes < 1000000) {
		sprintf(buffer, "%dKB", bytes / 1000);
	}
	else if (bytes < 1000000000) {
		sprintf(buffer, "%dMB", bytes / 1000000);
	}
	else {
		sprintf(buffer, "%dGB", bytes / 1000000000);
	}

	return std::string(buffer);
}

std::string Log::formatTime(double time)
{
	char buffer[64];

	if (time == 0.0) {
		sprintf(buffer, "0s");
	}

	if (time < 0.0) {
		sprintf(buffer, "%.3gs", time);
	}
	else if (time >= 0.1) {
		sprintf(buffer, "%.3gs", time);
	}
	else {
		sprintf(buffer, "%.3gms", time * 1000.0);
	}

	return std::string(buffer);
}

} // namespace RBX

static void timeStamp(std::ofstream& stream, bool includeDate)
{
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);

	char s[256];

	if (includeDate) {
		sprintf(s, "%02u.%02u.%u ", systemTime.wDay, systemTime.wMonth, systemTime.wYear);
		stream << s;
	}

	sprintf(s, "%02u:%02u.%03u ", systemTime.wHour, systemTime.wMinute, systemTime.wMilliseconds);
	stream << s;
	stream.flush();
}

namespace RBX {

Log::Log(const char* logFile, const char* name) : worstSeverity(Information), logFile(logFile), stream(logFile)
{
	::timeStamp(stream, true);
	stream << "Log \"" << name << "\"\n";
	stream.flush();
}

Log::~Log()
{
	::timeStamp(stream, true);
	stream << "End Log\n";
}

void Log::writeEntry(Severity severity, const char* message)
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

	*currentStream() << message;
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
