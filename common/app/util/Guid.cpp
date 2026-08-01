#include "util/Guid.h"

#include "decomp.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x1003d4e0
void Guid::assign(Data data)
{
	this->data = data;
}

// STUB: WEBSERVICE 0x1003d540
int Guid::compare(const Guid& a, const Guid& b)
{
	STUB(0x1003d540);
	return 0;
}

// STUB: WEBSERVICE 0x1003d5d0
int Guid::compare(const Guid& other) const
{
	STUB(0x1003d5d0);
	return 0;
}

// STUB: WEBSERVICE 0x1003d700
Guid::Guid()
{
	STUB(0x1003d700);
}

} // namespace RBX
