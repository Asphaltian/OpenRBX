#include "network/CrashReporter.h"

#include <signal.h>
#include <stdio.h>
#include <string.h>

CrashReporter* CrashReporter::singleton;

LONG CrashReporter::ProcessException(_EXCEPTION_POINTERS*)
{
	return EXCEPTION_CONTINUE_SEARCH;
}

static void crashSignal(int signal)
{
	if (CrashReporter::singleton) {
		fprintf(
			stderr,
			"%s %s crashed (signal %d)\n",
			CrashReporter::singleton->controls.appName,
			CrashReporter::singleton->controls.appVersion,
			signal
		);
	}
	::signal(signal, SIG_DFL);
	raise(signal);
}

void CrashReporter::Start()
{
	signal(SIGSEGV, crashSignal);
	signal(SIGABRT, crashSignal);
	signal(SIGFPE, crashSignal);
	signal(SIGILL, crashSignal);
	signal(SIGBUS, crashSignal);
}
