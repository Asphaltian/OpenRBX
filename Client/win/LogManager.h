#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "network/CrashReporter.h"
#include "placeholder.h"
#include "util/Log.h"
#include "util/boost.hpp"

#include <atlpath.h>
#include <boost/scoped_ptr.hpp>
#include <exception>
#include <imagehlp.h>
#include <string>
#include <vector>
#include <windows.h>

class MainLogManager;

class LogManager
{
	RBX::Log* log;
	static bool logsEnabled;

protected:
	const DWORD threadID;
	std::string name;
	static MainLogManager* mainLogManager;

public:
	RBX::Log* getLog();
	static MainLogManager* getMainLogManger();
	static HRESULT ReportCOMError(const GUID&, const char*, HRESULT);
	static HRESULT ReportCOMError(const GUID&, const wchar_t*, HRESULT);
	static HRESULT ReportCOMError(const GUID&, HRESULT);
	static HRESULT ReportExceptionAsCOMError(const GUID&, const std::exception&);
	static void ReportException(const std::exception&);
	static void ReportLastError(const char*);
	static void ReportEvent(WORD, HRESULT, const char*, int);
	static void ReportEvent(WORD, const char*, const char*, int);
	static void ReportEvent(WORD type, const char* message);
	ATL::CPath GetLogPath() const;
	virtual ~LogManager();
	virtual std::string getLogFileName() = 0;

protected:
	LogManager(const char* name) : log(NULL), threadID(GetCurrentThreadId()), name(name) {}
};

class RobloxCrashReporter : public CrashReporter
{
public:
	static bool silent;

	RobloxCrashReporter(const char* outputPath, const char* appName, MINIDUMP_TYPE mdType);

	virtual LONG ProcessException(_EXCEPTION_POINTERS* info);
};

class MainLogManager : public RBX::ILogProvider, public LogManager
{
	boost::scoped_ptr<RobloxCrashReporter> crashReporter;

public:
	MainLogManager(const char* productName);
	virtual ~MainLogManager();

	virtual RBX::Log* provideLog();
	virtual std::string getLogFileName();

	std::string MakeLogFileName(const char* prefix);

	bool hasErrorLogs() const;
	bool hasCrashLogs() const;
	std::vector<std::string> archiveCrashLogs();
	void CullLogs(const char* folder, int maxLogs);
	void WriteCrashDump();

private:
	std::string guid;

	static bool handleDebugAssert(
		const char* expression,
		const std::string& message,
		const char* filename,
		int lineNumber,
		bool& ignoreAlways,
		bool useGuiPrompt
	);
	static bool handleG3DFailure(
		const char* expression,
		const std::string& message,
		const char* filename,
		int lineNumber,
		bool& ignoreAlways,
		bool useGuiPrompt
	);
};

class ThreadLogManager : public LogManager
{
	ThreadLogManager() : LogManager(RBX::get_thread_name()) {}

public:
	static ThreadLogManager* getCurrent();

	virtual ~ThreadLogManager() {}

protected:
	virtual std::string getLogFileName();
};

void FUN_0042b3c0(const std::exception& e);

#endif // LOGMANAGER_H
