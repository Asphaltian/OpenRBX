#ifndef UTIL_STANDARDOUT_H
#define UTIL_STANDARDOUT_H

#include "util/Events.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
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

// SIZE 0x28
struct StandardOutMessage
{
	MessageType type;    // 0x00
	std::string message; // 0x04
	__int64 time;        // 0x20
};

// VTABLE: WEBSERVICE 0x1022cfe0
// SIZE 0x28
class StandardOut : public boost::enable_shared_from_this<StandardOut>, public Notifier<StandardOut, StandardOutMessage>
{
public:
	static boost::shared_ptr<StandardOut> singleton();
	static void print_exception(const boost::function0<void>& job, MessageType type, bool rethrow);

	void print(MessageType type, const std::exception& error);
	void print(MessageType type, const char* format, ...);

	virtual ~StandardOut();

	// SYNTHETIC: WEBSERVICE 0x10032190
	// RBX::StandardOut::`scalar deleting destructor'

	boost::mutex sync; // 0x20
};

} // namespace RBX

#endif // UTIL_STANDARDOUT_H
