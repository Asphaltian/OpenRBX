#include "decomp.h"
#include "util/Utilities.h"

#include <ostream>

namespace RBX {

Debugable::AssertAction Debugable::assertAction = Debugable::IgnoreAssert;
bool Debugable::validatingDebug;

// STUB: WEBSERVICE 0x10071180
void Debugable::dump(std::ostream& stream)
{
	STUB(0x10071180);
}

} // namespace RBX
