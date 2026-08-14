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

	// FUNCTION: WEBSERVICE 0x100484b0 FOLDED
	boost::shared_ptr<Instance> getTarget() const { return target; }

protected:
	bool operatorLess(const InstanceHandle& other) const;

public:
	bool operator<(const InstanceHandle& other) const { return operatorLess(other); }

private:
	boost::shared_ptr<Instance> target; // 0x00
};

} // namespace RBX

#endif // UTIL_HANDLE_H
