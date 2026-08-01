#include "util/StandardOut.h"

#include "decomp.h"

namespace RBX {

DECOMP_SIZE_ASSERT(StandardOut, 0x28)
DECOMP_SIZE_ASSERT(StandardOutMessage, 0x28)

// STUB: WEBSERVICE 0x100010d0
void StandardOut::print(MessageType type, const std::exception& error)
{
	STUB(0x100010d0);
}

// STUB: WEBSERVICE 0x10031e40
void StandardOut::print(MessageType type, const char* format, ...)
{
	STUB(0x10031e40);
}

// STUB: WEBSERVICE 0x100320e0
boost::shared_ptr<StandardOut> StandardOut::singleton()
{
	STUB(0x100320e0);
	return boost::shared_ptr<StandardOut>();
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
