#ifndef UTIL_IMETRIC_H
#define UTIL_IMETRIC_H

#include "placeholder.h"

#include <string>

namespace RBX {

class __declspec(novtable) IMetric
{
public:
	IMetric() {}
	virtual ~IMetric() {}

	virtual std::string getMetric(const std::string&) const = 0;
};

} // namespace RBX

#endif // UTIL_IMETRIC_H
