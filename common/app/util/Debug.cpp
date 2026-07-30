#include "util/Utilities.h"

#include <ostream>
#include <typeinfo>

namespace RBX {

Debugable::~Debugable()
{
}

// FUNCTION: WEBSERVICE 0x10071180
void Debugable::dump(std::ostream& stream)
{
	stream << typeid(*this).name();
}

} // namespace RBX
