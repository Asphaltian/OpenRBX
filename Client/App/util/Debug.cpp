#include "decomp.h"
#include "util/Utilities.h"

#include <ostream>
#include <typeinfo>

namespace RBX {

Debugable::AssertAction Debugable::assertAction = Debugable::IgnoreAssert;
bool Debugable::validatingDebug;

// FUNCTION: WEBSERVICE 0x10071180
void Debugable::dump(std::ostream& stream)
{
	stream << typeid(*this).name();
}

} // namespace RBX
