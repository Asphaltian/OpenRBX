#ifndef UTIL_LOG_H
#define UTIL_LOG_H

#include <fstream>
#include <string>

namespace RBX {

class Log;

class __declspec(novtable) ILogProvider
{
public:
	virtual Log* provideLog() = 0;
};

class Log
{
public:
	enum Severity
	{
		Information = 0,
		Warning = 1,
		Error = 2
	};

	static std::string formatMem(unsigned int bytes);
	static std::string formatTime(double seconds);

	static Severity aggregateWorstSeverity;

	Severity worstSeverity;

	void writeEntry(Severity severity, const char* entry);
	std::string timeStamp();

	static void setLogProvider(ILogProvider* provider);

	Log(const char* logFile, const char* name);
	virtual ~Log();

	std::string logFile;

	static Log* current() { return provider != NULL ? provider->provideLog() : NULL; }

private:
	static std::ofstream* currentStream();

	std::ofstream stream;
	static ILogProvider* provider;
};

} // namespace RBX

#endif // UTIL_LOG_H
