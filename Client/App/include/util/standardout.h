#ifndef UTIL_STANDARDOUT_H
#define UTIL_STANDARDOUT_H

#include "util/Events.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <ctime>
#include <exception>
#include <string>

namespace RBX {

enum MessageType
{
	MESSAGE_OUTPUT = 0,
	MESSAGE_INFO = 1,
	MESSAGE_WARNING = 2,
	MESSAGE_ERROR = 3
};

struct StandardOutMessage
{
	StandardOutMessage(MessageType type, const char* message) : type(type), message(message) { _time64(&time); }

	MessageType type;
	std::string message;
	__int64 time;
};

class StandardOut : public boost::enable_shared_from_this<StandardOut>, public Notifier<StandardOut, StandardOutMessage>
{
public:
	static boost::shared_ptr<StandardOut> singleton();
	static void print_exception(const boost::function0<void>& job, MessageType type, bool rethrow);

	void print(MessageType type, const std::exception& error);
	void print(MessageType type, const char* format, ...);

	boost::mutex sync;
};

} // namespace RBX

#endif // UTIL_STANDARDOUT_H
