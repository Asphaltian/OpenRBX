#ifndef UTIL_BOOST_HPP
#define UTIL_BOOST_HPP

#include "placeholder.h"

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

class worker_thread : public boost::noncopyable
{
	struct data
	{
		boost::mutex sync;
		boost::condition wakeCondition;
		bool endRequest;

		data() : endRequest(false) {}
	};

	boost::shared_ptr<data> _data;
	boost::thread thread;

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

} // namespace RBX

#endif // UTIL_BOOST_HPP
