#ifndef UTIL_EVALUATOR_H
#define UTIL_EVALUATOR_H

#include "placeholder.h"

#include <string>

namespace RBX {

class __declspec(novtable) Evaluator
{
public:
	virtual std::string evaluate(const std::string& valueName) const = 0;
};

} // namespace RBX

#endif // UTIL_EVALUATOR_H
