#include "util/boost.hpp"

#include "util/StandardOut.h"

#include <boost/bind.hpp>
#include <boost/thread/once.hpp>
#include <boost/thread/tss.hpp>
#include <string>
#include <windows.h>

static boost::thread_specific_ptr<std::string>* foo;

namespace RBX {

namespace boost_detail {

boost::once_flag once_init_foo = BOOST_ONCE_INIT;

void init_foo();

} // namespace boost_detail

static boost::thread_specific_ptr<std::string>& threadName()
{
	boost::call_once(&boost_detail::init_foo, boost_detail::once_init_foo);
	return *::foo;
}

typedef struct tagTHREADNAME_INFO
{
	DWORD dwType;
	LPCSTR szName;
	DWORD dwThreadID;
	DWORD dwFlags;
} THREADNAME_INFO;

void SetThreadName(DWORD dwThreadID, LPCSTR szThreadName)
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = szThreadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;

	__try {
#ifdef _DEBUG
		RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), (ULONG_PTR*) &info);
#endif
	}
	__except (EXCEPTION_CONTINUE_EXECUTION) {
	}
}

class ThreadPrioritySetter
{
	const int old;
	bool success;

public:
	ThreadPrioritySetter(int priority)
		: old(GetThreadPriority(GetCurrentThread())), success(SetThreadPriority(GetCurrentThread(), priority) == TRUE)
	{
	}

	~ThreadPrioritySetter()
	{
		if (success) {
			SetThreadPriority(GetCurrentThread(), old);
		}
	}
};

worker_thread::~worker_thread()
{
	boost::mutex::scoped_lock lock(_data->sync);
	_data->endRequest = true;
	_data->wakeCondition.notify_all();
}

void worker_thread::join()
{
	{
		boost::mutex::scoped_lock lock(_data->sync);
		_data->endRequest = true;
		_data->wakeCondition.notify_all();
	}
	thread.join();
}

void worker_thread::wake()
{
	boost::mutex::scoped_lock lock(_data->sync);
	_data->wakeCondition.notify_all();
}

void worker_thread::threadProc(boost::shared_ptr<data> data, const boost::function0<work_result>& work_function)
{
	while (true) {
		if (data->endRequest) {
			return;
		}

		if (work_function() != more) {
			boost::mutex::scoped_lock lock(data->sync);
			data->wakeCondition.wait(lock);
		}
	}
}

namespace boost_detail {

void init_foo()
{
	static boost::thread_specific_ptr<std::string> value;

	::foo = &value;
}

} // namespace boost_detail

void set_thread_name(const char* name)
{
	threadName().reset(new std::string(name));
	SetThreadName(GetCurrentThreadId(), name);
}

const char* get_thread_name()
{
	static const char* none = "rbx_thread";

	std::string* name = threadName().get();
	return name ? name->c_str() : none;
}

void run_in_background(const boost::function0<void>& threadfunc, std::string name)
{
	try {
		set_thread_name(name.c_str());
		ThreadPrioritySetter set(THREAD_PRIORITY_BELOW_NORMAL);
		threadfunc();
	}
	catch (std::exception& e) {
		StandardOut::singleton()->print(MESSAGE_ERROR, "Crashed via C++ exception in run_in_background:", e.what());
	}
}

boost::function0<void> background_function(const boost::function0<void>& threadfunc, const char* name)
{
	return boost::bind(&run_in_background, threadfunc, std::string(name));
}

worker_thread::worker_thread(const boost::function0<work_result>& work_function, const char* name)
	: _data(new data()), thread(background_function(boost::bind(&threadProc, _data, work_function), name))
{
}

} // namespace RBX
