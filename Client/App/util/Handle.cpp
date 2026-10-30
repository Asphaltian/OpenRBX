#include "util/Handle.h"

#include "v8tree/Instance.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x10098f70
bool InstanceHandle::empty() const
{
	return target.get() == 0;
}

// FUNCTION: WEBSERVICE 0x10098f80
// FUNCTION: ROBLOX 0x005c9300
void InstanceHandle::linkTo(boost::shared_ptr<Instance> target)
{
	this->target = target;
}

// FUNCTION: WEBSERVICE 0x10099000
// FUNCTION: ROBLOX 0x005c9380
bool InstanceHandle::operatorLess(const InstanceHandle& other) const
{
	return target < other.target;
}

// FUNCTION: WEBSERVICE 0x10099020
// STUB: ROBLOX 0x005c93a0
InstanceHandle::InstanceHandle(Instance* target) : target(shared_from(target))
{
}

} // namespace RBX
