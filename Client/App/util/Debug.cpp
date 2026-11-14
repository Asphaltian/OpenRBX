#include "placeholder.h"
#include "util/Utilities.h"

#include <ostream>
#include <typeinfo>

namespace RBX {

Debugable::AssertAction Debugable::assertAction = Debugable::IgnoreAssert;
bool Debugable::validatingDebug;

void Debugable::dump(std::ostream& stream)
{
	stream << typeid(*this).name();
}

} // namespace RBX
