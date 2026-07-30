#include "util/Handle.h"

#include "v8tree/Instance.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x10098f70
bool InstanceHandle::empty() const
{
	return target.get() == 0;
}

// FUNCTION: WEBSERVICE 0x10098f80
void InstanceHandle::linkTo(boost::shared_ptr<Instance> value)
{
	target = value;
}

// FUNCTION: WEBSERVICE 0x10099000
bool InstanceHandle::operatorLess(const InstanceHandle& other) const
{
	return target < other.target;
}

} // namespace RBX
