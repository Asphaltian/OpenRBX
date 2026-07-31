#include "util/Guid.h"

#include "decomp.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x1003d4e0
void Guid::assign(Data data)
{
	this->data = data;
}

// STUB: WEBSERVICE 0x1003d700
Guid::Guid()
{
	STUB(0x1003d700);
}

} // namespace RBX
