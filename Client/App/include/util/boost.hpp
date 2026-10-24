#ifndef UTIL_BOOST_HPP
#define UTIL_BOOST_HPP

#include "decomp.h"

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

namespace RBX {

void set_thread_name(const char* name);
const char* get_thread_name();

boost::function0<void> background_function(const boost::function0<void>& threadfunc, const char* name);

// SIZE 0x14
class worker_thread : public boost::noncopyable
{
	// SIZE 0x24
	struct data
	{
		boost::mutex sync;              // 0x00
		boost::condition wakeCondition; // 0x08
		bool endRequest;                // 0x20

		// FUNCTION: WEBSERVICE 0x10032340
		data() : endRequest(false) {}
	};

	boost::shared_ptr<data> _data; // 0x00
	boost::thread thread;          // 0x08

public:
	enum work_result
	{
		done,
		more
	};

	worker_thread(const boost::function0<work_result>& work_function, const char* name);
	~worker_thread();

	void wake();
	void join();

private:
	static void threadProc(boost::shared_ptr<data> data, const boost::function0<work_result>& work_function);
};

DECOMP_SIZE_ASSERT(worker_thread, 0x14)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10221410
// `RBX::boost_detail::init_foo'::`2'::`dynamic atexit destructor for 'value''
// clang-format on

} // namespace RBX

#endif // UTIL_BOOST_HPP
