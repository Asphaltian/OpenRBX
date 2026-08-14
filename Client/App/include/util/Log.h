#ifndef UTIL_LOG_H
#define UTIL_LOG_H

#include <fstream>
#include <string>

namespace RBX {

class Log;

// SIZE 0x04
class __declspec(novtable) ILogProvider
{
public:
	virtual Log* provideLog() = 0; // vtable+0x00
};

// VTABLE: WEBSERVICE 0x1022d894
// SIZE 0xac
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

	Severity worstSeverity; // 0x04

	void writeEntry(Severity severity, const char* entry);
	std::string timeStamp();

	static void setLogProvider(ILogProvider* provider);

	Log(const char* logFile, const char* name);
	virtual ~Log(); // vtable+0x00

	// SYNTHETIC: WEBSERVICE 0x1003e140
	// RBX::Log::`scalar deleting destructor'

	std::string logFile; // 0x08

	static Log* current() { return provider != NULL ? provider->provideLog() : NULL; }

private:
	static std::ofstream* currentStream();

	std::ofstream stream; // 0x24
	static ILogProvider* provider;
};

} // namespace RBX

#endif // UTIL_LOG_H
