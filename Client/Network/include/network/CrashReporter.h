#ifndef NETWORK_CRASHREPORTER_H
#define NETWORK_CRASHREPORTER_H

#include "placeholder.h"

#include <windows.h>

enum CrashReportAction
{
	AOC_WRITE_TO_DISK = 4,
	AOC_SILENT_MODE = 8
};

struct CrashReportControls
{
	int actionToTake;
	char appName[128];
	char appVersion[128];
	char pathToMinidump[260];
	char SMTPServer[128];
	char SMTPAccountName[64];
	char emailSender[64];
	char emailSubjectPrefix[128];
	char emailBody[1024];
	char emailRecipient[64];
	int minidumpType;
};

class CrashReporter
{
public:
	static CrashReporter* singleton;

	CrashReportControls controls;

	CrashReporter() { singleton = this; }

	void Start();

	virtual LONG ProcessException(_EXCEPTION_POINTERS* ExceptionInfo);
};

#endif // NETWORK_CRASHREPORTER_H
