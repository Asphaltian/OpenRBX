#include "util/Handle.h"

#include "v8tree/Instance.h"

namespace RBX {

bool InstanceHandle::empty() const
{
	return target.get() == 0;
}

void InstanceHandle::linkTo(boost::shared_ptr<Instance> target)
{
	this->target = target;
}

bool InstanceHandle::operatorLess(const InstanceHandle& other) const
{
	return target < other.target;
}

InstanceHandle::InstanceHandle(Instance* target) : target(shared_from(target))
{
}

} // namespace RBX
