#ifndef UTIL_EVALUATOR_H
#define UTIL_EVALUATOR_H

#include "decomp.h"

#include <string>

namespace RBX {

// SIZE 0x4
class __declspec(novtable) Evaluator
{
public:
	virtual std::string evaluate(const std::string& valueName) const = 0; // vtable+0x00
};

DECOMP_SIZE_ASSERT(Evaluator, 0x4)

} // namespace RBX

#endif // UTIL_EVALUATOR_H
