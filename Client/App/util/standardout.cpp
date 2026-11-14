#include "util/standardout.h"

#include "placeholder.h"
#include "util/Log.h"

#include <G3D/format.h>
#include <cstdarg>
#include <ctime>

namespace RBX {

void StandardOut::print(MessageType type, const std::exception& exp)
{
	print(type, exp.what());
}

void StandardOut::print(MessageType type, const char* format, ...)
{
	va_list arguments;
	va_start(arguments, format);
	std::string message = G3D::vformat(format, arguments);
	va_end(arguments);

	if (Log::current()) {
		if (type == MESSAGE_ERROR) {
			Log::current()->writeEntry(Log::Error, message.c_str());
		}
		else if (type == MESSAGE_WARNING) {
			Log::current()->writeEntry(Log::Warning, message.c_str());
		}
		else if (type == MESSAGE_INFO) {
			Log::current()->writeEntry(Log::Information, message.c_str());
		}
	}

	if (hasListeners()) {
		boost::mutex::scoped_lock lock(sync);

		if (hasListeners()) {
			raise(StandardOutMessage(type, message.c_str()));
		}
	}
}

boost::shared_ptr<StandardOut> StandardOut::singleton()
{
	static boost::shared_ptr<StandardOut> standardOut(new StandardOut());

	return standardOut;
}

void StandardOut::print_exception(const boost::function0<void>& f, MessageType type, bool rethrow)
{
	try {
		f();
	}
	catch (const std::exception& e) {
		singleton()->print(type, e);

		if (rethrow) {
			throw;
		}
	}
}

} // namespace RBX
