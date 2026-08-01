#include "util/standardout.h"

#include "decomp.h"
#include "util/Log.h"

#include <G3D/format.h>
#include <cstdarg>
#include <ctime>

namespace RBX {

DECOMP_SIZE_ASSERT(StandardOut, 0x28)
DECOMP_SIZE_ASSERT(StandardOutMessage, 0x28)

// FUNCTION: WEBSERVICE 0x100010d0
void StandardOut::print(MessageType type, const std::exception& error)
{
	print(type, error.what());
}

// STUB: WEBSERVICE 0x10031e40
void StandardOut::print(MessageType type, const char* format, ...)
{
	va_list arguments;
	va_start(arguments, format);
	std::string message = G3D::vformat(format, arguments);
	va_end(arguments);

	if (Log::current() != NULL) {
		switch (type) {
		case MESSAGE_INFO:
			Log::current()->writeEntry(Log::Information, message.c_str());
			break;
		case MESSAGE_WARNING:
			Log::current()->writeEntry(Log::Warning, message.c_str());
			break;
		case MESSAGE_ERROR:
			Log::current()->writeEntry(Log::Error, message.c_str());
			break;
		}
	}

	if (hasListeners()) {
		boost::mutex::scoped_lock lock(sync);

		if (hasListeners()) {
			StandardOutMessage event;
			event.type = type;
			event.message = message;
			event.time = _time64(NULL);
			raise(event);
		}
	}
}

// STUB: WEBSERVICE 0x100320e0
boost::shared_ptr<StandardOut> StandardOut::singleton()
{
	static boost::shared_ptr<StandardOut> standardOut(new StandardOut());

	return standardOut;
}

// STUB: WEBSERVICE 0x100321b0
void StandardOut::print_exception(const boost::function0<void>& job, MessageType type, bool rethrow)
{
	STUB(0x100321b0);
}

StandardOut::~StandardOut()
{
}

} // namespace RBX
