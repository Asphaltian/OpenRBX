#ifndef UTIL_HANDLE_H
#define UTIL_HANDLE_H

#include <boost/shared_ptr.hpp>

namespace RBX {

class Instance;

class InstanceHandle
{
public:
	InstanceHandle() {}
	InstanceHandle(Instance* target);

	void linkTo(boost::shared_ptr<Instance> target);

	bool empty() const;

	boost::shared_ptr<Instance> getTarget() const { return target; }

	bool operatorLess(const InstanceHandle& other) const;

	bool operator<(const InstanceHandle& other) const { return operatorLess(other); }

private:
	boost::shared_ptr<Instance> target; // 0x00
};

} // namespace RBX

#endif // UTIL_HANDLE_H
