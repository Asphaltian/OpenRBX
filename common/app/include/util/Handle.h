#ifndef UTIL_HANDLE_H
#define UTIL_HANDLE_H

#include <boost/shared_ptr.hpp>

namespace RBX {

class Instance;

class InstanceHandle
{
public:
	InstanceHandle(Instance* target);

	void linkTo(boost::shared_ptr<Instance> value);

	bool empty() const;
	bool operatorLess(const InstanceHandle& other) const;

private:
	boost::shared_ptr<Instance> target; // 0x00
};

} // namespace RBX

#endif // UTIL_HANDLE_H
