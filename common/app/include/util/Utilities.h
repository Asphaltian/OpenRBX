#ifndef UTIL_UTILITIES_H
#define UTIL_UTILITIES_H

#include <boost/shared_ptr.hpp>
#include <iosfwd>

namespace RBX {

class Debugable
{
public:
	enum AssertAction
	{
		ASSERT_ACTION_IGNORE = 0,
		ASSERT_ACTION_BREAK = 1,
		ASSERT_ACTION_CRASH = 2,
	};

	virtual ~Debugable();
	virtual void dump(std::ostream& stream);
};

template <class T>
class CopyOnWrite
{
private:
	boost::shared_ptr<T> value; // 0x00
};

} // namespace RBX

#endif // UTIL_UTILITIES_H
